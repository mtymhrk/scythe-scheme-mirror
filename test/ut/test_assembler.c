#include "assembler.c"

#include "scythe/object.h"
#include "scythe/vm.h"
#include "scythe/refstk.h"
#include "scythe/assembler.h"
#include "scythe/compiler.h"
#include "scythe/iseq.h"

#include "test.h"

TEST_GROUP(assembler);

static ScmScythe *scy;
static ScmRefStackInfo rsi;

TEST_SETUP(assembler)
{
  scy = ut_scythe_setup(false);
  scm_ref_stack_save(&rsi);
}

TEST_TEAR_DOWN(assembler)
{
  scm_ref_stack_restore(&rsi);
  ut_scythe_tear_down(scy);
}

static ScmObj
make_iseq(const char *asmbl)
{
  ScmObj iseq = SCM_OBJ_INIT, lst = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&iseq, &lst);

  lst = ut_read_cstr(asmbl);

  iseq = scm_assemble(scm_unprintable_asm(lst), SCM_OBJ_NULL);
  TEST_ASSERT_TRUE(scm_iseq_p(iseq));

  return iseq;
}

static void
check_following(scm_byte_t *ip)
{
  scm_opcode_t op;

  op = scm_external_opcode(SCM_VMINST_GET_OP(ip));
  TEST_ASSERT_EQUAL_INT(SCM_OPCODE_NOP, op);
}

static void
test_assemble_noopd(const char *asmbl, uint8_t code)
{
  ScmObj iseq;
  scm_byte_t *ip;
  scm_opcode_t actual_op;

  SCM_REFSTK_INIT_REG(&iseq);

  iseq = make_iseq(asmbl);
  ip = scm_iseq_to_ip(iseq);

  actual_op = scm_external_opcode(SCM_VMINST_GET_OP(ip));
  SCM_VMINST_FETCH_OPD_NOOPD(ip);

  TEST_ASSERT_EQUAL_INT(code, actual_op);

  check_following(ip);
}

static void
test_assemble_obj(const char *asmbl, uint8_t code, ScmObj obj)
{
  ScmObj iseq = SCM_OBJ_INIT, actual_immv = SCM_OBJ_INIT;
  scm_byte_t *ip;
  scm_opcode_t actual_op;

  SCM_REFSTK_INIT_REG(&obj,
                      &iseq, &actual_immv);

  iseq = make_iseq(asmbl);
  ip = scm_iseq_to_ip(iseq);

  actual_op = scm_external_opcode(SCM_VMINST_GET_OP(ip));
  SCM_VMINST_FETCH_OPD_OBJ(ip, actual_immv);

  TEST_ASSERT_EQUAL_INT(code, actual_op);
  TEST_ASSERT_SCM_EQUAL(obj, actual_immv);

  check_following(ip);
}

static void
test_assemble_obj_obj(const char *asmbl, uint8_t code, ScmObj obj1, ScmObj obj2)
{
  ScmObj iseq = SCM_OBJ_INIT;
  ScmObj actual_immv1 = SCM_OBJ_INIT, actual_immv2 = SCM_OBJ_INIT;
  scm_byte_t *ip;
  scm_opcode_t actual_op;

  SCM_REFSTK_INIT_REG(&obj1, &obj2,
                      &iseq, &actual_immv1, &actual_immv2);

  iseq = make_iseq(asmbl);
  ip = scm_iseq_to_ip(iseq);

  actual_op = scm_external_opcode(SCM_VMINST_GET_OP(ip));
  SCM_VMINST_FETCH_OPD_OBJ_OBJ(ip, actual_immv1, actual_immv2);

  TEST_ASSERT_EQUAL_INT(code, actual_op);
  TEST_ASSERT_SCM_EQUAL(obj1, actual_immv1);
  TEST_ASSERT_SCM_EQUAL(obj2, actual_immv2);

  check_following(ip);
}

static void
test_assemble_si(const char *asmbl, uint8_t code, int si)
{
  ScmObj iseq = SCM_OBJ_INIT;
  scm_byte_t *ip;
  scm_opcode_t actual_op;
  int actual_si;

  SCM_REFSTK_INIT_REG(&iseq);

  iseq = make_iseq(asmbl);
  ip = scm_iseq_to_ip(iseq);

  actual_op = scm_external_opcode(SCM_VMINST_GET_OP(ip));
  SCM_VMINST_FETCH_OPD_SI(ip, actual_si);

  TEST_ASSERT_EQUAL_INT(code, actual_op);
  TEST_ASSERT_EQUAL_INT(si, actual_si);

  check_following(ip);
}

