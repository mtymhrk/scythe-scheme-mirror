#include "scythe/object.h"
#include "scythe/fcd.h"
#include "scythe/fixnum.h"
#include "scythe/bignum.h"

#include "test.h"

TEST_GROUP(fcd_bignum);

static ScmEvaluator *ev;
static ScmRefStackInfo rsi;

TEST_SETUP(fcd_bignum)
{
  ev = scm_capi_evaluator();
  scm_capi_evaluator_make_vm(ev);
  scm_fcd_ref_stack_save(&rsi);
}

TEST_TEAR_DOWN(fcd_bignum)
{
  scm_fcd_ref_stack_restore(&rsi);
  scm_capi_evaluator_end(ev);
}

static void
check_list_elements(ScmObj lst, bool (*check)(ScmObj elm))
{
  ScmObj l = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst,
                      &l);

  for (l = lst; scm_fcd_pair_p(l); l = scm_fcd_cdr(l))
    TEST_ASSERT_TRUE(check(scm_fcd_car(l)));
}

TEST(fcd_bignum, fcd_bignum_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);

  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));
}

TEST(fcd_bignum, fcd_bignum_p__return_false_1)
{
  TEST_ASSERT_FALSE(scm_fcd_bignum_p(SCM_EOF_OBJ));
}

TEST(fcd_bignum, fcd_bignum_p__return_false_2)
{
  TEST_ASSERT_FALSE(scm_fcd_bignum_p(SCM_OBJ_NULL));
}

TEST(fcd_bignum, fcd_bignum_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);

  TEST_ASSERT_SCM_TRUE(scm_fcd_bignum_P(bn));
}

TEST(fcd_bignum, fcd_bignum_P__return_false)
{
  TEST_ASSERT_SCM_FALSE(scm_fcd_bignum_P(SCM_EOF_OBJ));
}

TEST(fcd_bignum, fcd_number_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_TRUE(scm_fcd_number_p(bn));
}

TEST(fcd_bignum, fcd_number_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_fcd_number_P(bn));
}

TEST(fcd_bignum, fcd_complex_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_TRUE(scm_fcd_complex_p(bn));
}

TEST(fcd_bignum, fcd_complex_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_fcd_complex_P(bn));
}

TEST(fcd_bignum, fcd_real_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_TRUE(scm_fcd_real_p(bn));
}

TEST(fcd_bignum, fcd_real_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_fcd_real_P(bn));
}

TEST(fcd_bignum, fcd_rational_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_TRUE(scm_fcd_rational_p(bn));
}

TEST(fcd_bignum, fcd_rational_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_fcd_rational_P(bn));
}

TEST(fcd_bignum, fcd_integer_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_TRUE(scm_fcd_integer_p(bn));
}

TEST(fcd_bignum, fcd_integer_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_fcd_integer_P(bn));
}

TEST(fcd_bignum, fcd_exact_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_TRUE(scm_fcd_exact_p(bn));
}

TEST(fcd_bignum, fcd_exact_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_fcd_exact_P(bn));
}

TEST(fcd_bignum, fcd_inexact_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_FALSE(scm_fcd_inexact_p(bn));
}

TEST(fcd_bignum, fcd_inexact_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_FALSE(scm_fcd_inexact_P(bn));
}

TEST(fcd_bignum, fcd_exact_integer_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_TRUE(scm_fcd_exact_integer_p(bn));
}

TEST(fcd_bignum, fcd_exact_integer_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_fcd_exact_integer_P(bn));
}

TEST(fcd_bignum, fcd_finite_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_TRUE(scm_fcd_finite_p(bn));
}

TEST(fcd_bignum, fcd_finite_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_fcd_finite_P(bn));
}

TEST(fcd_bignum, fcd_infinite_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_FALSE(scm_fcd_infinite_p(bn));
}

TEST(fcd_bignum, fcd_infinite_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_FALSE(scm_fcd_infinite_P(bn));
}

TEST(fcd_bignum, fcd_nan_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_FALSE(scm_fcd_nan_p(bn));
}

TEST(fcd_bignum, fcd_nan_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_FALSE(scm_fcd_nan_P(bn));
}

