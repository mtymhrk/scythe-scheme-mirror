#include "module.c"

#include "scythe/object.h"
#include "scythe/vm.h"
#include "scythe/refstk.h"
#include "scythe/pair.h"
#include "scythe/symbol.h"
#include "scythe/module.h"

#include "test.h"

TEST_GROUP(module);

static ScmScythe *scy;
static ScmRefStackInfo rsi;

static ScmObj undef;
static ScmObj module;
static ScmObj name;
static ScmObj gloc;
static ScmObj symbol;
static ScmObj syntax;

TEST_SETUP(module)
{
  scy = ut_scythe_setup(true);
  scm_ref_stack_save(&rsi);

  undef = SCM_UNDEF_OBJ;
  assert(scm_obj_not_null_p(undef));

  module = name = gloc = symbol = syntax = undef;
  scm_register_extra_rfrn(SCM_REF_MAKE(module));
  scm_register_extra_rfrn(SCM_REF_MAKE(name));
  scm_register_extra_rfrn(SCM_REF_MAKE(gloc));
  scm_register_extra_rfrn(SCM_REF_MAKE(symbol));
  scm_register_extra_rfrn(SCM_REF_MAKE(syntax));
}

TEST_TEAR_DOWN(module)
{
  scm_ref_stack_restore(&rsi);
  ut_scythe_tear_down(scy);
}

static void
find_module(const char *n)
{
  module = name = undef;

  name = scm_make_symbol_from_cstr(n, SCM_ENC_ASCII);
  name = scm_list(1, name);
  TEST_ASSERT_EQUAL_INT(0, scm_find_module(name, SCM_CSETTER_L(module)));
}

static void
make_module(const char *n)
{
  module = name = undef;

  name = scm_make_symbol_from_cstr(n, SCM_ENC_ASCII);
  name = scm_list(1, name);
  module = scm_make_module(name);
}

static void
import_module(const char *n, bool res)
{
  ScmObj mod = SCM_OBJ_INIT, nam = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&mod, &nam);

  nam = name;
  mod = module;

  find_module(n);

  scm_module_import(mod, module, res);

  name = nam;
  module = mod;
}

static void
get_gloc(const char *n)
{
  gloc = symbol = undef;

  symbol = scm_make_symbol_from_cstr(n, SCM_ENC_ASCII);
  gloc = scm_module_gloc(module, symbol);
}

static void
find_gloc(const char *n)
{
  gloc = symbol = undef;

  symbol = scm_make_symbol_from_cstr(n, SCM_ENC_ASCII);
  scm_module_find_gloc(module, symbol, SCM_CSETTER_L(gloc));
}

TEST(module, make_module)
{
  make_module("test");

  TEST_ASSERT_TRUE(scm_module_p(module));
  TEST_ASSERT_SCM_EQUAL(name, scm_module_name(module));
}

TEST(module, make_module__already_exist)
{
  make_module("main");

  TEST_ASSERT_SCM_NULL(module);
}

TEST(module, import)
{
  ScmObj main_mod = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&main_mod);

  find_module("main");

  main_mod = module;

  module = SCM_OBJ_NULL;

  make_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_module_import(module, main_mod, false));
}

TEST(module, find_module)
{
  ScmObj mod = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&mod);

  make_module("test");

  mod = module;

  find_module("test");

  TEST_ASSERT_SCM_EQ(mod, module);
}

TEST(module, find_module__not_exist)
{
  find_module("test");

  TEST_ASSERT_SCM_NULL(module);
}

TEST(module, make_gloc)
{
  make_module("test");
  get_gloc("var");

  TEST_ASSERT_TRUE(scm_gloc_p(gloc));
  TEST_ASSERT_SCM_EQ(symbol, scm_gloc_symbol(gloc));
  TEST_ASSERT_SCM_EQ(SCM_UNINIT_OBJ, scm_gloc_variable_value(gloc));
  TEST_ASSERT_SCM_EQ(SCM_UNINIT_OBJ, scm_gloc_keyword_value(gloc));
}

