#ifndef INCLUDE_SCYTHE_H__
#define INCLUDE_SCYTHE_H__

#include <stdbool.h>

#include "scythe/object.h"
#include "scythe/earray.h"
#include "scythe/bedrock.h"

typedef struct ScmScytheRec ScmScythe;

enum { SCM_SCYTHE_S_DOWN, SCM_SCYTHE_S_UP };

struct ScmScytheRec {
  int stat;
  ScmBedrock *bedrock;
  ScmObj vm;
  ScmObj refstack;

  struct {
    ScmGlobalConf gconf;
  } conf;
};

int scm_scythe_initialize(ScmScythe *scy);
void scm_scythe_finalize(ScmScythe *scy);
ScmScythe *scm_scythe_new();
void scm_scythe_end(ScmScythe *scy);
void scm_scythe_switch(ScmScythe *scy);
int scm_scythe_bootup(ScmScythe *scy);
void scm_scythe_shutdown(ScmScythe *scy);
int scm_scythe_add_load_path(ScmScythe *scy, const char *path);
void scm_scythe_clear_load_path(ScmScythe *scy);
int scm_scythe_add_load_suffix(ScmScythe *scy, const char *suffix);
void scm_scythe_clear_load_suffix(ScmScythe *scy);
int scm_scythe_set_system_encoding(ScmScythe *scy, const char *enc);
void scm_scythe_clear_system_encoding(ScmScythe *scy);
int scm_scythe_set_external_encoding(ScmScythe *scy, const char *enc);
void scm_scythe_clear_external_encoding(ScmScythe *scy);
int scm_scythe_default_setup(ScmScythe *scy);
int scm_scythe_load_core(ScmScythe *scy);
int scm_scythe_apply(ScmScythe *scy,
                     const char *cmd, const char * const *args, size_t n);
int scm_prepare_scythe(void);

static inline void
scm_scythe_enable(ScmScythe *scy)
{
  scm_assert(scy != NULL);

  scm_scythe_switch(scy);
}

static inline void
scm_scythe_disable(ScmScythe *scy)
{
  scm_scythe_switch(NULL);
}

static inline bool
scm_scythe_conf_modifiable_p(ScmScythe *scy)
{
  scm_assert(scy != NULL);

  return ((scy->stat == SCM_SCYTHE_S_DOWN) ? true : false);
}

static inline int
scm_scythe_run_repl(ScmScythe *scy)
{
  return scm_scythe_apply(scy, "repl", NULL, 0);
}

static inline int
scm_scythe_exec_file(ScmScythe *scy, const char * const *argv, size_t n)
{
  return scm_scythe_apply(scy, "exec-file", argv, n);
}

static inline int
scm_scythe_eval_str(ScmScythe *scy, const char *expr)
{
  return scm_scythe_apply(scy, "eval-string", &expr, 1);
}

static inline int
scm_scythe_compile_file(ScmScythe *scy, const char *path, const char *output)
{
  const char *args[] = { path, output };
  return scm_scythe_apply(scy, "compile-file", args, (output == NULL) ? 1 : 2);
}


#endif  /* INCLUDE_SCYTHE_H__ */
