#include "scythe/object.h"
#include "scythe/refstk.h"
#include "scythe/fixnum.h"
#include "scythe/bignum.h"
#include "scythe/number.h"
#include "scythe/pair.h"
#include "scythe/api.h"

#include "test.h"

TEST_GROUP(api_bignum);

static ScmScythe *scy;
static ScmRefStackInfo rsi;

TEST_SETUP(api_bignum)
{
  scy = ut_scythe_setup(false);
  scm_ref_stack_save(&rsi);
}

TEST_TEAR_DOWN(api_bignum)
{
  scm_ref_stack_restore(&rsi);
  ut_scythe_tear_down(scy);
}

static void
check_list_elements(ScmObj lst, bool (*check)(ScmObj elm))
{
  ScmObj l = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst,
                      &l);

  for (l = lst; scm_pair_p(l); l = scm_cdr(l))
    TEST_ASSERT_TRUE(check(scm_car(l)));
}

TEST(api_bignum, api_number_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_api_number_P(bn));
}

TEST(api_bignum, api_complex_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_api_complex_P(bn));
}

TEST(api_bignum, api_real_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_api_real_P(bn));
}

TEST(api_bignum, api_rational_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_api_rational_P(bn));
}

TEST(api_bignum, api_integer_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_api_integer_P(bn));
}

TEST(api_bignum, api_exact_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_api_exact_P(bn));
}

TEST(api_bignum, api_inexact_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_FALSE(scm_api_inexact_P(bn));
}

TEST(api_bignum, api_exact_integer_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_api_exact_integer_P(bn));
}

TEST(api_bignum, api_finite_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_api_finite_P(bn));
}

TEST(api_bignum, api_infinite_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_FALSE(scm_api_infinite_P(bn));
}

TEST(api_bignum, api_nan_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_FALSE(scm_api_nan_P(bn));
}

TEST(api_bignum, api_num_eq_P_lst__return_true)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = ut_read_cstr("(4611686018427387904 4611686018427387904 4611686018427387904)");
  check_list_elements(lst, scm_bignum_p);

  TEST_ASSERT_SCM_TRUE(scm_api_num_eq_P_lst(lst));
}

TEST(api_bignum, api_num_eq_P__return_false)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = ut_read_cstr("(4611686018427387904 4611686018427387904 4611686018427387905)");
  check_list_elements(lst, scm_bignum_p);

  TEST_ASSERT_SCM_FALSE(scm_api_num_eq_P_lst(lst));
}

TEST(api_bignum, api_num_lt_P_lst__less)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = ut_read_cstr("(-4611686018427387905 4611686018427387904 9223372036854775807)");
  check_list_elements(lst, scm_bignum_p);

  TEST_ASSERT_SCM_TRUE(scm_api_num_lt_P_lst(lst));
}

TEST(api_bignum, api_num_lt_P_lst__equal)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = ut_read_cstr("(-4611686018427387905 4611686018427387904 4611686018427387904)");
  check_list_elements(lst, scm_bignum_p);

  TEST_ASSERT_SCM_FALSE(scm_api_num_lt_P_lst(lst));
}

TEST(api_bignum, api_num_lt_P_lst__greater)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = ut_read_cstr("(4611686018427387904 9223372036854775807 -4611686018427387905)");
  check_list_elements(lst, scm_bignum_p);

  TEST_ASSERT_SCM_FALSE(scm_api_num_lt_P_lst(lst));
}

TEST(api_bignum, api_num_gt_P_lst__less)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = ut_read_cstr("(4611686018427387904 -4611686018427387905 9223372036854775807)");
  check_list_elements(lst, scm_bignum_p);

  TEST_ASSERT_SCM_FALSE(scm_api_num_gt_P_lst(lst));
}

TEST(api_bignum, api_num_gt_P_lst__equal)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = ut_read_cstr("(9223372036854775807 4611686018427387904 4611686018427387904)");
  check_list_elements(lst, scm_bignum_p);

  TEST_ASSERT_SCM_FALSE(scm_api_num_gt_P_lst(lst));
}

TEST(api_bignum, api_num_gt_P_lst__greater)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = ut_read_cstr("(9223372036854775807 4611686018427387904 -4611686018427387905)");
  check_list_elements(lst, scm_bignum_p);

  TEST_ASSERT_SCM_TRUE(scm_api_num_gt_P_lst(lst));
}

