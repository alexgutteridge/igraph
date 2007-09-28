#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.independent_vertex_sets(min_size,max_size) -> Array
 *
 * Find all independent vertex sets in a graph
 *
 * A vertex set is considered independent if there are no edges between them.
 *
 * If you are interested in the size of the largest independent vertex set, 
 * use IGraph#independence_number() instead.
 */

VALUE cIGraph_independent_vertex_sets(VALUE self, VALUE min, VALUE max){

  igraph_t *graph;
  igraph_vector_ptr_t res;
  igraph_vector_t *vec;
  int i;
  int j;
  VALUE independent_vertex_set;
  VALUE object;
  VALUE independent_vertex_sets = rb_ary_new();

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_ptr_init(&res,0);

  igraph_independent_vertex_sets(graph, &res, NUM2INT(min), NUM2INT(max));

  for(i=0; i<igraph_vector_ptr_size(&res); i++){
    independent_vertex_set = rb_ary_new();
    rb_ary_push(independent_vertex_sets,independent_vertex_set);
    vec = VECTOR(res)[i];
    for(j=0; j<igraph_vector_size(vec); j++){
      vec = VECTOR(res)[i];
      object = cIGraph_get_vertex_object(self,VECTOR(*vec)[j]);
      rb_ary_push(independent_vertex_set,object);
    }
  }

  for(i=0;i<igraph_vector_ptr_size(&res);i++){
    igraph_vector_destroy(VECTOR(res)[i]);
    free(VECTOR(res)[i]);
  }

  igraph_vector_ptr_destroy(&res);

  return independent_vertex_sets;
  
}

/* call-seq:
 *   graph.largest_independent_vertex_sets() -> Array
 *
 * Finds the largest independent vertex set(s) in a graph.

 * An independent vertex set is largest if there is no other independent 
 * vertex set with more vertices in the graph. 
 */

VALUE cIGraph_largest_independent_vertex_sets(VALUE self){

  igraph_t *graph;
  igraph_vector_ptr_t res;
  igraph_vector_t *vec;
  int i;
  int j;
  VALUE independent_vertex_set;
  VALUE object;
  VALUE independent_vertex_sets = rb_ary_new();

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_ptr_init(&res,0);

  igraph_largest_independent_vertex_sets(graph, &res);

  for(i=0; i<igraph_vector_ptr_size(&res); i++){
    independent_vertex_set = rb_ary_new();
    rb_ary_push(independent_vertex_sets,independent_vertex_set);
    vec = VECTOR(res)[i];
    for(j=0; j<igraph_vector_size(vec); j++){
      vec = VECTOR(res)[i];
      object = cIGraph_get_vertex_object(self,VECTOR(*vec)[j]);
      rb_ary_push(independent_vertex_set,object);
    }
  }

  for(i=0;i<igraph_vector_ptr_size(&res);i++){
    igraph_vector_destroy(VECTOR(res)[i]);
    free(VECTOR(res)[i]);
  }

  igraph_vector_ptr_destroy(&res);

  return independent_vertex_sets;
  
}

/* call-seq:
 *   graph.maximal_independent_vertex_sets() -> Array
 *
 * Find all maximal independent vertex sets of a graph
 *
 * A maximal independent vertex set is an independent vertex set which can't 
 * be extended any more by adding a new vertex to it.
 *
 * The algorithm used here is based on the following paper: S. Tsukiyama, 
 * M. Ide, H. Ariyoshi and I. Shirawaka. A new algorithm for generating all 
 * the maximal independent sets. SIAM J Computing, 6:505--517, 1977.
 *
 * The implementation was originally written by Kevin O'Neill and modified 
 * by K M Briggs in the Very Nauty Graph Library. I simply re-wrote it to 
 * use igraph's data structures.
 *
 * If you are interested in the size of the largest independent vertex set, 
 * use IGraph#independence_number() instead. 
 */

VALUE cIGraph_maximal_independent_vertex_sets(VALUE self){

  igraph_t *graph;
  igraph_vector_ptr_t res;
  igraph_vector_t *vec;
  int i;
  int j;
  VALUE independent_vertex_set;
  VALUE object;
  VALUE independent_vertex_sets = rb_ary_new();

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_ptr_init(&res,0);

  igraph_maximal_independent_vertex_sets(graph, &res);

  for(i=0; i<igraph_vector_ptr_size(&res); i++){
    independent_vertex_set = rb_ary_new();
    rb_ary_push(independent_vertex_sets,independent_vertex_set);
    vec = VECTOR(res)[i];
    for(j=0; j<igraph_vector_size(vec); j++){
      vec = VECTOR(res)[i];
      object = cIGraph_get_vertex_object(self,VECTOR(*vec)[j]);
      rb_ary_push(independent_vertex_set,object);
    }
  }

  for(i=0;i<igraph_vector_ptr_size(&res);i++){
    igraph_vector_destroy(VECTOR(res)[i]);
    free(VECTOR(res)[i]);
  }

  igraph_vector_ptr_destroy(&res);

  return independent_vertex_sets;
  
}

/* call-seq:
 *   graph.independence_number() -> Integer
 *
 * Find the independence number of the graph
 *
 * The independence number of a graph is the cardinality of the largest 
 * independent vertex set. 
 */

VALUE cIGraph_independence_number(VALUE self){

  igraph_t *graph;
  igraph_integer_t res;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_independence_number(graph, &res);

  return INT2NUM(res);
  
}
