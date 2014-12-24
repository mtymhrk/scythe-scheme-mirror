#ifndef INCLUDE_FCD_PROCEDURE_H__
#define INCLUDE_FCD_PROCEDURE_H__

#include <stdbool.h>

#include "scythe/object.h"
#include "scythe/fcd_type.h"

/*******************************************************************/
/*  Procedure                                                      */
/*******************************************************************/

bool scm_fcd_procedure_p(ScmObj proc);
ScmObj scm_fcd_procedure_P(ScmObj proc);
int scm_fcd_arity(ScmObj proc);
bool scm_fcd_procedure_flg_set_p(ScmObj proc, SCM_PROC_FLG_T flg);


/*******************************************************************/
/*  Subrutine                                                      */
/*******************************************************************/

bool scm_fcd_subrutine_p(ScmObj obj);
ScmObj scm_fcd_make_subrutine(ScmSubrFunc func, int arity, unsigned int flags,
                              ScmObj module);
int scm_fcd_call_subrutine(ScmObj subr, int argc, const ScmObj *argv);
ScmObj scm_fcd_subrutine_module(ScmObj subr);


/*******************************************************************/
/*  Closure                                                        */
/*******************************************************************/

bool scm_fcd_closure_p(ScmObj obj);
ScmObj scm_fcd_make_closure(ScmObj iseq, ScmObj env, int arity);
ScmObj scm_fcd_closure_to_iseq(ScmObj clsr);
scm_byte_t *scm_fcd_closure_to_ip(ScmObj clsr);
ScmObj scm_fcd_closure_env(ScmObj clsr);


/*******************************************************************/
/*  Parameter                                                      */
/*******************************************************************/

bool scm_fcd_parameter_p(ScmObj obj);
ScmObj scm_fcd_make_parameter(ScmObj conv);
ScmObj scm_fcd_parameter_init_val(ScmObj prm);
ScmObj scm_fcd_parameter_converter(ScmObj prm);
void scm_fcd_parameter_set_init_val(ScmObj prm, ScmObj val);
ScmObj scm_fcd_parameter_value(ScmObj prm);


/*******************************************************************/
/*  Continuation                                                   */
/*******************************************************************/

bool scm_fcd_continuation_p(ScmObj obj);
ScmObj scm_fcd_capture_cont(void);
ScmObj scm_fcd_cont_capture_obj(ScmObj cont);


#endif /* INCLUDE_FCD_PROCEDURE_H__ */