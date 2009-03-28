#ifndef INCLUDED_MEMORY_H__
#define INCLUDED_MEMORY_H__

#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct ScmMemHeapBlockRec ScmMemHeapBlock;
typedef struct ScmMemHeapRec ScmMemHeap;
typedef struct ScmMemRec ScmMem;
typedef struct ScmForwardRec ScmForward;

#define SCM_FORWORD(obj) ((ScmFoward *)(obj))

#include "basichash.h"
#include "object.h"

typedef void (*SCM_MEM_FINALIZER)(ScmObj obj);

struct ScmMemHeapBlockRec {
  struct ScmMemHeapBlockRec *next;
  struct ScmMemHeapBlockRec *prev;
  size_t size;
  size_t used;
  uint8_t heap[0];
};

#define SCM_MEM_HEAP_NEW_BLOCK(block, sz)              \
  do {                                                 \
    (block) = malloc(sizeof(ScmMemHeapBlock) + (sz));  \
    if ((block) != NULL) {                             \
      (block)->next = NULL;                            \
      (block)->prev = NULL;                            \
      (block)->size = (sz);                            \
      (block)->used = 0;                               \
    }                                                  \
  } while(0)

#define SCM_MEM_HEAP_DELEATE_BLOCK(block)            \
  do {                                               \
    free(block);                                     \
    (block) = NULL;                                  \
  } while(0)

#define SCM_MEM_HEAP_BLOCK_NEXT(block) ((block)->next)
#define SCM_MEM_HEAP_BLOCK_PREV(block) ((block)->prev)
#define SCM_MEM_HEAP_BLOCK_SIZE(block) ((block)->size)
#define SCM_MEM_HEAP_BLOCK_USED(block) ((block)->used)
#define SCM_MEM_HEAP_BLOCK_FREE(block) ((block)->size - (block)->used)
#define SCM_MEM_HEAP_BLOCK_HEAD(block) ((block)->heap)
#define SCM_MEM_HEAP_BLOCK_ALLOCATED(block, sz) ((block)->used += (sz))
#define SCM_MEM_HEAP_BLOCK_DEALLOCATED(block, sz) ((block)->used -= (sz))
#define SCM_MEM_HEAP_BLOCK_FREE_PTR(block) \
  ((void *)((block)->heap + (block)->used))
#define SCM_MEM_HEAP_BLOCK_PTR_OFFSET(block, ptr) \
  ((size_t)((uint8_t *)(ptr) - block->heap))
#define SCM_MEM_HEAP_BLOCK_PTR_IS_ALLOCATED(block, ptr) \
  (SCM_MEM_HEAP_BLOCK_PTR_OFFSET(block, ptr) < SCM_MEM_HEAP_BLOCK_USED(block))
#define SCM_MEM_HEAP_BLOCK_NEXT_OBJ(block, obj) \
  SCM_OBJ((uint8_t *)obj + SCM_TYPE_INFO_OBJ_SIZE_FROM_OBJ(obj))
#define SCM_MEM_HEAP_BLOCK_FOR_EACH_OBJ(block, obj)                     \
  for ((obj) = SCM_OBJ(SCM_MEM_HEAP_BLOCK_HEAD(block));                 \
       SCM_MEM_HEAP_BLOCK_PTR_IS_ALLOCATED(block, obj);                 \
       obj = SCM_MEM_HEAP_BLOCK_NEXT_OBJ(block, obj))
#define SCM_MEM_HEAP_BLOCK_CLEAN(block) ((block)->used = 0)
#define SCM_MEM_HEAP_BLOCK_IS_OBJ_IN_BLOCK(block, obj) \
  ((block)->heap <= (uint8_t *)obj \
   && (uint8_t *)obj < (block)->heap + (block)->used)

struct ScmMemHeapRec {
  ScmMemHeapBlock *head;
  ScmMemHeapBlock *tail;
  ScmMemHeapBlock *current;
  void *free;
  int nr_block;
  int nr_free_block;
};

#define SCM_MEM_HEAP_CUR_BLOCK_FREE_SIZE(heap) \
  (((heap)->current == NULL) ? 0 : SCM_MEM_HEAP_BLOCK_FREE((heap)->current))
