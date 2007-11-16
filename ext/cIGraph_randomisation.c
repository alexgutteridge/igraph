#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   g.rewire_edges(prob) -> IGraph
 *
 * Rewire the edges of a graph with constant probability This function 
 * rewires the edges of a graph with a constant probability. More precisely 
 * each end point of each edge is rewired to an uniformly randomly chosen 
 * vertex with constant probability prob.
 *
 * prob: The rewiring probability a constant between zero and one (inclusive). 
 */
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

/* call-seq:
 *   g.rewire(n) -> IGraph
 *
 * Randomly rewires a graph while preserving the degree distribution.
 *
 * This function generates a new graph based on the original one by randomly 
 * rewiring edges while preserving the original graph's degree distribution.
 *
 * n: Number of rewiring trials to perform.
 */
VALUE cIGraph_rewire(VALUE self, VALUE n){

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
