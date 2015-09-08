
#include "scythe/object.h"
#include "scythe/fcd.h"

#include "test.h"

TEST_GROUP(fcd_equivalence);

static ScmScythe *scy;
static ScmRefStackInfo rsi;

TEST_SETUP(fcd_equivalence)
{
  scy = ut_scythe_setup(false);
  scm_fcd_ref_stack_save(&rsi);
}

TEST_TEAR_DOWN(fcd_equivalence)
{
  scm_fcd_ref_stack_restore(&rsi);
  ut_scythe_tear_down(scy);
}

TEST(fcd_equivalence, fcd_eq_p__symbol_return_true)
{
  ScmObj sym1 = SCM_OBJ_INIT, sym2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym1, &sym2);

  sym1 = ut_read_cstr("aaa");
  sym2 = ut_read_cstr("aaa");

  TEST_ASSERT_TRUE(scm_fcd_eq_p(sym1, sym2));
}

TEST(fcd_equivalence, fcd_eq_p__symbol_return_false)
{
  ScmObj sym1 = SCM_OBJ_INIT, sym2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym1, &sym2);

  sym1 = ut_read_cstr("aaa");
  sym2 = ut_read_cstr("bbb");

  TEST_ASSERT_FALSE(scm_fcd_eq_p(sym1, sym2));
}

TEST(fcd_equivalence, fcd_eq_p__list__return_true)
{
  ScmObj lst1 = SCM_OBJ_INIT, lst2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst1, &lst2);

  lst1 = ut_read_cstr("(a b c)");
  lst2 = lst1;

  TEST_ASSERT_TRUE(scm_fcd_eq_p(lst1, lst2));
}

TEST(fcd_equivalence, fcd_eq_p__list__return_false)
{
  ScmObj lst1 = SCM_OBJ_INIT, lst2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst1, &lst2);

  lst1 = ut_read_cstr("(a b c)");
  lst2 = ut_read_cstr("(a b c)");

  TEST_ASSERT_FALSE(scm_fcd_eq_p(lst1, lst2));
}

TEST(fcd_equivalence, fcd_eq_p__empty_list__return_true)
{
  ScmObj lst1 = SCM_OBJ_INIT, lst2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&lst1, &lst2);

  lst1 = ut_read_cstr("()");
  lst2 = ut_read_cstr("()");

  TEST_ASSERT_TRUE(scm_fcd_eq_p(lst1, lst2));
}

TEST(fcd_equivalence, fcd_eq_p__string__return_true)
{
  ScmObj str1 = SCM_OBJ_INIT, str2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&str1, &str2);

  str1 = ut_read_cstr("\"abc\"");
  str2 = str1;

  TEST_ASSERT_TRUE(scm_fcd_eq_p(str1, str2));
}

TEST(fcd_equivalence, fcd_eq_p__string__return_false)
{
  ScmObj str1 = SCM_OBJ_INIT, str2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&str1, &str2);

  str1 = ut_read_cstr("\"abc\"");
  str2 = ut_read_cstr("\"abc\"");

  TEST_ASSERT_FALSE(scm_fcd_eq_p(str1, str2));
}

TEST(fcd_equivalence, fcd_eq_p__empty_string__return_false)
{
  ScmObj str1 = SCM_OBJ_INIT, str2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&str1, &str2);

  str1 = ut_read_cstr("\"\"");
  str2 = ut_read_cstr("\"\"");

  TEST_ASSERT_FALSE(scm_fcd_eq_p(str1, str2));
}

TEST(fcd_equivalence, fcd_eq_p__fixnum__return_true)
{
  ScmObj num1 = SCM_OBJ_INIT, num2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&num1, &num2);

  num1 = ut_read_cstr("123");
  num2 = ut_read_cstr("123");

  TEST_ASSERT_TRUE(scm_fcd_eq_p(num1, num2));
}

TEST(fcd_equivalence, fcd_eq_p__fixnum__return_false)
{
  ScmObj num1 = SCM_OBJ_INIT, num2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&num1, &num2);

  num1 = ut_read_cstr("123");
  num2 = ut_read_cstr("321");

  TEST_ASSERT_FALSE(scm_fcd_eq_p(num1, num2));
}

TEST(fcd_equivalence, fcd_eq_p__bignum__return_true)
{
  ScmObj num1 = SCM_OBJ_INIT, num2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&num1, &num2);

  num1 = ut_read_cstr("99999999999999999999999999999999999");
  num2 = num1;

  TEST_ASSERT_TRUE(scm_fcd_eq_p(num1, num2));
}