#define SCM_MEM_HEAP_IS_CUR_BLOCK_TAIL(heap)  ((heap)->current == (heap)->tail)
#define SCM_MEM_HEAP_NR_BLOCK(heap) ((heap)->nr_block)
#define SCM_MEM_HEAP_NR_FREE_BLOCK(heap) ((heap)->nr_free_block)
#define SCM_MEM_HEAP_NR_USED_BLOCK(heap) \
  ((heap)->nr_block - (heap)->nr_free_block)


#define SCM_MEM_HEAP_ADD_BLOCK(heap, block)                             \
  do {                                                                  \
    if ((heap)->head == NULL)                                           \
      (heap)->head = (block);                                           \
    else                                                                \
      (heap)->tail->next = (block);                                     \
    (block)->next = NULL;                                               \
    (block)->prev = (heap)->tail;                                       \
    (heap)->tail = (block);                                             \
    (heap)->nr_block++;                                                 \
    if ((heap)->current == NULL) {                                      \
      (heap)->current = (block);                                        \
      (heap)->free = SCM_MEM_HEAP_BLOCK_FREE_PTR((heap)->current);      \
    }                                                                   \
    else                                                                \
      (heap)->nr_free_block++;                                          \
  } while(0)

#define SCM_MEM_HEAP_DEL_BLOCK(heap)                                    \
  do {                                                                  \
    if ((heap)->tail != NULL) {                                         \
      ScmMemHeapBlock *b = (heap)->tail;                                \
                                                                        \
      if (SCM_MEM_HEAP_IS_CUR_BLOCK_TAIL(heap)) {                       \
        (heap)->current = SCM_MEM_HEAP_BLOCK_PREV(b);                   \
        (heap)->free = SCM_MEM_HEAP_BLOCK_FREE_PTR((heap)->current);    \
      }                                                                 \
      else                                                              \
        (heap)->nr_free_block--;                                        \
                                                                        \
      if (SCM_MEM_HEAP_BLOCK_PREV(b) == NULL) {                         \
        (heap)->head = NULL;                                            \
        (heap)->tail = NULL;                                            \
      }                                                                 \
      else {                                                            \
        (heap)->tail = SCM_MEM_HEAP_BLOCK_PREV(b);                      \
        (heap)->tail->next = NULL;                                      \
      }                                                                 \
      (heap)->nr_block--;                                               \
                                                                        \
      SCM_MEM_HEAP_DELEATE_BLOCK(b);                                    \
    }                                                                   \
  } while(0)                                     

#define SCM_MEM_HEAP_RELEASE_BLOCKS(heap, nr_leave)             \
  do {                                                          \
    int i, n = SCM_MEM_HEAP_NR_BLOCK(heap) - (nr_leave);        \
    for (i = 0; i < n; i++)                                     \
      SCM_MEM_HEAP_DEL_BLOCK(heap);                             \
  } while(0)

#define SCM_MEM_HEAP_DELETE_HEAP(heap)          \
  do {                                          \
    SCM_MEM_HEAP_RELEASE_BLOCKS(heap, 0);       \
    free(heap);                                 \
    (heap) = NULL;                              \
  } while(0)

#define SCM_MEM_HEAP_NEW_HEAP(heap, nr_blk, sz) \
  do {                                              \
    int i;                                          \
                                                    \
    (heap) = malloc(sizeof(*(heap)));               \
    if ((heap) != NULL) {                           \
      (heap)->head = NULL;                          \
      (heap)->tail = NULL;                          \
      (heap)->current = NULL;                       \
      (heap)->free = NULL;                          \
      (heap)->nr_block = 0;                         \
      (heap)->nr_free_block = 0;                    \
                                                    \
      for (i = 0; i < (nr_blk); i++) {              \
        ScmMemHeapBlock *block;                     \
        SCM_MEM_HEAP_NEW_BLOCK(block, sz);          \
        if ((block) == NULL) {                      \
          SCM_MEM_HEAP_DELETE_HEAP(heap);           \
          break;                                    \
        }                                           \
        SCM_MEM_HEAP_ADD_BLOCK(heap, block);        \
      }                                             \
                                                    \
      if ((heap) != NULL)                           \
        (heap)->current = (heap)->head;             \
                                                    \
    }                                               \
  } while(0)

