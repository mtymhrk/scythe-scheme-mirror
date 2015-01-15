#ifndef INCLUDE_FCD_MISCOBJECTS_H__
#define FCD_MISCOBJECTS_H

#include <stdbool.h>

#include "scythe/object.h"
#include "scythe/fcd_type.h"


/*******************************************************/
/*  Nil (empty list)                                   */
/*******************************************************/

ScmObj scm_fcd_nil_new(SCM_MEM_TYPE_T mtype);
ScmObj scm_fcd_nil(void);
bool scm_fcd_nil_p(ScmObj obj);
ScmObj scm_fcd_nil_P(ScmObj obj);

#define SCM_NIL_OBJ scm_fcd_nil()


/*******************************************************/
/*  Booleans                                           */
/*******************************************************/

bool scm_fcd_boolean_p(ScmObj obj);
ScmObj scm_fcd_boolean_P(ScmObj obj);
ScmObj scm_fcd_bool_new(SCM_MEM_TYPE_T mtype, bool value);
ScmObj scm_fcd_true(void);
ScmObj scm_fcd_false(void);
bool scm_fcd_true_object_p(ScmObj obj);
bool scm_fcd_false_object_p(ScmObj obj);
bool scm_fcd_true_p(ScmObj obj);
bool scm_fcd_false_p(ScmObj obj);
ScmObj scm_fcd_not(ScmObj obj);

#define SCM_TRUE_OBJ scm_fcd_true()
#define SCM_FALSE_OBJ scm_fcd_false()


/*******************************************************/
/*  EOF                                                */
/*******************************************************/

ScmObj scm_fcd_eof_new(SCM_MEM_TYPE_T mtype);
ScmObj scm_fcd_eof(void);
bool scm_fcd_eof_object_p(ScmObj obj);
ScmObj scm_fcd_eof_object_P(ScmObj obj);

#define SCM_EOF_OBJ scm_fcd_eof()


/*******************************************************/
/*  Undef                                              */
/*******************************************************/

ScmObj scm_fcd_undef_new(SCM_MEM_TYPE_T mtype);
ScmObj scm_fcd_undef(void);
bool scm_fcd_undef_object_p(ScmObj obj);

#define SCM_UNDEF_OBJ scm_fcd_undef()


/*******************************************************/
/*  Landmine (uninitialized)                           */
/*******************************************************/

ScmObj scm_fcd_landmine_new(SCM_MEM_TYPE_T mtype);
ScmObj scm_fcd_landmine(void);
bool scm_fcd_landmine_object_p(ScmObj obj);

#define SCM_LANDMINE_OBJ scm_fcd_landmine()
#define SCM_UNINIT_OBJ scm_fcd_landmine()


/*******************************************************/
/*  Boxing                                             */
/*******************************************************/

typedef struct ScmBoxRec ScmBox;
#define SCM_BOX(obj) ((ScmBox *)(obj))

struct ScmBoxRec {
  ScmObjHeader header;
  ScmObj obj;
};

bool scm_fcd_box_object_p(ScmObj obj);
ScmObj scm_fcd_box_new(SCM_MEM_TYPE_T mtype, ScmObj obj);

static inline ScmObj
scm_fcd_box_unbox(ScmObj box)
{
  scm_assert(scm_fcd_box_object_p(box));
  return SCM_BOX(box)->obj;
}

static inline void
scm_fcd_box_update(ScmObj box, ScmObj obj)
{
  scm_assert(scm_fcd_box_object_p(box));
  scm_assert(scm_obj_not_null_p(obj));

  SCM_SLOT_SETQ(ScmBox, box, obj, obj);
}


#endif /* INCLUDE_FCD_MISCOBJECTS_H__ */