static void
test_assemble_si_si(const char *asmbl, uint8_t code, int si1, int si2)
{
  ScmObj iseq = SCM_OBJ_INIT;
  scm_byte_t *ip;
  scm_opcode_t actual_op;
  int actual_si1, actual_si2;

  SCM_REFSTK_INIT_REG(&iseq);

  iseq = make_iseq(asmbl);
  ip = scm_iseq_to_ip(iseq);

  actual_op = scm_external_opcode(SCM_VMINST_GET_OP(ip));
  SCM_VMINST_FETCH_OPD_SI_SI(ip, actual_si1, actual_si2);

  TEST_ASSERT_EQUAL_INT(code, actual_op);
  TEST_ASSERT_EQUAL_INT(si1, actual_si1);
  TEST_ASSERT_EQUAL_INT(si2, actual_si2);

  check_following(ip);
}

static void
test_assemble_iof(const char *asmbl, uint8_t code, int si)
{
  ScmObj iseq = SCM_OBJ_INIT;
  scm_byte_t *ip;
  scm_opcode_t actual_op;
  int actual_si;

  SCM_REFSTK_INIT_REG(&iseq);

  iseq = make_iseq(asmbl);
  ip = scm_iseq_to_ip(iseq);

  actual_op = scm_external_opcode(SCM_VMINST_GET_OP(ip));
  SCM_VMINST_FETCH_OPD_IOF(ip, actual_si);

  TEST_ASSERT_EQUAL_INT(code, actual_op);
  TEST_ASSERT_EQUAL_INT(si, actual_si);

  check_following(ip);
}

static void
test_assemble_pinst_qqtemplate(const char *asmbl,
                               uint8_t code, const char *tmpl)
{
  ScmObj iseq = SCM_OBJ_INIT, t = SCM_OBJ_INIT, qq = SCM_OBJ_INIT;
  ScmObj actual_immv = SCM_OBJ_INIT;
  scm_byte_t *ip;
  scm_opcode_t actual_op;
  bool cmp;

  SCM_REFSTK_INIT_REG(&iseq, &t, &qq, &actual_immv);

  t = ut_read_cstr(tmpl);
  qq = scm_compile_qq_template(t);
  iseq = make_iseq(asmbl);
  ip = scm_iseq_to_ip(iseq);

  actual_op = scm_external_opcode(SCM_VMINST_GET_OP(ip));
  SCM_VMINST_FETCH_OPD_OBJ(ip, actual_immv);

  TEST_ASSERT_EQUAL_INT(code, actual_op);
  TEST_ASSERT_EQUAL_INT(0, scm_qqtmpl_eq(qq, actual_immv, &cmp));
  TEST_ASSERT_TRUE(cmp);

  check_following(ip);
}

TEST(assembler, nop)
{
  test_assemble_noopd("((nop)(nop))", SCM_OPCODE_NOP);
}

TEST(assembler, halt)
{
  test_assemble_noopd("((halt)(nop))", SCM_OPCODE_HALT);
}

TEST(assembler, cframe)
{
  test_assemble_si("((cframe (label 0))(nop)(label 0))",
                   SCM_OPCODE_CFRAME, SCM_INST_SZ_NOP);
}

TEST(assembler, eframe)
{
  test_assemble_si("((eframe 20)(nop))", SCM_OPCODE_EFRAME, 20);
}

TEST(assembler, epop)
{
  test_assemble_noopd("((epop)(nop))", SCM_OPCODE_EPOP);
}

TEST(assembler, eshift)
{
  test_assemble_si("((eshift -1)(nop))", SCM_OPCODE_ESHIFT, -1);
}

TEST(assembler, push)
{
  test_assemble_noopd("((push)(nop))", SCM_OPCODE_PUSH);
}

TEST(assembler, mvpush)
{
  test_assemble_noopd("((mvpush)(nop))", SCM_OPCODE_MVPUSH);
}

