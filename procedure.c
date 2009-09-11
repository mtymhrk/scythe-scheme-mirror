#include "procedure.h"
#include "object.h"
#include "vm.h"
#include "obuffer.h"

ScmPrimProc *SCM_PRIM_PROC_CONS;

ScmTypeInfo SCM_PRIM_PROC_TYPE_INFO = {
  scm_prim_proc_pretty_print,   /* pp_func              */
  sizeof(ScmPrimProc),          /* obj_size             */
  NULL,                         /* gc_ini_func          */
  NULL,                         /* gc_fin_func          */
  NULL,                         /* gc_accept_func       */
  NULL,                         /* gc_accpet_func_weak  */
};

void
scm_prim_proc_cons_init(void)
{
  
}

void
scm_prim_proc_pretty_print(ScmObj obj, ScmOBuffer *obuffer)
{
  /* TODO: write me */
  return;
}
