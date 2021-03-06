#include "format.c"

#include "scythe/object.h"
#include "scythe/refstk.h"
#include "scythe/string.h"
#include "scythe/format.h"

#include "test.h"

TEST_GROUP(format);

static ScmScythe *scy;
static ScmRefStackInfo rsi;

TEST_SETUP(format)
{
  scy = ut_scythe_setup(false);
  scm_ref_stack_save(&rsi);
}

TEST_TEAR_DOWN(format)
{
  scm_ref_stack_restore(&rsi);
  ut_scythe_tear_down(scy);
}

TEST(format, format_lst)
{
  ScmObj actual = SCM_OBJ_INIT, expected = SCM_OBJ_INIT;
  ScmObj format = SCM_OBJ_INIT, arg = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&actual, &expected, &format, &arg);

  format = ut_read_cstr("\"[~a] [~s] [~~] [~%]\"");
  arg = ut_read_cstr("(\"foo\" \"bar\")");
  expected = ut_read_cstr("\"[foo] [\\\"bar\\\"] [~] [\\n]\"");

  actual = scm_format_lst(format, arg);

  TEST_ASSERT_SCM_TRUE(scm_string_eq_P(expected, actual));
}

TEST(format, format_lst__too_many_arguments)
{
  ScmObj actual = SCM_OBJ_INIT, expected = SCM_OBJ_INIT;
  ScmObj format = SCM_OBJ_INIT, arg = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&actual, &expected, &format, &arg);

  format = ut_read_cstr("\"[~a] [~s] [~~] [~%]\"");
  arg = ut_read_cstr("(\"foo\" \"bar\" \"baz\")");
  expected = ut_read_cstr("\"[foo] [\\\"bar\\\"] [~] [\\n]\"");

  actual = scm_format_lst(format, arg);

  TEST_ASSERT_SCM_TRUE(scm_string_eq_P(expected, actual));
}

TEST(format, format_lst__error_too_few_arguments)
{
  ScmObj actual = SCM_OBJ_INIT, format = SCM_OBJ_INIT, arg = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&actual, &format, &arg);

  format = ut_read_cstr("\"[~a] [~s] [~~] [~%]\"");
  arg = ut_read_cstr("(\"foo\")");

  actual = scm_format_lst(format, arg);

  TEST_ASSERT_SCM_NULL(actual);
}

TEST(format, format_cv)
{
  ScmObj actual = SCM_OBJ_INIT, expected = SCM_OBJ_INIT;
  ScmObj format = SCM_OBJ_INIT, arg[2] = { SCM_OBJ_INIT, SCM_OBJ_INIT };

  SCM_REFSTK_INIT_REG(&actual, &expected, &format);
  SCM_REFSTK_REG_ARY(arg, 2);

  format = ut_read_cstr("\"[~a] [~s] [~~] [~%]\"");
  arg[0] = arg[1] = ut_read_cstr("\"foo\"");
  expected = ut_read_cstr("\"[foo] [\\\"foo\\\"] [~] [\\n]\"");

  actual = scm_format_cv(format, arg, 2);

  TEST_ASSERT_SCM_TRUE(scm_string_eq_P(expected, actual));
}

TEST(format, format_cv__too_many_arguments)
{
  ScmObj actual = SCM_OBJ_INIT, expected = SCM_OBJ_INIT;
  ScmObj format = SCM_OBJ_INIT;
  ScmObj arg[3] = { SCM_OBJ_INIT, SCM_OBJ_INIT, SCM_OBJ_INIT };

  SCM_REFSTK_INIT_REG(&actual, &expected, &format);
  SCM_REFSTK_REG_ARY(arg, 3);

  format = ut_read_cstr("\"[~a] [~s] [~~] [~%]\"");
  arg[0] = arg[1] = arg[2] = ut_read_cstr("\"foo\"");
  expected = ut_read_cstr("\"[foo] [\\\"foo\\\"] [~] [\\n]\"");

  actual = scm_format_cv(format, arg, 3);

  TEST_ASSERT_SCM_TRUE(scm_string_eq_P(expected, actual));
}

TEST(format, format_cv__error_too_few_arguments)
{
  ScmObj actual = SCM_OBJ_INIT, format = SCM_OBJ_INIT;
  ScmObj arg[1] = { SCM_OBJ_INIT };

  SCM_REFSTK_INIT_REG(&actual, &format);
  SCM_REFSTK_REG_ARY(arg, 1);

  format = ut_read_cstr("\"[~a] [~s] [~~] [~%]\"");
  arg[0] = ut_read_cstr("\"foo\"");

  actual = scm_format_cv(format, arg, 1);

  TEST_ASSERT_SCM_NULL(actual);
}

TEST(format, format)
{
  ScmObj actual = SCM_OBJ_INIT, expected = SCM_OBJ_INIT;
  ScmObj format = SCM_OBJ_INIT, arg[2] = { SCM_OBJ_INIT, SCM_OBJ_INIT };

  SCM_REFSTK_INIT_REG(&actual, &expected, &format);
  SCM_REFSTK_REG_ARY(arg, 2);

  format = ut_read_cstr("\"[~a] [~s] [~~] [~%]\"");
  arg[0] = arg[1] = ut_read_cstr("\"foo\"");
  expected = ut_read_cstr("\"[foo] [\\\"foo\\\"] [~] [\\n]\"");

  actual = scm_format(format, arg[0], arg[1], SCM_OBJ_NULL);

  TEST_ASSERT_SCM_TRUE(scm_string_eq_P(expected, actual));
}

TEST(format, format_cstr)
{
  ScmObj actual = SCM_OBJ_INIT, expected = SCM_OBJ_INIT;
  ScmObj arg[2] = { SCM_OBJ_INIT, SCM_OBJ_INIT };
  const char *format = "[~a] [~s] [~~] [~%]";

  SCM_REFSTK_INIT_REG(&actual, &expected);
  SCM_REFSTK_REG_ARY(arg, 2);

  arg[0] = arg[1] = ut_read_cstr("\"foo\"");
  expected = ut_read_cstr("\"[foo] [\\\"foo\\\"] [~] [\\n]\"");

  actual = scm_format_cstr(format, arg[0], arg[1], SCM_OBJ_NULL);

  TEST_ASSERT_SCM_TRUE(scm_string_eq_P(expected, actual));
}