TEST(assembler, return)
{
  test_assemble_noopd("((return)(nop))", SCM_OPCODE_RETURN);
}

TEST(assembler, pcall)
{
  test_assemble_si("((pcall 5)(nop))", SCM_OPCODE_PCALL, 5);
}

TEST(assembler, call)
{
  test_assemble_si("((call 5)(nop))", SCM_OPCODE_CALL, 5);
}

TEST(assembler, tcall)
{
  test_assemble_si("((tcall 5)(nop))", SCM_OPCODE_TAIL_CALL, 5);
}

TEST(assembler, gref)
{
  ScmObj obj1 = SCM_OBJ_INIT, obj2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&obj1, &obj2);

  obj1 = ut_read_cstr("vvv");
  obj2 = ut_read_cstr("xxx");

  test_assemble_obj_obj("((gref vvv xxx)(nop))",
                        SCM_OPCODE_GREF, obj1, obj2);
}

TEST(assembler, gdef)
{
  ScmObj obj1 = SCM_OBJ_INIT, obj2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&obj1, &obj2);

  obj1 = ut_read_cstr("vvv");
  obj2 = ut_read_cstr("xxx");

  test_assemble_obj_obj("((gdef vvv xxx)(nop))",
                        SCM_OPCODE_GDEF, obj1, obj2);
}

TEST(assembler, gset)
{
  ScmObj obj1 = SCM_OBJ_INIT, obj2 = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&obj1, &obj2);

  obj1 = ut_read_cstr("vvv");
  obj2 = ut_read_cstr("xxx");

  test_assemble_obj_obj("((gset vvv xxx)(nop))",
                        SCM_OPCODE_GSET, obj1, obj2);
}

TEST(assembler, lref)
{
  test_assemble_si_si("((lref -4 12)(nop))", SCM_OPCODE_LREF, -4, 12);
}

TEST(assembler, lset)
{
  test_assemble_si_si("((lset -6 13)(nop))", SCM_OPCODE_LSET, -6, 13);
}

TEST(assembler, immval)
{
  ScmObj obj = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&obj);

  obj = ut_read_cstr("aaa");

  test_assemble_obj("((immval aaa)(nop))", SCM_OPCODE_IMMVAL, obj) ;
}

TEST(assembler, label_jmp)
{
  test_assemble_iof("((label 0)(jmp (label 0))(nop))",
                    SCM_OPCODE_JMP, -(int)SCM_INST_SZ_JMP);
}

TEST(assembler, label_jmpt)
{
  test_assemble_iof("((label 0)(jmpt (label 0))(nop))",
                    SCM_OPCODE_JMPT, -(int)SCM_INST_SZ_JMPT);
}

TEST(assembler, label_jmpf)
{
  test_assemble_iof("((label 0)(jmpf (label 0))(nop))",
                    SCM_OPCODE_JMPF, -(int)SCM_INST_SZ_JMPF);
}

TEST(assembler, lbox)
{
  test_assemble_si_si("((lbox -8 14)(nop))", SCM_OPCODE_LBOX, -8, 14);
}

TEST(assembler, close)
{
  ScmObj iseq = SCM_OBJ_INIT, actual_immv = SCM_OBJ_INIT;
  scm_byte_t *ip;
  scm_opcode_t actual_op;
  int actual_si1, actual_si2;

  SCM_REFSTK_INIT_REG(&iseq, &actual_immv);

  iseq = make_iseq("((close 11 21 ((nop)))(nop))");
  ip = scm_iseq_to_ip(iseq);

  actual_op = scm_external_opcode(SCM_VMINST_GET_OP(ip));
  SCM_VMINST_FETCH_OPD_SI_SI_OBJ(ip, actual_si1, actual_si2, actual_immv);

  TEST_ASSERT_EQUAL_INT(SCM_OPCODE_CLOSE, actual_op);
  TEST_ASSERT_EQUAL_INT(11, actual_si1);
  TEST_ASSERT_EQUAL_INT(21, actual_si2);
  TEST_ASSERT_TRUE(scm_iseq_p(actual_immv));

  check_following(ip);
}

TEST(assembler, demine)
{
  test_assemble_si_si("((demine 15 16)(nop))", SCM_OPCODE_DEMINE, 15, 16);
}