TEST(module, make_gloc__already_exist)
{
  ScmObj prev = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&prev);

  make_module("test");
  get_gloc("var");
  prev = gloc;
  get_gloc("var");

  TEST_ASSERT_SCM_EQ(prev, gloc);
}

TEST(module, find_gloc)
{
  make_module("test");
  get_gloc("var");
  find_gloc("var");

  TEST_ASSERT_TRUE(scm_gloc_p(gloc));
  TEST_ASSERT_SCM_EQ(symbol, scm_gloc_symbol(gloc));
}

TEST(module, find_gloc__not_exist)
{
  ScmObj sym = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym);

  make_module("test");
  find_gloc("var");

  TEST_ASSERT_SCM_NULL(gloc);
}

TEST(module, gloc_bind_variable)
{
  ScmObj expected = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&expected);

  make_module("test");
  get_gloc("var");

  expected = SCM_EOF_OBJ;

  scm_gloc_bind_variable(gloc, expected);
  TEST_ASSERT_SCM_EQ(expected, scm_gloc_variable_value(gloc));
}

TEST(module, gloc_bind_keyword)
{
  ScmObj expected = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&expected);

  make_module("test");
  get_gloc("var");

  expected = SCM_EOF_OBJ;

  scm_gloc_bind_keyword(gloc, expected);
  TEST_ASSERT_SCM_EQ(expected, scm_gloc_keyword_value(gloc));
}

