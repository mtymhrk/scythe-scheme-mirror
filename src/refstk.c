#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

#include "scythe/object.h"
#include "scythe/refstk.h"

ScmTypeInfo SCM_REFSTACK_TYPE_INFO = {
  .name                            = "refstack",
  .flags                           = SCM_TYPE_FLG_MMO,
  .obj_print_func                  = NULL,
  .obj_size                        = sizeof(ScmRefStack),
  .gc_ini_func                     = scm_ref_stack_gc_initialize,
  .gc_fin_func                     = NULL,
  .gc_accept_func                  = scm_ref_stack_gc_accept,
  .gc_accept_func_weak             = NULL,
  .extra                           = NULL,
};

int
scm_ref_stack_initialize(ScmObj stack)
{
  scm_assert_obj_type(stack, &SCM_REFSTACK_TYPE_INFO);

  SCM_REFSTACK(stack)->stack = NULL;
  return 0;
}

ScmObj
scm_ref_stack_new(scm_mem_type_t mtype)
{
  ScmObj stack = SCM_OBJ_INIT;

  stack = scm_alloc_mem(&SCM_REFSTACK_TYPE_INFO, 0, mtype);
  if (scm_obj_null_p(stack)) return SCM_OBJ_NULL;

  if (scm_ref_stack_initialize(stack) < 0)
    return SCM_OBJ_NULL;

  return stack;
}


void
scm_ref_stack_gc_initialize(ScmObj obj)
{
  scm_assert_obj_type(obj, &SCM_REFSTACK_TYPE_INFO);

  SCM_REFSTACK(obj)->stack = NULL;
}

int
scm_ref_stack_gc_accept(ScmObj obj, ScmGCRefHandler handler)
{
  ScmRefStackBlock *block;
  int rslt = SCM_GC_REF_HANDLER_VAL_INIT;

  for (block = SCM_REFSTACK(obj)->stack; block != NULL; block = block->next) {
    switch (block->type) {
    case SCM_REFSTACK_RARY:
      for (ScmObj **p = block->ref.rary; *p != NULL; p++) {
        rslt = SCM_GC_CALL_REF_HANDLER(handler, obj, SCM_REF_DEREF(*p));
        if (scm_gc_ref_handler_failure_p(rslt))
          return rslt;
      }
      break;
    case SCM_REFSTACK_ARY:
      for (size_t i = 0; i < block->ref.ary.n; i++) {
        rslt = SCM_GC_CALL_REF_HANDLER(handler, obj, block->ref.ary.head[i]);
        if (scm_gc_ref_handler_failure_p(rslt))
          return rslt;
      }
      break;
    default:
      scm_assert(false);        /* must not happen */
      break;
    }
  }

  return rslt;
}


/***************************************************************************/
/*  Facade                                                                 */
/***************************************************************************/

ScmObj scm__current_ref_stack;
