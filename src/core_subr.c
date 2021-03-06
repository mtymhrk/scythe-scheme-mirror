#include <sys/types.h>
#include <stddef.h>
#include <stdbool.h>

#include "scythe/object.h"
#include "scythe/bedrock.h"
#include "scythe/refstk.h"
#include "scythe/vm.h"
#include "scythe/assembler.h"
#include "scythe/equivalence.h"
#include "scythe/file.h"
#include "scythe/compiler.h"
#include "scythe/exception.h"
#include "scythe/format.h"
#include "scythe/iseq.h"
#include "scythe/marshal.h"
#include "scythe/miscobjects.h"
#include "scythe/module.h"
#include "scythe/pair.h"
#include "scythe/procedure.h"
#include "scythe/core_subr.h"

#include "scythe/api.h"

static ssize_t
scm_subr_list_to_cv(ScmObj lst, ScmObj *ary, size_t n)
{
  ssize_t len;

  SCM_REFSTK_INIT_REG(&lst);

  len = 0;
  for (size_t i = 0; i < n; i++) {
    if (scm_pair_p(lst)) {
      ary[i] = scm_car(lst);
      lst = scm_cdr(lst);
      len++;
    }
    else {
      ary[i] = SCM_OBJ_NULL;
    }
  }

  return len;
}


/*******************************************************************/
/*  Dynamic bindings                                               */
/*******************************************************************/

int
scm_subr_func_make_parameter(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj prm = SCM_OBJ_INIT, conv = SCM_OBJ_INIT;
  ScmObj postproc = SCM_OBJ_INIT, arg = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &prm, &conv,
                      &postproc, &arg);

  conv = SCM_OBJ_NULL;
  if (scm_pair_p(argv[1])) {
    conv = scm_car(argv[1]);
    if (!scm_procedure_p(conv)) {
      scm_error("make-parameter: procedure required, but got", 1, conv);
      return -1;
    }
  }

  prm = scm_make_parameter(argv[0], conv);
  if (scm_obj_null_p(prm)) return -1;

  if (scm_obj_null_p(conv))
    return scm_return_val_1(prm);

  arg = scm_list(2, argv[0], SCM_TRUE_OBJ);
  if (scm_obj_null_p(arg)) return -1;

  return scm_capi_trampolining(prm, arg, SCM_OBJ_NULL, SCM_OBJ_NULL);
}


/*******************************************************************/
/*  Equivalence predicates                                         */
/*******************************************************************/

int
scm_subr_func_eq_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_eq_P(argv[0], argv[1]));
}

int
scm_subr_func_eqv_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_eqv_P(argv[0], argv[1]));
}

int
scm_subr_func_equal_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_equal_P(argv[0], argv[1]));
}


/*******************************************************************/
/*  Numbers                                                        */
/*******************************************************************/

int
scm_subr_func_number_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_number_P(argv[0]));
}

int
scm_subr_func_complex_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_complex_P(argv[0]));
}

int
scm_subr_func_real_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_real_P(argv[0]));
}

int
scm_subr_func_rational_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_rational_P(argv[0]));
}

int
scm_subr_func_integer_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_integer_P(argv[0]));
}

int
scm_subr_func_exact_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_exact_P(argv[0]));
}

int
scm_subr_func_inexact_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_inexact_P(argv[0]));
}

int
scm_subr_func_exact_integer_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_exact_integer_P(argv[0]));
}

int
scm_subr_func_finite_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_finite_P(argv[0]));
}

int
scm_subr_func_infinite_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_infinite_P(argv[0]));
}

int
scm_subr_func_nan_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_nan_P(argv[0]));
}

int
scm_subr_func_num_eq_P(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[1], argv[2]);
  if (scm_obj_null_p(lst)) return -1;

  lst = scm_cons(argv[0], lst);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_num_eq_P_lst(lst));
}

int
scm_subr_func_num_lt_P(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[1], argv[2]);
  if (scm_obj_null_p(lst)) return -1;

  lst = scm_cons(argv[0], lst);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_num_lt_P_lst(lst));
}

int
scm_subr_func_num_gt_P(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[1], argv[2]);
  if (scm_obj_null_p(lst)) return -1;

  lst = scm_cons(argv[0], lst);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_num_gt_P_lst(lst));
}

int
scm_subr_func_num_le_P(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[1], argv[2]);
  if (scm_obj_null_p(lst)) return -1;

  lst = scm_cons(argv[0], lst);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_num_le_P_lst(lst));
}

int
scm_subr_func_num_ge_P(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[1], argv[2]);
  if (scm_obj_null_p(lst)) return -1;

  lst = scm_cons(argv[0], lst);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_num_ge_P_lst(lst));
}

int
scm_subr_func_zero_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_zero_P(argv[0]));
}

int
scm_subr_func_positive_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_positive_P(argv[0]));
}

int
scm_subr_func_negative_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_negative_P(argv[0]));
}

int
scm_subr_func_odd_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_odd_P(argv[0]));
}

int
scm_subr_func_even_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_even_P(argv[0]));
}

int
scm_subr_func_max(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[0], argv[1]);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_max_lst(lst));
}

int
scm_subr_func_min(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[0], argv[1]);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_min_lst(lst));
}

int
scm_subr_func_plus(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_plus_lst(argv[0]));
}

int
scm_subr_func_mul(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_mul_lst(argv[0]));
}

int
scm_subr_func_minus(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[0], argv[1]);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_minus_lst(lst));
}

int
scm_subr_func_div(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("/: not implemented", 0);
  return -1;
}

int
scm_subr_func_abs(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_num_abs(argv[0]));
}

int
scm_subr_func_floor_div(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj val[2] = { SCM_OBJ_INIT, SCM_OBJ_INIT };
  int r;

  SCM_REFSTK_INIT_REG(&subr);
  SCM_REFSTK_REG_ARY(val, sizeof(val)/sizeof(val[0]));

  r = scm_capi_floor_div(argv[0], argv[1],
                         SCM_CSETTER_L(val[0]), SCM_CSETTER_L(val[1]));
  if (r < 0) return -1;

  return scm_return_val(val, sizeof(val)/sizeof(val[0]));
}

