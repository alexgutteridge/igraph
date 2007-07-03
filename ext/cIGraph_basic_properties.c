#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.are_connected?(from,to) -> true/false
 *
 * Returns a boolean specifying whether the two vertices specified are
 * connected by an edge.
 *
 * Example:
 *
 *   IGraph.new([1,2,3,4],true)
 *   graph.are_connected?(1,2) #returns true
 *
 */

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
