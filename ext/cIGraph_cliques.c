#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.cliques(min_size,max_size) -> Array
 *
 * Find all or some cliques in a graph
 *
 * Cliques are fully connected subgraphs of a graph.
 *
 * If you are only interested in the size of the largest clique in the 
 * graph, use IGraph#clique_number instead. 
 */

VALUE cIGraph_cliques(VALUE self, VALUE min, VALUE max){

  igraph_t *graph;
  igraph_vector_ptr_t res;
  igraph_vector_t *vec;
  int i;
  int j;
  VALUE clique;
  VALUE object;
  VALUE cliques = rb_ary_new();

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_ptr_init(&res,0);

  igraph_cliques(graph, &res, NUM2INT(min), NUM2INT(max));

  for(i=0; i<igraph_vector_ptr_size(&res); i++){
    clique = rb_ary_new();
    rb_ary_push(cliques,clique);
    vec = VECTOR(res)[i];
    for(j=0; j<igraph_vector_size(vec); j++){
      vec = VECTOR(res)[i];
      object = cIGraph_get_vertex_object(self,VECTOR(*vec)[j]);
      rb_ary_push(clique,object);
    }
  }

  for(i=0;i<igraph_vector_ptr_size(&res);i++){
    igraph_vector_destroy(VECTOR(res)[i]);
    free(VECTOR(res)[i]);
  }

  igraph_vector_ptr_destroy(&res);

  return cliques;
  
}

/* call-seq:
 *   graph.largest_cliques() -> Array
 *
 * Finds the largest clique(s) in a graph.
 *
 * A clique is largest (quite intuitively) if there is no other clique in 
 * the graph which contains more vertices.
 *
 * Note that this is not neccessarily the same as a maximal clique, ie. 
 * the largest cliques are always maximal but a maximal clique is not always 
 * largest. 
 */

VALUE cIGraph_largest_cliques(VALUE self){

  igraph_t *graph;
  igraph_vector_ptr_t res;
  igraph_vector_t *vec;
  int i;
  int j;
  VALUE clique;
  VALUE object;
  VALUE cliques = rb_ary_new();

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_ptr_init(&res,0);

  igraph_largest_cliques(graph, &res);

  for(i=0; i<igraph_vector_ptr_size(&res); i++){
    clique = rb_ary_new();
    rb_ary_push(cliques,clique);
    vec = VECTOR(res)[i];
    for(j=0; j<igraph_vector_size(vec); j++){
      vec = VECTOR(res)[i];
      object = cIGraph_get_vertex_object(self,VECTOR(*vec)[j]);
      rb_ary_push(clique,object);
    }
  }

  for(i=0;i<igraph_vector_ptr_size(&res);i++){
    igraph_vector_destroy(VECTOR(res)[i]);
    free(VECTOR(res)[i]);
  }

  igraph_vector_ptr_destroy(&res);

  return cliques;
  
}

/* call-seq:
 *   graph.maximal_cliques() -> Array
 *
 * Find all maximal cliques of a graph
 *
 * A maximal clique is a clique which can't be extended any more by adding a 
 * new vertex to it. This is actually implemented by looking for a maximal 
 * independent vertex set in the complementer of the graph.
 *
 * If you are only interested in the size of the largest clique in the 
 * graph, use IGraph#clique_number instead. 
 */

VALUE cIGraph_maximal_cliques(VALUE self){

  igraph_t *graph;
  igraph_vector_ptr_t res;
  igraph_vector_t *vec;
  int i;
  int j;
  VALUE clique;
  VALUE object;
  VALUE cliques = rb_ary_new();

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_ptr_init(&res,0);

  igraph_maximal_cliques(graph, &res);

  for(i=0; i<igraph_vector_ptr_size(&res); i++){
    clique = rb_ary_new();
    rb_ary_push(cliques,clique);
    vec = VECTOR(res)[i];
    for(j=0; j<igraph_vector_size(vec); j++){
      vec = VECTOR(res)[i];
      object = cIGraph_get_vertex_object(self,VECTOR(*vec)[j]);
      rb_ary_push(clique,object);
    }
  }

  for(i=0;i<igraph_vector_ptr_size(&res);i++){
    igraph_vector_destroy(VECTOR(res)[i]);
    free(VECTOR(res)[i]);
  }

  igraph_vector_ptr_destroy(&res);

  return cliques;
  
}

/* call-seq:
 *   graph.clique_number() -> Integer
 *
 * Find the clique number of the graph
 *
 * The clique number of a graph is the size of the largest clique. 
 */

VALUE cIGraph_clique_number(VALUE self){

  igraph_t *graph;
  igraph_integer_t res;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_clique_number(graph, &res);

  return INT2NUM(res);
  
}