int
scm_subr_func_floor_quo(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_floor_quo(argv[0], argv[1]));
}

int
scm_subr_func_floor_rem(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_floor_rem(argv[0], argv[1]));
}

int
scm_subr_func_truncate_div(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj val[2] = { SCM_OBJ_INIT, SCM_OBJ_INIT };
  int r;

  SCM_REFSTK_INIT_REG(&subr);
  SCM_REFSTK_REG_ARY(val, sizeof(val)/sizeof(val[0]));

  r = scm_capi_truncate_div(argv[0], argv[1],
                            SCM_CSETTER_L(val[0]), SCM_CSETTER_L(val[1]));
  if (r < 0) return -1;

  return scm_return_val(val, sizeof(val)/sizeof(val[0]));
}

int
scm_subr_func_truncate_quo(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_truncate_quo(argv[0], argv[1]));
}

int
scm_subr_func_truncate_rem(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_truncate_rem(argv[0], argv[1]));
}


/*******************************************************************/
/*  Booleans                                                       */
/*******************************************************************/

int
scm_subr_func_not(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_not(argv[0]));
}

int
scm_subr_func_boolean_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_boolean_P(argv[0]));
}


/*******************************************************************/
/*  Pair and Lists                                                 */
/*******************************************************************/

int
scm_subr_func_pair_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_pair_P(argv[0]));
}

int
scm_subr_func_cons(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_cons(argv[0], argv[1]));
}

int
scm_subr_func_car(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_car(argv[0]));
}

int
scm_subr_func_cdr(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_cdr(argv[0]));
}

int
scm_subr_func_set_car_i(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_set_car_i(argv[0], argv[1]));
}

int
scm_subr_func_set_cdr_i(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_set_cdr_i(argv[0], argv[1]));
}

int
scm_subr_func_null_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_nil_P(argv[0]));
}

int
scm_subr_func_list_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_list_P(argv[0]));
}

int
scm_subr_func_make_list(ScmObj subr, int argc, const ScmObj *argv)
{
  if (argc > 2) {
    scm_capi_error("make-list: too many arguments", 0);
    return -1;
  }

  return scm_return_val_1(scm_api_make_list(argv[0],
                                            ((argc > 1) ?
                                             argv[1] : SCM_OBJ_NULL)));
}

int
scm_subr_func_list(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val(argv, 1);
}

int
scm_subr_func_length(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_length(argv[0]));
}

int
scm_subr_func_append(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_append_lst(argv[0]));
}

int
scm_subr_func_reverse(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_reverse(argv[0]));
}

int
scm_subr_func_list_tail(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_list_tail(argv[0], argv[1]));
}

int
scm_subr_func_list_ref(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_list_ref(argv[0], argv[1]));
}

int
scm_subr_func_list_set_i(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_list_set_i(argv[0], argv[1], argv[2]));
}

int
scm_subr_func_memq(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_memq(argv[0], argv[1]));
}

int
scm_subr_func_memv(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_memv(argv[0], argv[1]));
}

int
scm_subr_func_member(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("member: not implemented", 0);
  return -1;
}

int
scm_subr_func_assq(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_assq(argv[0], argv[1]));
}

int
scm_subr_func_assv(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_assv(argv[0], argv[1]));
}

int
scm_subr_func_assoc(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("assoc: not implemented", 0);
  return -1;
}

int
scm_subr_func_list_copy(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_list_copy(argv[0]));
}


/*******************************************************************/
/*  Symbols                                                        */
/*******************************************************************/

int
scm_subr_func_symbol_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_symbol_P(argv[0]));
}

int
scm_subr_func_symbol_eq_P(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[1], argv[2]);
  if (scm_obj_null_p(lst)) return -1;

  lst = scm_cons(argv[0], lst);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_symbol_eq_P_lst(lst));
}

int
scm_subr_func_symbol_to_string(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_symbol_to_string(argv[0]));
}

int
scm_subr_func_string_to_symbol(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_string_to_symbol(argv[0]));
}


/*******************************************************************/
/*  Characters                                                     */
/*******************************************************************/

int
scm_subr_func_char_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_char_P(argv[0]));
}

int
scm_subr_func_char_eq_P(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[1], argv[2]);
  if (scm_obj_null_p(lst)) return -1;

  lst = scm_cons(argv[0], lst);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_char_eq_P_lst(lst));
}

int
scm_subr_func_char_lt_P(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_api_cons(argv[1], argv[2]);
  if (scm_obj_null_p(lst)) return -1;

  lst = scm_cons(argv[0], lst);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_char_lt_P_lst(lst));
}

int
scm_subr_func_char_gt_P(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[1], argv[2]);
  if (scm_obj_null_p(lst)) return -1;

  lst = scm_cons(argv[0], lst);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_char_gt_P_lst(lst));
}

int
scm_subr_func_char_le_P(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[1], argv[2]);
  if (scm_obj_null_p(lst)) return -1;

  lst = scm_cons(argv[0], lst);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_char_le_P_lst(lst));
}

int
scm_subr_func_char_ge_P(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[1], argv[2]);
  if (scm_obj_null_p(lst)) return -1;

  lst = scm_cons(argv[0], lst);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_char_ge_P_lst(lst));
}

int
scm_subr_func_char_ci_eq_P(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("char-ci=?: not implemented", 0);
  return -1;
}

int
scm_subr_func_char_ci_lt_P(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("char-ci<?: not implemented", 0);
  return -1;
}

int
scm_subr_func_char_ci_gt_P(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("char-ci>?: not implemented", 0);
  return -1;
}

int
scm_subr_func_char_ci_le_P(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("char-ci<=?: not implemented", 0);
  return -1;
}

