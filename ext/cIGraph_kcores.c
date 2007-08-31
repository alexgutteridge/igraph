#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.coreness(mode) -> Array
 *
 * Finding the coreness of the vertices in a network. The k-core of a 
 * graph is a maximal subgraph in which each vertex has at least degree k.
 * (Degree here means the degree in the subgraph of course.). The coreness 
 * of a vertex is the highest order of a k-core containing the vertex.
 *
 * This function implements the algorithm presented in Vladimir Batagelj, 
 * Matjaz Zaversnik: An O(m) Algorithm for Cores Decomposition of Networks. 
 */
VALUE cIGraph_coreness(VALUE self, VALUE mode){

  igraph_t *graph;
  igraph_neimode_t pmode = NUM2INT(mode);
  igraph_vector_t cores;
  int i;
  VALUE result = rb_ary_new();
  
  Data_Get_Struct(self, igraph_t, graph);

  //vector to hold the results of the calculations
  igraph_vector_init(&cores,0);

  igraph_coreness(graph,&cores,pmode);

  for(i=0; i< igraph_vector_size(&cores); i++){
    rb_ary_push(result,VECTOR(cores)[i]);
  }

  igraph_vector_destroy(&cores);

  return result;

}