TEST(api_bignum, api_num_le_P_lst__less)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = ut_read_cstr("(-4611686018427387905 4611686018427387904 9223372036854775807)");
  check_list_elements(lst, scm_bignum_p);

  TEST_ASSERT_SCM_TRUE(scm_api_num_le_P_lst(lst));
}

TEST(api_bignum, api_num_le_P_lst__equal)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = ut_read_cstr("(-4611686018427387905 4611686018427387904 4611686018427387904)");
  check_list_elements(lst, scm_bignum_p);

  TEST_ASSERT_SCM_TRUE(scm_api_num_le_P_lst(lst));
}

TEST(api_bignum, api_num_le_P_lst__greater)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = ut_read_cstr("(4611686018427387904 9223372036854775807 -4611686018427387905)");
  check_list_elements(lst, scm_bignum_p);

  TEST_ASSERT_SCM_FALSE(scm_api_num_le_P_lst(lst));
}

TEST(api_bignum, api_num_ge_P_lst__less)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = ut_read_cstr("(4611686018427387904 -4611686018427387905 9223372036854775807)");
  check_list_elements(lst, scm_bignum_p);

  TEST_ASSERT_SCM_FALSE(scm_api_num_ge_P_lst(lst));
}

TEST(api_bignum, api_num_ge_P_lst__equal)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = ut_read_cstr("(9223372036854775807 4611686018427387904 4611686018427387904)");
  check_list_elements(lst, scm_bignum_p);

  TEST_ASSERT_SCM_TRUE(scm_api_num_ge_P_lst(lst));
}

TEST(api_bignum, api_num_ge_P_lst__greater)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst);

  lst = ut_read_cstr("(9223372036854775807 4611686018427387904 -4611686018427387905)");
  check_list_elements(lst, scm_bignum_p);

  TEST_ASSERT_SCM_TRUE(scm_api_num_ge_P_lst(lst));
}

IGNORE_TEST(api_bignum, api_zero_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = ut_read_cstr("0");
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_api_zero_P(bn));
}

IGNORE_TEST(api_bignum, api_zero_P__return_false)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = ut_read_cstr("-1");
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_FALSE(scm_api_zero_P(bn));
}

TEST(api_bignum, api_positive_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_api_positive_P(bn));
}

TEST(api_bignum, api_positive_P__return_false)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_FALSE(scm_api_positive_P(bn));
}

TEST(api_bignum, api_negative_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_api_negative_P(bn));
}

TEST(api_bignum, api_negative_P__return_false)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MAX + 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_FALSE(scm_api_negative_P(bn));
}

TEST(api_bignum, api_odd_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_api_odd_P(bn));
}

TEST(api_bignum, api_odd_P__return_false)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MIN - 2);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_FALSE(scm_api_odd_P(bn));
}

TEST(api_bignum, api_even_P__return_true)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MIN - 2);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_TRUE(scm_api_even_P(bn));
}

TEST(api_bignum, api_even_P__return_false)
{
  ScmObj bn = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn);

  bn = scm_make_number_from_sword(SCM_FIXNUM_MIN - 1);
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_SCM_FALSE(scm_api_even_P(bn));
}

TEST(api_bignum, api_max_lst)
{
  ScmObj lst = SCM_OBJ_INIT, expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst, &expected, &actual);

  lst = ut_read_cstr("(4611686018427387904 -4611686018427387905 4611686018427388160 4611686022722355200 -4611686022722355200)");
  check_list_elements(lst, scm_bignum_p);
  expected = ut_read_cstr("4611686022722355200");

  actual = scm_api_max_lst(lst);

  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected, actual));
}

TEST(api_bignum, api_min_lst)
{
  ScmObj lst = SCM_OBJ_INIT, expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst, &expected, &actual);

  lst = ut_read_cstr("(4611686018427387904 -4611686018427387905 4611686018427388160 4611686022722355200 -4611686022722355200)");
  check_list_elements(lst, scm_bignum_p);
  expected = ut_read_cstr("-4611686022722355200");

  actual = scm_api_min_lst(lst);

  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected, actual));
}

