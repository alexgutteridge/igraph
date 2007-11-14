#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

VALUE cIGraph_rewire_edges(VALUE self, VALUE prop){

  igraph_t *graph;
  igraph_t *copy_graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, copy_graph);
  Data_Get_Struct(self,      igraph_t, graph);

  igraph_copy(copy_graph,graph);
  igraph_rewire_edges(copy_graph,NUM2DBL(prop));

  return new_graph;

}

VALUE cIGraph_rewire(VALUE self, VALUE n, VALUE mode){

  igraph_t *graph;
  igraph_t *copy_graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, copy_graph);
  Data_Get_Struct(self,      igraph_t, graph);

  igraph_copy(copy_graph,graph);
  igraph_rewire(copy_graph,NUM2INT(n),0);

  return new_graph;

}
