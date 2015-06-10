#include <limits.h>

#include "scythe/object.h"
#include "scythe/impl_utils.h"
#include "scythe/fcd.h"
#include "scythe/chashtbl.h"
#include "scythe/module.h"


/****************************************************************************/
/*  GLoc                                                                    */
/****************************************************************************/

ScmTypeInfo SCM_GLOC_TYPE_INFO = {
  .name                = "gloc",
  .flags               = SCM_TYPE_FLG_MMO,
  .obj_print_func      = NULL,
  .obj_size            = sizeof(ScmGLoc),
  .gc_ini_func         = scm_gloc_gc_initialize,
  .gc_fin_func         = NULL,
  .gc_accept_func      = scm_gloc_gc_accept,
  .gc_accept_func_weak = NULL,
  .extra               = NULL,
};

int
scm_gloc_initialize(ScmObj gloc, ScmObj sym, ScmObj val)
{
  scm_assert_obj_type(gloc, &SCM_GLOC_TYPE_INFO);
  scm_assert(scm_fcd_symbol_p(sym));
  scm_assert(scm_obj_not_null_p(val));

  SCM_SLOT_SETQ(ScmGLoc, gloc, sym, sym);
  SCM_SLOT_SETQ(ScmGLoc, gloc, val, val);
  SCM_GLOC(gloc)->flags = 0;

  return 0;
}

void
scm_gloc_bind_variable(ScmObj gloc, ScmObj val)
{
  scm_assert_obj_type(gloc, &SCM_GLOC_TYPE_INFO);
  scm_assert(scm_obj_not_null_p(val) && !scm_fcd_landmine_object_p(val));

  SCM_GLOC(gloc)->flags &= ~(unsigned int)SCM_GLOC_FLG_KEYWORD;
  SCM_SLOT_SETQ(ScmGLoc, gloc, val, val);
}

void
scm_gloc_bind_keyword(ScmObj gloc, ScmObj val)
{
  scm_assert_obj_type(gloc, &SCM_GLOC_TYPE_INFO);
  scm_assert(scm_obj_not_null_p(val) && !scm_fcd_landmine_object_p(val));

  SCM_GLOC(gloc)->flags |= SCM_GLOC_FLG_KEYWORD;
  SCM_SLOT_SETQ(ScmGLoc, gloc, val, val);
}

void
scm_gloc_export(ScmObj gloc)
{
  scm_assert_obj_type(gloc, &SCM_GLOC_TYPE_INFO);

  SCM_GLOC(gloc)->flags |= SCM_GLOC_FLG_EXPORT;
}

void
scm_gloc_gc_initialize(ScmObj obj, ScmObj mem)
{
  scm_assert_obj_type(obj, &SCM_GLOC_TYPE_INFO);

  SCM_GLOC(obj)->sym = SCM_OBJ_NULL;
  SCM_GLOC(obj)->val = SCM_OBJ_NULL;
}

int
scm_gloc_gc_accept(ScmObj obj, ScmObj mem, ScmGCRefHandlerFunc handler)
{
  int rslt = SCM_GC_REF_HANDLER_VAL_INIT;

  scm_assert_obj_type(obj, &SCM_GLOC_TYPE_INFO);

  rslt = SCM_GC_CALL_REF_HANDLER(handler, obj, SCM_GLOC(obj)->sym, mem);
  if (scm_gc_ref_handler_failure_p(rslt)) return rslt;

  return SCM_GC_CALL_REF_HANDLER(handler, obj, SCM_GLOC(obj)->val, mem);
}


/****************************************************************************/
/*  GLoc (interface)                                                        */
/****************************************************************************/

bool
scm_fcd_gloc_p(ScmObj obj)
{
  return scm_obj_type_p(obj, &SCM_GLOC_TYPE_INFO);
}

ScmObj
scm_fcd_gloc_new(SCM_MEM_TYPE_T mtype, ScmObj sym)
{
  ScmObj gloc = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&gloc, &sym);

  gloc = scm_fcd_mem_alloc(&SCM_GLOC_TYPE_INFO, 0, mtype);
  if (scm_obj_null_p(gloc)) return SCM_OBJ_NULL;

  if (scm_gloc_initialize(gloc, sym, SCM_UNINIT_OBJ) < 0)
    return SCM_OBJ_NULL;

  return gloc;
}

