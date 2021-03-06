#include "scythe/object.h"
#include "scythe/refstk.h"
#include "scythe/module.h"
#include "scythe/pair.h"
#include "scythe/symbol.h"
#include "scythe/api.h"

#include "test.h"

TEST_GROUP(api_module);

static ScmScythe *scy;
static ScmRefStackInfo rsi;

static ScmObj undef;
static ScmObj module;
static ScmObj name;
static ScmObj gloc;
static ScmObj symbol;
static ScmObj syntax;

TEST_SETUP(api_module)
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

TEST_TEAR_DOWN(api_module)
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

TEST(api_module, define_global_syx)
{
  ScmObj sym = SCM_OBJ_INIT, syx = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &syx, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  syx = SCM_EOF_OBJ;

  make_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_define_global_syx(module, sym, syx, false));
  TEST_ASSERT_EQUAL_INT(0, scm_capi_refer_global_syx(module,
                                                     sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_EQ(syx, actual);
}

TEST(api_module, define_global_syx__already_bound)
{
  ScmObj sym = SCM_OBJ_INIT, syx1 = SCM_OBJ_INIT, syx2 = SCM_OBJ_INIT;
  ScmObj actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &syx1, &syx2, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  syx1 = SCM_EOF_OBJ;
  syx2 = SCM_UNDEF_OBJ;

  make_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_define_global_syx(module, sym, syx1, false));

  TEST_ASSERT_EQUAL_INT(0, scm_capi_define_global_syx(module, sym, syx2, false));
  TEST_ASSERT_EQUAL_INT(0, scm_capi_refer_global_syx(module,
                                                     sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_EQ(syx2, actual);
}

TEST(api_module, global_syx_ref__unbound)
{
  ScmObj sym = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);

  make_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_refer_global_syx(module,
                                                     sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_NULL(actual);
}

TEST(api_module, global_syx_ref__refer_exported_symbol_of_imported_module)
{
  ScmObj sym = SCM_OBJ_INIT, syx = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &syx, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  syx = SCM_EOF_OBJ;

  make_module("imp");
  make_module("test");
  import_module("imp", false);
  find_module("imp");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_define_global_syx(module, sym, syx, true));

  find_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_refer_global_syx(module,
                                                     sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_EQ(syx, actual);
}

TEST(api_module, global_syx_ref__refer_unexported_symbol_of_imported_module)
{
  ScmObj sym = SCM_OBJ_INIT, syx = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &syx, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  syx = SCM_EOF_OBJ;

  make_module("imp");
  make_module("test");
  import_module("imp", false);
  find_module("imp");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_define_global_syx(module, sym, syx, false));

  find_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_refer_global_syx(module,
                                                     sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_NULL(actual);
}

TEST(api_module, global_syx_ref__refer_exported_symbol_of_imported_module__restrictive)
{
  ScmObj sym = SCM_OBJ_INIT, syx = SCM_OBJ_INIT, actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&sym, &syx, &actual);

  sym = scm_make_symbol_from_cstr("var", SCM_ENC_ASCII);
  syx = SCM_EOF_OBJ;

  make_module("imp");
  make_module("test");
  import_module("imp", true);
  find_module("imp");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_define_global_syx(module, sym, syx, true));

  find_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_refer_global_syx(module,
                                                     sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_EQ(syx, actual);
}

TEST(api_module, global_syx_ref__refer_exported_symbol_of_imported_module__restrictive_2)
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

  TEST_ASSERT_EQUAL_INT(0, scm_capi_define_global_syx(module, sym, syx, true));

  find_module("test");

  TEST_ASSERT_EQUAL_INT(0, scm_capi_refer_global_syx(module,
                                                     sym, SCM_CSETTER_L(actual)));
  TEST_ASSERT_SCM_NULL(actual);
}
