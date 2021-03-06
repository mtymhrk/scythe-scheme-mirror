#include "scythe/vminst.h"

const int scm_opfmt_table[SCM_VMINST_NR_OP] = {
    SCM_OPFMT_NOOPD,            /* SCM_OPCODE_NOP */
    SCM_OPFMT_NOOPD,            /* SCM_OPCODE_HALT */
    SCM_OPFMT_SI,               /* SCM_OPCODE_INT */
    SCM_OPFMT_IOF,              /* SCM_OPCODE_CFRAME */
    SCM_OPFMT_SI,               /* SCM_OPCODE_EFRAME */
    SCM_OPFMT_NOOPD,            /* SCM_OPCODE_EPOP */
    SCM_OPFMT_SI   ,            /* SCM_OPCODE_ESHIFT */
    SCM_OPFMT_OBJ,              /* SCM_OPCODE_IMMVAL */
    SCM_OPFMT_NOOPD,            /* SCM_OPCODE_PUSH */
    SCM_OPFMT_NOOPD,            /* SCM_OPCODE_MVPUSH */
    SCM_OPFMT_NOOPD,            /* SCM_OPCODE_RETURN */
    SCM_OPFMT_SI,               /* SCM_OPCODE_PCALL */
    SCM_OPFMT_SI,               /* SCM_OPCODE_CALL */
    SCM_OPFMT_SI,               /* SCM_OPCODE_TAIL_CALL */
    SCM_OPFMT_OBJ_OBJ,          /* SCM_OPCODE_GREF */
    SCM_OPFMT_OBJ_OBJ,          /* SCM_OPCODE_GDEF */
    SCM_OPFMT_OBJ_OBJ,          /* SCM_OPCODE_GSET */
    SCM_OPFMT_SI_SI,            /* SCM_OPCODE_LREF */
    SCM_OPFMT_SI_SI,            /* SCM_OPCODE_LSET */
    SCM_OPFMT_IOF,              /* SCM_OPCODE_JMP */
    SCM_OPFMT_IOF,              /* SCM_OPCODE_JMPT */
    SCM_OPFMT_IOF,              /* SCM_OPCODE_JMPF */
    SCM_OPFMT_SI_SI,            /* SCM_OPCODE_LBOX */
    SCM_OPFMT_SI_SI,            /* SCM_OPCODE_LBREF */
    SCM_OPFMT_SI_SI,            /* SCM_OPCODE_LBSET */
    SCM_OPFMT_SI_SI_OBJ,        /* SCM_OPCODE_CLOSE */
    SCM_OPFMT_SI_SI,            /* SCM_OPCODE_DEMINE */
    SCM_OPFMT_SI,               /* SCM_OPCODE_EMINE */
    SCM_OPFMT_SI_SI,            /* SCM_OPCODE_EDEMINE */
    SCM_OPFMT_SI,               /* SCM_OPCODE_MRVC */
    SCM_OPFMT_NOOPD,            /* SCM_OPCODE_MRVE */
    SCM_OPFMT_OBJ               /* SCM_OPCODE_MODULE */
};
