#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.density(loops) -> Float
 *
 * Calculate the density of a graph.
 *
 * The density of a graph is simply the ratio number of edges and the number 
 * of possible edges. Note that density is ill-defined for graphs with 
 * multiple and/or loop edges, so consider calling IGraph#simplify() on the 
 * graph if you know that it contains multiple or loop edges. 
 *
 */

VALUE cIGraph_density(VALUE self, VALUE loops){

  igraph_t *graph;
  igraph_bool_t l = 0;
  igraph_real_t r;

  if(loops == Qtrue)
    l = 1;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_density(graph,&r,l);

  return rb_float_new(r);

}

/* call-seq:
 *   graph.simplify(multiple,loops) -> nil
 *
 * Removes loop and/or multiple edges from the graph.
 * multiple: Logical, if true, multiple edges will be removed. loops: Logical,
 * if true, loops (self edges) will be removed. 
 *
 */

VALUE cIGraph_simplify(VALUE self, VALUE mult, VALUE loops){

  igraph_t *graph;
  igraph_bool_t l = 0;
  igraph_bool_t m = 0;

  if(loops == Qtrue)
    l = 1;
  if(mult == Qtrue)
    m = 1;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_simplify(graph,m,l);

  return Qnil;

}

/* call-seq:
 *   graph.reciprocity(loops) -> Float
 *
 * Calculates the reciprocity of a directed graph.
 *
 * A vertex pair (A, B) is said to be reciprocal if there are edges between 
 * them in both directions. The reciprocity of a directed graph is the 
 * proportion of all possible (A, B) pairs which are reciprocal, provided 
 * there is at least one edge between A and B. The reciprocity of an empty 
 * graph is undefined (results in an error code). Undirected graphs always 
 * have a reciprocity of 1.0 unless they are empty.  
 *
 */

VALUE cIGraph_reciprocity(VALUE self, VALUE loops){

  igraph_t *graph;
  igraph_bool_t l = 0;
  igraph_real_t r;

  if(loops == Qtrue)
    l = 1;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_reciprocity(graph,&r,l);

  return rb_float_new(r);

}

/* call-seq:
 *   graph.bibcoupling(varray) -> Array
 *
 * Bibliographic coupling.
 *
 * The bibliographic coupling of two vertices is the number of other 
 * vertices they both cite. The 
 * bibliographic coupling score for each given vertex and all other 
 * vertices in the graph will be calculated.
 *
 */
VALUE cIGraph_bibcoupling(VALUE self, VALUE vs){

  igraph_t *graph;
  igraph_vs_t vids;
  igraph_vector_t vidv;
  igraph_matrix_t res;
  int i;
  int j;
  VALUE row;
  VALUE path_length;
  VALUE matrix = rb_ary_new();
  int n_row;
  int n_col;

  Data_Get_Struct(self, igraph_t, graph);

  n_row = NUM2INT(rb_funcall(vs,rb_intern("length"),0));
  n_col = igraph_vcount(graph);

  //matrix to hold the results of the calculations
  igraph_matrix_init(&res,n_row,n_col);

  //Convert an array of vertices to a vector of vertex ids
  igraph_vector_init_int(&vidv,0);
  cIGraph_vertex_arr_to_id_vec(self,vs,&vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&vids,&vidv);

  igraph_bibcoupling(graph,&res,vids);

  for(i=0; i<igraph_matrix_nrow(&res); i++){
    row = rb_ary_new();
    rb_ary_push(matrix,row);
    for(j=0; j<igraph_matrix_ncol(&res); j++){
      path_length = INT2NUM(MATRIX(res,i,j));
      rb_ary_push(row,path_length);
    }
  }

  igraph_vector_destroy(&vidv);
  igraph_matrix_destroy(&res);
  igraph_vs_destroy(&vids);

  return matrix;

}

/* call-seq:
 *   graph.cocitation(varray) -> Array
 *
 * Cocitation coupling.
 *
 * Two vertices are cocited if there is another vertex citing both of them. 
 * igraph_cocitation() simply counts how many types two vertices are cocited. 
 * The cocitation score for each given vertex and all other vertices in the 
 * graph will be calculated. 
 *
 */
VALUE cIGraph_cocitation(VALUE self, VALUE vs){

  igraph_t *graph;
  igraph_vs_t vids;
  igraph_vector_t vidv;
  igraph_matrix_t res;
  int i;
  int j;
  VALUE row;
  VALUE path_length;
  VALUE matrix = rb_ary_new();
  int n_row;
  int n_col;

  Data_Get_Struct(self, igraph_t, graph);

  n_row = NUM2INT(rb_funcall(vs,rb_intern("length"),0));
  n_col = igraph_vcount(graph);

  //matrix to hold the results of the calculations
  igraph_matrix_init(&res,n_row,n_col);

  //Convert an array of vertices to a vector of vertex ids
  igraph_vector_init_int(&vidv,0);
  cIGraph_vertex_arr_to_id_vec(self,vs,&vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&vids,&vidv);

  igraph_cocitation(graph,&res,vids);

  for(i=0; i<igraph_matrix_nrow(&res); i++){
    row = rb_ary_new();
    rb_ary_push(matrix,row);
    for(j=0; j<igraph_matrix_ncol(&res); j++){
      path_length = INT2NUM(MATRIX(res,i,j));
      rb_ary_push(row,path_length);
    }
  }

  igraph_vector_destroy(&vidv);
  igraph_matrix_destroy(&res);
  igraph_vs_destroy(&vids);

  return matrix;

}

/* call-seq:
 *   graph.get_adjacency(type) -> Array
 *
 * Returns the adjacency matrix of a graph
 *
 */
VALUE cIGraph_get_adjacency(VALUE self, VALUE mode){

  igraph_t *graph;
  igraph_get_adjacency_t pmode = NUM2INT(mode);
  igraph_matrix_t res;
  int i;
  int j;
  VALUE row;
  VALUE path_length;
  VALUE matrix = rb_ary_new();
  int n;

  Data_Get_Struct(self, igraph_t, graph);

  n = igraph_vcount(graph);

  //matrix to hold the results of the calculations
  igraph_matrix_init(&res,n,n);

  igraph_get_adjacency(graph,&res,pmode);

  for(i=0; i<igraph_matrix_nrow(&res); i++){
    row = rb_ary_new();
    rb_ary_push(matrix,row);
    for(j=0; j<igraph_matrix_ncol(&res); j++){
      path_length = INT2NUM(MATRIX(res,i,j));
      rb_ary_push(row,path_length);
    }
  }

  igraph_matrix_destroy(&res);

  return matrix;

}