TEST(assembler, emine)
{
  test_assemble_si("((emine 17)(nop))", SCM_OPCODE_EMINE, 17);
}

TEST(assembler, edemine)
{
  test_assemble_si_si("((edemine 18 19)(nop))", SCM_OPCODE_EDEMINE, 18, 19);
}

TEST(assembler, mrvc)
{
  test_assemble_si("((mrvc 5)(nop))", SCM_OPCODE_MRVC, 5);
}

TEST(assembler, mrve)
{
  test_assemble_noopd("((mrve)(nop))", SCM_OPCODE_MRVE);
}

TEST(assembler, mrve_size)
{
  TEST_ASSERT_EQUAL_INT(SCM_INST_SZ_NOP, SCM_INST_SZ_MRVE);
}

TEST(assembler, module)
{
  test_assemble_obj("((module (main))(nop))",
                    SCM_OPCODE_MODULE,
                    ut_read_cstr("(main)"));
}

TEST(assembler, pi_undef)
{
  test_assemble_obj("((undef)(nop))", SCM_OPCODE_IMMVAL, SCM_UNDEF_OBJ);
}

TEST(assembler, pi_uninit)
{
  test_assemble_obj("((uninit)(nop))", SCM_OPCODE_IMMVAL, SCM_UNINIT_OBJ);
}

TEST(assembler, pi_qqtemplate)
{
  test_assemble_pinst_qqtemplate("((qqtemplate (a ,expr c))(nop))",
                                 SCM_OPCODE_IMMVAL,
                                 "(a ,expr c)");
}

static void
test_disassemble(const char *expected, const char *assembler)
{
  ScmObj iseq = SCM_OBJ_INIT, e = SCM_OBJ_INIT, a = SCM_OBJ_INIT;
  ScmObj actual = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&iseq, &e, &a, &actual);

  e = ut_read_cstr(expected);
  a = ut_read_cstr(assembler);

  iseq = scm_assemble(scm_unprintable_asm(a), SCM_OBJ_NULL);
  actual = scm_printable_asm(scm_disassemble(iseq));

  TEST_ASSERT_SCM_EQUAL(e, actual);
}

TEST(assembler, disasm_noopd)
{
  test_disassemble("((halt))", "((halt))");
}

TEST(assembler, disasm_obj)
{
  test_disassemble("((immval a))", "((immval a))");
}

TEST(assembler, disasm_obj_obj)
{
  test_disassemble("((gref abc def))", "((gref abc def))");
}

TEST(assembler, disasm_si)
{
  test_disassemble("((int 0))", "((int 0))");
}

TEST(assembler, disasm_si_si)
{
  test_disassemble("((lref 123 456))", "((lref 123 456))");
}

TEST(assembler, disasm_si_si_obj)
{
  test_disassemble("((close 1 1 ((nop))))", "((close 1 1 ((nop))))");
}

TEST(assembler, disasm_iof__1)
{
  test_disassemble("((label 0)"
                   "   (nop)"
                   "   (jmp (label 0)))",
                   "((label 0)"
                   "   (nop)"
                   "   (jmp (label 0)))");
}

TEST(assembler, disasm_iof__2)
{
  test_disassemble("(  (jmp (label 0))"
                   "   (nop)"
                   " (label 0))",
                   "(  (jmp (label 0))"
                   "   (nop)"
                   " (label 0))");

}

TEST(assembler, disasm_iof__3)
{
  test_disassemble("(  (jmp (label 1))"
                   "   (nop)"
                   "   (jmp (label 0))"
                   "   (nop)"
                   " (label 0)"
                   "   (nop)"
                   " (label 1))",
                   "(  (jmp (label 0))"
                   "   (nop)"
                   "   (jmp (label 1))"
                   "   (nop)"
                   " (label 1)"
                   "   (nop)"
                   " (label 0))");
}

TEST(assembler, disasm_undef)
{
  test_disassemble("((undef))", "((undef))");
}

TEST(assembler, disasm_uninit)
{
  test_disassemble("((uninit))", "((uninit))");
}

TEST(assembler, disasm_qqtemplate)
{
  test_disassemble("((qqtemplate (a ,expr c)))",
                   "((qqtemplate (a ,expr c)))");
}