int
scm_subr_func_char_ci_ge_P(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("char-ci>=?: not implemented", 0);
  return -1;
}

int
scm_subr_func_char_alphabetic_P(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("char-alphabetic?: not implemented", 0);
  return -1;
}

int
scm_subr_func_char_numeric_P(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("char-numeric?: not implemented", 0);
  return -1;
}

int
scm_subr_func_char_whitespace_P(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("char-whitespace?: not implemented", 0);
  return -1;
}

int
scm_subr_func_char_upper_case_P(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("char-upper-case?: not implemented", 0);
  return -1;
}

int
scm_subr_func_char_lower_case_P(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("char-lower-case?: not implemented", 0);
  return -1;
}

int
scm_subr_func_digit_value(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("digit-value?: not implemented", 0);
  return -1;
}

int
scm_subr_func_char_to_integer(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_char_to_integer(argv[0]));
}

int
scm_subr_func_integer_to_char(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_capi_integer_to_char(argv[0], NULL));
}

int
scm_subr_func_char_upcase(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("char-upcase: not implemented", 0);
  return -1;
}

int
scm_subr_func_char_downcase(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("char-downcase: not implemented", 0);
  return -1;
}

int
scm_subr_func_char_foldcase(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("char-foldcase: not implemented", 0);
  return -1;
}


/*******************************************************************/
/*  Strings                                                        */
/*******************************************************************/

int
scm_subr_func_string_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_string_P(argv[0]));
}

int
scm_subr_func_make_string(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("make-string: not implemented", 0);
  return -1;
}

int
scm_subr_func_string(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_string_lst(argv[0]));
}

int
scm_subr_func_string_length(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_string_length(argv[0]));
}

int
scm_subr_func_string_bytesize(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_string_bytesize(argv[0]));
}

int
scm_subr_func_string_ref(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_string_ref(argv[0], argv[1]));
}

int
scm_subr_func_string_set_i(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_string_set_i(argv[0], argv[1], argv[2]));
}

int
scm_subr_func_string_eq_P(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[1], argv[2]);
  if (scm_obj_null_p(lst)) return -1;

  lst = scm_cons(argv[0], lst);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_string_eq_P_lst(lst));
}

int
scm_subr_func_string_ci_eq_P(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("string-ci=?: not implemented", 0);
  return -1;
}

int
scm_subr_func_string_lt_P(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_api_cons(argv[1], argv[2]);
  if (scm_obj_null_p(lst)) return -1;

  lst = scm_api_cons(argv[0], lst);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_string_lt_P_lst(lst));
}

int
scm_subr_func_string_ci_lt_P(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("string-ci<?: not implemented", 0);
  return -1;
}

int
scm_subr_func_string_gt_P(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[1], argv[2]);
  if (scm_obj_null_p(lst)) return -1;

  lst = scm_cons(argv[0], lst);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_string_gt_P_lst(lst));
}

int
scm_subr_func_string_ci_gt_P(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("string-ci>?: not implemented", 0);
  return -1;
}

int
scm_subr_func_string_le_P(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[1], argv[2]);
  if (scm_obj_null_p(lst)) return -1;

  lst = scm_cons(argv[0], lst);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_string_le_P_lst(lst));
}

int
scm_subr_func_string_ci_le_P(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("string-ci<=?: not implemented", 0);
  return -1;
}

int
scm_subr_func_string_ge_P(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &lst);

  lst = scm_cons(argv[1], argv[2]);
  if (scm_obj_null_p(lst)) return -1;

  lst = scm_cons(argv[0], lst);
  if (scm_obj_null_p(lst)) return -1;

  return scm_return_val_1(scm_api_string_ge_P_lst(lst));
}

int
scm_subr_func_string_ci_ge_P(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("string-ci>=?: not implemented", 0);
  return -1;
}

int
scm_subr_func_string_upcase(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("string-upcase: not implemented", 0);
  return -1;
}

int
scm_subr_func_string_downcase(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("string-downcase: not implemented", 0);
  return -1;
}

int
scm_subr_func_string_foldcase(ScmObj subr, int argc, const ScmObj *argv)
{
  scm_capi_error("string-foldcase: not implemented", 0);
  return -1;
}

int
scm_subr_func_substring(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_substring(argv[0], argv[1], argv[2]));
}

int
scm_subr_func_string_append(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_string_append_lst(argv[0]));
}

int
scm_subr_func_string_to_list(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj start_end[2] = { SCM_OBJ_INIT,  SCM_OBJ_INIT };
  ssize_t r;

  SCM_REFSTK_INIT_REG(&subr);
  SCM_REFSTK_REG_ARY(start_end, sizeof(start_end)/sizeof(start_end[0]));

  r = scm_subr_list_to_cv(argv[1],
                          start_end, sizeof(start_end)/sizeof(start_end[0]));
  if (r < 0) return -1;

  return scm_return_val_1(scm_api_string_to_list(argv[0],
                                                 start_end[0], start_end[1]));
}

int
scm_subr_func_list_to_string(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_list_to_string(argv[0]));
}

int
scm_subr_func_string_copy(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj start_end[2] = { SCM_OBJ_INIT,  SCM_OBJ_INIT };
  ssize_t r;

  SCM_REFSTK_INIT_REG(&subr);
  SCM_REFSTK_REG_ARY(start_end, sizeof(start_end)/sizeof(start_end[0]));

  r = scm_subr_list_to_cv(argv[1],
                          start_end, sizeof(start_end)/sizeof(start_end[0]));
  if (r < 0) return -1;

  return scm_return_val_1(scm_api_string_copy(argv[0],
                                              start_end[0], start_end[1]));
}