TEST(module, define_global_var)
{
  ScmObj sym = SCM_OBJ_INIT, val = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &val, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  val = SCM_EOF_OBJ;

  make_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_define_global_var(module, sym, val, false));
  TEST_ASSERT_EQUAL_INT(0, scm_refer_global_var(module,
                                                sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_EQ(val, actual);
}

TEST(module, define_global_var__already_bound)
{
  ScmObj sym = SCM_OBJ_INIT, val1 = SCM_OBJ_INIT, val2 = SCM_OBJ_INIT;
  ScmObj actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &val1, &val2, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  val1 = SCM_EOF_OBJ;
  val2 = SCM_FALSE_OBJ;

  make_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_define_global_var(module, sym, val1, false));
  TEST_ASSERT_EQUAL_INT(0, scm_define_global_var(module, sym, val2, false));
  TEST_ASSERT_EQUAL_INT(0, scm_refer_global_var(module,
                                                sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_EQ(val2, actual);
}

TEST(module, global_var_ref__unbound)
{
  ScmObj sym = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);

  make_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_refer_global_var(module,
                                                sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_NULL(actual);
}

TEST(module, global_var_ref__refer_exported_symbol_of_imported_module)
{
  ScmObj sym = SCM_OBJ_INIT, val = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &val, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  val = SCM_EOF_OBJ;

  make_module("imp");
  make_module("test");
  import_module("imp", false);
  find_module("imp");

  TEST_ASSERT_EQUAL_INT(0, scm_define_global_var(module, sym, val, true));

  find_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_refer_global_var(module,
                                                sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_EQ(val, actual);
}

TEST(module, global_var_ref__refer_unexported_symbol_of_imported_module)
{
  ScmObj sym = SCM_OBJ_INIT, val = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &val, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  val = SCM_EOF_OBJ;

  make_module("imp");
  make_module("test");
  import_module("imp", false);
  find_module("imp");

  TEST_ASSERT_EQUAL_INT(0, scm_define_global_var(module, sym, val, false));

  find_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_refer_global_var(module,
                                                sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_NULL(actual);
}

TEST(module, global_var_ref__refer_exported_symbol_of_imported_module__restrictive)
{
  ScmObj sym = SCM_OBJ_INIT, val = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &val, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  val = SCM_EOF_OBJ;

  make_module("imp");
  make_module("test");
  import_module("imp", true);
  find_module("imp");

  TEST_ASSERT_EQUAL_INT(0, scm_define_global_var(module, sym, val, true));

  find_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_refer_global_var(module,
                                                sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_EQ(val, actual);
}

TEST(module, global_var_ref__refer_exported_symbol_of_imported_module__restrictive__2)
{
  ScmObj sym = SCM_OBJ_INIT, val = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &val, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  val = SCM_EOF_OBJ;

  make_module("imp-a");
  make_module("imp-b");
  import_module("imp-a", true);
  make_module("test");
  import_module("imp-b", false);
  find_module("imp-a");

  TEST_ASSERT_EQUAL_INT(0, scm_define_global_var(module, sym, val, true));

  find_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_refer_global_var(module,
                                                sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_NULL(actual);
}

TEST(module, define_global_syx)
{
  ScmObj sym = SCM_OBJ_INIT, syx = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &syx, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  syx = SCM_EOF_OBJ;

  make_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_define_global_syx(module, sym, syx, false));
  TEST_ASSERT_EQUAL_INT(0, scm_refer_global_syx(module,
                                                sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_EQ(syx, actual);
}

TEST(module, define_global_syx__already_bound)
{
  ScmObj sym = SCM_OBJ_INIT, syx1 = SCM_OBJ_INIT, syx2 = SCM_OBJ_INIT;
  ScmObj actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &syx1, &syx2, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  syx1 = SCM_EOF_OBJ;
  syx2 = SCM_UNDEF_OBJ;

  make_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_define_global_syx(module, sym, syx1, false));

  TEST_ASSERT_EQUAL_INT(0, scm_define_global_syx(module, sym, syx2, false));
  TEST_ASSERT_EQUAL_INT(0, scm_refer_global_syx(module,
                                                sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_EQ(syx2, actual);
}

TEST(module, global_syx_ref__unbound)
{
  ScmObj sym = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);

  make_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_refer_global_syx(module,
                                                sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_NULL(actual);
}

TEST(module, global_syx_ref__refer_exported_symbol_of_imported_module)
{
  ScmObj sym = SCM_OBJ_INIT, syx = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &syx, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  syx = SCM_EOF_OBJ;

  make_module("imp");
  make_module("test");
  import_module("imp", false);
  find_module("imp");

  TEST_ASSERT_EQUAL_INT(0, scm_define_global_syx(module, sym, syx, true));

  find_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_refer_global_syx(module,
                                                sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_EQ(syx, actual);
}

TEST(module, global_syx_ref__refer_unexported_symbol_of_imported_module)
{
  ScmObj sym = SCM_OBJ_INIT, syx = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &syx, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  syx = SCM_EOF_OBJ;

  make_module("imp");
  make_module("test");
  import_module("imp", false);
  find_module("imp");

  TEST_ASSERT_EQUAL_INT(0, scm_define_global_syx(module, sym, syx, false));

  find_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_refer_global_syx(module,
                                                sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_NULL(actual);
}

TEST(module, global_syx_ref__refer_exported_symbol_of_imported_module__restrictive)
{
  ScmObj sym = SCM_OBJ_INIT, syx = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &syx, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  syx = SCM_EOF_OBJ;

  make_module("imp");
  make_module("test");
  import_module("imp", true);
  find_module("imp");

  TEST_ASSERT_EQUAL_INT(0, scm_define_global_syx(module, sym, syx, true));

  find_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_refer_global_syx(module,
                                                sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_EQ(syx, actual);
}

TEST(module, global_syx_ref__refer_exported_symbol_of_imported_module__restrictive_2)
{
  ScmObj sym = SCM_OBJ_INIT, syx = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &syx, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  syx = SCM_EOF_OBJ;

  make_module("imp-a");
  make_module("imp-b");
  import_module("imp-a", true);
  make_module("test");
  import_module("imp-b", false);
  find_module("imp-a");

  TEST_ASSERT_EQUAL_INT(0, scm_define_global_syx(module, sym, syx, true));

  find_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_refer_global_syx(module,
                                                sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_NULL(actual);
}

