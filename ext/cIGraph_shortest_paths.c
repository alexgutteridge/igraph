#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

VALUE cIGraph_shortest_paths(VALUE self, VALUE from, VALUE mode){

  igraph_t *graph;
  igraph_vs_t vids;
  igraph_vector_t vidv;
  igraph_neimode_t pmode = NUM2INT(mode);
  igraph_matrix_t res;
  int i;
  int j;
  VALUE row;
  VALUE matrix = rb_ary_new();
  int n_row;
  int n_col;

  Data_Get_Struct(self, igraph_t, graph);

  n_row = NUM2INT(rb_funcall(from,rb_intern("length"),0));
  n_col = igraph_vcount(graph);

  //matrix to hold the results of the calculations
  igraph_matrix_init(&res,n_row,n_col);

  //Convert an array of vertices to a vector of vertex ids
  cIGraph_vertex_arr_to_id_vec(self,from,&vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&vids,&vidv);

  igraph_shortest_paths(graph,&res,vids,pmode);

  for(i=0; i<igraph_matrix_nrow(&res); i++){
    row = rb_ary_new();
    rb_ary_push(matrix,row);
    for(j=0; j<igraph_matrix_ncol(&res); j++){
      rb_ary_push(row,INT2NUM(MATRIX(res,i,j)));
    }
  }

  igraph_vector_destroy(&vidv);
  igraph_matrix_destroy(&res);
  igraph_vs_destroy(&vids);

  return matrix;

}

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

  n_paths = NUM2INT(rb_funcall(to,rb_intern("length"),0));

  //vector to hold the results of the calculations
  igraph_vector_ptr_init(&res,0);
  for(i=0;i<n_paths;i++){
    path_v = malloc(sizeof(igraph_vector_t));
    igraph_vector_init(path_v,0);
    igraph_vector_ptr_push_back(&res,path_v);
  }

  //Convert an array of vertices to a vector of vertex ids
  cIGraph_vertex_arr_to_id_vec(self,to,&to_vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&to_vids,&to_vidv);

  //The id of the vertex from where we are counting
  from_vid = cIGraph_get_vertex_id(self, from);

  igraph_get_shortest_paths(graph,&res,from_vid,to_vids,pmode);

  for(i=0; i<n_paths; i++){
    path = rb_ary_new();
    rb_ary_push(matrix,path);
    for(j=0; j<igraph_vector_size(VECTOR(res)[i]); j++){
      path_v = VECTOR(res)[i];
      rb_ary_push(path,cIGraph_get_vertex_object(self,VECTOR(*path_v)[j]));
    }
  }

  for(i=0;i<n_paths;i++){
    igraph_vector_destroy(VECTOR(res)[i]);
  }

  igraph_vector_destroy(&to_vidv);
  igraph_vector_ptr_destroy(&res);
  igraph_vs_destroy(&to_vids);

  return matrix;

}