TEST(fcd_bignum, fcd_num_eq__return_true)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_eq(bn1, bn2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_bignum, fcd_num_eq__return_false)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_eq(bn1, bn2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_bignum, fcd_num_eq__transitive)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT, bn3 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &bn3);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  bn3 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn3));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_eq(bn1, bn2, &actual));
  TEST_ASSERT_TRUE(actual);

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_eq(bn2, bn3, &actual));
  TEST_ASSERT_TRUE(actual);

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_eq(bn1, bn3, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_bignum, fcd_num_eq_P_lst__return_true)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = read_cstr("(4611686018427387904 4611686018427387904 4611686018427387904)");
  check_list_elements(lst, scm_fcd_bignum_p);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P_lst(lst));
}

TEST(fcd_bignum, fcd_num_eq_P__return_false)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = read_cstr("(4611686018427387904 4611686018427387904 4611686018427387905)");
  check_list_elements(lst, scm_fcd_bignum_p);

  TEST_ASSERT_SCM_FALSE(scm_fcd_num_eq_P_lst(lst));
}

TEST(fcd_bignum, fcd_num_lt__less)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 2);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_lt(bn1, bn2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_bignum, fcd_num_lt__equal)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_lt(bn1, bn2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_bignum, fcd_num_lt__greater)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 2);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_lt(bn1, bn2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_bignum, fcd_num_lt__transitive)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT, bn3 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &bn3);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  bn3 = scm_fcd_make_number_from_sword(SCM_SWORD_MAX);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn3));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_lt(bn1, bn2, &actual));
  TEST_ASSERT_TRUE(actual);

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_lt(bn2, bn3, &actual));
  TEST_ASSERT_TRUE(actual);

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_lt(bn1, bn3, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_bignum, fcd_num_lt_P_lst__ltss)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = read_cstr("(-4611686018427387905 4611686018427387904 9223372036854775807)");
  check_list_elements(lst, scm_fcd_bignum_p);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_lt_P_lst(lst));
}

TEST(fcd_bignum, fcd_num_lt_P_lst__equal)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = read_cstr("(-4611686018427387905 4611686018427387904 4611686018427387904)");
  check_list_elements(lst, scm_fcd_bignum_p);

  TEST_ASSERT_SCM_FALSE(scm_fcd_num_lt_P_lst(lst));
}

TEST(fcd_bignum, fcd_num_lt_P_lst__greater)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = read_cstr("(4611686018427387904 9223372036854775807 -4611686018427387905)");
  check_list_elements(lst, scm_fcd_bignum_p);

  TEST_ASSERT_SCM_FALSE(scm_fcd_num_lt_P_lst(lst));
}

TEST(fcd_bignum, fcd_num_gt__less)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 2);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_gt(bn1, bn2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_bignum, fcd_num_gt__equal)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_gt(bn1, bn2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_bignum, fcd_num_gt__greater)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 2);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_gt(bn1, bn2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_bignum, fcd_num_gt__transitive)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT, bn3 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &bn3);

  bn1 = scm_fcd_make_number_from_sword(SCM_SWORD_MAX);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  bn3 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn3));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_gt(bn1, bn2, &actual));
  TEST_ASSERT_TRUE(actual);

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_gt(bn2, bn3, &actual));
  TEST_ASSERT_TRUE(actual);

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_gt(bn1, bn3, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_bignum, fcd_num_gt_P_lst__less)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = read_cstr("(4611686018427387904 -4611686018427387905 9223372036854775807)");
  check_list_elements(lst, scm_fcd_bignum_p);

  TEST_ASSERT_SCM_FALSE(scm_fcd_num_gt_P_lst(lst));
}

TEST(fcd_bignum, fcd_num_gt_P_lst__equal)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = read_cstr("(9223372036854775807 4611686018427387904 4611686018427387904)");
  check_list_elements(lst, scm_fcd_bignum_p);

  TEST_ASSERT_SCM_FALSE(scm_fcd_num_gt_P_lst(lst));
}

TEST(fcd_bignum, fcd_num_gt_P_lst__greater)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = read_cstr("(9223372036854775807 4611686018427387904 -4611686018427387905)");
  check_list_elements(lst, scm_fcd_bignum_p);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_gt_P_lst(lst));
}