ScmObj
scm_fcd_gloc_variable_value(ScmObj gloc)
{
  scm_assert(scm_fcd_gloc_p(gloc));

  if (scm_gloc_variable_p(gloc))
    return scm_gloc_value(gloc);
  else
    return SCM_UNINIT_OBJ;
}

ScmObj
scm_fcd_gloc_keyword_value(ScmObj gloc)
{
  scm_assert(scm_fcd_gloc_p(gloc));

  if (scm_gloc_keyword_p(gloc))
    return scm_gloc_value(gloc);
  else
    return SCM_UNINIT_OBJ;
}

ScmObj
scm_fcd_gloc_symbol(ScmObj gloc)
{
  scm_assert(scm_fcd_gloc_p(gloc));
  return scm_gloc_symbol(gloc);
}

void
scm_fcd_gloc_bind_variable(ScmObj gloc, ScmObj val)
{
  scm_assert(scm_fcd_gloc_p(gloc));
  scm_assert(scm_obj_not_null_p(val) && !scm_fcd_landmine_object_p(val));

  scm_gloc_bind_variable(gloc, val);
}

void
scm_fcd_gloc_bind_keyword(ScmObj gloc, ScmObj val)
{
  scm_assert(scm_fcd_gloc_p(gloc));
  scm_assert(scm_obj_not_null_p(val) && !scm_fcd_landmine_object_p(val));

  scm_gloc_bind_keyword(gloc, val);
}


/****************************************************************************/
/*  Module                                                                  */
/****************************************************************************/

ScmTypeInfo SCM_MODULE_TYPE_INFO = {
  .name                = "module",
  .flags               = SCM_TYPE_FLG_MMO,
  .obj_print_func      = scm_module_obj_print,
  .obj_size            = sizeof(ScmModule),
  .gc_ini_func         = scm_module_gc_initialize,
  .gc_fin_func         = scm_module_gc_finalize,
  .gc_accept_func      = scm_module_gc_accept,
  .gc_accept_func_weak = NULL,
  .extra               = NULL,
};

#define SCM_MODULE_GLOCTBL_SIZE 256

static size_t
scm_module_hash_func(ScmCHashTblKey key)
{
  return scm_fcd_symbol_hash_value(SCM_OBJ(key));
}

static bool
scm_module_cmp_func(ScmCHashTblKey key1, ScmCHashTblKey key2)
{
  return scm_fcd_eq_p(SCM_OBJ(key1), SCM_OBJ(key2));
}

static int
scm_module_search_gloc(ScmObj mod, ScmObj sym, scm_csetter_t *setter)
{
  bool found;
  int rslt;

  SCM_REFSTK_INIT_REG(&mod, &sym);

  scm_assert_obj_type(mod, &SCM_MODULE_TYPE_INFO);
  scm_assert(scm_fcd_symbol_p(sym));

  rslt = scm_chash_tbl_get(SCM_MODULE(mod)->gloctbl,
                           sym, (ScmCHashTblVal *)setter, &found);
  if (rslt != 0) return -1;

  if (!found) scm_csetter_setq(setter, SCM_OBJ_NULL);

  return 0;
}

static int
scm_module_define(ScmObj mod, ScmObj sym, ScmObj val, bool export, bool keyword)
{
  ScmObj gloc = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&mod, &sym, &val,
                      &gloc);

  scm_assert_obj_type(mod, &SCM_MODULE_TYPE_INFO);
  scm_assert(scm_fcd_symbol_p(sym));
  scm_assert(scm_obj_not_null_p(val) && !scm_fcd_landmine_object_p(val));

  gloc = scm_module_gloc(mod, sym);
  if (scm_obj_null_p(gloc)) return -1;

  if (keyword)
    scm_gloc_bind_keyword(gloc, val);
  else
    scm_gloc_bind_variable(gloc, val);

  if (export)
    scm_gloc_export(gloc);

  return 0;
}

