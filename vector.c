#include <stdbool.h>
#include <assert.h>

#include "memory.h"
#include "object.h"
#include "nil.h"
#include "vector.h"
#include "printer.h"

struct ScmVectorRec {
  ScmObjHeader header;
  ScmObj *array;
  size_t length;
};

static void
scm_vector_pretty_print(ScmObj obj, ScmPrinter *printer)
{
  ScmVector *vector;


  assert(obj != NULL); assert(scm_vector_is_vector(obj));
  assert(printer != NULL);

  vector = SCM_VECTOR(obj);

  scm_printer_concatenate_string(printer, "#(");
  if (vector->length > 0) {
    size_t nloop = vector->length - 1;
    size_t i;
    for (i = 0; i < nloop; i++) {
      scm_obj_pretty_print(vector->array[i], printer);
      scm_printer_concatenate_char(printer, ' ');
    }
    scm_obj_pretty_print(vector->array[i], printer);
  }
  scm_printer_concatenate_char(printer, ')');
}

ScmVector *
scm_vector_construct(size_t length)
{
  ScmVector *vector;
  int i;

  vector = scm_memory_allocate(sizeof(ScmVector));
  scm_obj_init(SCM_OBJ(vector), SCM_OBJ_TYPE_VECTOR, scm_vector_pretty_print);

  if (length > 0)
    vector->array = scm_memory_allocate(sizeof(ScmObj) * length);
  else
    vector->array = NULL;

  vector->length = length;

  /* initial value of elements of the vector is nil. */
  /* the initial value is not specified in R5RS */
  for (i = 0; i < length; i++)
    vector->array[i] = SCM_OBJ(scm_nil_instance());

  return vector;
}

void
scm_vector_destruct(ScmVector *vector)
{
  assert(vector != NULL);

  scm_memory_release(vector->array);
  scm_memory_release(vector);
}

size_t
scm_vector_length(ScmVector *vector)
{
  assert(vector != NULL);
  return vector->length;
}

ScmObj
scm_vector_ref(ScmVector *vector, size_t index)
{
  assert(vector != NULL);
  assert(index < vector->length);

  return vector->array[index];
}

ScmObj
scm_vector_set(ScmVector *vector, size_t index, ScmObj obj)
{
  assert(vector != NULL);
  assert(index < vector->length);

  return vector->array[index] = obj;
}

bool
scm_vector_is_vector(ScmObj obj)
{
  assert(obj != NULL);

  return (scm_obj_type(obj) == SCM_OBJ_TYPE_VECTOR);
}