TEST(fcd_bignum, fcd_num_le__less)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 2);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_le(bn1, bn2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_bignum, fcd_num_le__equal)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_le(bn1, bn2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_bignum, fcd_num_le__greater)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 2);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_le(bn1, bn2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_bignum, fcd_num_le__transitive)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT, bn3 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &bn3);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  bn3 = scm_fcd_make_number_from_sword(SCM_SWORD_MAX);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn3));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_le(bn1, bn2, &actual));
  TEST_ASSERT_TRUE(actual);

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_le(bn2, bn3, &actual));
  TEST_ASSERT_TRUE(actual);

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_le(bn1, bn3, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_bignum, fcd_num_le_P_lst__less)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = read_cstr("(-4611686018427387905 4611686018427387904 9223372036854775807)");
  check_list_elements(lst, scm_fcd_bignum_p);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_le_P_lst(lst));
}

TEST(fcd_bignum, fcd_num_le_P_lst__equal)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = read_cstr("(-4611686018427387905 4611686018427387904 4611686018427387904)");
  check_list_elements(lst, scm_fcd_bignum_p);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_le_P_lst(lst));
}

TEST(fcd_bignum, fcd_num_le_P_lst__greater)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = read_cstr("(4611686018427387904 9223372036854775807 -4611686018427387905)");
  check_list_elements(lst, scm_fcd_bignum_p);

  TEST_ASSERT_SCM_FALSE(scm_fcd_num_le_P_lst(lst));
}

TEST(fcd_bignum, fcd_num_ge__less)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 2);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_ge(bn1, bn2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_bignum, fcd_num_ge__equal)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_ge(bn1, bn2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_bignum, fcd_num_ge__greater)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 2);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_ge(bn1, bn2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_bignum, fcd_num_ge__transitive)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT, bn3 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &bn3);

  bn1 = scm_fcd_make_number_from_sword(SCM_SWORD_MAX);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  bn3 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn3));

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_ge(bn1, bn2, &actual));
  TEST_ASSERT_TRUE(actual);

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_ge(bn2, bn3, &actual));
  TEST_ASSERT_TRUE(actual);

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_num_ge(bn1, bn3, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_bignum, fcd_num_ge_P_lst__less)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = read_cstr("(4611686018427387904 -4611686018427387905 9223372036854775807)");
  check_list_elements(lst, scm_fcd_bignum_p);

  TEST_ASSERT_SCM_FALSE(scm_fcd_num_ge_P_lst(lst));
}

TEST(fcd_bignum, fcd_num_ge_P_lst__equal)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = read_cstr("(9223372036854775807 4611686018427387904 4611686018427387904)");
  check_list_elements(lst, scm_fcd_bignum_p);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_ge_P_lst(lst));
}

TEST(fcd_bignum, fcd_num_ge_P_lst__greater)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = read_cstr("(9223372036854775807 4611686018427387904 -4611686018427387905)");
  check_list_elements(lst, scm_fcd_bignum_p);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_ge_P_lst(lst));
}

IGNORE_TEST(fcd_bignum, fcd_zero_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = read_cstr("0");
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_TRUE(scm_fcd_zero_p(bn));
}

IGNORE_TEST(fcd_bignum, fcd_zero_p__return_false)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = read_cstr("1");
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_FALSE(scm_fcd_zero_p(bn));
}

IGNORE_TEST(fcd_bignum, fcd_zero_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = read_cstr("0");
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_fcd_zero_P(bn));
}

IGNORE_TEST(fcd_bignum, fcd_zero_P__return_false)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = read_cstr("-1");
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_FALSE(scm_fcd_zero_P(bn));
}

TEST(fcd_bignum, fcd_positive_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_TRUE(scm_fcd_positive_p(bn));
}

TEST(fcd_bignum, fcd_positive_p__return_false)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_FALSE(scm_fcd_positive_p(bn));
}

TEST(fcd_bignum, fcd_positive_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_fcd_positive_P(bn));
}

TEST(fcd_bignum, fcd_positive_P__return_false)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_FALSE(scm_fcd_positive_P(bn));
}

