#ifndef INCLUDE_FCD_SYMBOL_H__
#define INCLUDE_FCD_SYMBOL_H__

#include <stdbool.h>

#include "scythe/object.h"
#include "scythe/encoding.h"

bool scm_fcd_symbol_p(ScmObj obj);
ScmObj scm_fcd_symbol_P(ScmObj obj);
bool scm_fcd_symbol_eq_p(ScmObj sym1, ScmObj sym2);
ScmObj scm_fcd_symbol_eq_P(ScmObj sym1, ScmObj sym2);
ScmObj scm_fcd_symbol_eq_P_lst(ScmObj lst);
ScmObj scm_fcd_symbol_to_string(ScmObj sym);
ScmObj scm_fcd_string_to_symbol(ScmObj str);
ScmObj scm_fcd_make_symbol_from_cstr(const char *str, ScmEncoding *enc);
ScmObj scm_fcd_make_symbol_from_bin(const void *data, size_t size,
                                    ScmEncoding *enc);
size_t scm_fcd_symbol_bytesize(ScmObj sym);
char *scm_fcd_symbol_to_cstr(ScmObj sym, char *cstr, size_t size);
size_t scm_fcd_symbol_hash_value(ScmObj sym);

#endif /* INCLUDE_FCD_SYMBOL_H__ */