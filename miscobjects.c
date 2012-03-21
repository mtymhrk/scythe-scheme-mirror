#include <stdbool.h>
#include <assert.h>

#include "object.h"
#include "reference.h"
#include "api.h"
#include "miscobjects.h"


/*******************************************************/
/*  ScmEOF                                             */
/*******************************************************/

ScmTypeInfo SCM_EOF_TYPE_INFO = {
  .pp_func             = scm_eof_pretty_print,
  .obj_size            = sizeof(ScmEOF),
  .gc_ini_func         = NULL,
  .gc_fin_func         = NULL,
  .gc_accept_func      = NULL,
  .gc_accept_func_weak = NULL,
};

void
scm_eof_initialize(ScmObj eof)
{
  return;                       /* nothing to do */
}

void
scm_eof_finalize(ScmObj eof)
{
  return;                       /* nothing to do */
}

ScmObj
scm_eof_new(SCM_MEM_TYPE_T mtype)         /* GC OK */
{
  ScmObj eof;

  eof = scm_capi_mem_alloc(&SCM_EOF_TYPE_INFO, mtype);
  if (scm_obj_null_p(eof)) return SCM_OBJ_NULL; /* [ERR]: [through] */

  scm_eof_initialize(eof);

  return eof;
}

int
scm_eof_pretty_print(ScmObj obj, ScmObj port, bool write_p)
{
  ScmObj str = SCM_OBJ_INIT;

  SCM_STACK_FRAME_PUSH(&obj, &port, &str);

  scm_assert_obj_type(obj, &SCM_EOF_TYPE_INFO);

  str = scm_capi_make_string_from_cstr("#<eof>", SCM_ENC_ASCII);
  if (scm_obj_null_p(str)) return -1;

  port = scm_api_write_string(port, str);
  if (scm_obj_null_p(port)) return -1;

  return 0;
}


/*******************************************************/
/*  ScmBool                                            */
/*******************************************************/

ScmTypeInfo SCM_BOOL_TYPE_INFO = {
  .pp_func             = scm_bool_pretty_print,
  .obj_size            = sizeof(ScmBool),
  .gc_ini_func         = NULL,
  .gc_fin_func         = NULL,
  .gc_accept_func      = NULL,
  .gc_accept_func_weak = NULL,
};

void
scm_bool_initialize(ScmObj obj, bool value) /* GC OK */
{
  scm_assert_obj_type(obj, &SCM_BOOL_TYPE_INFO);

  SCM_BOOL_VALUE(obj) = value;
}

void
scm_bool_finalize(ScmObj obj)   /* GC OK */
{
  return;                       /* nothing to do */
}

ScmObj
scm_bool_new(SCM_MEM_TYPE_T mtype, bool value)  /* GC OK */
{
  ScmObj bl = SCM_OBJ_INIT;;

  SCM_STACK_FRAME_PUSH(&bl);

  bl = scm_capi_mem_alloc(&SCM_BOOL_TYPE_INFO, mtype);
  if (scm_obj_null_p(bl)) return SCM_OBJ_NULL; /* [ERR]: [through] */

  scm_bool_initialize(bl, value);

  return bl;
}

bool
scm_bool_value(ScmObj bl)       /* GC OK */
{
  scm_assert_obj_type(bl, &SCM_BOOL_TYPE_INFO);

  return SCM_BOOL_VALUE(bl);
}

int
scm_bool_pretty_print(ScmObj obj, ScmObj port, bool write_p)
{
  ScmObj str = SCM_OBJ_INIT;

  SCM_STACK_FRAME_PUSH(&obj, &port, &str);

  scm_assert_obj_type(obj, &SCM_BOOL_TYPE_INFO);

  if (SCM_BOOL(obj)->value == true)
    str = scm_capi_make_string_from_cstr("#t", SCM_ENC_ASCII);
  else
    str = scm_capi_make_string_from_cstr("#f", SCM_ENC_ASCII);

  port = scm_api_write_string(port, str);
  if (scm_obj_null_p(port)) return -1;

  return 0;
}


/*******************************************************/
/*  ScmNil                                             */
/*******************************************************/

ScmTypeInfo SCM_NIL_TYPE_INFO = {
  .pp_func             = scm_nil_pretty_print,
  .obj_size            = sizeof(ScmNil),
  .gc_ini_func         = NULL,
  .gc_fin_func         = NULL,
  .gc_accept_func      = NULL,
  .gc_accept_func_weak = NULL,
};

void
scm_nil_initialize(ScmObj nil)  /* GC OK */
{
  return;                       /* nothing to do */
}

void
scm_nil_finalize(ScmObj nil)    /* GC OK */
{
  return;                       /* nothing to do */
}

ScmObj
scm_nil_new(SCM_MEM_TYPE_T mtype)         /* GC OK */
{
  ScmObj nil = SCM_OBJ_INIT;

  SCM_STACK_FRAME_PUSH(&nil);

  nil = scm_capi_mem_alloc(&SCM_NIL_TYPE_INFO, mtype);
  if (scm_obj_null_p(nil)) return SCM_OBJ_NULL; /* [ERR]: [through] */

  scm_nil_initialize(nil);

  return nil;
}

int
scm_nil_pretty_print(ScmObj obj, ScmObj port, bool write_p)
{
  ScmObj str = SCM_OBJ_INIT;

  SCM_STACK_FRAME_PUSH(&obj, &port, &str);

  scm_assert_obj_type(obj, &SCM_NIL_TYPE_INFO);

  str = scm_capi_make_string_from_cstr("()", SCM_ENC_ASCII);
  if (scm_obj_null_p(str)) return -1;

  port = scm_api_write_string(port, str);
  if (scm_obj_null_p(port)) return -1;

  return 0;
}