int
scm_subr_func_string_copy_i(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj start_end[2] = { SCM_OBJ_INIT,  SCM_OBJ_INIT };
  ssize_t r;

  SCM_REFSTK_INIT_REG(&subr);
  SCM_REFSTK_REG_ARY(start_end, sizeof(start_end)/sizeof(start_end[0]));

  r = scm_subr_list_to_cv(argv[3],
                          start_end, sizeof(start_end)/sizeof(start_end[0]));
  if (r < 0) return -1;

  return scm_return_val_1(scm_api_string_copy_i(argv[0], argv[1], argv[2],
                                                start_end[0], start_end[1]));
}

int
scm_subr_func_string_fill_i(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj start_end[2] = { SCM_OBJ_INIT,  SCM_OBJ_INIT };
  ssize_t r;

  SCM_REFSTK_INIT_REG(&subr);
  SCM_REFSTK_REG_ARY(start_end, sizeof(start_end)/sizeof(start_end[0]));

  r = scm_subr_list_to_cv(argv[3],
                          start_end, sizeof(start_end)/sizeof(start_end[0]));
  if (r < 0) return -1;

  return scm_return_val_1(scm_api_string_fill_i(argv[0], argv[1],
                                                start_end[0], start_end[1]));
}


/*******************************************************************/
/*  Vectors                                                        */
/*******************************************************************/

int
scm_subr_func_vector_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_vector_P(argv[0]));
}

int
scm_subr_func_make_vector(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj fill = SCM_OBJ_INIT;
  ssize_t r;

  SCM_REFSTK_INIT_REG(&subr,
                      &fill);

  r = scm_subr_list_to_cv(argv[1], &fill, 1);
  if (r < 0) return -1;

  return scm_return_val_1(scm_api_make_vector(argv[0], fill));
}

int
scm_subr_func_vector(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_vector_lst(argv[0]));
}

int
scm_subr_func_vector_length(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_vector_length(argv[0]));
}

int
scm_subr_func_vector_ref(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_vector_ref(argv[0], argv[1]));
}

int
scm_subr_func_vector_set_i(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_vector_set_i(argv[0], argv[1], argv[2]));
}

int
scm_subr_func_vector_to_list(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj start_end[2] = { SCM_OBJ_INIT, SCM_OBJ_INIT };
  ssize_t r;

  SCM_REFSTK_INIT_REG(&subr);
  SCM_REFSTK_REG_ARY(start_end,  sizeof(start_end)/sizeof(start_end[0]));

  r = scm_subr_list_to_cv(argv[1],
                          start_end, sizeof(start_end)/sizeof(start_end[0]));
  if (r < 0) return -1;

  return scm_return_val_1(scm_api_vector_to_list(argv[0],
                                                 start_end[0], start_end[1]));
}

int
scm_subr_func_list_to_vector(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_list_to_vector(argv[0]));
}

int
scm_subr_func_vector_to_string(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj start_end[2] = { SCM_OBJ_INIT, SCM_OBJ_INIT };
  ssize_t r;

  SCM_REFSTK_INIT_REG(&subr);
  SCM_REFSTK_REG_ARY(start_end,  sizeof(start_end)/sizeof(start_end[0]));

  r = scm_subr_list_to_cv(argv[1],
                          start_end, sizeof(start_end)/sizeof(start_end[0]));
  if (r < 0) return -1;

  return scm_return_val_1(scm_api_vector_to_string(argv[0],
                                                   start_end[0], start_end[1]));
}

int
scm_subr_func_string_to_vector(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj start_end[2] = { SCM_OBJ_INIT, SCM_OBJ_INIT };
  ssize_t r;

  SCM_REFSTK_INIT_REG(&subr);
  SCM_REFSTK_REG_ARY(start_end,  sizeof(start_end)/sizeof(start_end[0]));

  r = scm_subr_list_to_cv(argv[1],
                          start_end, sizeof(start_end)/sizeof(start_end[0]));
  if (r < 0) return -1;

  return scm_return_val_1(scm_api_string_to_vector(argv[0],
                                                   start_end[0], start_end[1]));
}

int
scm_subr_func_vector_copy(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj start_end[2] = { SCM_OBJ_INIT, SCM_OBJ_INIT };
  ssize_t r;

  SCM_REFSTK_INIT_REG(&subr);
  SCM_REFSTK_REG_ARY(start_end,  sizeof(start_end)/sizeof(start_end[0]));

  r = scm_subr_list_to_cv(argv[1],
                          start_end, sizeof(start_end)/sizeof(start_end[0]));
  if (r < 0) return -1;

  return scm_return_val_1(scm_api_vector_copy(argv[0],
                                              start_end[0], start_end[1]));
}

int
scm_subr_func_vector_copy_i(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj start_end[2] = { SCM_OBJ_INIT, SCM_OBJ_INIT };
  ssize_t r;

  SCM_REFSTK_INIT_REG(&subr);
  SCM_REFSTK_REG_ARY(start_end,  sizeof(start_end)/sizeof(start_end[0]));

  r = scm_subr_list_to_cv(argv[3],
                          start_end, sizeof(start_end)/sizeof(start_end[0]));
  if (r < 0) return -1;

  return scm_return_val_1(scm_api_vector_copy_i(argv[0], argv[1], argv[2],
                                                start_end[0], start_end[1]));
}

int
scm_subr_func_vector_append(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_vector_append_lst(argv[0]));
}

int
scm_subr_func_vector_fill_i(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj start_end[2] = { SCM_OBJ_INIT, SCM_OBJ_INIT };
  ssize_t r;

  SCM_REFSTK_INIT_REG(&subr);
  SCM_REFSTK_REG_ARY(start_end,  sizeof(start_end)/sizeof(start_end[0]));

  r = scm_subr_list_to_cv(argv[2],
                          start_end, sizeof(start_end)/sizeof(start_end[0]));
  if (r < 0) return -1;

  return scm_return_val_1(scm_api_vector_fill_i(argv[0], argv[1],
                                                start_end[0], start_end[1]));
}


/*******************************************************************/
/*  Control features                                               */
/*******************************************************************/

int
scm_subr_func_procedure_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_procedure_P(argv[0]));
}

