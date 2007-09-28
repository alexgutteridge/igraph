#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.laplacian(mode) -> Array
 *
 *  Returns the Laplacian matrix of a graph
 *
 * The graph Laplacian matrix is similar to an adjacency matrix but 
 * contains -1's instead of 1's and the vertex degrees are included 
 * in the diagonal. So the result for edge i--j is -1 if i!=j and is 
 * equal to the degree of vertex i if i==j. igraph_laplacian will work 
 * on a directed graph (although this does not seem to make much sense) 
 * and ignores loops.
 *
 * Mode is a boolean specifying whether the normalized version should be used
 * The normalised Laplacian matrix has 1 in the diagonal 
 * and -1/sqrt(d[i]d[j]) if there is an edge from i to j.
 *
 * The first version of this function was written by Vincent Matossian. 
 */
VALUE cIGraph_laplacian(VALUE self, VALUE mode){

  igraph_t *graph;
  igraph_bool_t pmode = 0;
  igraph_matrix_t res;
  int i;
  int j;
  VALUE row;
  VALUE val;
  VALUE matrix = rb_ary_new();

  if(mode == Qtrue)
    pmode = 1;

  Data_Get_Struct(self, igraph_t, graph);

  //matrix to hold the results of the calculations
  igraph_matrix_init(&res,igraph_vcount(graph),igraph_vcount(graph));

  IGRAPH_CHECK(igraph_laplacian(graph,&res,pmode));

  for(i=0; i<igraph_matrix_nrow(&res); i++){
    row = rb_ary_new();
    rb_ary_push(matrix,row);
    for(j=0; j<igraph_matrix_ncol(&res); j++){
      val = rb_float_new(MATRIX(res,i,j));
      rb_ary_push(row,val);
    }
  }

  igraph_matrix_destroy(&res);

  return matrix;

}
