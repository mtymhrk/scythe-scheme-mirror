#ifndef INCLUDE_FCD_ISEQ_H__
#define INCLUDE_FCD_ISEQ_H__

#include <stdbool.h>
#include <stddef.h>

#include "scythe/object.h"
#include "scythe/vminst.h"

bool scm_fcd_iseq_p(ScmObj obj);
ScmObj scm_fcd_make_iseq(void);
scm_byte_t *scm_fcd_iseq_to_ip(ScmObj iseq);
size_t scm_fcd_iseq_length(ScmObj iseq);
int scm_fcd_iseq_eq(ScmObj iseq1, ScmObj iseq2, bool *rslt);
ssize_t scm_fcd_iseq_push_inst_va(ScmObj iseq, scm_opcode_t op, va_list ap);
ssize_t scm_fcd_iseq_push_inst(ScmObj iseq, scm_opcode_t op, ...);
int scm_fcd_iseq_push_br_dst(ScmObj iseq, size_t offset);
size_t scm_fcd_iseq_nr_br_dst(ScmObj iseq);
const size_t *scm_fcd_iseq_br_dsts(ScmObj iseq);
int scm_fcd_iseq_update_oprand_iof(ScmObj iseq, size_t offset, int iof);
int scm_fcd_inst_update_oprand_obj(scm_byte_t *ip, ScmObj clsr, ScmObj obj);

#endif /* INCLUDE_FCD_ISEQ_H__ */