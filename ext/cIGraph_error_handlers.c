#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

void cIGraph_error_handler(const char *reason, const char *file,
                             int line, int igraph_errno) {
  rb_raise(cIGraphError, reason);
}

void cIGraph_warning_handler(const char *reason, const char *file,
                             int line, int igraph_errno) {
  rb_warning(reason);
}
