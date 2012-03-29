#ifndef INCLUDE_VM_H__
#define INCLUDE_VM_H__

#include <stdint.h>

typedef struct ScmBedrockRec ScmBedrock;
typedef struct ScmVMRec ScmVM;

#define SCM_VM(obj) ((ScmVM *)(obj))

#include "object.h"
#include "memory.h"
#include "reference.h"
#include "api.h"

/***************************************************************************/
/*  ScmBedrock                                                             */
/***************************************************************************/

typedef enum {
  SCM_BEDROCK_ERR_NONE,
  SCM_BEDROCK_ERR_FATAL,
  SCM_BEDROCK_ERR_ERROR,
} SCM_BEDROCK_ERROR_TYPE_T;

extern ScmBedrock *scm_bedrock__current_br;

struct ScmBedrockRec {
  /*** C Lang Stack ***/
  ScmRefStack *ref_stack;

  /*** Error Status ***/
  struct {
    SCM_BEDROCK_ERROR_TYPE_T type;
    char *message;
  } err;

  SCM_ENC_T encoding;
};

ScmBedrock *scm_bedrock_new(void);
void scm_bedrock_end(ScmBedrock *br);
void scm_bedrock_clean(ScmBedrock *br);
void scm_bedrock_fatal(ScmBedrock *br, const char *msg);
void scm_bedrock_fatal_fmt(ScmBedrock *br, const char *msgfmt, va_list ap);
bool scm_bedrock_fatal_p(ScmBedrock *br);
bool scm_bedrock_error_p(ScmBedrock *br);

inline ScmBedrock *
scm_bedrock_current_br(void)
{
  return scm_bedrock__current_br;
}

inline void
scm_bedrock_change_current_br(ScmBedrock *br)
{
  scm_bedrock__current_br = br;
}

inline SCM_ENC_T
scm_bedrock_encoding(ScmBedrock *br)
{
  scm_assert(br != NULL);
  return br->encoding;
}


/***************************************************************************/
/*  ScmVM                                                                  */
/***************************************************************************/

extern ScmTypeInfo SCM_VM_TYPE_INFO;
extern ScmObj scm_vm__current_vm;
  /* vm.c の外部が scm_vm__current_vm を直接参照するのは禁止。
     scm_vm_current_vm() 経由で取得すること。 */

struct ScmVMRec {
  ScmObjHeader header;

  ScmBedrock *bedrock;
  ScmMem *mem;

  struct {
    ScmObj symtbl;                /* Symbol Table */
    ScmObj gloctbl;

    struct {
      ScmObj in;
      ScmObj out;
      ScmObj err;
    } stdio;

    struct {
      ScmObj in;
      ScmObj out;
    } curio;
  } ge;


  /*** VM Stack ***/
  ScmObj *stack;
  size_t stack_size;

  /*** VM Registers ***/
  struct {
    ScmObj *sp;                   /* stack pointer */
    ScmObj *fp;                   /* frame pointer */
    ScmObj cp;                    /* closure pointer */
    uint8_t *ip;                  /* instruction pointer */
    ScmObj isp;                   /* instruction sequence object */
    ScmObj val;                   /* value register */
  } reg;

  /*** Constant Values ***/
  struct {
    ScmObj nil;
    ScmObj eof;
    ScmObj b_true;
    ScmObj b_false;
  } cnsts;

  /*** Trampolining ***/
  struct {
    ScmObj code;
  } trmp;
};

/* private functions ******************************************************/

#ifdef SCM_UNIT_TEST

int scm_vm_setup_singletons(ScmObj vm);
void scm_vm_clean_singletons(ScmObj vm);
int scm_vm_setup_global_env(ScmObj vm);
void scm_vm_clean_global_env(ScmObj vm);
void scm_vm_clean_eval_env(ScmObj vm);