int
scm_subr_func_apply(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj arg = SCM_OBJ_INIT, prv = SCM_OBJ_INIT, cur = SCM_OBJ_INIT;
  ScmObj itr = SCM_OBJ_INIT, obj = SCM_OBJ_INIT, nxt = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &arg, &prv, &cur,
                      &itr, &obj, &nxt);

  if (!scm_procedure_p(argv[0])) {
    scm_capi_error("apply: procedure required, but got", 1, argv[0]);
    return -1;
  }

  arg = scm_cons(argv[1], argv[2]);
  if (scm_obj_null_p(arg)) return -1;

  prv = SCM_OBJ_NULL;
  for (itr = arg; scm_pair_p(itr); itr = nxt) {
    obj = scm_car(itr);
    nxt = scm_cdr(itr);

    if (scm_pair_p(nxt)) {
      cur = scm_cons(obj, SCM_NIL_OBJ);
      if (scm_obj_null_p(cur)) return -1;
    }
    else {
      if (!scm_pair_p(obj) && !scm_nil_p(obj)) {
        scm_capi_error("apply: list required, but got", 1, obj);
        return -1;
      }
      cur = obj;
    }

    if (scm_obj_null_p(prv))
      arg = cur;
    else
      scm_set_cdr(prv, cur);

    prv = cur;
  }

  if (scm_obj_null_p(itr)) return -1;

  return scm_trampolining(argv[0], arg, SCM_OBJ_NULL, SCM_OBJ_NULL);
}


/*******************************************************************/
/*  Exceptions                                                     */
/*******************************************************************/

int
scm_subr_func_raise(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_capi_raise(argv[0]);
}

int
scm_subr_func_raise_continuable(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_capi_raise_continuable(argv[0]);
}

int
scm_subr_func_error(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj r = SCM_OBJ_INIT;

  r = scm_api_error_lst(argv[0], argv[1]);
  if (scm_obj_null_p(r)) return -1;

  return 0;
}

int
scm_subr_func_error_object_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_error_object_P(argv[0]));
}

int
scm_subr_func_error_object_message(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_error_object_message(argv[0]));
}

int
scm_subr_func_error_object_irritants(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_error_object_irritants(argv[0]));
}

int
scm_subr_func_read_error_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_read_error_P(argv[0]));
}

int
scm_subr_func_file_error_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_file_error_P(argv[0]));
}


/*******************************************************************/
/*  Ports                                                          */
/*******************************************************************/

int
scm_subr_func_open_input_file(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_open_input_file(argv[0]));
}


/*******************************************************************/
/*  Input Output                                                   */
/*******************************************************************/


int
scm_subr_func_read(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_read(scm_pair_p(argv[0]) ?
                                       scm_car(argv[0]) : SCM_OBJ_NULL));
}

int
scm_subr_func_write(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_write(argv[0],
                                        (scm_pair_p(argv[1]) ?
                                         scm_car(argv[1]) : SCM_OBJ_NULL)));
}

int
scm_subr_func_write_shared(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_write_shared(argv[0],
                                               (scm_pair_p(argv[1]) ?
                                                scm_car(argv[1]) : SCM_OBJ_NULL)));
}

int
scm_subr_func_write_simple(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_write_simple(argv[0],
                                               (scm_pair_p(argv[1]) ?
                                                scm_car(argv[1]) : SCM_OBJ_NULL)));
}

int
scm_subr_func_display(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_display(argv[0],
                                          (scm_pair_p(argv[1]) ?
                                           scm_car(argv[1]) : SCM_OBJ_NULL)));
}

int
scm_subr_func_newline(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_newline(scm_pair_p(argv[0]) ?
                                          scm_car(argv[0]) : SCM_OBJ_NULL));
}

int
scm_subr_func_flush_output_port(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_flush_output_port(scm_pair_p(argv[1]) ?
                                                    scm_car(argv[1]) : SCM_OBJ_NULL));
}

int
scm_subr_func_eof_object_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_eof_object_P(argv[0]));
}


/*******************************************************************/
/*  Continuation                                                   */
/*******************************************************************/

int
scm_subr_func_callcc(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj cont = SCM_OBJ_INIT, args = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &cont, &args);

  if (argc != 1) {
    scm_capi_error("call/cc: 1 argumetn is require, but got ", 0);
    return -1;
  }

  cont = scm_make_continuation();
  if (scm_obj_null_p(cont)) return -1;

  args = scm_list(1, cont);
  if (scm_obj_null_p(args)) return -1;

  return scm_capi_trampolining(argv[0], args, SCM_OBJ_NULL, SCM_OBJ_NULL);
}


/*******************************************************************/
/*  Multiple Return Values                                         */
/*******************************************************************/

int
scm_subr_func_values(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val(argv, argc);
}

int
scm_subr_func_call_with_values(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_capi_trampolining(argv[0], SCM_NIL_OBJ, argv[1], SCM_OBJ_NULL);
}


/*******************************************************************/
/*  Eval                                                           */
/*******************************************************************/

int
scm_subr_func_eval_asm(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj code = SCM_OBJ_INIT;
  int r;

  SCM_REFSTK_INIT_REG(&subr,
                      &code);

  if (argc != 1) {
    /* TODO: change error message */
    scm_capi_error("eval-asm: 1 argument is require, but got ", 0);
    return -1;
  }

  if (scm_pair_p(argv[0])) {
    code = scm_make_assembler(SCM_OBJ_NULL);
    if (scm_obj_null_p(code)) return -1;

    code = scm_api_assemble(argv[0], code);
    if (scm_obj_null_p(code)) return -1;
  }
  else if (scm_iseq_p(argv[0])) {
    code = scm_make_assembler(argv[0]);
    if (scm_obj_null_p(code)) return -1;
  }
  else {
    scm_capi_error("eval-asm: argument is not pair or iseq", 1, argv[0]);
    return -1;
  }

  r = scm_asm_push(code, SCM_OPCODE_RETURN);
  if (r < 0) return -1;

  r = scm_asm_commit(code);
  if (r < 0) return -1;

  code = scm_make_closure(scm_asm_iseq(code), SCM_OBJ_NULL, 0);
  if (scm_obj_null_p(code)) return -1;

  return scm_trampolining(code, SCM_NIL_OBJ, SCM_OBJ_NULL, SCM_OBJ_NULL);
}

