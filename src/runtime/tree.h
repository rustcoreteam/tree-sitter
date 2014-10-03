#ifndef RUNTIME_TREE_H_
#define RUNTIME_TREE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "tree_sitter/parser.h"

typedef enum {
  TSTreeOptionsHidden = 1,
  TSTreeOptionsExtra = 2,
  TSTreeOptionsWrapper = 4,
} TSTreeOptions;

struct TSTree {
  TSSymbol symbol;
  TSTreeOptions options;
  size_t ref_count;
  TSLength padding;
  TSLength size;
  char lookahead_char;
  size_t child_count;
  size_t visible_child_count;
  struct TSTree **children;
};

typedef struct {
  TSTree *tree;
  TSLength offset;
} TSTreeChild;

static inline int ts_tree_is_extra(const TSTree *tree) {
  return (tree->options & TSTreeOptionsExtra);
}

static inline int ts_tree_is_visible(const TSTree *tree) {
  return !(tree->options & TSTreeOptionsHidden);
}

static inline void ts_tree_set_extra(TSTree *tree) {
  tree->options = (TSTreeOptions)(tree->options | TSTreeOptionsExtra);
}

static inline int ts_tree_is_wrapper(const TSTree *tree) {
  return (tree->options & TSTreeOptionsWrapper);
}

TSTree *ts_tree_make_leaf(TSSymbol, TSLength, TSLength, bool);
TSTree *ts_tree_make_node(TSSymbol, size_t, TSTree **, bool);
TSTree *ts_tree_make_error(TSLength size, TSLength padding, char lookahead_char);
void ts_tree_retain(TSTree *tree);
void ts_tree_release(TSTree *tree);
bool ts_tree_eq(const TSTree *tree1, const TSTree *tree2);
char *ts_tree_string(const TSTree *tree, const char **names);
char *ts_tree_error_string(const TSTree *tree, const char **names);
TSTree **ts_tree_children(const TSTree *tree, size_t *count);
TSTreeChild *ts_tree_visible_children(const TSTree *tree, size_t *count);
TSLength ts_tree_total_size(const TSTree *tree);

#ifdef __cplusplus
}
#endif

#endif  // RUNTIME_TREE_H_