TEST(fcd_equivalence, fcd_eq_p__bignum__return_false)
{
  ScmObj num1 = SCM_OBJ_INIT, num2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&num1, &num2);

  num1 = ut_read_cstr("99999999999999999999999999999999999");
  num2 = ut_read_cstr("99999999999999999999999999999999999");

  TEST_ASSERT_FALSE(scm_fcd_eq_p(num1, num2));
}

TEST(fcd_equivalence, fcd_eq_p__char__return_true)
{
  ScmObj chr1 = SCM_OBJ_INIT, chr2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&chr1, &chr2);

  chr1 = ut_read_cstr("#\\a");
  chr2 = chr1;

  TEST_ASSERT_TRUE(scm_fcd_eq_p(chr1, chr2));
}

TEST(fcd_equivalence, fcd_eq_p__char__return_false)
{
  ScmObj chr1 = SCM_OBJ_INIT, chr2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&chr1, &chr2);

  chr1 = ut_read_cstr("#\\a");
  chr2 = ut_read_cstr("#\\a");

  TEST_ASSERT_FALSE(scm_fcd_eq_p(chr1, chr2));
}

TEST(fcd_equivalence, fcd_eq_p__SCM_OBJ_NULL__return_false)
{
  TEST_ASSERT_FALSE(scm_fcd_eq_p(SCM_OBJ_NULL, SCM_OBJ_NULL));
}

TEST(fcd_equivalence, fcd_eq_P__return_true)
{
  ScmObj sym1 = SCM_OBJ_INIT, sym2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym1, &sym2);

  sym1 = ut_read_cstr("aaa");
  sym2 = ut_read_cstr("aaa");

  TEST_ASSERT_SCM_TRUE(scm_fcd_eq_P(sym1, sym2));
}

TEST(fcd_equivalence, fcd_eq_P__return_false)
{
  ScmObj sym1 = SCM_OBJ_INIT, sym2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym1, &sym2);

  sym1 = ut_read_cstr("aaa");
  sym2 = ut_read_cstr("bbb");

  TEST_ASSERT_SCM_FALSE(scm_fcd_eq_P(sym1, sym2));
}

TEST(fcd_equivalence, fcd_eqv__symbol__return_true)
{
  ScmObj sym1 = SCM_OBJ_INIT, sym2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&sym1, &sym2);

  sym1 = ut_read_cstr("aaa");
  sym2 = ut_read_cstr("aaa");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_eqv(sym1, sym2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_equivalence, fcd_eqv__symbol__return_false)
{
  ScmObj sym1 = SCM_OBJ_INIT, sym2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&sym1, &sym2);

  sym1 = ut_read_cstr("aaa");
  sym2 = ut_read_cstr("bbb");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_eqv(sym1, sym2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_equivalence, fcd_eqv__list__return_true)
{
  ScmObj lst1 = SCM_OBJ_INIT, lst2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&lst1, &lst2);

  lst1 = ut_read_cstr("(a b c)");
  lst2 = lst1;

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_eqv(lst1, lst2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_equivalence, fcd_eqv__list__return_false)
{
  ScmObj lst1 = SCM_OBJ_INIT, lst2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&lst1, &lst2);

  lst1 = ut_read_cstr("(a b c)");
  lst2 = ut_read_cstr("(a b c)");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_eqv(lst1, lst2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_equivalence, fcd_eqv__empty_list__return_true)
{
  ScmObj lst1 = SCM_OBJ_INIT, lst2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&lst1, &lst2);

  lst1 = ut_read_cstr("()");
  lst2 = ut_read_cstr("()");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_eqv(lst1, lst2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_equivalence, fcd_eqv__string__return_true)
{
  ScmObj str1 = SCM_OBJ_INIT, str2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&str1, &str2);

  str1 = ut_read_cstr("\"abc\"");
  str2 = str1;

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_eqv(str1, str2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_equivalence, fcd_eqv__string__return_false)
{
  ScmObj str1 = SCM_OBJ_INIT, str2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&str1, &str2);

  str1 = ut_read_cstr("\"abc\"");
  str2 = ut_read_cstr("\"abc\"");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_eqv(str1, str2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_equivalence, fcd_eqv__empty_string__return_false)
{
  ScmObj str1 = SCM_OBJ_INIT, str2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&str1, &str2);

  str1 = ut_read_cstr("\"\"");
  str2 = ut_read_cstr("\"\"");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_eqv(str1, str2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_equivalence, fcd_eqv__fixnum__return_true)
{
  ScmObj num1 = SCM_OBJ_INIT, num2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&num1, &num2);

  num1 = ut_read_cstr("123");
  num2 = ut_read_cstr("123");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_eqv(num1, num2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_equivalence, fcd_eqv__fixnum__return_false)
{
  ScmObj num1 = SCM_OBJ_INIT, num2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&num1, &num2);

  num1 = ut_read_cstr("123");
  num2 = ut_read_cstr("321");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_eqv(num1, num2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_equivalence, fcd_eqv__bignum__return_true)
{
  ScmObj num1 = SCM_OBJ_INIT, num2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&num1, &num2);

  num1 = ut_read_cstr("99999999999999999999999999999999999");
  num2 = ut_read_cstr("99999999999999999999999999999999999");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_eqv(num1, num2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_equivalence, fcd_eqv__bignum__return_false)
{
  ScmObj num1 = SCM_OBJ_INIT, num2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&num1, &num2);

  num1 = ut_read_cstr("99999999999999999999999999999999999");
  num2 = ut_read_cstr("88888888888888888888888888888888888");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_eqv(num1, num2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_equivalence, fcd_eqv__char__return_true)
{
  ScmObj chr1 = SCM_OBJ_INIT, chr2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&chr1, &chr2);

  chr1 = ut_read_cstr("#\\a");
  chr2 = ut_read_cstr("#\\a");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_eqv(chr1, chr2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_equivalence, fcd_eqv__char__return_false)
{
  ScmObj chr1 = SCM_OBJ_INIT, chr2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&chr1, &chr2);

  chr1 = ut_read_cstr("#\\a");
  chr2 = ut_read_cstr("#\\b");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_eqv(chr1, chr2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_equivalence, fcd_eqv__different_obj_type__return_false)
{
  bool actual;

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_eqv(SCM_FALSE_OBJ, SCM_NIL_OBJ, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_equivalence, fcd_eqv_P__return_true)
{
  ScmObj sym1 = SCM_OBJ_INIT, sym2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym1, &sym2);

  sym1 = ut_read_cstr("aaa");
  sym2 = ut_read_cstr("aaa");

  TEST_ASSERT_SCM_TRUE(scm_fcd_eqv_P(sym1, sym2));
}

