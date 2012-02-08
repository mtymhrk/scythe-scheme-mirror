#include <cutter.h>

#include "object.h"
#include "vm.h"
#include "miscobjects.h"
#include "char.h"

static ScmObj vm = SCM_OBJ_INIT;

void
cut_startup(void)
{
  SCM_SETQ_PRIM(vm, scm_vm_new());
}

void
cut_shutdown(void)
{
  scm_vm_end(vm);
}

void
test_scm_char_new(void)
{
  ScmObj chr = SCM_OBJ_INIT;
  scm_char_t c;

  SCM_CHR_SET_ASCII(c, 'a');
  SCM_SETQ(chr, scm_char_new(SCM_MEM_ALLOC_HEAP, c, SCM_ENCODING_ASCII));

  cut_assert_true(SCM_OBJ_NOT_NULL_P(chr));
  cut_assert(SCM_OBJ_IS_TYPE(SCM_OBJ(chr), &SCM_CHAR_TYPE_INFO));
}

void
test_scm_char_value_a(void)
{
  ScmObj chr = SCM_OBJ_INIT;
  scm_char_t c;

  SCM_CHR_SET_ASCII(c, 'a');
  SCM_SETQ(chr, scm_char_new(SCM_MEM_ALLOC_HEAP, c, SCM_ENCODING_ASCII));

  cut_assert_true(SCM_OBJ_NOT_NULL_P(chr));
  cut_assert_equal_int('a',
                       scm_char_value(chr).ascii);

}

void
test_scm_char_is_char(void)
{
  ScmObj chr = SCM_OBJ_INIT;
  scm_char_t c;

  SCM_CHR_SET_ASCII(c, 'a');
  SCM_SETQ(chr, scm_char_new(SCM_MEM_ALLOC_HEAP, c, SCM_ENCODING_ASCII));

  cut_assert_true(scm_char_is_char(chr));
}

void
test_scm_char_is_char_not_char(void)
{
  ScmObj nil = scm_nil_instance();

  cut_assert_false(scm_char_is_char(nil));
}
