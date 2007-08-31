#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.to_directed(mode)
 *
 * Converts the graph to a directed graph. 
 */
VALUE cIGraph_to_directed(VALUE self, VALUE mode){

  igraph_t *graph;
  igraph_neimode_t pmode = NUM2INT(mode);
  int ret;

  Data_Get_Struct(self, igraph_t, graph);
  IGRAPH_CHECK(ret = igraph_to_directed(graph,pmode));

  return INT2NUM(ret);

}

/* call-seq:
 *   graph.to_undirected(mode)
 *
 * Converts the graph to a directed graph. 
 */
VALUE cIGraph_to_undirected(VALUE self, VALUE mode){

  igraph_t *graph;
  igraph_neimode_t pmode = NUM2INT(mode);
  int ret;

  Data_Get_Struct(self, igraph_t, graph);
  IGRAPH_CHECK(ret = igraph_to_undirected(graph,pmode));

  return INT2NUM(ret);

}