static int
scm_module_find_exported_sym(ScmObj mod, ScmObj sym, scm_csetter_t *setter)
{
  ScmObj lst = SCM_OBJ_INIT, elm = SCM_OBJ_INIT, imp = SCM_OBJ_INIT;
  ScmObj res = SCM_OBJ_INIT, gloc = SCM_OBJ_INIT;
  int rslt;

  SCM_REFSTK_INIT_REG(&mod, &sym,
                      &lst, &elm, &imp,
                      &res, &gloc);

  scm_assert_obj_type(mod, &SCM_MODULE_TYPE_INFO);
  scm_assert(scm_fcd_symbol_p(sym));
  scm_assert(setter != NULL);

  if (SCM_MODULE(mod)->in_searching)
    return 0;

  SCM_MODULE(mod)->in_searching = true;

  rslt = scm_module_search_gloc(mod, sym, setter);
  if (rslt < 0) goto err;

  gloc = scm_csetter_val(setter);
  if (scm_obj_not_null_p(gloc) && scm_gloc_exported_p(gloc))
    goto done;

  scm_csetter_setq(setter, SCM_OBJ_NULL);

  for (lst = SCM_MODULE(mod)->imports;
       scm_fcd_pair_p(lst);
       lst = scm_fcd_cdr(lst)) {
    elm = scm_fcd_car(lst);
    imp = scm_fcd_car(elm);
    res = scm_fcd_cdr(elm);
    if (scm_fcd_true_p(res))
      continue;

    rslt = scm_module_find_exported_sym(imp, sym, setter);
    if (rslt < 0) goto err;

    if (scm_obj_not_null_p(scm_csetter_val(setter)))
      goto done;
  }

 done:
  SCM_MODULE(mod)->in_searching = false;
  return 0;

 err:
  SCM_MODULE(mod)->in_searching = false;
  return -1;
}

int
scm_module_initialize(ScmObj mod, ScmObj name)
{
  SCM_REFSTK_INIT_REG(&mod, &name);

  scm_assert_obj_type(mod, &SCM_MODULE_TYPE_INFO);
  scm_assert(scm_fcd_pair_p(name));

  SCM_SLOT_SETQ(ScmModule, mod, name, name);

  SCM_MODULE(mod)->gloctbl = scm_chash_tbl_new(mod,
                                               SCM_MODULE_GLOCTBL_SIZE,
                                               SCM_CHASH_TBL_SCMOBJ,
                                               SCM_CHASH_TBL_SCMOBJ,
                                               scm_module_hash_func,
                                               scm_module_cmp_func);
  if (SCM_MODULE(mod)->gloctbl == NULL) return -1;

  SCM_MODULE(mod)->imports = SCM_NIL_OBJ;
  SCM_MODULE(mod)->in_searching = false;

  return 0;
}

void
scm_module_finalize(ScmObj mod)
{
  scm_assert_obj_type(mod, &SCM_MODULE_TYPE_INFO);

  if (SCM_MODULE(mod)->gloctbl != NULL) {
    scm_chash_tbl_end(SCM_MODULE(mod)->gloctbl);
    SCM_MODULE(mod)->gloctbl = NULL;
  }
}

int
scm_module_import(ScmObj mod, ScmObj imp, bool res)
{
  ScmObj elm = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&mod, &imp,
                      &elm);

  scm_assert_obj_type(mod, &SCM_MODULE_TYPE_INFO);
  scm_assert_obj_type(imp, &SCM_MODULE_TYPE_INFO);

  elm = scm_fcd_cons(imp, res ? SCM_TRUE_OBJ : SCM_FALSE_OBJ);
  if (scm_obj_null_p(elm)) return -1;

  elm = scm_fcd_cons(elm, SCM_MODULE(mod)->imports);
  if (scm_obj_null_p(elm)) return -1;

  SCM_SLOT_SETQ(ScmModule, mod, imports, elm);

  return 0;
}

int
scm_module_define_variable(ScmObj mod, ScmObj sym, ScmObj val, bool export)
{
  return scm_module_define(mod, sym, val, export, false);
}

int
scm_module_define_keyword(ScmObj mod, ScmObj sym, ScmObj val, bool export)
{
  return scm_module_define(mod, sym, val, export, true);
}

int
scm_module_export(ScmObj mod, ScmObj sym)
{
  ScmObj gloc = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&mod, &sym,
                      &gloc);

  scm_assert_obj_type(mod, &SCM_MODULE_TYPE_INFO);
  scm_assert(scm_fcd_symbol_p(sym));

  gloc = scm_module_gloc(mod, sym);
  if (scm_obj_null_p(gloc)) return -1;

  scm_gloc_export(gloc);

  return 0;
}

