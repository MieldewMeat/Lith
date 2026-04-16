// src/arena.c
#include "../../include/lith.hpp"
#include <stddef.h>  // gives you max_align_t on MSVC
#include <stdlib.h>
#include <string.h>

#define ZITH_DEFAULT_BLOCK_SIZE (64 * 1024)

#ifdef _MSC_VER
  typedef union {
      long long   _ll;
      long double _ld;
      void       *_ptr;
  } max_align_t;
#endif

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable: 4200)
#endif

typedef struct LithArenaBlock {
    struct LithArenaBlock *next;
    size_t offset;
    size_t capacity;
    char data[];
} LithArenaBlock;

#ifdef _MSC_VER
  #pragma warning(pop)
#endif

struct LithArena {
    LithArenaBlock *head;
    size_t initial_block_size;
};

static inline size_t align_up(const size_t size, const size_t alignment) {
    return (size + alignment - 1) & ~(alignment - 1);
}

struct LithArena *lith_arena_create(size_t initial_block_size) {
    if (initial_block_size == 0) initial_block_size = ZITH_DEFAULT_BLOCK_SIZE;
    struct LithArena *arena = (LithArena*)calloc(1, sizeof(LithArena));
    if (!arena) return NULL;
    arena->initial_block_size = initial_block_size;
    return arena;
}

void *lith_arena_alloc(LithArena *arena, size_t size) {
    if (!arena || size == 0) return NULL;

    const size_t alignment = _Alignof(max_align_t);
    size = align_up(size, alignment);

    LithArenaBlock *block = arena->head;
    if (!block || block->offset + size > block->capacity) {
        // Allocate new block
        size_t block_size = arena->initial_block_size;
        if (size > block_size) block_size = size; // accommodate large allocs

        const size_t total_alloc = sizeof(LithArenaBlock) + block_size;
        LithArenaBlock *new_block = (LithArenaBlock *) malloc(total_alloc);
        if (!new_block) return NULL;

        new_block->next = arena->head;
        new_block->offset = 0;
        new_block->capacity = block_size;
        arena->head = new_block;
        block = new_block;
    }

    void *ptr = &block->data[block->offset];
    block->offset += size;
    return ptr;
}

char *lith_arena_strdup(LithArena *arena, const char *str) {
    if (!str) return NULL;
    const size_t len = strlen(str);
    void *copy = lith_arena_alloc(arena, len + 1);
    if (copy) memcpy(copy, str, len + 1);
    return copy;
}

char *lith_arena_str(LithArena *arena, const char *str, const size_t len) {
    if (!str) return NULL;
    char *copy = (char*)lith_arena_alloc(arena, len + 1);
    if (!copy) return NULL;
    memcpy(copy, str, len);
    copy[len] = '\0';
    return copy;
}

void lith_arena_reset(LithArena *arena) {
    if (!arena) return;
    for (LithArenaBlock *b = arena->head; b; b = b->next)
        b->offset = 0;
}

void lith_arena_destroy(LithArena *arena) {
    if (!arena) return;
    lith_arena_reset(arena);
    free(arena);
}