#define SCM_MEM_HEAP_SHIFT(heap)                                        \
  do {                                                                  \
    if ((heap)->current != NULL) {                                      \
      (heap)->current = SCM_MEM_HEAP_BLOCK_NEXT((heap)->current);       \
      if ((heap)->current == NULL) {                                    \
        (heap)->free = NULL;                                            \
      }                                                                 \
      else {                                                            \
        (heap)->free = SCM_MEM_HEAP_BLOCK_FREE_PTR((heap)->current);    \
        (heap)->nr_free_block--;                                        \
      }                                                                 \
    }                                                                   \
  } while(0)

#define SCM_MEM_HEAP_REWIND(heap)                                       \
  do {                                                                  \
    (heap)->current = (heap)->head;                                     \
    (heap)->free = SCM_MEM_HEAP_BLOCK_FREE_PTR((heap)->current);        \
    if ((heap)->nr_block > 1)                                           \
      (heap)->nr_free_block = (heap)->nr_block - 1;                     \
    else                                                                \
      (heap)->nr_free_block = 0;                                        \
  } while(0)

#define SCM_MEM_HEAP_ALLOC(heap, size, ptr)                             \
  do {                                                                  \
    *(ptr) = NULL;                                                      \
    while ((heap)->current != NULL && *(ptr) == NULL) {                 \
      if ((size) <= SCM_MEM_HEAP_CUR_BLOCK_FREE_SIZE(heap)) {           \
        *(ptr) = (heap)->free;                                          \
        SCM_MEM_HEAP_BLOCK_ALLOCATED((heap)->current, (size));          \
        (heap)->free = SCM_MEM_HEAP_BLOCK_FREE_PTR((heap)->current);    \
      }                                                                 \
      else {                                                            \
        SCM_MEM_HEAP_SHIFT(heap);                                       \
      }                                                                 \
    }                                                                   \
  } while(0)

#define SCM_MEM_HEAP_CANCEL_ALLOC(heap, size)                           \
  do {                                                                  \
    SCM_MEM_HEAP_BLOCK_DEALLOCATED((heap)->current, (size));            \
    (heap)->free = SCM_MEM_HEAP_BLOCK_FREE_PTR((heap)->current);        \
    if ((heap)->free == SCM_MEM_HEAP_BLOCK_HEAD((heap)->current)        \
        && (heap)->current != (heap)->head) {                           \
      (heap)->current = SCM_MEM_HEAP_BLOCK_PREV((heap)->current);       \
      (heap)->free = SCM_MEM_HEAP_BLOCK_FREE_PTR((heap)->current);      \
      (heap)->nr_free_block++;                                          \
    }                                                                   \
  } while(0)

#define SCM_MEM_HEAP_FOR_EACH_BLOCK(heap, block) \
  for ((block) = heap->head;                     \
       (block) != NULL;                          \
       (block) = SCM_MEM_HEAP_BLOCK_NEXT(block))


struct ScmMemRec {
  ScmBasicHashTable *to_obj_tbl;
  ScmBasicHashTable *from_obj_tbl;
  ScmMemHeap *to_heap;
  ScmMemHeap *from_heap;
  ScmMemHeap *persistent;
  ScmObj **extra_root_set;
  int nr_extra_root;
};


extern const ScmTypeInfo SCM_FORWARD_TYPE_INFO;

void *scm_memory_allocate(size_t size);
void *scm_memory_release(void *block);

ScmMem *scm_mem_construct(void);
ScmMem *scm_mem_destruct(ScmMem *mem);
ScmMem *scm_mem_alloc(ScmMem *mem, SCM_OBJ_TYPE_T type, ScmObj *box);
void scm_mem_gc_start(ScmMem *mem);
ScmMem *scm_mem_alloc_persist(ScmMem *mem, SCM_OBJ_TYPE_T type, ScmObj *box);

#endif /* INCLUDED_MEMORY_H__ */