ScmObj
scm_module_gloc(ScmObj mod, ScmObj sym)
{
  ScmObj gloc = SCM_OBJ_INIT;
  bool found;
  int rslt;

  SCM_REFSTK_INIT_REG(&mod, &sym,
                      &gloc);

  scm_assert_obj_type(mod, &SCM_MODULE_TYPE_INFO);
  scm_assert(scm_fcd_symbol_p(sym));

  rslt = scm_chash_tbl_get(SCM_MODULE(mod)->gloctbl, sym,
                           (ScmCHashTblVal *)SCM_CSETTER_L(gloc), &found);

  if (rslt != 0) return SCM_OBJ_NULL;

  if (found) return gloc;

  gloc = scm_fcd_gloc_new(SCM_MEM_HEAP, sym);
  if (scm_obj_null_p(gloc)) return SCM_OBJ_NULL;

  rslt = scm_chash_tbl_insert(SCM_MODULE(mod)->gloctbl, sym, gloc);
  if (rslt != 0) return SCM_OBJ_NULL;

  return gloc;
}

int
scm_module_find_sym(ScmObj mod, ScmObj sym, scm_csetter_t *setter)
{
  ScmObj lst = SCM_OBJ_INIT, elm = SCM_OBJ_INIT, imp = SCM_OBJ_INIT;
  int rslt;

  SCM_REFSTK_INIT_REG(&mod, &sym,
                      &lst, &elm, &imp);

  scm_assert_obj_type(mod, &SCM_MODULE_TYPE_INFO);
  scm_assert(scm_fcd_symbol_p(sym));
  scm_assert(setter != NULL);

  if (SCM_MODULE(mod)->in_searching)
    return 0;

  SCM_MODULE(mod)->in_searching = true;

  rslt = scm_module_search_gloc(mod, sym, setter);
  if (rslt < 0) goto err;

  if (scm_obj_not_null_p(scm_csetter_val(setter)))
    goto done;

  for (lst = SCM_MODULE(mod)->imports;
       scm_fcd_pair_p(lst);
       lst = scm_fcd_cdr(lst)) {
    elm = scm_fcd_car(lst);
    imp = scm_fcd_car(elm);
    rslt = scm_module_find_exported_sym(imp, sym, setter);
    if (rslt < 0) goto err;;

    if (scm_obj_not_null_p(scm_csetter_val(setter)))
      goto done;
  }

 done:
  SCM_MODULE(mod)->in_searching = false;
  return 0;

 err:
  SCM_MODULE(mod)->in_searching = false;
  return -1;
}

int
scm_module_obj_print(ScmObj obj, ScmObj port, int kind,
                     ScmObjPrintHandler handler)
{
  scm_assert_obj_type(obj, &SCM_MODULE_TYPE_INFO);

  return scm_fcd_pformat_cstr(port, "#<module ~a>",
                               SCM_MODULE(obj)->name, SCM_OBJ_NULL);
}

void
scm_module_gc_initialize(ScmObj obj, ScmObj mem)
{
  scm_assert_obj_type(obj, &SCM_MODULE_TYPE_INFO);

  SCM_MODULE(obj)->name = SCM_OBJ_NULL;
  SCM_MODULE(obj)->imports = SCM_OBJ_NULL;
  SCM_MODULE(obj)->gloctbl = NULL;
}

void
scm_module_gc_finalize(ScmObj obj)
{
  scm_module_finalize(obj);
}

int
scm_module_gc_accept(ScmObj obj, ScmObj mem, ScmGCRefHandlerFunc handler)
{
  int rslt = SCM_GC_REF_HANDLER_VAL_INIT;

  scm_assert_obj_type(obj, &SCM_MODULE_TYPE_INFO);
  scm_assert(scm_obj_not_null_p(mem));
  scm_assert(handler != NULL);

  rslt = SCM_GC_CALL_REF_HANDLER(handler, obj, SCM_MODULE(obj)->name, mem);
  if (scm_gc_ref_handler_failure_p(rslt)) return rslt;

  rslt = SCM_GC_CALL_REF_HANDLER(handler, obj, SCM_MODULE(obj)->imports, mem);
  if (scm_gc_ref_handler_failure_p(rslt)) return rslt;

  if (SCM_MODULE(obj)->gloctbl != NULL) {
    rslt = scm_chash_tbl_gc_accept(SCM_MODULE(obj)->gloctbl,
                                   obj, mem, handler, false);
    if (scm_gc_ref_handler_failure_p(rslt)) return rslt;
  }

  return rslt;
}


/****************************************************************************/
/*  Module (interface)                                                      */
/****************************************************************************/

extern inline bool
scm_fcd_module_p(ScmObj obj)
{
  return scm_obj_type_p(obj, &SCM_MODULE_TYPE_INFO);
}