TEST(fcd_bignum, fcd_negative_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_TRUE(scm_fcd_negative_p(bn));
}

TEST(fcd_bignum, fcd_negative_p__return_false)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_FALSE(scm_fcd_negative_p(bn));
}

TEST(fcd_bignum, fcd_negative_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_fcd_negative_P(bn));
}

TEST(fcd_bignum, fcd_negative_P__return_false)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_FALSE(scm_fcd_negative_P(bn));
}

TEST(fcd_bignum, fcd_odd_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 2);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_TRUE(scm_fcd_odd_p(bn));
}

TEST(fcd_bignum, fcd_odd_p__return_false)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_FALSE(scm_fcd_odd_p(bn));
}

TEST(fcd_bignum, fcd_odd_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_fcd_odd_P(bn));
}

TEST(fcd_bignum, fcd_odd_P__return_false)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 2);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_FALSE(scm_fcd_odd_P(bn));
}




TEST(fcd_bignum, fcd_even_p__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_TRUE(scm_fcd_even_p(bn));
}

TEST(fcd_bignum, fcd_even_p__return_false)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 2);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_FALSE(scm_fcd_even_p(bn));
}

TEST(fcd_bignum, fcd_even_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 2);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_fcd_even_P(bn));
}

TEST(fcd_bignum, fcd_even_P__return_false)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_SCM_FALSE(scm_fcd_even_P(bn));
}

TEST(fcd_bignum, fcd_max__first)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 2);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  actual = scm_fcd_max(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(bn1, actual));
}

TEST(fcd_bignum, fcd_max__second)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 2);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  actual = scm_fcd_max(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(bn2, actual));
}

TEST(fcd_bignum, fcd_max__same)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  actual = scm_fcd_max(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(bn1, actual));
}

TEST(fcd_bignum, fcd_max_lst)
{
  ScmObj lst = SCM_OBJ_INIT, expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst, &expected, &actual);

  lst = read_cstr("(4611686018427387904 -4611686018427387905 4611686018427388160 4611686022722355200 -4611686022722355200)");
  check_list_elements(lst, scm_fcd_bignum_p);
  expected = read_cstr("4611686022722355200");

  actual = scm_fcd_max_lst(lst);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_min__first)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 2);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  actual = scm_fcd_min(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(bn1, actual));
}

TEST(fcd_bignum, fcd_min__second)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 2);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  actual = scm_fcd_min(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(bn2, actual));
}

TEST(fcd_bignum, fcd_min__same)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  actual = scm_fcd_min(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(bn1, actual));
}