int
scm_subr_func_eval__post_compile(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj proc = SCM_OBJ_INIT;
  int r;

  SCM_REFSTK_INIT_REG(&subr,
                      &proc);

  r = scm_asm_push(argv[0], SCM_OPCODE_RETURN);
  if (r < 0) return -1;

  r = scm_asm_commit(argv[0]);
  if (r < 0) return -1;

  proc = scm_make_closure(scm_asm_iseq(argv[0]), SCM_OBJ_NULL, 0);
  if (scm_obj_null_p(proc)) return -1;

  return scm_trampolining(proc, SCM_NIL_OBJ, SCM_OBJ_NULL, SCM_OBJ_NULL);
}

int
scm_subr_func_eval(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj compile = SCM_OBJ_INIT, postproc = SCM_OBJ_INIT;
  ScmObj args = SCM_OBJ_INIT, cmpl = SCM_OBJ_INIT;
  int r;

  SCM_REFSTK_INIT_REG(&subr,
                      &compile, &postproc,
                      &args, &cmpl);

  r = scm_cached_global_var_ref(SCM_CACHED_GV_COMPILE, SCM_CSETTER_L(compile));
  if (r < 0) return -1;

  if (scm_obj_null_p(compile)) {
    scm_capi_error("unbound variable: compile", 0);
    return -1;
  }

  if (scm_nil_p(argv[1])) {
    cmpl = scm_make_compiler(SCM_OBJ_NULL);
    if (scm_obj_null_p(cmpl)) return -1;

    args = scm_list(2, argv[0], cmpl);
  }
  else {
    args = scm_cons(argv[0], argv[1]);
  }

  if (scm_obj_null_p(args)) return -1;

  postproc = scm_premade_procedure(SCM_PREMADE_PROC_EVAL__POST_COMPILE);
  return scm_capi_trampolining(compile, args, postproc, SCM_OBJ_NULL);
}


/*******************************************************************/
/*  System interface                                               */
/*******************************************************************/

int
scm_subr_func_eval_file__loop(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj port = SCM_OBJ_INIT, cmpl = SCM_OBJ_INIT, eval = SCM_OBJ_INIT;
  ScmObj exp = SCM_OBJ_INIT, args = SCM_OBJ_INIT, ret = SCM_OBJ_INIT;
  int r;

  SCM_REFSTK_INIT_REG(&subr,
                      &port, &cmpl, &eval,
                      &exp, &args, &ret);

  port = scm_car(argv[0]);
  if (scm_obj_null_p(port)) return -1;

  cmpl = scm_cdr(argv[0]);
  if (scm_obj_null_p(cmpl)) return -1;

  exp = scm_api_read(port);
  if (scm_obj_null_p(exp)) return -1;

  if (scm_eof_object_p(exp)) {
    return scm_return_val_1(SCM_UNDEF_OBJ);
  }

  r = scm_cached_global_var_ref(SCM_CACHED_GV_EVAL, SCM_CSETTER_L(eval));
  if (r < 0) return -1;

  if (scm_obj_null_p(eval)) {
    scm_capi_error("unbound variable: eval", 0);
    return -1;
  }

  args = scm_list(2, exp, cmpl);

  return scm_capi_trampolining(eval, args, subr, argv[0]);
}

static int
scm_eval_file(ScmObj subr, ScmObj path, ScmObj env)
{
  ScmObj port = SCM_OBJ_INIT, cmpl = SCM_OBJ_INIT;
  ScmObj loop = SCM_OBJ_INIT, args = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr, &path, &env,
                      &port, &cmpl,
                      &loop, &args);


  port = scm_api_open_input_file(path);
  if (scm_obj_null_p(port)) return -1;

  cmpl = scm_make_compiler(env);
  if (scm_obj_null_p(cmpl)) return -1;

  args = scm_cons(port, cmpl);
  if (scm_obj_null_p(args)) return -1;

  args = scm_list(2, args, SCM_UNDEF_OBJ);
  if (scm_obj_null_p(args)) return -1;

  loop = scm_premade_procedure(SCM_PREMADE_PROC_EVAL_FILE__LOOP);
  return scm_trampolining(loop, args, SCM_OBJ_NULL, SCM_OBJ_NULL);
}

int
scm_subr_func_load(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj path = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&subr,
                      &path);

  /* XXX: 現状、load の第 2 引数は無視する */

  path = scm_search_load_file(argv[0]);
  if (scm_obj_null_p(path)) return -1;

  if (scm_false_p(path)) {
    scm_capi_error("load: failed to find file", 1, argv[0]);
    return -1;
  }

  return scm_eval_file(subr, path, SCM_OBJ_NULL);
}

int
scm_subr_func_file_exists_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_file_exists_P(argv[0]));
}

int
scm_subr_func_delete_file(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_delete_file(argv[0]));
}

int
scm_subr_func_exit(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_exit(scm_pair_p(argv[0]) ?
                                       scm_car(argv[0]) : SCM_OBJ_NULL));
}


/*******************************************************************/
/*  format                                                         */
/*******************************************************************/

int
scm_subr_func_format(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_format_lst(argv[0], argv[1]));
}


/*******************************************************************/
/*  Modules                                                        */
/*******************************************************************/

int
scm_subr_func_module_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_module_P(argv[0]));
}

int
scm_subr_func_module_name(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_module_name(argv[0]));
}

int
scm_subr_func_module_export(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_module_export(argv[0], argv[1]));
}


/*******************************************************************/
/*  Internals (Record)                                             */
/*******************************************************************/

