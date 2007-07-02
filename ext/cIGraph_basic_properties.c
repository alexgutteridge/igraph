#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

VALUE cIGraph_are_connected(VALUE self, VALUE from, VALUE to){

  igraph_t *graph;
  igraph_integer_t from_i;
  igraph_integer_t to_i;
  igraph_bool_t res;

  Data_Get_Struct(self, igraph_t, graph);

  from_i = cIGraph_get_vertex_id(self,from);
  to_i   = cIGraph_get_vertex_id(self,to);

  igraph_are_connected(graph,from_i,to_i,&res);
  
  return res ? Qtrue : Qfalse;

}
