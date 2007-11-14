#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.maxflow_value(source,target,capacity) -> Max Flow
 *
 *  Maximum flow in a network with the push/relabel algorithm
 *
 * This function implements the Goldberg-Tarjan algorithm for calculating 
 * value of the maximum flow in a directed or undirected graph. The algorithm 
 * was given in Andrew V. Goldberg, Robert E. Tarjan: A New Approach to the 
 * Maximum-Flow Problem, Journal of the ACM, 35(4), 921-940, 1988.
 *
 * Note that the value of the maximum flow is the same as the minimum cut in 
 * the graph. 
 *
 */

VALUE cIGraph_maxflow_value(VALUE self, VALUE source, VALUE target, VALUE capacity){

  igraph_t *graph;
  igraph_integer_t from_i;
  igraph_integer_t to_i;
  igraph_real_t value;
  
  int i;

  igraph_vector_t capacity_v;

  igraph_vector_init(&capacity_v, 0);
  for(i=0;i<RARRAY(capacity)->len;i++){
    igraph_vector_push_back(&capacity_v,NUM2DBL(RARRAY(capacity)->ptr[i]));
  }

  Data_Get_Struct(self, igraph_t, graph);

  from_i = cIGraph_get_vertex_id(self,source);
  to_i   = cIGraph_get_vertex_id(self,target);

  igraph_maxflow_value(graph,&value,from_i,to_i,&capacity_v);
  
  igraph_vector_destroy(&capacity_v);

  return rb_float_new(value);

}

/* call-seq:
 *   graph.st_mincut_value(source,target,capacity) -> Max Flow
 *
 *  Maximum flow in a network with the push/relabel algorithm
 *
 * This function implements the Goldberg-Tarjan algorithm for calculating 
 * value of the maximum flow in a directed or undirected graph. The algorithm 
 * was given in Andrew V. Goldberg, Robert E. Tarjan: A New Approach to the 
 * Maximum-Flow Problem, Journal of the ACM, 35(4), 921-940, 1988.
 *
 * Note that the value of the maximum flow is the same as the minimum cut in 
 * the graph. 
 *
 */

VALUE cIGraph_st_mincut_value(VALUE self, VALUE source, VALUE target, VALUE capacity){

  igraph_t *graph;
  igraph_integer_t from_i;
  igraph_integer_t to_i;
  igraph_real_t value;
  
  int i;

  igraph_vector_t capacity_v;

  igraph_vector_init(&capacity_v, 0);
  for(i=0;i<RARRAY(capacity)->len;i++){
    igraph_vector_push_back(&capacity_v,NUM2DBL(RARRAY(capacity)->ptr[i]));
  }

  Data_Get_Struct(self, igraph_t, graph);

  from_i = cIGraph_get_vertex_id(self,source);
  to_i   = cIGraph_get_vertex_id(self,target);

  igraph_st_mincut_value(graph,&value,from_i,to_i,&capacity_v);
  
  igraph_vector_destroy(&capacity_v);

  return rb_float_new(value);

}

/* call-seq:
 *   graph.mincut_value(capacity) -> Float
 *
 * The minimum edge cut in a graph is the total minimum weight of the edges 
 * needed to remove from the graph to make the graph not strongly connected.
 * (If the original graph is not strongly connected then this is zero.) Note
 * that in undirected graphs strong connectedness is the same as weak 
 * connectedness.
 *
 */

VALUE cIGraph_mincut_value(VALUE self, VALUE capacity){

  igraph_t *graph;
  igraph_real_t value;
  
  int i;

  igraph_vector_t capacity_v;

  igraph_vector_init(&capacity_v, 0);
  for(i=0;i<RARRAY(capacity)->len;i++){
    igraph_vector_push_back(&capacity_v,NUM2DBL(RARRAY(capacity)->ptr[i]));
  }

  Data_Get_Struct(self, igraph_t, graph);

  igraph_mincut_value(graph,&value,&capacity_v);
  
  igraph_vector_destroy(&capacity_v);

  return rb_float_new(value);

}

/* call-seq:
 *   graph.mincut(capacity) -> Array
 *
 * Calculates the minimum cut in a graph. This function calculates the 
 * minimum cut in a graph. Right now it is implemented only for undirected 
 * graphs, in which case it uses the Stoer-Wagner algorithm, as described 
 * in M. Stoer and F. Wagner: A simple min-cut algorithm, Journal of the 
 * ACM, 44 585-591, 1997.
 *
 */

VALUE cIGraph_mincut(VALUE self, VALUE capacity){

  VALUE res_ary;

  VALUE p1_a;
  VALUE p2_a;
  VALUE cut_a;

  igraph_t *graph;
  igraph_real_t value;
  
  int i;

  igraph_vector_t p1;
  igraph_vector_t p2;
  igraph_vector_t cut;

  igraph_vector_t capacity_v;

  igraph_vector_init(&p1,  0);
  igraph_vector_init(&p2,  0);
  igraph_vector_init(&cut, 0);  

  igraph_vector_init(&capacity_v, 0);
  for(i=0;i<RARRAY(capacity)->len;i++){
    igraph_vector_push_back(&capacity_v,NUM2DBL(RARRAY(capacity)->ptr[i]));
  }

  Data_Get_Struct(self, igraph_t, graph);

  igraph_mincut(graph,&value,&p1,&p2,&cut,&capacity_v);

  p1_a = rb_ary_new();
  for(i=0;i<igraph_vector_size(&p1);i++){
    rb_ary_push(p1_a,cIGraph_get_vertex_object(self,VECTOR(p1)[i]));
  }
  p2_a = rb_ary_new();
  for(i=0;i<igraph_vector_size(&p2);i++){
    rb_ary_push(p2_a,cIGraph_get_vertex_object(self,VECTOR(p2)[i]));
  }
  cut_a = rb_ary_new();
  for(i=0;i<igraph_vector_size(&cut);i++){
    rb_ary_push(cut_a,INT2NUM(VECTOR(cut)[i]));
  }

  res_ary = rb_ary_new3(4,rb_float_new(value),p1_a,p2_a,cut_a);

  igraph_vector_destroy(&p1);
  igraph_vector_destroy(&p2);
  igraph_vector_destroy(&cut);
  
  igraph_vector_destroy(&capacity_v);

  return res_ary;

}

