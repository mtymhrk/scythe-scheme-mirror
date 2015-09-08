#ifndef INCLUDE_FCD_FILE_H__
#define INCLUDE_FCD_FILE_H__

#include <stdbool.h>

#include "scythe/object.h"

#define SCM_LOAD_PATH_VARIABLE_NAME "*load-path*"

ScmObj scm_fcd_search_load_file(ScmObj name);
int scm_fcd_file_exists(ScmObj path, bool *rslt);
int scm_fcd_delete_file(ScmObj path);

#endif /* INCLUDE_FCD_FILE_H__ */
