#ifndef INCLUDE_REFERENCE_H__
#define INCLUDE_REFERENCE_H__

#include <stdint.h>
#include <stdarg.h>

typedef struct ScmRefStackBlockRec ScmRefStackBlock;
typedef struct ScmRefStackInfoRec ScmRefStackInfo;
typedef struct ScmRefStackRec ScmRefStack;


#include "object.h"
#include "impl_utils.h"

struct ScmRefStackBlockRec {
  ScmRefStackBlock *next;
  ScmRefStackBlock *prev;
  size_t size;
  ScmRef *sp;
  ScmRef stack[0];
};

struct ScmRefStackRec {
  ScmRefStackBlock *head;
  ScmRefStackBlock *tail;
  ScmRefStackBlock *current;
};

struct ScmRefStackInfoRec {
  ScmRefStackBlock *current;
  ScmRef *sp;
};

/* private functions */

#ifdef SCM_UNIT_TEST

bool scm_ref_stack_block_full_p(ScmRefStackBlock *block);
ScmRefStackBlock *scm_ref_stack_new_block(size_t sz);
void scm_ref_stack_block_push(ScmRefStackBlock *block, ScmRef ref);
void scm_ref_stack_add_block(ScmRefStack *stack, ScmRefStackBlock *block);
void scm_ref_stack_decrease_block(ScmRefStack *stack);
void scm_ref_stack_shift_stack_block(ScmRefStack *stack);
ScmRefStack *scm_ref_stack_add_new_block(ScmRefStack *stack, size_t size);
ScmRefStack *scm_ref_stack_growth_if_needed(ScmRefStack *stack);
void scm_ref_stack_decrease_if_possible(ScmRefStack *stack);

#endif

/* public functions */
ScmRefStack *scm_ref_stack_initialize(ScmRefStack *stack, size_t size);
void scm_ref_stack_finalize(ScmRefStack *stack);
ScmRefStack *scm_ref_stack_new(size_t size);
void scm_ref_stack_end(ScmRefStack *stack);
ScmRefStack *scm_ref_stack_push_va(ScmRefStack *stack, va_list ap);
ScmRefStack *scm_ref_stack_push(ScmRefStack *stack, ...);
ScmRef scm_ref_stack_alloc(ScmRefStack *stack, ScmObj init);
void scm_ref_stack_save(ScmRefStack *stack, ScmRefStackInfo *info);
void scm_ref_stack_restore(ScmRefStack *stack, ScmRefStackInfo *info);
void scm_ref_stack_push_current_stack(int dummy, ...);
void scm_ref_stack_save_current_stack(ScmRefStackInfo *info);
void scm_ref_stack_restore_current_stack(ScmRefStackInfo *info);
int scm_ref_stack_gc_accept(ScmRefStack *stack, ScmObj owner,
                            ScmObj mem, ScmGCRefHandlerFunc handler);



#define SCM_STACK_FRAME                         \
  __attribute__((__cleanup__(scm_ref_stack_restore_current_stack)))     \
  ScmRefStackInfo SCM_CONCAT_SYMBOL__(scm_ref_stack_frame__, __LINE__) \
  = { NULL, NULL };                                                     \
  scm_ref_stack_save_current_stack(&SCM_CONCAT_SYMBOL__(scm_ref_stack_frame__, __LINE__));

#define SCM_STACK_PUSH(...)                                             \
  scm_ref_stack_push_current_stack(0, __VA_ARGS__, NULL)

#define SCM_STACK_FRAME_PUSH(...) \
  SCM_STACK_FRAME; SCM_STACK_PUSH(__VA_ARGS__);

#endif /* INCLUDE_REFERENCE_H__ */