ScmObj
scm_fcd_module_P(ScmObj obj)
{
  return (scm_fcd_module_p(obj) ? SCM_TRUE_OBJ : SCM_FALSE_OBJ);
}

extern inline bool
scm_fcd_module_name_p(ScmObj obj)
{
  return (scm_fcd_symbol_p(obj) || scm_fcd_pair_p(obj));

}

extern inline bool
scm_fcd_module_specifier_p(ScmObj obj)
{
  return (scm_fcd_module_p(obj)
          || scm_fcd_symbol_p(obj) || scm_fcd_pair_p(obj));
}

ScmObj
scm_fcd_module_new(SCM_MEM_TYPE_T mtype, ScmObj name)
{
  ScmObj mod = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&name,
                      &mod);

  scm_assert(scm_fcd_pair_p(name));

  mod = scm_fcd_mem_alloc(&SCM_MODULE_TYPE_INFO, 0, mtype);
  if (scm_obj_null_p(mod)) return SCM_OBJ_NULL;

  if (scm_module_initialize(mod, name) < 0)
    return SCM_OBJ_NULL;

  return mod;
}

ScmObj
scm_fcd_make_module(ScmObj name)
{
  ScmObj tree = SCM_OBJ_INIT, mod = SCM_OBJ_INIT;
  int rslt;

  SCM_REFSTK_INIT_REG(&name,
                      &tree, &mod);

  scm_assert(scm_fcd_module_name_p(name));

  tree = scm_fcd_current_module_tree();
  rslt = scm_moduletree_find(tree, name, SCM_CSETTER_L(mod));
  if (rslt < 0) return SCM_OBJ_NULL;

  if (scm_obj_not_null_p(mod)) {
    scm_fcd_error("failed to make a module: already exist", 1, name);
    return SCM_OBJ_NULL;
  }

  return scm_moduletree_module(tree, name);
}

int
scm_fcd_find_module(ScmObj name, scm_csetter_t *mod)
{
  scm_assert(scm_fcd_module_name_p(name));
  return scm_moduletree_find(scm_fcd_current_module_tree(), name, mod);
}

ScmObj
scm_fcd_module_name(ScmObj module)
{
  scm_assert(scm_fcd_module_p(module));
  return scm_module_name(module);
}

static ScmObj
get_module(ScmObj spec)
{
  ScmObj mod = SCM_OBJ_INIT;
  int r;

  SCM_REFSTK_INIT_REG(&spec,
                      &mod);

  scm_assert(scm_fcd_module_specifier_p(spec));

  if (scm_fcd_module_p(spec))
    return spec;

  r = scm_fcd_find_module(spec, SCM_CSETTER_L(mod));
  if (r < 0) return SCM_OBJ_NULL;

  if (scm_obj_null_p(mod)) {
    scm_fcd_error("failed to find loaded module: no such a module", 1, spec);
    return SCM_OBJ_NULL;
  }

  return mod;
}

int
scm_fcd_module_import(ScmObj module, ScmObj imported, bool restrictive)
{
  SCM_REFSTK_INIT_REG(&module, &imported);

  scm_assert(scm_fcd_module_p(module));
  scm_assert(scm_fcd_module_specifier_p(imported));

  imported = get_module(imported);
  if (scm_obj_null_p(imported)) return -1;

  return scm_module_import(module, imported, restrictive);
}

int
scm_fcd_module_export(ScmObj module, ScmObj sym)
{
  scm_assert(scm_fcd_module_p(module));
  scm_assert(scm_fcd_symbol_p(sym));

  return scm_module_export(module, sym);
}

ScmObj
scm_fcd_get_gloc(ScmObj module, ScmObj sym)
{
  scm_assert(scm_fcd_module_p(module));
  scm_assert(scm_fcd_symbol_p(sym));
  return scm_module_gloc(module, sym);
}

int
scm_fcd_find_gloc(ScmObj module, ScmObj sym, scm_csetter_t *gloc)
{
  scm_assert(scm_fcd_module_p(module));
  scm_assert(scm_fcd_symbol_p(sym));

  if (gloc != NULL)
    return scm_module_find_sym(module, sym, gloc);
  else
    return 0;
}


/****************************************************************************/
/*  ModuleTree                                                              */
/****************************************************************************/