void scm_vm_stack_push(ScmObj vm, ScmObj elm);
ScmObj scm_vm_stack_pop(ScmObj vm);
void scm_vm_stack_shorten(ScmObj vm, size_t n);
void scm_vm_stack_shift(ScmObj vm, size_t nelm, size_t nshift);

void scm_vm_return_to_caller(ScmObj vm, uint32_t nr_arg);

ScmObj scm_vm_make_trampolining_code(ScmObj vm, ScmObj clsr,
                                     ScmObj args, uint32_t nr_arg_cf,
                                     ScmObj callback);

void scm_vm_op_call(ScmObj vm,
                    uint32_t nr_arg, uint32_t nr_arg_cf, bool tail_p);
void scm_vm_op_immval(ScmObj vm, size_t immv_idx);
void scm_vm_op_push(ScmObj vm);
void scm_vm_op_frame(ScmObj vm);
void scm_vm_op_return(ScmObj vm, uint32_t nr_arg);
void scm_vm_op_gref(ScmObj vm, size_t immv_idx);
void scm_vm_op_gdef(ScmObj vm, size_t immv_idx);
void scm_vm_op_gset(ScmObj vm, size_t immv_idx);
void scm_vm_op_jmp(ScmObj vm, int32_t dst);

#endif

/* public functions ******************************************************/

void scm_vm_initialize(ScmObj vm, ScmBedrock *bedrock);
int scm_vm_init_scmobjs(ScmObj vm);
void scm_vm_finalize(ScmObj vm);
ScmObj scm_vm_new(void);
void scm_vm_end(ScmObj vm);

void scm_vm_setup_system(ScmObj vm);
void scm_vm_run(ScmObj vm, ScmObj iseq);

int scm_vm_setup_trampolining(ScmObj vm, ScmObj target,
                              ScmObj args, int nr_arg_cf,
                              ScmObj (*callback)(int argc, ScmObj *argv));

void scm_vm_gc_initialize(ScmObj obj, ScmObj mem);
void scm_vm_gc_finalize(ScmObj obj);
int scm_vm_gc_accept(ScmObj obj, ScmObj mem, ScmGCRefHandlerFunc handler);

inline ScmObj
scm_vm_current_input_port(ScmObj vm)
{
  scm_assert_obj_type(vm, &SCM_VM_TYPE_INFO);

  return SCM_VM(vm)->ge.curio.in;
}

inline ScmObj
scm_vm_current_output_port(ScmObj vm)
{
  scm_assert_obj_type(vm, &SCM_VM_TYPE_INFO);

  return SCM_VM(vm)->ge.curio.out;
}

inline ScmObj
scm_vm_current_vm(void)
{
  return scm_vm__current_vm;
}

inline void
scm_vm_change_current_vm(ScmObj vm)
{
  scm_vm__current_vm = vm;
  if (scm_obj_not_null_p(vm))
    scm_bedrock_change_current_br(SCM_VM(vm)->bedrock);
}


inline ScmMem *
scm_vm_current_mm(void)
{
  return SCM_VM(scm_vm__current_vm)->mem;
}

inline ScmObj
scm_vm_current_symtbl(void)
{
  return SCM_VM(scm_vm__current_vm)->ge.symtbl;
}

inline ScmObj
scm_vm_current_gloctbl(void)
{
  return SCM_VM(scm_vm__current_vm)->ge.gloctbl;
}

inline ScmObj
scm_vm_nil_instance(void)
{
  return SCM_VM(scm_vm__current_vm)->cnsts.nil;
}

inline ScmObj
scm_vm_eof_instance(void)
{
  return SCM_VM(scm_vm__current_vm)->cnsts.eof;
}

inline ScmObj
scm_vm_bool_true_instance(void)
{
  return SCM_VM(scm_vm__current_vm)->cnsts.b_true;
}

inline ScmObj
scm_vm_bool_false_instance(void)
{
  return SCM_VM(scm_vm__current_vm)->cnsts.b_false;
}

#endif /* INCLUDE_VM_H__ */