TEST(api_bignum, api_plus_lst)
{
  ScmObj lst = SCM_OBJ_INIT, expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst, &expected, &actual);

  lst = ut_read_cstr("(4611686018427387904 4611686022722355200 9223372036854775808)");
  check_list_elements(lst, scm_bignum_p);
  expected = ut_read_cstr("18446744078004518912");

  actual = scm_api_plus_lst(lst);

  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected, actual));
}

TEST(api_bignum, api_mul_lst)
{
  ScmObj lst = SCM_OBJ_INIT, expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst, &expected, &actual);

  lst = ut_read_cstr("(4611686018427387904 4611686022722355200 9223372036854775808)");
  check_list_elements(lst, scm_bignum_p);
  expected = ut_read_cstr("196159429413521478536231284250700179592733985096492646400");

  actual = scm_api_mul_lst(lst);

  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected, actual));
}

TEST(api_bignum, api_minus_lst)
{
  ScmObj lst = SCM_OBJ_INIT, expected = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst, &expected, &actual);

  lst = ut_read_cstr("(4611686018427387904 4611686022722355200 9223372036854775808)");
  check_list_elements(lst, scm_bignum_p);
  expected = ut_read_cstr("-9223372041149743104");

  actual = scm_api_minus_lst(lst);

  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected, actual));
}

TEST(api_bignum, capi_floor_div__1)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;
  ScmObj expected_quo = SCM_OBJ_INIT, expected_rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &quo, &rem,
                      &expected_quo, &expected_rem);

  bn1 = ut_read_cstr("23058430092136939520");
  bn2 = ut_read_cstr("9223372036854775808");
  TEST_ASSERT_TRUE(scm_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_bignum_p(bn2));

  expected_quo = ut_read_cstr("2");
  expected_rem = ut_read_cstr("4611686018427387904");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_floor_div(bn1, bn2,
                                              SCM_CSETTER_L(quo),
                                              SCM_CSETTER_L(rem)));

  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected_quo, quo));
  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected_rem, rem));
}

TEST(api_bignum, capi_floor_div__2)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;
  ScmObj expected_quo = SCM_OBJ_INIT, expected_rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &quo, &rem,
                      &expected_quo, &expected_rem);

  bn1 = ut_read_cstr("-23058430092136939520");
  bn2 = ut_read_cstr("9223372036854775808");
  TEST_ASSERT_TRUE(scm_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_bignum_p(bn2));

  expected_quo = ut_read_cstr("-3");
  expected_rem = ut_read_cstr("4611686018427387904");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_floor_div(bn1, bn2,
                                              SCM_CSETTER_L(quo),
                                              SCM_CSETTER_L(rem)));

  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected_quo, quo));
  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected_rem, rem));
}

TEST(api_bignum, capi_floor_div__3)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;
  ScmObj expected_quo = SCM_OBJ_INIT, expected_rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &quo, &rem,
                      &expected_quo, &expected_rem);

  bn1 = ut_read_cstr("23058430092136939520");
  bn2 = ut_read_cstr("-9223372036854775808");
  TEST_ASSERT_TRUE(scm_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_bignum_p(bn2));

  expected_quo = ut_read_cstr("-3");
  expected_rem = ut_read_cstr("-4611686018427387904");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_floor_div(bn1, bn2,
                                              SCM_CSETTER_L(quo),
                                              SCM_CSETTER_L(rem)));

  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected_quo, quo));
  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected_rem, rem));
}

TEST(api_bignum, capi_floor_div__4)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;
  ScmObj expected_quo = SCM_OBJ_INIT, expected_rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &quo, &rem,
                      &expected_quo, &expected_rem);

  bn1 = ut_read_cstr("-23058430092136939520");
  bn2 = ut_read_cstr("-9223372036854775808");
  TEST_ASSERT_TRUE(scm_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_bignum_p(bn2));

  expected_quo = ut_read_cstr("2");
  expected_rem = ut_read_cstr("-4611686018427387904");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_floor_div(bn1, bn2,
                                              SCM_CSETTER_L(quo),
                                              SCM_CSETTER_L(rem)));

  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected_quo, quo));
  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected_rem, rem));
}