ScmTypeInfo SCM_MODULETREE_TYPE_INFO = {
  .name                              = "moduletree",
  .flags                             = SCM_TYPE_FLG_MMO,
  .obj_print_func                    = NULL,
  .obj_size                          = sizeof(ScmModuleTree),
  .gc_ini_func                       = scm_moduletree_gc_initialize,
  .gc_fin_func                       = scm_moduletree_gc_finalize,
  .gc_accept_func                    = scm_moduletree_gc_accept,
  .gc_accept_func_weak               = NULL,
  .extra                             = NULL
};

enum { ADD, UPDATE, FIND };

static ScmModuleTreeNode *
scm_moduletree_make_node(void)
{
  ScmModuleTreeNode *node;

  node = scm_fcd_malloc(sizeof(ScmModuleTreeNode));
  if (node == NULL) return NULL;

  node->name = SCM_OBJ_NULL;
  node->module = SCM_OBJ_NULL;

  node->branches = scm_fcd_malloc(sizeof(ScmModuleTreeNode *)
                                   * SCM_MODULETREE_DEFAULT_BRANCH_SIZE);
  if (node->branches == NULL) {
    scm_fcd_free(node);
    return NULL;
  }

  node->capacity = SCM_MODULETREE_DEFAULT_BRANCH_SIZE;
  node->used = 0;

  return node;
}

static int
scm_moduletree_free_node(ScmModuleTreeNode *node)
{
  if (node == NULL) return 0;

  scm_fcd_free(node->branches);
  scm_fcd_free(node);

  return 0;
}

static int
scm_moduletree_free_tree(ScmModuleTreeNode *root)
{
  int r;

  if (root == NULL) return 0;

  for (size_t i = 0; i < root->used; i++) {
    r = scm_moduletree_free_tree(root->branches[i]);
    if (r < 0) return -1;
  }

  r = scm_moduletree_free_node(root);
  if (r < 0) return -1;

  return 0;
}

static int
scm_moduletree_node_gc_accept(ScmObj tree, ScmModuleTreeNode *node,
                              ScmObj mem, ScmGCRefHandlerFunc handler)
{
  int rslt = SCM_GC_REF_HANDLER_VAL_INIT;

  if (node == NULL) return rslt;

  rslt = SCM_GC_CALL_REF_HANDLER(handler, tree, node->name, mem);
  if (scm_gc_ref_handler_failure_p(rslt)) return rslt;

  rslt = SCM_GC_CALL_REF_HANDLER(handler, tree, node->module, mem);
  if (scm_gc_ref_handler_failure_p(rslt)) return rslt;

  for (size_t i = 0; i < node->used; i++) {
    rslt = scm_moduletree_node_gc_accept(tree, node->branches[i], mem, handler);
    if (scm_gc_ref_handler_failure_p(rslt)) return rslt;
  }

  return 0;
}

static ScmModuleTreeNode *
scm_moduletree_add_branche(ScmModuleTreeNode *node, ScmObj name)
{
  ScmModuleTreeNode *new;

  new = scm_moduletree_make_node();
  if (new == NULL) return NULL;

  new->name = name;

  if (node->used >= node->capacity) {
    size_t new_cap;
    ScmModuleTreeNode **new_bra;

    if (node->capacity == SIZE_MAX) {
      scm_fcd_error("failed to register a module: buffer overlfow", 0);
      return NULL;
    }
    else if (node->capacity > SIZE_MAX / 2) {
      new_cap = SIZE_MAX;
    }
    else {
      new_cap = node->capacity * 2;
    }

    new_bra = scm_fcd_realloc(node->branches,
                               sizeof(ScmModuleTreeNode) * new_cap);
    if (new_bra == NULL) return NULL;

    node->branches = new_bra;
    node->capacity = new_cap;
  }

  node->branches[node->used++] = new;

  return new;
}

