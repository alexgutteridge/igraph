#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.neighbourhood_size(vertices,order,mode) -> Array
 *
 * Calculates the length of the shortest paths from each of the vertices in
 * the varray Array to all of the other vertices in the graph. The result
 * is returned as an Array of Array. Each top-level Array contains the results
 * for a vertex in the varray Array. Each entry in the Array is the path length
 * to another vertex in the graph in vertex order (the order the vertices were
 * added to the graph. (This should probalby be changed to give a Hash of Hash
 * to allow easier look up.)
 */
VALUE cIGraph_neighborhood_size(VALUE self, VALUE from, VALUE order, VALUE mode){

  igraph_t *graph;
  igraph_vs_t vids;
  igraph_vector_t vidv;
  igraph_neimode_t pmode = NUM2INT(mode);
  igraph_vector_t res;
  int i;

  VALUE sizes = rb_ary_new();

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_init(&res,0);

  //Convert an array of vertices to a vector of vertex ids
  cIGraph_vertex_arr_to_id_vec(self,from,&vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&vids,&vidv);

  igraph_neighborhood_size(graph,&res,vids,NUM2INT(order),pmode);

  for(i=0; i<igraph_vector_size(&res); i++){
    rb_ary_push(sizes,INT2NUM(VECTOR(res)[i]));
  }

  igraph_vector_destroy(&vidv);
  igraph_vector_destroy(&res);
  igraph_vs_destroy(&vids);

  return sizes;

}

/* call-seq:
 *   graph.neighbourhood(vertices,order,mode) -> Array
 *
 * Calculates the length of the shortest paths from each of the vertices in
 * the varray Array to all of the other vertices in the graph. The result
 * is returned as an Array of Array. Each top-level Array contains the results
 * for a vertex in the varray Array. Each entry in the Array is the path length
 * to another vertex in the graph in vertex order (the order the vertices were
 * added to the graph. (This should probalby be changed to give a Hash of Hash
 * to allow easier look up.)
 */
VALUE cIGraph_neighborhood(VALUE self, VALUE from, VALUE order, VALUE mode){

  igraph_t *graph;
  igraph_vs_t vids;
  igraph_vector_t vidv;
  igraph_neimode_t pmode = NUM2INT(mode);
  igraph_vector_ptr_t res;
  igraph_vector_t *path_v;
  int i;
  int j;

  VALUE matrix = rb_ary_new();
  VALUE neighbourhood;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_ptr_init(&res,0);

  //Convert an array of vertices to a vector of vertex ids
  cIGraph_vertex_arr_to_id_vec(self,from,&vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&vids,&vidv);

  igraph_neighborhood(graph,&res,vids,NUM2INT(order),pmode);

  for(i=0; i<igraph_vector_ptr_size(&res); i++){
    neighbourhood = rb_ary_new();
    rb_ary_push(matrix,neighbourhood);
    path_v = VECTOR(res)[i];
    for(j=0; j<igraph_vector_size(VECTOR(res)[i]); j++){
      rb_ary_push(neighbourhood,cIGraph_get_vertex_object(self,VECTOR(*path_v)[j]));
    }
  }

  for(i=0;i<igraph_vector_ptr_size(&res);i++){
    igraph_vector_destroy(VECTOR(res)[i]);
  }  

  igraph_vector_destroy(&vidv);
  igraph_vector_ptr_destroy(&res);
  igraph_vs_destroy(&vids);

  return matrix;

}

/* call-seq:
 *   graph.neighbourhood_graph(vertices,order,mode) -> Array
 *
 * Calculates the length of the shortest paths from each of the vertices in
 * the varray Array to all of the other vertices in the graph. The result
 * is returned as an Array of Array. Each top-level Array contains the results
 * for a vertex in the varray Array. Each entry in the Array is the path length
 * to another vertex in the graph in vertex order (the order the vertices were
 * added to the graph. (This should probalby be changed to give a Hash of Hash
 * to allow easier look up.)
 */
VALUE cIGraph_neighborhood_graphs(VALUE self, VALUE from, VALUE order, VALUE mode){

  igraph_t *graph;
  igraph_vs_t vids;
  igraph_vector_t vidv;
  igraph_neimode_t pmode = NUM2INT(mode);
  igraph_vector_ptr_t res;
  int i;

  VALUE result = rb_ary_new();

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_ptr_init(&res,0);

  //Convert an array of vertices to a vector of vertex ids
  cIGraph_vertex_arr_to_id_vec(self,from,&vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&vids,&vidv);

  igraph_neighborhood_graphs(graph,&res,vids,NUM2INT(order),pmode);

  for(i=0; i<igraph_vector_ptr_size(&res); i++){
    rb_ary_push(result,cIGraph_create_derived_graph(self,VECTOR(res)[i]));
  }

  igraph_vector_destroy(&vidv);
  igraph_vector_ptr_destroy(&res);
  igraph_vs_destroy(&vids);

  return result;

}



  
