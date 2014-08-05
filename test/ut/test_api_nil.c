#include "api.h"

#include "test.h"

TEST_GROUP(api_nil);

static ScmEvaluator *ev;

TEST_SETUP(api_nil)
{
  ev = scm_capi_evaluator();
  scm_capi_evaluator_make_vm(ev);
}

TEST_TEAR_DOWN(api_nil)
{
  scm_capi_evaluator_end(ev);
}

TEST(api_nil, capi_nil_p__return_true)
{
  TEST_ASSERT_TRUE(scm_capi_nil_p(SCM_NIL_OBJ));
}

TEST(api_nil, capi_nil_p__return_false)
{
  TEST_ASSERT_FALSE(scm_capi_nil_p(SCM_EOF_OBJ));
}

TEST(api_nil, api_nil_P__return_true)
{
  TEST_ASSERT_SCM_TRUE(scm_api_nil_P(SCM_NIL_OBJ));
}

TEST(api_nil, api_nil_P__return_false)
{
  TEST_ASSERT_SCM_FALSE(scm_api_nil_P(SCM_EOF_OBJ));
}

TEST(api_nil, api_nil_P__return_ERROR)
{
  TEST_ASSERT_SCM_NULL(scm_api_nil_P(SCM_OBJ_NULL));
}