static int
scm_moduletree_access(ScmModuleTreeNode *root, ScmObj path, int mode,
                      ScmModuleTreeNode **node)
{
  ScmObj name = SCM_OBJ_INIT;

  SCM_REFSTK_INIT_REG(&path,
                      &name);

  scm_assert(root != NULL);
  scm_assert(scm_fcd_pair_p(path));
  scm_assert(node != NULL);

  name = scm_fcd_car(path);
  if (!scm_fcd_symbol_p(name)) {
    scm_fcd_error("failed to access module: "
                   "module name must be a list whose members are symbols", 0);
    return -1;
  }

  path = scm_fcd_cdr(path);
  *node = NULL;
  for (size_t i = 0; i < root->used; i++) {
    if (scm_fcd_eq_p(name, root->branches[i]->name)) {
      *node = root->branches[i];
      break;
    }
  }

  if (scm_fcd_nil_p(path)) {
    if (mode == ADD || mode == UPDATE) {
      if (*node == NULL) {
        ScmModuleTreeNode *new = scm_moduletree_add_branche(root, name);
        if (new == NULL) return -1;
        *node = new;
      }
      else if (mode == ADD) {
        *node = NULL;
      }
    }

    return 0;
  }

  if (*node == NULL && (mode == ADD || mode == UPDATE)) {
    ScmModuleTreeNode *new = scm_moduletree_add_branche(root, name);
    if (new == NULL) return -1;
    *node = new;
  }

  if (*node != NULL)
    return scm_moduletree_access(*node, path, mode, node);
  else
    return 0;
}

static ScmObj
scm_moduletree_normailize_name(ScmObj name)
{
  scm_assert(scm_fcd_symbol_p(name) || scm_fcd_pair_p(name));

  if (scm_fcd_pair_p(name)) return name;

  return scm_fcd_list(1, name);
}

int
scm_moduletree_initialize(ScmObj tree)
{
  scm_assert_obj_type(tree, &SCM_MODULETREE_TYPE_INFO);

  SCM_MODULETREE(tree)->root = scm_moduletree_make_node();
  if (SCM_MODULETREE(tree)->root == NULL) return -1;

  return 0;
}

void
scm_moduletree_finalize(ScmObj tree)
{
  scm_assert_obj_type(tree, &SCM_MODULETREE_TYPE_INFO);

  scm_moduletree_free_tree(SCM_MODULETREE(tree)->root);
  SCM_MODULETREE(tree)->root = NULL;
}

ScmObj
scm_moduletree_module(ScmObj tree, ScmObj name)
{
  ScmObj path = SCM_OBJ_INIT;
  ScmModuleTreeNode *node;
  bool need_copy;
  int rslt;

  SCM_REFSTK_INIT_REG(&tree, &name,
                      &path);

  scm_assert_obj_type(tree, &SCM_MODULETREE_TYPE_INFO);
  scm_assert(scm_fcd_symbol_p(name) || scm_fcd_pair_p(name));

  path = scm_moduletree_normailize_name(name);
  if (scm_obj_null_p(path)) return SCM_OBJ_NULL;

  need_copy = scm_fcd_eq_p(path, name);

  rslt = scm_moduletree_access(SCM_MODULETREE(tree)->root, path, UPDATE, &node);
  if (rslt < 0) return SCM_OBJ_NULL;

  if (scm_obj_null_p(node->module)) {
    if (need_copy) {
      path = scm_fcd_list_copy(path);
      if (scm_obj_null_p(path)) return SCM_OBJ_NULL;
    }

    node->module = scm_fcd_module_new(SCM_MEM_HEAP, path);
    if (scm_obj_null_p(node->module)) return SCM_OBJ_NULL;
  }

  return node->module;
}

int
scm_moduletree_find(ScmObj tree, ScmObj name, scm_csetter_t *mod)
{
  ScmModuleTreeNode *node;
  int rslt;

  SCM_REFSTK_INIT_REG(&tree, &name);

  scm_assert_obj_type(tree, &SCM_MODULETREE_TYPE_INFO);
  scm_assert(scm_fcd_symbol_p(name) || scm_fcd_pair_p(name));
  scm_assert(mod != NULL);

  name = scm_moduletree_normailize_name(name);
  if (scm_obj_null_p(name)) return -1;

  rslt = scm_moduletree_access(SCM_MODULETREE(tree)->root, name, FIND, &node);
  if (rslt < 0) return -1;

  if (node == NULL)
    scm_csetter_setq(mod, SCM_OBJ_NULL);
  else
    scm_csetter_setq(mod, node->module);

  return 0;
}

int
scm_moduletree_clean(ScmObj tree)
{
  scm_assert_obj_type(tree, &SCM_MODULETREE_TYPE_INFO);

  scm_moduletree_free_tree(SCM_MODULETREE(tree)->root);
  SCM_MODULETREE(tree)->root = scm_moduletree_make_node();
  if (SCM_MODULETREE(tree)->root == NULL) return -1;

  return 0;
}

void
scm_moduletree_gc_initialize(ScmObj obj, ScmObj mem)
{
  scm_assert_obj_type(obj, &SCM_MODULETREE_TYPE_INFO);

  SCM_MODULETREE(obj)->root = NULL;
}