TEST(fcd_bignum, fcd_min_lst)
{
  ScmObj lst = SCM_OBJ_INIT, expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst, &expected, &actual);

  lst = read_cstr("(4611686018427387904 -4611686018427387905 4611686018427388160 4611686022722355200 -4611686022722355200)");
  check_list_elements(lst, scm_fcd_bignum_p);
  expected = read_cstr("-4611686022722355200");

  actual = scm_fcd_min_lst(lst);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_plus__1)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &expected, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 2);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  expected = read_cstr("9223372036854775809");

  actual = scm_fcd_plus(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_plus__2)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &expected, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  expected = read_cstr("-1");

  actual = scm_fcd_plus(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_plus__3)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &expected, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  expected = read_cstr("-1");

  actual = scm_fcd_plus(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_plus__4)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &expected, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 2);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  expected = read_cstr("-9223372036854775811");

  actual = scm_fcd_plus(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_plus_lst)
{
  ScmObj lst = SCM_OBJ_INIT, expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst, &expected, &actual);

  lst = read_cstr("(4611686018427387904 4611686022722355200 9223372036854775808)");
  check_list_elements(lst, scm_fcd_bignum_p);
  expected = read_cstr("18446744078004518912");

  actual = scm_fcd_plus_lst(lst);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_mul__1)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &expected, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 2);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  expected = read_cstr("21267647932558653971072598982912901120");

  actual = scm_fcd_mul(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_mul__2)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &expected, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  expected = read_cstr("-21267647932558653971072598982912901120");

  actual = scm_fcd_mul(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_mul__3)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &expected, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 2);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 2);;
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  expected = read_cstr("-21267647932558653980295971019767676930");

  actual = scm_fcd_mul(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_mul__4)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &expected, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 2);;
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  expected = read_cstr("21267647932558653980295971019767676930");

  actual = scm_fcd_mul(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_mul_lst)
{
  ScmObj lst = SCM_OBJ_INIT, expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst, &expected, &actual);

  lst = read_cstr("(4611686018427387904 4611686022722355200 9223372036854775808)");
  check_list_elements(lst, scm_fcd_bignum_p);
  expected = read_cstr("196159429413521478536231284250700179592733985096492646400");

  actual = scm_fcd_mul_lst(lst);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_minus__1)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &expected, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 2);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  expected = read_cstr("-1");

  actual = scm_fcd_minus(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_minus__2)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &expected, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 2);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  expected = read_cstr("-9223372036854775810");

  actual = scm_fcd_minus(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_minus__3)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &expected, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 2);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  expected = read_cstr("9223372036854775810");

  actual = scm_fcd_minus(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_minus__4)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &expected, &actual);

  bn1 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  bn2 = scm_fcd_make_number_from_sword(SCM_FIXNUM_MIN - 2);
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));
  expected = read_cstr("1");

  actual = scm_fcd_minus(bn1, bn2);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_minus_lst)
{
  ScmObj lst = SCM_OBJ_INIT, expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst, &expected, &actual);

  lst = read_cstr("(4611686018427387904 4611686022722355200 9223372036854775808)");
  check_list_elements(lst, scm_fcd_bignum_p);
  expected = read_cstr("-9223372041149743104");

  actual = scm_fcd_minus_lst(lst);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_abs__positive)
{
  ScmObj bn = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn, &actual);

  bn = read_cstr("4611686018427387904");
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  actual = scm_fcd_abs(bn);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(bn, actual));
}

TEST(fcd_bignum, fcd_abs__negative)
{
  ScmObj bn = SCM_OBJ_INIT, expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn, &expected, &actual);

  bn = read_cstr("-4611686018427387905");
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));
  expected = read_cstr("4611686018427387905");

  actual = scm_fcd_abs(bn);

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected, actual));
}

TEST(fcd_bignum, fcd_floor_div__1)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;
  ScmObj expected_quo = SCM_OBJ_INIT, expected_rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &quo, &rem,
                      &expected_quo, &expected_rem);

  bn1 = read_cstr("23058430092136939520");
  bn2 = read_cstr("9223372036854775808");
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  expected_quo = read_cstr("2");
  expected_rem = read_cstr("4611686018427387904");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_floor_div(bn1, bn2,
                                             SCM_CSETTER_L(quo),
                                             SCM_CSETTER_L(rem)));

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected_quo, quo));
  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected_rem, rem));
}

TEST(fcd_bignum, fcd_floor_div__2)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;
  ScmObj expected_quo = SCM_OBJ_INIT, expected_rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &quo, &rem,
                      &expected_quo, &expected_rem);

  bn1 = read_cstr("-23058430092136939520");
  bn2 = read_cstr("9223372036854775808");
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  expected_quo = read_cstr("-3");
  expected_rem = read_cstr("4611686018427387904");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_floor_div(bn1, bn2,
                                             SCM_CSETTER_L(quo),
                                             SCM_CSETTER_L(rem)));

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected_quo, quo));
  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected_rem, rem));
}

TEST(fcd_bignum, fcd_floor_div__3)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;
  ScmObj expected_quo = SCM_OBJ_INIT, expected_rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &quo, &rem,
                      &expected_quo, &expected_rem);

  bn1 = read_cstr("23058430092136939520");
  bn2 = read_cstr("-9223372036854775808");
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  expected_quo = read_cstr("-3");
  expected_rem = read_cstr("-4611686018427387904");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_floor_div(bn1, bn2,
                                             SCM_CSETTER_L(quo),
                                             SCM_CSETTER_L(rem)));

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected_quo, quo));
  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected_rem, rem));
}

