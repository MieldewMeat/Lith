#include <cstddef>
#include <cstdlib>
#include <stdlib.h>


struct Slice{
    void* data = nullptr;
    const size_t size = 0;
};

struct Allocator{
    static Slice alloc(const size_t size){
        void* ptr = malloc(size);
        if (ptr == NULL)
            abort();
        return {ptr, size};
    }
    static void dealloc(const Slice& src){
        free(src.data);
    }
};