void
scm_moduletree_gc_finalize(ScmObj obj)
{
  scm_moduletree_finalize(obj);
}

int
scm_moduletree_gc_accept(ScmObj obj, ScmObj mem, ScmGCRefHandlerFunc handler)
{
  scm_assert_obj_type(obj, &SCM_MODULETREE_TYPE_INFO);
  scm_assert(scm_obj_not_null_p(mem));
  scm_assert(handler != NULL);

  return scm_moduletree_node_gc_accept(obj, SCM_MODULETREE(obj)->root,
                                       mem, handler);
}


/****************************************************************************/
/*  ModuleTree (interface)                                                  */
/****************************************************************************/

ScmObj
scm_fcd_moduletree_new(SCM_MEM_TYPE_T mtype)
{
  ScmObj tree = SCM_OBJ_INIT;

  tree = scm_fcd_mem_alloc(&SCM_MODULETREE_TYPE_INFO, 0, mtype);
  if (scm_obj_null_p(tree)) return SCM_OBJ_NULL;

  if (scm_moduletree_initialize(tree) < 0)
    return SCM_OBJ_NULL;

  return tree;
}


/****************************************************************************/
/*  Global Veriable (interface)                                             */
/****************************************************************************/

int
scm_fcd_define_global_var(ScmObj module, ScmObj sym, ScmObj val, bool export)
{
  SCM_REFSTK_INIT_REG(&module, &sym, &val);

  scm_assert(scm_fcd_module_specifier_p(module));
  scm_assert(scm_fcd_symbol_p(sym));
  scm_assert(scm_obj_not_null_p(val) && !scm_fcd_landmine_object_p(val));

  module = get_module(module);
  if (scm_obj_null_p(module)) return -1;

  return scm_module_define_variable(module, sym, val, export);
}

int
scm_fcd_define_global_syx(ScmObj module, ScmObj sym, ScmObj syx, bool export)
{
  SCM_REFSTK_INIT_REG(&module, &sym, &syx);

  scm_assert(scm_fcd_module_specifier_p(module));
  scm_assert(scm_fcd_symbol_p(sym));
  scm_assert(scm_obj_not_null_p(syx) && !scm_fcd_landmine_object_p(syx));

  module = get_module(module);
  if (scm_obj_null_p(module)) return -1;

  return scm_module_define_keyword(module, sym, syx, export);
}

int
scm_fcd_global_var_ref(ScmObj module, ScmObj sym, scm_csetter_t *val)
{
  ScmObj gloc = SCM_OBJ_INIT, v = SCM_OBJ_INIT;
  int rslt;

  SCM_REFSTK_INIT_REG(&module, &sym,
                      &gloc, &v);

  scm_assert(scm_fcd_module_specifier_p(module));
  scm_assert(scm_fcd_symbol_p(sym));

  module = get_module(module);
  if (scm_obj_null_p(module)) return -1;

  rslt = scm_module_find_sym(module, sym, SCM_CSETTER_L(gloc));
  if (rslt < 0) return -1;

  if (scm_obj_not_null_p(gloc)) {
    v = scm_fcd_gloc_variable_value(gloc);
    if (scm_fcd_landmine_object_p(v))
      v = SCM_OBJ_NULL;
  }
  else {
    v = SCM_OBJ_NULL;
  }

  if (val != NULL)
    scm_csetter_setq(val, v);

  return 0;
}

int
scm_fcd_global_syx_ref(ScmObj module, ScmObj sym, scm_csetter_t *syx)
{
  ScmObj gloc = SCM_OBJ_INIT, v = SCM_OBJ_INIT;
  int rslt;

  SCM_REFSTK_INIT_REG(&module, &sym,
                      &gloc, &v);


  scm_assert(scm_fcd_module_specifier_p(module));
  scm_assert(scm_fcd_symbol_p(sym));

  module = get_module(module);
  if (scm_obj_null_p(module)) return -1;

  rslt = scm_module_find_sym(module, sym, SCM_CSETTER_L(gloc));
  if (rslt < 0) return -1;

  if (scm_obj_not_null_p(gloc)) {
    v = scm_fcd_gloc_keyword_value(gloc);
    if (scm_fcd_landmine_object_p(v))
      v = SCM_OBJ_NULL;
  }
  else
    v = SCM_OBJ_NULL;

  if (syx != NULL)
    scm_csetter_setq(syx, v);

  return 0;
}
