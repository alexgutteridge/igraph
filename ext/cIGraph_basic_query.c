#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

VALUE cIGraph_vcount(VALUE self){
  igraph_t *graph;

  Data_Get_Struct(self, igraph_t, graph);
  return INT2NUM(igraph_vcount(graph));
}

VALUE cIGraph_ecount(VALUE self){
  igraph_t *graph;

  Data_Get_Struct(self, igraph_t, graph);
  return INT2NUM(igraph_ecount(graph));
}

VALUE cIGraph_edge(VALUE self, VALUE eid){
    igraph_t *graph;
    igraph_integer_t from = 0;
    igraph_integer_t to   = 0;
    VALUE from_r;
    VALUE to_r;

    Data_Get_Struct(self, igraph_t, graph);

    igraph_edge(graph,NUM2INT(eid),&from,&to);

    from_r = cIGraph_get_vertex_object(self,from);
    to_r   = cIGraph_get_vertex_object(self,to);

    return rb_ary_new3(2, from_r, to_r);

}

VALUE cIGraph_get_eid(VALUE self, VALUE from, VALUE to, VALUE directed){
    igraph_t *graph;
    igraph_integer_t eid = 0;
    int from_i;
    int to_i;
    igraph_bool_t directed_b = 0;

    Data_Get_Struct(self, igraph_t, graph);

    from_i = cIGraph_get_vertex_id(self,from);
    to_i   = cIGraph_get_vertex_id(self,to);

    if(directed)
      directed_b = 1;
    
    igraph_get_eid(graph,&eid,from_i,to_i,directed_b);
  
    return INT2NUM(eid);

}

VALUE cIGraph_neighbors(VALUE self, VALUE v, VALUE mode){

  igraph_t *graph;
  igraph_integer_t pnode;
  igraph_neimode_t pmode = NUM2INT(mode);
  igraph_vector_t neis;
  int i;
  VALUE neighbors = rb_ary_new();

  igraph_vector_init_int(&neis,0);

  Data_Get_Struct(self, igraph_t, graph);

  pnode = cIGraph_get_vertex_id(self,v);

  igraph_neighbors(graph,&neis,pnode,pmode);

  for(i=0;i<igraph_vector_size(&neis);i++){
    rb_ary_push(neighbors,cIGraph_get_vertex_object(self,VECTOR(neis)[i]));
  }

  igraph_vector_destroy(&neis);

  return neighbors;

}

VALUE cIGraph_adjacent(VALUE self, VALUE v, VALUE mode){

  igraph_t *graph;
  igraph_integer_t pnode;
  igraph_neimode_t pmode = NUM2INT(mode);
  igraph_vector_t eids;
  int i;
  VALUE eids_r = rb_ary_new();

  igraph_vector_init_int(&eids,0);

  Data_Get_Struct(self, igraph_t, graph);

  pnode = cIGraph_get_vertex_id(self,v);

  igraph_adjacent(graph,&eids,pnode,pmode);

  for(i=0;i<igraph_vector_size(&eids);i++){
    rb_ary_push(eids_r,INT2NUM(VECTOR(eids)[i]));
  }

  igraph_vector_destroy(&eids);

  return eids_r;

}

VALUE cIGraph_is_directed(VALUE self){
  igraph_t *graph;

  Data_Get_Struct(self, igraph_t, graph);

  return igraph_is_directed(graph) ? Qtrue : Qfalse;

}

VALUE cIGraph_degree(VALUE self, VALUE v, VALUE mode, VALUE loops){
  igraph_t *graph;
  igraph_vs_t vids;
  igraph_vector_t vidv;
  igraph_neimode_t pmode = NUM2INT(mode);
  igraph_bool_t loop_mode = loops ? 1 : 0;
  igraph_vector_t res;
  int i;
  VALUE degree_r = rb_ary_new();

  //vector to hold the results of the degree calculations
  igraph_vector_init_int(&res,0);

  Data_Get_Struct(self, igraph_t, graph);

  //Convert an array of vertices to a vector of vertex ids
  cIGraph_vertex_arr_to_id_vec(self,v,&vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&vids,&vidv);

  igraph_degree(graph,&res,vids,pmode,loop_mode);

  for(i=0;i<igraph_vector_size(&res);i++){
    rb_ary_push(degree_r,INT2NUM(VECTOR(res)[i]));
  }

  igraph_vector_destroy(&vidv);
  igraph_vector_destroy(&res);
  igraph_vs_destroy(&vids);

  return degree_r;

}
