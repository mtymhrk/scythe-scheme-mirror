#include <cutter.h>

#include "object.h"
#include "nil.h"
#include "char.h"

void
test_scm_char_construct(void)
{
  scm_char_t c;
  ScmChar *charv;

  SCM_CHR_SET_ASCII(c, 'a');
  charv = scm_char_construct(c, SCM_ENCODING_ASCII);

  cut_assert_not_null(charv);
  cut_assert(SCM_OBJ_IS_TYPE(SCM_OBJ(charv), &SCM_CHAR_TYPE_INFO));
}

void
test_scm_char_value_a(void)
{
  scm_char_t c;
  ScmChar *charv;

  SCM_CHR_SET_ASCII(c, 'a');
  charv = scm_char_construct(c, SCM_ENCODING_ASCII);

  cut_assert_not_null(charv);
  cut_assert_equal_int('a',
                       scm_char_value(charv).ascii);

}

void
test_scm_char_is_char(void)
{
  scm_char_t c;
  ScmChar *charv;

  SCM_CHR_SET_ASCII(c, 'a');
  charv = scm_char_construct(c, SCM_ENCODING_ASCII);

  cut_assert_true(scm_char_is_char(SCM_OBJ(charv)));
}

void
test_scm_char_is_char_not_char(void)
{
  ScmObj nil = SCM_OBJ(scm_nil_instance());

  cut_assert_false(scm_char_is_char(nil));
}
