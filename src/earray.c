#include <stddef.h>

#include "scythe/memory.h"
#include "scythe/impl_utils.h"
#include "scythe/earray.h"

int
eary_init(EArray *ary, size_t rs, size_t ns)
{
  ary->cap = ns;
  ary->used = 0;
  ary->rs = rs;
  if (ns == 0) {
    ary->vec = NULL;
  }
  else {
    ary->vec = scm_malloc(rs * ns);
    if (ary->vec == NULL) return -1;
  }

  return 0;
}

void
eary_fin(EArray *ary)
{
  scm_free(ary->vec);
  ary->vec = NULL;
}

int
eary_expand(EArray *ary, size_t ndd)
{
  if (ary->cap > SIZE_MAX / EARY_MAG)
    return -1;

  if (ary->cap == 0)
    ary->cap = EARY_DEFAULT_CAP;

  size_t ns = ary->cap * EARY_MAG;
  while (ndd > ns) {
    if (ns > SIZE_MAX / EARY_MAG)
      return -1;
    ns *= EARY_MAG;
  }

  void *p = scm_realloc(ary->vec, ary->rs * ns);
  if (p == NULL)
    return -1;

  ary->vec = p;
  ary->cap = ns;

  return 0;
}

int
eary_contract(EArray *ary)
{
  if (ary->used >= ary->cap)
    return 0;

  void *p = scm_realloc(ary->vec, ary->rs * ary->used);
  if (p == NULL) return -1;

  ary->vec = p;
  ary->cap = ary->used;
  return 0;
}
