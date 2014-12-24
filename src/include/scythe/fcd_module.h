#ifndef INCLUDE_FCD_MODULE_H__
#define INCLUDE_FCD_MODULE_H__

#include <stdbool.h>

#include <scythe/object.h>

bool scm_fcd_gloc_p(ScmObj obj);
ScmObj scm_fcd_gloc_value(ScmObj gloc);
ScmObj scm_fcd_gloc_symbol(ScmObj gloc);
void scm_fcd_gloc_bind(ScmObj gloc, ScmObj val);
bool scm_fcd_module_p(ScmObj obj);
bool scm_fcd_module_name_p(ScmObj obj);
bool scm_fcd_module_specifier_p(ScmObj obj);
ScmObj scm_fcd_make_module(ScmObj name);
int scm_fcd_find_module(ScmObj name, scm_csetter_t *mod);
ScmObj scm_fcd_module_name(ScmObj module);
int scm_fcd_import(ScmObj module, ScmObj imported, bool restrictive);
ScmObj scm_fcd_get_gloc(ScmObj module, ScmObj sym);
int scm_fcd_find_gloc(ScmObj module, ScmObj sym, scm_csetter_t *gloc);
int scm_fcd_define_global_var(ScmObj module, ScmObj sym, ScmObj val,
                              bool export);
int scm_fcd_define_global_syx(ScmObj module, ScmObj sym, ScmObj syx,
                              bool export);
int scm_fcd_global_var_ref(ScmObj module, ScmObj sym, scm_csetter_t *val);
int scm_fcd_global_syx_ref(ScmObj module, ScmObj sym, scm_csetter_t *syx);


#endif /* INCLUDE_FCD_MODULE_H__ */