int
scm_subr_func_record_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_record_P(argv[0]));
}

int
scm_subr_func_make_record_type(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_make_record_type(argv[0]));
}

int
scm_subr_func_make_record(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_make_record(argv[0], argv[1], argv[2]));
}

int
scm_subr_func_record_type(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_record_type(argv[0]));
}

int
scm_subr_func_record_ref(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_record_ref(argv[0], argv[1]));
}

int
scm_subr_func_record_set_i(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_record_set_i(argv[0], argv[1], argv[2]));
}


/*******************************************************************/
/*  Internals (Compiler)                                           */
/*******************************************************************/

int
scm_subr_func_make_assembler(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_make_assebmler(scm_pair_p(argv[0]) ?
                                                 scm_car(argv[0]) : SCM_OBJ_NULL));
}

int
scm_subr_func_assembler_assign_label_id_i(ScmObj subr,
                                          int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_assembler_assgin_label_id_i(argv[0]));
}

int
scm_subr_func_assembler_push_i(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_assembler_push_i_cv(argv[0],
                                                      argv + 1,
                                                      (size_t)(argc - 1)));
}

int
scm_subr_func_assembler_commit_i(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_assembler_commit_i(argv[0]));
}

int
scm_subr_func_compiler_P(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_compiler_P(argv[0]));
}

int
scm_subr_func_make_compiler(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_make_compiler(scm_pair_p(argv[0]) ?
                                                scm_car(argv[0]) : SCM_OBJ_NULL));
}

int
scm_subr_func_compiler_base_env(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_compiler_base_env(argv[0]));
}

int
scm_subr_func_compiler_select_base_env_i(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_compiler_select_base_env_i(argv[0], argv[1]));
}

int
scm_subr_func_compiler_select_module_i(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_compiler_select_module_i(argv[0], argv[1]));
}

int
scm_subr_func_compiler_current_expr(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_compiler_current_expr(argv[0]));
}

int
scm_subr_func_compiler_select_expr_i(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_compiler_select_expr_i(argv[0], argv[1]));
}

int
scm_subr_func_global_variable_bind(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj obj = SCM_OBJ_INIT;
  bool export;
  int r;

  SCM_REFSTK_INIT_REG(&subr,
                      &obj);

  export = false;
  if (scm_pair_p(argv[3])) {
    obj = scm_car(argv[3]);
    if (scm_obj_null_p(obj)) return -1;

    if (!scm_boolean_p(obj)) {
      scm_capi_error("global-variable-bind: invalid argument", 1, obj);
      return -1;
    }

    export = scm_true_object_p(obj);
  }

  r = scm_capi_define_global_var(argv[0], argv[1], argv[2], export);
  if (r < 0) return -1;

  return scm_return_val_1(SCM_UNDEF_OBJ);
}

int
scm_subr_func_global_syntax_bind(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj obj = SCM_OBJ_INIT;
  bool export;
  int r;

  SCM_REFSTK_INIT_REG(&subr,
                      &obj);

  export = false;
  if (scm_pair_p(argv[3])) {
    obj = scm_car(argv[3]);
    if (scm_obj_null_p(obj)) return -1;

    if (!scm_boolean_p(obj)) {
      scm_capi_error("global-syntax-bind: invalid argument", 1, obj);
      return -1;
    }

    export = scm_true_object_p(obj);
  }

  r = scm_capi_define_global_syx(argv[0], argv[1], argv[2], export);
  if (r < 0) return -1;

  return scm_return_val_1(SCM_UNDEF_OBJ);
}

int
scm_subr_func_global_syntax_ref(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj val = SCM_OBJ_INIT;
  int r;

  SCM_REFSTK_INIT_REG(&subr,
                      &val);

  r = scm_capi_refer_global_syx(argv[0], argv[1], SCM_CSETTER_L(val));
  if (r < 0) return -1;

  if (scm_obj_null_p(val)) {
    if (scm_nil_p(argv[2])) {
      scm_capi_error("unbound syntax", 1, argv[1]);
      return -1;
    }

    val = scm_car(argv[2]);
    if (scm_obj_null_p(val)) return -1;
  }

  return scm_return_val_1(val);
}

int
scm_subr_func_compile_qq_template(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_compile_qq_template(argv[0]));
}

int
scm_subr_func_substitute_qq_template(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_substitute_qq_template_lst(argv[0], argv[1]));
}

int
scm_subr_func_qq_template_num_of_unquoted(ScmObj subr,
                                          int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_qq_template_num_of_unquoted(argv[0]));
}

int
scm_subr_func_qq_template_unquoted(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_qq_template_unquoted(argv[0], argv[1]));
}


/*******************************************************************/
/*  Internals (dynamic environment)                                */
/*******************************************************************/

int
scm_subr_func_push_exception_handler(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_push_exception_handler(argv[0]));
}

int
scm_subr_func_pop_exception_handler(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_pop_exception_handler());
}

int
scm_subr_func_push_dynamic_bindings(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_push_dynamic_bindings(argv[0]));
}

int
scm_subr_func_pop_dynamic_bindings(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_pop_dynamic_bindings());
}

int
scm_subr_func_push_dynamic_wind_handler(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_push_dynamic_wind_handler(argv[0], argv[1]));
}

int
scm_subr_func_pop_dynamic_wind_handler(ScmObj subr, int argc, const ScmObj *argv)
{
  return scm_return_val_1(scm_api_pop_dynamic_wind_handler());
}


/*******************************************************************/
/*  Internals (commands)                                           */
/*******************************************************************/

static int
scm_subr_func_repl__post_load(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj proc = SCM_OBJ_INIT;
  int r;

  SCM_REFSTK_INIT_REG(&subr,
                      &proc);

  r = scm_refer_global_var_cstr((const char *[]){ "scythe", "repl"}, 2,
                                "read-eval-print-loop", SCM_CSETTER_L(proc));
  if (r < 0) return -1;

  if (scm_obj_null_p(proc)) {
    scm_error("unbound variable: read-eval-print-loop", 0);
    return -1;
  }

  return scm_capi_trampolining(proc, SCM_NIL_OBJ, SCM_OBJ_NULL, SCM_OBJ_NULL);
}

