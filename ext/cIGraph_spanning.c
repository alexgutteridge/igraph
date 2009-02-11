#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.minimum_spanning_tree_unweighted() -> IGraph
 *
 * Calculates one minimum spanning tree of an unweighted graph.
 *
 * If the graph has more minimum spanning trees (this is always the case, 
 * except if it is a forest) this implementation returns only the same one.
 *
 * Directed graphs are considered as undirected for this computation.
 *
 * If the graph is not connected then its minimum spanning forest is returned.
 * This is the set of the minimum spanning trees of each component. 
 */
VALUE cIGraph_minimum_spanning_tree_unweighted(VALUE self){

  igraph_t *graph;
  igraph_t *n_graph = malloc(sizeof(igraph_t));
  VALUE n_graph_obj;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_minimum_spanning_tree_unweighted(graph,n_graph);

  n_graph_obj = Data_Wrap_Struct(cIGraph, cIGraph_mark, cIGraph_free, n_graph);

  return n_graph_obj;

}

/* call-seq:
 *   graph.minimum_spanning_tree_prim(weights) -> IGraph
 *
 * Calculates one minimum spanning tree of a weighted graph.
 *
 * This function uses Prim's method for carrying out the computation, see 
 * Prim, R.C.: Shortest connection networks and some generalizations, Bell 
 * System Technical Journal, Vol. 36, 1957, 1389--1401.
 *
 * If the graph has more than one minimum spanning tree, the current 
 * implementation returns always the same one.
 *
 * Directed graphs are considered as undirected for this computation.
 *
 * If the graph is not connected then its minimum spanning forest is returned.
 * This is the set of the minimum spanning trees of each component. 
 *
 * The weights Array must contain the weights of the the edges. in the same 
 * order as the simple edge iterator visits them. 
 */
VALUE cIGraph_minimum_spanning_tree_prim(VALUE self, VALUE weights){

  igraph_t *graph;
  igraph_t *n_graph = malloc(sizeof(igraph_t));
  VALUE n_graph_obj;
  igraph_vector_t weights_vec;
  int i;

  igraph_vector_init(&weights_vec,RARRAY_LEN(weights));

  Data_Get_Struct(self, igraph_t, graph);

  for(i=0;i<RARRAY_LEN(weights);i++){
    VECTOR(weights_vec)[i] = NUM2DBL(RARRAY_PTR(weights)[i]);
  }

  igraph_minimum_spanning_tree_prim(graph,n_graph,&weights_vec);

  n_graph_obj = Data_Wrap_Struct(cIGraph, cIGraph_mark, cIGraph_free, n_graph);

  igraph_vector_destroy(&weights_vec);

  return n_graph_obj;

}