TEST(api_bignum, capi_floor_div__division_by_zero)
{
  ScmObj bn = SCM_OBJ_INIT, fn = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn, &bn, &quo, &rem);

  bn = ut_read_cstr("23058430092136939520");
  fn = ut_read_cstr("0");
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_EQUAL_INT(-1, scm_capi_floor_div(bn, fn,
                                               SCM_CSETTER_L(quo),
                                               SCM_CSETTER_L(rem)));
}

TEST(api_bignum, capi_truncate_div__1)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;
  ScmObj expected_quo = SCM_OBJ_INIT, expected_rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &quo, &rem,
                      &expected_quo, &expected_rem);

  bn1 = ut_read_cstr("23058430092136939520");
  bn2 = ut_read_cstr("9223372036854775808");
  TEST_ASSERT_TRUE(scm_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_bignum_p(bn2));

  expected_quo = ut_read_cstr("2");
  expected_rem = ut_read_cstr("4611686018427387904");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_truncate_div(bn1, bn2,
                                                 SCM_CSETTER_L(quo),
                                                 SCM_CSETTER_L(rem)));

  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected_quo, quo));
  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected_rem, rem));
}

TEST(api_bignum, capi_truncate_div__2)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;
  ScmObj expected_quo = SCM_OBJ_INIT, expected_rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &quo, &rem,
                      &expected_quo, &expected_rem);

  bn1 = ut_read_cstr("-23058430092136939520");
  bn2 = ut_read_cstr("9223372036854775808");
  TEST_ASSERT_TRUE(scm_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_bignum_p(bn2));

  expected_quo = ut_read_cstr("-2");
  expected_rem = ut_read_cstr("-4611686018427387904");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_truncate_div(bn1, bn2,
                                                 SCM_CSETTER_L(quo),
                                                 SCM_CSETTER_L(rem)));

  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected_quo, quo));
  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected_rem, rem));
}

TEST(api_bignum, capi_truncate_div__3)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;
  ScmObj expected_quo = SCM_OBJ_INIT, expected_rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &quo, &rem,
                      &expected_quo, &expected_rem);

  bn1 = ut_read_cstr("23058430092136939520");
  bn2 = ut_read_cstr("-9223372036854775808");
  TEST_ASSERT_TRUE(scm_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_bignum_p(bn2));

  expected_quo = ut_read_cstr("-2");
  expected_rem = ut_read_cstr("4611686018427387904");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_truncate_div(bn1, bn2,
                                                 SCM_CSETTER_L(quo),
                                                 SCM_CSETTER_L(rem)));

  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected_quo, quo));
  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected_rem, rem));
}

TEST(api_bignum, capi_truncate_div__4)
{
  ScmObj bn1 = SCM_OBJ_INIT, bn2 = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;
  ScmObj expected_quo = SCM_OBJ_INIT, expected_rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn1, &bn2, &quo, &rem,
                      &expected_quo, &expected_rem);

  bn1 = ut_read_cstr("-23058430092136939520");
  bn2 = ut_read_cstr("-9223372036854775808");
  TEST_ASSERT_TRUE(scm_bignum_p(bn1));
  TEST_ASSERT_TRUE(scm_bignum_p(bn2));

  expected_quo = ut_read_cstr("2");
  expected_rem = ut_read_cstr("-4611686018427387904");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_truncate_div(bn1, bn2,
                                                 SCM_CSETTER_L(quo),
                                                 SCM_CSETTER_L(rem)));

  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected_quo, quo));
  TEST_ASSERT_SCM_TRUE(scm_num_eq_P(expected_rem, rem));
}

TEST(api_bignum, capi_truncate_div__division_by_zero)
{
  ScmObj bn = SCM_OBJ_INIT, fn = SCM_OBJ_INIT;
  ScmObj quo = SCM_OBJ_INIT, rem = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&bn, &bn, &quo, &rem);

  bn = ut_read_cstr("23058430092136939520");
  fn = ut_read_cstr("0");
  TEST_ASSERT_TRUE(scm_bignum_p(bn));

  TEST_ASSERT_EQUAL_INT(-1, scm_capi_truncate_div(bn, fn,
                                                  SCM_CSETTER_L(quo),
                                                  SCM_CSETTER_L(rem)));
}