int
scm_subr_func_repl(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj proc = SCM_OBJ_INIT, arg = SCM_OBJ_INIT, post = SCM_OBJ_INIT;
  int r;

  SCM_REFSTK_INIT_REG(&subr,
                      &proc, &arg, &post);

  /* TODO:
   *   load プロシージャを使っているため、repl の呼び出しの度にファイルがロー
   *   ドされている。module のロード機能が実装し、一度だけファイルをロードす
   *   るようにする
   */
  r = scm_refer_global_var_cstr((const char *[]){ "scythe", "base"}, 2,
                                "load", SCM_CSETTER_L(proc));
  if (r < 0) return -1;

  if (scm_obj_null_p(proc)) {
    scm_error("unbound variable: load", 0);
    return -1;
  }

  arg = scm_make_string_from_cstr("scythe/repl", SCM_ENC_SRC);
  if (scm_obj_null_p(arg)) return -1;

  arg = scm_cons(arg, SCM_NIL_OBJ);
  if (scm_obj_null_p(arg)) return -1;

  post = scm_make_subrutine(scm_subr_func_repl__post_load, 1, 0,
                           scm_proc_env(subr));
  if (scm_obj_null_p(post)) return -1;

  return scm_capi_trampolining(proc, arg, post, SCM_OBJ_NULL);
}

int
scm_subr_func_exec_file(ScmObj subr, int argc, const ScmObj *argv)
{
  /* TODO:
   *
   * argv[0] で指定されているファイル名が - (ハイフン) の場合、標準入力から
   * S 式よ読み取って実行する。
   *
   * ファイルの内容を実行した後、(main) モジュールにグローバル変数 main がプ
   * ロシージャに束縛されている場合、そのプロシージャを呼び出す。そして、そ
   * のの戻り値はプロセスの exit status にする。
   */

  return scm_eval_file(subr, argv[0], SCM_OBJ_NULL);
}


int
scm_subr_func_eval_string(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj port = SCM_OBJ_INIT, exp = SCM_OBJ_INIT, eval = SCM_OBJ_INIT;
  ScmObj args = SCM_OBJ_INIT;
  int r;

  SCM_REFSTK_INIT_REG(&subr,
                      &port, &exp, &eval,
                      &args);

  port = scm_api_open_input_string(argv[0]);
  if (scm_obj_null_p(port)) return -1;

  exp = scm_api_read(port);
  if (scm_obj_null_p(exp)) return -1;

  r = scm_cached_global_var_ref(SCM_CACHED_GV_EVAL, SCM_CSETTER_L(eval));
  if (r < 0) return -1;

  if (scm_obj_null_p(eval)) {
    scm_capi_error("unbound variable: eval", 0);
    return -1;
  }

  args = scm_cons(exp, SCM_NIL_OBJ);
  if (scm_obj_null_p(args)) return -1;

  return scm_capi_trampolining(eval, args, SCM_OBJ_NULL, SCM_OBJ_NULL);
}

static int
scm_subr_func_compile_file__postproc(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj iseq = SCM_OBJ_INIT, port = SCM_OBJ_INIT, val = SCM_OBJ_INIT;
  void *marshaled = NULL;
  size_t size;
  int r;

  SCM_REFSTK_INIT_REG(&subr,
                      &iseq, &port);

  val = SCM_OBJ_NULL;

  iseq = scm_asm_iseq(argv[1]);
  if (scm_obj_null_p(iseq)) goto end;

  marshaled = scm_marshal(&size, iseq, SCM_OBJ_NULL);
  if (marshaled == NULL) goto end;

  port = scm_api_open_binary_output_file(argv[0]);
  if (scm_obj_null_p(port)) goto end;

  r = scm_write_cbytes(marshaled, size, port);
  if (r < 0) goto end;

  val = SCM_UNDEF_OBJ;

 end:
  if (marshaled != NULL) scm_free(marshaled);
  if (scm_obj_not_null_p(port)) scm_close_port(port);
  return scm_return_val_1(val);
}

int
scm_subr_func_compile_file(ScmObj subr, int argc, const ScmObj *argv)
{
  ScmObj proc = SCM_OBJ_INIT, mod = SCM_OBJ_INIT, args = SCM_OBJ_INIT;
  ScmObj out = SCM_OBJ_INIT, postproc = SCM_OBJ_INIT;
  int r;

  SCM_REFSTK_INIT_REG(&subr,
                      &proc, &mod, &args,
                      &out, &postproc);

  out = (scm_pair_p(argv[1]) ?
         scm_car(argv[1]) : scm_format_cstr("~a.out", argv[0]));
  if (scm_obj_null_p(out)) return -1;

  r = scm_find_module_cstr((const char *[]){"main"}, 1, SCM_CSETTER_L(mod));
  if (r < 0) return -1;

  if (scm_obj_null_p(mod)) {
    scm_error("failed to compile file: inexistent module: (main)", 0);
    return -1;
  }

  r = scm_refer_global_var_cstr((const char *[]){ "scythe", "internal", "compile"},
                                3, "compile-file", SCM_CSETTER_L(proc));
  if (r < 0) return -1;

  if (scm_obj_null_p(proc)) {
    scm_error("failed to compile file: unbound variable: compile-file", 0);
    return -1;
  }

  args = scm_list(2, argv[0], mod);
  if (scm_obj_null_p(args)) return -1;

  postproc = scm_make_subrutine(scm_subr_func_compile_file__postproc,
                                2, 0, SCM_OBJ_NULL);
  if (scm_obj_null_p(postproc)) return -1;

  return scm_capi_trampolining(proc, args, postproc, out);
}