TEST(fcd_equivalence, fcd_eqv_P__return_false)
{
  ScmObj sym1 = SCM_OBJ_INIT, sym2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym1, &sym2);

  sym1 = ut_read_cstr("aaa");
  sym2 = ut_read_cstr("bbb");

  TEST_ASSERT_SCM_FALSE(scm_fcd_eqv_P(sym1, sym2));
}

TEST(fcd_equivalence, fcd_equal__symbol__return_true)
{
  ScmObj sym1 = SCM_OBJ_INIT, sym2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&sym1, &sym2);

  sym1 = ut_read_cstr("aaa");
  sym2 = ut_read_cstr("aaa");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(sym1, sym2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_equivalence, fcd_equal__symbol__return_false)
{
  ScmObj sym1 = SCM_OBJ_INIT, sym2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&sym1, &sym2);

  sym1 = ut_read_cstr("aaa");
  sym2 = ut_read_cstr("bbb");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(sym1, sym2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_equivalence, fcd_equal__list__return_true)
{
  ScmObj lst1 = SCM_OBJ_INIT, lst2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&lst1, &lst2);

  lst1 = ut_read_cstr("(a b c)");
  lst2 = ut_read_cstr("(a b c)");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(lst1, lst2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_equivalence, fcd_equal__list__return_false)
{
  ScmObj lst1 = SCM_OBJ_INIT, lst2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&lst1, &lst2);

  lst1 = ut_read_cstr("(a b c)");
  lst2 = ut_read_cstr("(a b z)");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(lst1, lst2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_equivalence, fcd_equal__circularly_linked_list__return_true_1)
{
  ScmObj lst1 = SCM_OBJ_INIT, lst2 = SCM_OBJ_INIT, tail = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&lst1, &lst2, &tail);

  lst1 = ut_read_cstr("(a b c)");
  lst2 = ut_read_cstr("(a b c)");

  tail = scm_fcd_list_tail(lst1, 2);
  scm_fcd_set_cdr_i(tail, lst1);

  tail = scm_fcd_list_tail(lst2, 2);
  scm_fcd_set_cdr_i(tail, lst2);

  /* lst1 ;=>  #1=(a b c . #1#) */
  /* lst2 ;=>  #1=(a b c . #1#) */

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(lst1, lst2, &actual));
  TEST_ASSERT_TRUE(actual);
}

/* 循環構造が異なるリストについても真を返さなければならない (r7rs-draft-9) が、
 * 現状、偽を返す。また循環リストの read が未実装なのでテストケースを実行でき
 * ない。
 */
