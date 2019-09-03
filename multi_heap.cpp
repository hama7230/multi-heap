#include <iostream>
#include <vector>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;


class Heap {
    private:
        pthread_t thread;
        
        static void* run_malloc(void* a) {
            long _size;
            void* _ptr;
            _size = (long) a;
            _ptr = malloc(_size);
            return _ptr;
        }

        void* malloc_thread(long size) {
            void* result;
            pthread_create(&thread, NULL, run_malloc, (void*) size);
            pthread_join(thread, &result);
            return result;
        }
        
        static void* run_copy(void* a) {
            long* args = (long*) a;
            char* dst;
            char* src;
            long _size;
            dst = (char*)args[0];
            src = (char*)args[1];
            _size = (long)args[2];
            usleep(1);
            
            unsigned long i;
            for (i = 0; i<(unsigned long)_size; i++) {
                dst[i] = src[i];
            }
            pthread_exit(NULL);
        }

        void memcpy_thread(void* dst, void* src, long _size) {
            long args[3];
            args[0] = (long)dst;
            args[1] = (long)src;
            args[2] = _size;
            pthread_create(&thread, NULL, run_copy, (void*) args);
            usleep(1);
        }

    protected:
        void* ptr;
        long size;
    
    public:
        Heap() {
            
            cout << "Size: ";
            cin >> size;
            
            if (size < 0) {
               cout << "Invalid size" << endl;
               exit(1);
            }
            
            cout << "Main or Thread? (m/t): ";
            char tmp;
            cin >> tmp;

            if (tmp == 't') 
                ptr = malloc_thread(size);
            else 
                ptr = malloc(size);
            // cout << ptr << endl;
            
            if (ptr == nullptr) {
                exit(1);
            }
            // memset(ptr, 0, size);
        }
        virtual ~Heap() {
            free(ptr);
            ptr = nullptr;
            size = 0;
            memset(&thread, 0, sizeof(pthread_t));
        }

        void* get_ptr() {
            return ptr;
        }

        long get_size() {
            return size;
        }

        virtual long read_buf(long _size) {
            return _size;
        }

        virtual long write_buf() {
            return 0;
        }
        
        void copy(Heap *dst, Heap* src, long copy_size) {
            long dst_size, src_size;
            dst_size = dst->get_size();
            src_size = src->get_size();

            copy_size -= 1;
            // no checking for negative integer
            if (copy_size > dst_size || copy_size > src_size)
                return;
            
            cout << "Thread process? (y/n): ";
            char choice;
            cin >> choice;
            if (choice == 'y') {
                memcpy_thread(dst->get_ptr(), src->get_ptr(), copy_size);
            } else {
                memcpy(dst->get_ptr(), src->get_ptr(), copy_size);
            }

            return;
        }
};

class Char: public Heap {
    private:
    public:
        Char() {
        }
        ~Char() {
        }


        long read_buf(long _size) {
            ssize_t result = 0, tmp = 0;
            
            if (_size > size-1)
                return 0; 
            cout << "Content: ";
            do {
                tmp = read(0, (void*)((long)ptr+result), _size-result);
                if (tmp < 0)
                    exit(1);
                result += tmp;
                // cout << result << endl;
            } while (result < _size);
            
            ((char*)ptr)[_size] = '\x00';
            return result;
        }

        long write_buf() {
            char* _ptr = (char*)ptr;
            cout << _ptr << endl;
            return size;
        }

};

class Long: public Heap {
    private:

    public:
        Long() {
        }
        ~Long() {
        }

        long read_buf(long _size) {
            if (_size > (signed)(size/sizeof(long)))
                return 0;
            
            long* _ptr = (long*)ptr;
            for (int i=0; i<_size; i++) {
                long tmp;
                cin >> tmp;
                _ptr[i] = tmp;
            }
            
            return _size;
        }

        long write_buf() {
            long* _ptr = (long*)ptr;
            for (unsigned int i=0; i<size/sizeof(long); i++) 
                cout << _ptr[i] << endl;
            
            return size/sizeof(long);
        }
};

class Float: public Heap {
    private:

    public:
        Float() {
        }
        ~Float() {
        }
        
        long read_buf(long _size) {
            if (_size > (signed)(size/sizeof(double)))
                return 0;
            
            double* _ptr = (double*)ptr;
            for (int i=0; i<_size; i++) {
                double tmp;
                cin >> tmp;
                _ptr[i] = tmp;
            }
            
            return _size;
        }

        long write_buf() {
            double* _ptr = (double*)ptr;
            for (unsigned int i=0; i<size/sizeof(double); i++) 
                cout << _ptr[i] << endl;
            
            return size/sizeof(double);
        }
};

vector<Heap*> array;

void alloc_heap() {
    string choice;
    cout << "Which: ";
    cin >> choice;
    if (choice == "char") {
        Char* c = new Char();
        array.push_back(c);
    } else if (choice == "long") {
        Long* l = new Long();
        array.push_back(l);
    } else if (choice == "float") {
        Float* f = new Float();
        array.push_back(f);
    } else {
        cout << "Unknown" << endl;
        return;
    }
    cout << "Done" << endl;
}

void free_heap() {
    unsigned index;
    
    cout << "Index: ";
    cin >> index;
    if (index >= array.size()) {
        cout << "Invalid index"<< endl;
        return;
    }

    delete array[index];
    array[index] = nullptr;
    array.erase(index + array.begin());

    cout << "Done" << endl;
}

void write_heap() {
    unsigned index;
    
    cout << "Index: ";
    cin >> index;
    if (index >= array.size()) {
        cout << "Invalid index"<< endl;
        return;
    }

    Heap* h = array[index];
    h->write_buf();
}

void read_heap() {
    unsigned index;
    
    cout << "Index: ";
    cin >> index;
    if (index >= array.size()) {
        cout << "Invalid index"<< endl;
        return;
    }

    long size;
    cout << "Size: ";
    cin >> size;
    Heap* h = array[index];
    h->read_buf(size);

}

void copy_heap() {
    unsigned src_idx, dst_idx;
    Heap* src;
    Heap* dst;
    long size;

    cout << "Src index: ";
    cin >> src_idx;
    cout << "Dst index: ";
    cin >> dst_idx;

    if (src_idx >= array.size() || dst_idx >= array.size()) {
        cout << "Invalid index" << endl;
        return;
    }
    
    cout << "Size: ";
    cin >> size;

    src = array[src_idx];
    dst = array[dst_idx];
    dst->copy(dst, src, size);
}

void init_proc() {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
}

int main() {
    init_proc();
    cout << "\n##     ## ##     ## ##       ######## ####    ##     ## ########    ###    ########  \n###   ### ##     ## ##          ##     ##     ##     ## ##         ## ##   ##     ## \n#### #### ##     ## ##          ##     ##     ##     ## ##        ##   ##  ##     ## \n## ### ## ##     ## ##          ##     ##     ######### ######   ##     ## ########  \n##     ## ##     ## ##          ##     ##     ##     ## ##       ######### ##        \n##     ## ##     ## ##          ##     ##     ##     ## ##       ##     ## ##        \n##     ##  #######  ########    ##    ####    ##     ## ######## ##     ## ##\n"<< endl;
    while (1) {
        int choice;
        
        cout << "==============================\n1. Alloc\n2. Free\n3. Write\n4. Read\n5. Copy\n==============================\nYour choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                alloc_heap();
                break;
            case 2:
                free_heap();
                break;
            case 3:
                write_heap();
                break;
            case 4:
                read_heap();
                break;
            case 5:
                copy_heap();
                break;
            default:
                cout << "Invalid choice" << endl;
        };
    }
}
