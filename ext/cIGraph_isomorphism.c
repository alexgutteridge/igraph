#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.isomorphic(graph) -> True/False
 *
 * Decides whether two graphs are isomorphic
 *
 * From Wikipedia: The graph isomorphism problem or GI problem is the graph 
 * theory problem of determining whether, given two graphs G1 and G2, it is 
 * possible to permute (or relabel) the vertices of one graph so that it is 
 * equal to the other. Such a permutation is called a graph isomorphism. 
 */
VALUE cIGraph_isomorphic(VALUE self, VALUE g){

  igraph_bool_t res = 0;
  igraph_t *graph;
  igraph_t *graph2;

  Data_Get_Struct(self, igraph_t, graph);
  Data_Get_Struct(g,    igraph_t, graph2);

  IGRAPH_CHECK(igraph_isomorphic(graph,graph2,&res));

  return res == 0 ? Qfalse : Qtrue;

}

/* call-seq:
 *   graph.isomorphic_vf2(graph) -> True/False
 *
 * Decides whether two graphs are isomorphic
 *
 * This function is an implementation of the VF2 isomorphism algorithm, 
 * see P. Foggia, C. Sansone, M. Vento, An Improved algorithm for matching 
 * large graphs, Prof. of the 3rd IAPR-TC-15 International Workshop on 
 * Graph-based Representations, Italy, 2001.
 */
VALUE cIGraph_isomorphic_vf2(VALUE self, VALUE g){

  igraph_bool_t res = 0;
  igraph_t *graph;
  igraph_t *graph2;

  Data_Get_Struct(self, igraph_t, graph);
  Data_Get_Struct(g,    igraph_t, graph2);

  IGRAPH_CHECK(igraph_isomorphic_vf2(graph,graph2,&res));

  return res == 0 ? Qfalse : Qtrue;

}

/* call-seq:
 *   graph.isoclass() -> Integer
 *
 *  Determine the isomorphism class of a graph
 *
 * All graphs with a given number of vertices belong to a number of 
 * isomorpism classes, with every graph in a given class being isomorphic 
 * to each other.
 *
 * This function gives the isomorphism class (a number) of a graph. Two 
 * graphs have the same isomorphism class if and only if they are isomorphic.
 *
 * The first isomorphism class is numbered zero and it is the empty graph, 
 * the last isomorphism class is the full graph. The number of isomorphism 
 * class for directed graphs with three vertices is 16 (between 0 and 15), 
 * for undirected graph it is only 4. For graphs with four vertices it is 
 * 218 (directed) and 11 (undirected). 
 */
VALUE cIGraph_isoclass(VALUE self){

  int res = 0;
  igraph_t *graph;

  Data_Get_Struct(self, igraph_t, graph);

  IGRAPH_CHECK(igraph_isoclass(graph,&res));

  return INT2NUM(res);

}

/* call-seq:
 *   graph.isoclass_subgraph(vs) -> Integer
 *
 * Determine the isomorphism class of a subgraph given by the vertices given
 * in the Array vs.
 *
 */
VALUE cIGraph_isoclass_subgraph(VALUE self, VALUE vs){

  int res = 0;
  igraph_t *graph;
  igraph_vector_t vidv;

  Data_Get_Struct(self, igraph_t, graph);

  //Convert an array of vertices to a vector of vertex ids
  igraph_vector_init_int(&vidv,0);
  cIGraph_vertex_arr_to_id_vec(self,vs,&vidv);

  IGRAPH_CHECK(igraph_isoclass_subgraph(graph,&vidv,&res));

  igraph_vector_destroy(&vidv);

  return INT2NUM(res);

}

/* call-seq:
 *   IGraph.isoclass_create(vn,iso,dir) -> IGraph
 * 
 * Creates a graph with the number of vertices given by vn from the given 
 * isomorphism class iso and the direction boolean dir.
 *
 * This function is implemented only for graphs with three or four vertices.
 */
VALUE cIGraph_isoclass_create(VALUE self, VALUE vn, VALUE iso, VALUE dir){

  igraph_t *graph;
  VALUE new_graph;
  igraph_bool_t dir_b = 0;

  if(dir)
    dir_b = 1;

  new_graph = cIGraph_alloc(cIGraph);  
  Data_Get_Struct(new_graph, igraph_t, graph);

  IGRAPH_CHECK(igraph_isoclass_create(graph,NUM2INT(vn),NUM2INT(iso),dir_b));

  return new_graph;

}