IGNORE_TEST(fcd_equivalence, fcd_equal__circularly_linked_list__return_true_2)
{
  ScmObj lst1 = SCM_OBJ_INIT, lst2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&lst1, &lst2);

  lst1 = ut_read_cstr("#1=(a b . #1#)");
  lst2 = ut_read_cstr("#1=(a b a b . #1#)");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(lst1, lst2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_equivalence, fcd_equal__vector__return_true)
{
  ScmObj vec1 = SCM_OBJ_INIT, vec2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&vec1, &vec2);

  vec1 = ut_read_cstr("#(a b c)");
  vec2 = ut_read_cstr("#(a b c)");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(vec1, vec2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_equivalence, fcd_equal__vector__return_false)
{
  ScmObj vec1 = SCM_OBJ_INIT, vec2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&vec1, &vec2);

  vec1 = ut_read_cstr("#(a b c)");
  vec2 = ut_read_cstr("#(a b z)");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(vec1, vec2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_equivalence, fcd_equal__circularly_linked_vector__return_true_1)
{
  ScmObj vec1 = SCM_OBJ_INIT, vec2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&vec1, &vec2);

  vec1 = ut_read_cstr("#(a b c)");
  vec2 = ut_read_cstr("#(a b c)");

  scm_fcd_vector_set_i(vec1, 2, vec2);
  scm_fcd_vector_set_i(vec2, 2, vec1);

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(vec1, vec2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_equivalence, fcd_equal__string__return_true)
{
  ScmObj str1 = SCM_OBJ_INIT, str2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&str1, &str2);

  str1 = ut_read_cstr("\"abc\"");
  str2 = ut_read_cstr("\"abc\"");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(str1, str2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_equivalence, fcd_equal__string__return_false)
{
  ScmObj str1 = SCM_OBJ_INIT, str2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&str1, &str2);

  str1 = ut_read_cstr("\"abc\"");
  str2 = ut_read_cstr("\"abz\"");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(str1, str2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_equivalence, fcd_equal__fixnum__return_true)
{
  ScmObj num1 = SCM_OBJ_INIT, num2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&num1, &num2);

  num1 = ut_read_cstr("123");
  num2 = ut_read_cstr("123");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(num1, num2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_equivalence, fcd_equal__fixnum__return_false)
{
  ScmObj num1 = SCM_OBJ_INIT, num2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&num1, &num2);

  num1 = ut_read_cstr("123");
  num2 = ut_read_cstr("321");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(num1, num2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_equivalence, fcd_equal__bignum__return_true)
{
  ScmObj num1 = SCM_OBJ_INIT, num2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&num1, &num2);

  num1 = ut_read_cstr("99999999999999999999999999999999999");
  num2 = ut_read_cstr("99999999999999999999999999999999999");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(num1, num2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_equivalence, fcd_equal__bignum__return_false)
{
  ScmObj num1 = SCM_OBJ_INIT, num2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&num1, &num2);

  num1 = ut_read_cstr("99999999999999999999999999999999999");
  num2 = ut_read_cstr("88888888888888888888888888888888888");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(num1, num2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_equivalence, fcd_equal__char__return_true)
{
  ScmObj chr1 = SCM_OBJ_INIT, chr2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&chr1, &chr2);

  chr1 = ut_read_cstr("#\\a");
  chr2 = ut_read_cstr("#\\a");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(chr1, chr2, &actual));
  TEST_ASSERT_TRUE(actual);
}

TEST(fcd_equivalence, fcd_equal__char__return_false)
{
  ScmObj chr1 = SCM_OBJ_INIT, chr2 = SCM_OBJ_INIT;
  bool actual;

  SCM_REFSTK_INIT_REG(&chr1, &chr2);

  chr1 = ut_read_cstr("#\\a");
  chr2 = ut_read_cstr("#\\b");

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_equal(chr1, chr2, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_equivalence, fcd_equal__different_obj_type__return_false)
{
  bool actual;

  TEST_ASSERT_EQUAL_INT(0, scm_fcd_eqv(SCM_FALSE_OBJ, SCM_NIL_OBJ, &actual));
  TEST_ASSERT_FALSE(actual);
}

TEST(fcd_equivalence, fcd_equal_P__return_true)
{
  ScmObj sym1 = SCM_OBJ_INIT, sym2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym1, &sym2);

  sym1 = ut_read_cstr("aaa");
  sym2 = ut_read_cstr("aaa");

  TEST_ASSERT_SCM_EQUAL(sym1, sym2);
}

TEST(fcd_equivalence, fcd_equal_P__return_false)
{
  ScmObj sym1 = SCM_OBJ_INIT, sym2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym1, &sym2);

  sym1 = ut_read_cstr("aaa");
  sym2 = ut_read_cstr("bbb");

  TEST_ASSERT_SCM_FALSE(scm_fcd_equal_P(sym1, sym2));
}

