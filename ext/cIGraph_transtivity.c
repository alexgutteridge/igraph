#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.transitivity() -> Float
 *
 * Calculates the transitivity (clustering coefficient) of a graph.
 *
 * The transitivity measures the probability that two neighbors of a vertex 
 * are connected. More precisely this is the ratio of the triangles and 
 * connected triples in the graph, the result is a single real number or 
 * NaN (0/0) if there are no connected triples in the graph. Directed graphs 
 * are considered as undirected ones.
 */
VALUE cIGraph_transitivity(VALUE self){

  igraph_t *graph;
  igraph_real_t res;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_transitivity_undirected(graph,&res);

  return rb_float_new(res);

}

/* call-seq:
 *   graph.transitivity() -> Float
 *
 * Calculates the transitivity (clustering coefficient) of a graph.
 *
 * The transitivity measures the probability that two neighbors of a vertex 
 * are connected. More precisely this is the ratio of the triangles and 
 * connected triples in the graph, the result is a single real number or 
 * NaN (0/0) if there are no connected triples in the graph. Directed graphs 
 * are considered as undirected ones.
 */
VALUE cIGraph_transitivity_local(VALUE self, VALUE vs){

  igraph_t *graph;
  igraph_vs_t vids;
  igraph_vector_t vidv;
  igraph_vector_t res;
  VALUE trans = rb_ary_new();
  int i;

  //vector to hold the results of the calculations
  igraph_vector_init_int(&res,0);
 
  //Convert an array of vertices to a vector of vertex ids
  igraph_vector_init_int(&vidv,0);
  cIGraph_vertex_arr_to_id_vec(self,vs,&vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&vids,&vidv);

  Data_Get_Struct(self, igraph_t, graph);

  igraph_transitivity_local_undirected(graph,&res,vids);

  for(i=0;i<igraph_vector_size(&res);i++){
    rb_ary_push(trans,rb_float_new(VECTOR(res)[i]));
  }

  igraph_vector_destroy(&vidv);
  igraph_vector_destroy(&res);
  igraph_vs_destroy(&vids);

  return trans;

}
  
/* call-seq:
 *   graph.transitivity_avglocal() -> Float
 *
 * Calculates the transitivity (clustering coefficient) of a graph.
 *
 * The transitivity measures the probability that two neighbors of a vertex 
 * are connected. More precisely this is the ratio of the triangles and 
 * connected triples in the graph, the result is a single real number or 
 * NaN (0/0) if there are no connected triples in the graph. Directed graphs 
 * are considered as undirected ones.
 */
VALUE cIGraph_transitivity_avglocal(VALUE self){

  igraph_t *graph;
  igraph_real_t res;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_transitivity_avglocal_undirected(graph,&res);

  return rb_float_new(res);

}