TEST(fcd_bignum, fcd_floor_div__4)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;
  ScmObj expected_quo = SCM_OBJ_INIT, expected_rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &quo, &rem,
                      &expected_quo, &expected_rem);

  bn1 = read_cstr("-23058430092136939520");
  bn2 = read_cstr("-9223372036854775808");
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  expected_quo = read_cstr("2");
  expected_rem = read_cstr("-4611686018427387904");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_floor_div(bn1, bn2,
                                             SCM_CSETTER_L(quo),
                                             SCM_CSETTER_L(rem)));

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected_quo, quo));
  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected_rem, rem));
}

TEST(fcd_bignum, fcd_floor_div__division_by_zero)
{
  ScmObj bn = SCM_OBJ_INIT, fn = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn, &bn, &quo, &rem);

  bn = read_cstr("23058430092136939520");
  fn = read_cstr("0");
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_EQUAL_INT(-1, scm_fcd_floor_div(bn, fn,
                                              SCM_CSETTER_L(quo),
                                              SCM_CSETTER_L(rem)));
}

TEST(fcd_bignum, fcd_truncate_div__1)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;
  ScmObj expected_quo = SCM_OBJ_INIT, expected_rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &quo, &rem,
                      &expected_quo, &expected_rem);

  bn1 = read_cstr("23058430092136939520");
  bn2 = read_cstr("9223372036854775808");
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  expected_quo = read_cstr("2");
  expected_rem = read_cstr("4611686018427387904");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_truncate_div(bn1, bn2,
                                                SCM_CSETTER_L(quo),
                                                SCM_CSETTER_L(rem)));

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected_quo, quo));
  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected_rem, rem));
}

TEST(fcd_bignum, fcd_truncate_div__2)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;
  ScmObj expected_quo = SCM_OBJ_INIT, expected_rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &quo, &rem,
                      &expected_quo, &expected_rem);

  bn1 = read_cstr("-23058430092136939520");
  bn2 = read_cstr("9223372036854775808");
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  expected_quo = read_cstr("-2");
  expected_rem = read_cstr("-4611686018427387904");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_truncate_div(bn1, bn2,
                                                SCM_CSETTER_L(quo),
                                                SCM_CSETTER_L(rem)));

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected_quo, quo));
  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected_rem, rem));
}

TEST(fcd_bignum, fcd_truncate_div__3)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;
  ScmObj expected_quo = SCM_OBJ_INIT, expected_rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &quo, &rem,
                      &expected_quo, &expected_rem);

  bn1 = read_cstr("23058430092136939520");
  bn2 = read_cstr("-9223372036854775808");
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  expected_quo = read_cstr("-2");
  expected_rem = read_cstr("4611686018427387904");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_truncate_div(bn1, bn2,
                                                SCM_CSETTER_L(quo),
                                                SCM_CSETTER_L(rem)));

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected_quo, quo));
  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected_rem, rem));
}

TEST(fcd_bignum, fcd_truncate_div__4)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;
  ScmObj expected_quo = SCM_OBJ_INIT, expected_rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &quo, &rem,
                      &expected_quo, &expected_rem);

  bn1 = read_cstr("-23058430092136939520");
  bn2 = read_cstr("-9223372036854775808");
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn2));

  expected_quo = read_cstr("2");
  expected_rem = read_cstr("-4611686018427387904");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_truncate_div(bn1, bn2,
                                                SCM_CSETTER_L(quo),
                                                SCM_CSETTER_L(rem)));

  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected_quo, quo));
  TEST_ASSERT_SCM_TRUE(scm_fcd_num_eq_P(expected_rem, rem));
}

TEST(fcd_bignum, fcd_truncate_div__division_by_zero)
{
  ScmObj bn = SCM_OBJ_INIT, fn = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn, &bn, &quo, &rem);

  bn = read_cstr("23058430092136939520");
  fn = read_cstr("0");
  TEST_ASSERT_TRUE(scm_fcd_bignum_p(bn));

  TEST_ASSERT_EQUAL_INT(-1, scm_fcd_truncate_div(bn, fn,
                                                 SCM_CSETTER_L(quo),
                                                 SCM_CSETTER_L(rem)));
}