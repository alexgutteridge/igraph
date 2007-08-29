#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.shortest_paths(varray,mode) -> Array
 *
 * Calculates the length of the shortest paths from each of the vertices in
 * the varray Array to all of the other vertices in the graph. The result
 * is returned as an Array of Array. Each top-level Array contains the results
 * for a vertex in the varray Array. Each entry in the Array is the path length
 * to another vertex in the graph in vertex order (the order the vertices were
 * added to the graph. (This should probalby be changed to give a Hash of Hash
 * to allow easier look up.)
 */
VALUE cIGraph_shortest_paths(VALUE self, VALUE from, VALUE mode){

  igraph_t *graph;
  igraph_vs_t vids;
  igraph_vector_t vidv;
  igraph_neimode_t pmode = NUM2INT(mode);
  igraph_matrix_t res;
  int i;
  int j;
  VALUE row;
  VALUE path_length;
  VALUE matrix = rb_ary_new();
  int n_row;
  int n_col;

  Data_Get_Struct(self, igraph_t, graph);

  n_row = NUM2INT(rb_funcall(from,rb_intern("length"),0));
  n_col = igraph_vcount(graph);

  //matrix to hold the results of the calculations
  igraph_matrix_init(&res,n_row,n_col);

  //Convert an array of vertices to a vector of vertex ids
  igraph_vector_init_int(&vidv,0);
  cIGraph_vertex_arr_to_id_vec(self,from,&vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&vids,&vidv);

  igraph_shortest_paths(graph,&res,vids,pmode);

  for(i=0; i<igraph_matrix_nrow(&res); i++){
    row = rb_ary_new();
    rb_ary_push(matrix,row);
    for(j=0; j<igraph_matrix_ncol(&res); j++){
      path_length = MATRIX(res,i,j) == n_col ? Qnil : INT2NUM(MATRIX(res,i,j));
      rb_ary_push(row,path_length);
    }
  }

  igraph_vector_destroy(&vidv);
  igraph_matrix_destroy(&res);
  igraph_vs_destroy(&vids);

  return matrix;

}

/* call-seq:
 *   graph.get_shortest_paths(from,to_array,mode) -> Array
 *
 * Calculates the paths from the vertex specified as from to each vertex in the
 * to_array Array. Returns an Array of Arrays. Each top level Array represents
 * a path and each entry in each Array is a vertex on the path. mode
 * represents the type of shortest paths to be calculated: IGraph::OUT
 * the outgoing paths are calculated. IGraph::IN the incoming paths are 
 * calculated. IGraph::ALL the directed graph is considered as an undirected 
 * one for the computation. 
 */
VALUE cIGraph_get_shortest_paths(VALUE self, VALUE from, VALUE to, VALUE mode){

  igraph_t *graph;

  igraph_integer_t from_vid;
  igraph_vs_t to_vids;
  igraph_vector_t to_vidv;

  igraph_neimode_t pmode = NUM2INT(mode);

  igraph_vector_ptr_t res;
  igraph_vector_t *path_v;

  int i;
  int j;
  VALUE path;
  VALUE matrix = rb_ary_new();
  int n_paths;

  Data_Get_Struct(self, igraph_t, graph);

  n_paths = RARRAY(to)->len;

  //vector to hold the results of the calculations
  igraph_vector_ptr_init(&res,0);
  for(i=0;i<n_paths;i++){
    path_v = malloc(sizeof(igraph_vector_t));
    igraph_vector_init(path_v,0);
    igraph_vector_ptr_push_back(&res,path_v);
  }

  //Convert an array of vertices to a vector of vertex ids
  igraph_vector_init_int(&to_vidv,0);
  cIGraph_vertex_arr_to_id_vec(self,to,&to_vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&to_vids,&to_vidv);

  //The id of the vertex from where we are counting
  from_vid = cIGraph_get_vertex_id(self, from);

  igraph_get_shortest_paths(graph,&res,from_vid,to_vids,pmode);

  for(i=0; i<n_paths; i++){
    path = rb_ary_new();
    rb_ary_push(matrix,path);
    path_v = VECTOR(res)[i];
    for(j=0; j<igraph_vector_size(VECTOR(res)[i]); j++){
      rb_ary_push(path,cIGraph_get_vertex_object(self,VECTOR(*path_v)[j]));
    }
  }

  for(i=0;i<n_paths;i++){
    igraph_vector_destroy(VECTOR(res)[i]);
    free(VECTOR(res)[i]);
  }

  igraph_vector_destroy(&to_vidv);
  igraph_vector_ptr_destroy(&res);
  igraph_vs_destroy(&to_vids);

  return matrix;

}

/* call-seq:
 *   graph.get_all_shortest_paths(from,mode) -> Array
 *
 * Calculates the paths from the vertex specified as from to each vertex 
 * in the to_array Arrayevery other. Returns an Array of Arrays. Each top 
 * level Array represents a path and each entry in each Array is a vertex on 
 * the path. mode represents the type of shortest paths to be calculated: 
 * IGraph::OUT the outgoing paths are calculated. IGraph::IN the incoming 
 * paths are calculated. IGraph::ALL the directed graph is considered as an 
 * undirected one for the computation. In contrast to 
 * IGraph#get_shortest_paths all possible shortest paths are reported here. 
 */
VALUE cIGraph_get_all_shortest_paths(VALUE self, VALUE from, VALUE to, VALUE mode){

  igraph_t *graph;

  igraph_integer_t from_vid;
  igraph_neimode_t pmode = NUM2INT(mode);

  igraph_vs_t to_vids;
  igraph_vector_t to_vidv;

  igraph_vector_ptr_t res;
  igraph_vector_t *path_v;

  int i;
  int j;
  VALUE path;
  VALUE matrix = rb_ary_new();

  Data_Get_Struct(self, igraph_t, graph);

  //vector to hold the results of the calculations
  IGRAPH_FINALLY(igraph_vector_ptr_destroy,&res);
  IGRAPH_CHECK(igraph_vector_ptr_init(&res,0));

  //The id of the vertex from where we are counting
  from_vid = cIGraph_get_vertex_id(self, from);

  //Convert an array of vertices to a vector of vertex ids
  igraph_vector_init_int(&to_vidv,0);
  cIGraph_vertex_arr_to_id_vec(self,to,&to_vidv);
  //create vertex selector from the vecotr of ids
  IGRAPH_CHECK(igraph_vs_vector(&to_vids,&to_vidv));

  IGRAPH_CHECK(igraph_get_all_shortest_paths(graph,&res,NULL,from_vid,to_vids,pmode));

  for(i=0; i< igraph_vector_ptr_size(&res); i++){
    path = rb_ary_new();
    rb_ary_push(matrix,path);
    for(j=0; j<igraph_vector_size(VECTOR(res)[i]); j++){
      path_v = VECTOR(res)[i];
      rb_ary_push(path,cIGraph_get_vertex_object(self,VECTOR(*path_v)[j]));
    }
  }

  for(i=0;i<igraph_vector_ptr_size(&res);i++){
    igraph_vector_destroy(VECTOR(res)[i]);
    free(VECTOR(res)[i]);
  }

  igraph_vector_ptr_destroy(&res);
  igraph_vector_destroy(&to_vidv);

  IGRAPH_FINALLY_CLEAN(1);

  return matrix;

}

/* call-seq:
 *   graph.average_path_length(directed,uncon) -> Float
 *
 * Calculates the average geodesic length in a graph. directed should be a
 * boolean specifying whether to consider directed paths. unconn is another
 * boolean specifying what to do about unconnected graphs. If TRUE the 
 * average of the geodesics within the components will be returned, 
 * otherwise the number of vertices is used for the length of non-existing 
 * geodesics. (The rationale behind this is that this is always longer than 
 * the longest possible geodesic in a graph.)
 */
VALUE cIGraph_average_path_length(VALUE self, VALUE directed, VALUE unconn){

  igraph_t *graph;
  igraph_bool_t directed_b = 0;
  igraph_bool_t unconn_b   = 0;
  igraph_real_t res;

  if(directed)
    directed_b = 1;
  if(unconn)
    unconn_b = 1;
  
  Data_Get_Struct(self, igraph_t, graph);

  igraph_average_path_length(graph,&res,directed_b,unconn_b);

  return rb_float_new(res);

}

/* call-seq:
 *   graph.diameter(directed,uncon) -> Array
 *
 * Returns the longest path in the graph. directed should be a
 * boolean specifying whether to consider directed paths. unconn is another
 * boolean specifying what to do about unconnected graphs. If TRUE the 
 * average of the geodesics within the components will be returned, 
 * otherwise the number of vertices is used for the length of non-existing 
 * geodesics. (The rationale behind this is that this is always longer than 
 * the longest possible diamter in a graph.)
 */
VALUE cIGraph_diameter(VALUE self, VALUE directed, VALUE unconn){

  igraph_t *graph;
  igraph_bool_t directed_b = 0;
  igraph_bool_t unconn_b   = 0;
  igraph_vector_t res;
  int i;
  VALUE path = rb_ary_new();

  if(directed)
    directed_b = 1;
  if(unconn)
    unconn_b = 1;
  
  Data_Get_Struct(self, igraph_t, graph);

  //vector to hold the results of the calculations
  igraph_vector_init(&res,0);

  igraph_diameter(graph,NULL,NULL,NULL,&res,directed_b,unconn_b);

  for(i=0; i<igraph_vector_size(&res); i++){
    rb_ary_push(path,cIGraph_get_vertex_object(self,VECTOR(res)[i]));
  }

  igraph_vector_destroy(&res);

  return path;

}

/* call-seq:
 *   graph.diameter(directed,uncon) -> Array
 *
 * Returns the shortest cycle in the graph. directed should be a
 * boolean specifying whether to consider directed paths. unconn is another
 * boolean specifying what to do about unconnected graphs. If TRUE the 
 * average of the geodesics within the components will be returned, 
 * otherwise the number of vertices is used for the length of non-existing 
 * geodesics. (The rationale behind this is that this is always longer than 
 * the longest possible diamter in a graph.)
 */
VALUE cIGraph_girth(VALUE self){

  igraph_t *graph;
  igraph_vector_t res;
  igraph_integer_t girth = 0;
  int i;
  VALUE path = rb_ary_new();

  Data_Get_Struct(self, igraph_t, graph);

  //vector to hold the results of the calculations
  IGRAPH_FINALLY(igraph_vector_destroy,&res);
  IGRAPH_CHECK(igraph_vector_init(&res,0));

  IGRAPH_CHECK(igraph_girth(graph,&girth,&res));

  for(i=0; i<igraph_vector_size(&res); i++){
    rb_ary_push(path,cIGraph_get_vertex_object(self,VECTOR(res)[i]));
  }

  igraph_vector_destroy(&res);

  IGRAPH_FINALLY_CLEAN(1);

  return path;

}
