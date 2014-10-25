#ifndef INCLUDE_SYNTAX_H__
#define INCLUDE_SYNTAX_H__

#include <stdbool.h>

typedef struct ScmSyntaxRec ScmSyntax;

#define SCM_SYNTAX(obj) ((ScmSyntax *)(obj))

#include "scythe/object.h"
#include "scythe/api_type.h"

#define SCM_SYNTAX_KEYWORD_LEN_MAX 64

struct ScmSyntaxRec {
  ScmObjHeader header;
  ScmObj keyword;
  ScmObj handler;
};

extern ScmTypeInfo SCM_SYNTAX_TYPE_INFO;

int scm_syntax_initialize(ScmObj syx, ScmObj key, ScmObj handler);
ScmObj scm_syntax_new(SCM_MEM_TYPE_T mtype, ScmObj key, ScmObj handler);

int scm_syntax_obj_print(ScmObj obj, ScmObj port, bool ext_rep);
void scm_syntax_gc_initialize(ScmObj obj, ScmObj mem);
int scm_syntax_gc_accept(ScmObj obj, ScmObj mem, ScmGCRefHandlerFunc handler);

inline ScmObj
scm_syntax_keyword(ScmObj syx)
{
  scm_assert_obj_type(syx, &SCM_SYNTAX_TYPE_INFO);

  return SCM_SYNTAX(syx)->keyword;
}

inline ScmObj
scm_syntax_handler(ScmObj syx)
{
  scm_assert_obj_type(syx, &SCM_SYNTAX_TYPE_INFO);

  return SCM_SYNTAX(syx)->handler;
}

#endif /* INCLUDE_SYNTAX_H__ */