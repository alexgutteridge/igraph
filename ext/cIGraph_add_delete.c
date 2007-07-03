#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

VALUE cIGraph_add_edges(VALUE self, VALUE edges){

  igraph_t *graph;
  igraph_vector_t edge_v;
  VALUE vertex;
  VALUE object_h;
  int vid;
  int code = 0;

  //Initialize edge vector
  igraph_vector_init_int(&edge_v,0);
  object_h = rb_iv_get(self,"@object_ids");

  Data_Get_Struct(self, igraph_t, graph);

  //Loop through objects in edge Array
  vertex = rb_ary_shift(edges);
  while(vertex != Qnil){
    if(rb_funcall(object_h,rb_intern("has_key?"),1,vertex)){
      //If @vertices includes this vertex then look up the vertex number
      vid = NUM2INT(rb_hash_aref(object_h,vertex));
    } else {
      rb_raise(cIGraphError, "Unknown vertex in edge array. Use add_vertices first");
    }
    igraph_vector_push_back(&edge_v,vid);
    vertex = rb_ary_shift(edges);
  }

  if(igraph_vector_size(&edge_v) > 0){
    code = igraph_add_edges(graph,&edge_v,0);
  }

  igraph_vector_destroy(&edge_v);

  return INT2NUM(code);

}

VALUE cIGraph_add_vertices(VALUE self, VALUE vs){

  igraph_t *graph;
  VALUE vertex;
  VALUE object_h;
  VALUE id_h;
  int vertex_n;
  int code = 0;
  int length;

  object_h = rb_iv_get(self,"@object_ids");
  id_h     = rb_iv_get(self,"@id_objects");
  length   = NUM2INT(rb_funcall(vs,      rb_intern("length"),0));
  vertex_n = NUM2INT(rb_funcall(object_h,rb_intern("length"),0));

  Data_Get_Struct(self, igraph_t, graph);

  //Loop through objects in vertex array
  vertex = rb_ary_shift(vs);
  while(vertex != Qnil){
    if(rb_funcall(object_h,rb_intern("has_key?"),1,vertex)){
      //If @vertices includes this vertex then raise an error
      //Silently ignore
      //rb_raise(cIGraphError, "Vertex already added to graph");
      length--;
    } else {
      //otherwise add a new entry to Hash
      rb_hash_aset(object_h,vertex,INT2NUM(vertex_n));
      rb_hash_aset(id_h,    INT2NUM(vertex_n),vertex);
      vertex_n++;
    }
    vertex = rb_ary_shift(vs);
  }
  code = igraph_add_vertices(graph,length,0);

  return INT2NUM(code);

}

VALUE cIGraph_add_edge(VALUE self, VALUE from, VALUE to){

  igraph_t *graph;
  igraph_vector_t edge_v;
  VALUE object_h;
  int vid;
  int code = 0;

  //Initialize edge vector
  igraph_vector_init_int(&edge_v,0);
  object_h = rb_iv_get(self,"@object_ids");

  Data_Get_Struct(self, igraph_t, graph);

  if(rb_funcall(object_h,rb_intern("has_key?"),1,from)){
    //If @vertices includes this vertex then look up the vertex number
    vid = NUM2INT(rb_hash_aref(object_h,from));
  } else {
    rb_raise(cIGraphError, "Unknown vertex in edge array. Use add_vertices first");
  }
  igraph_vector_push_back(&edge_v,vid);

  if(rb_funcall(object_h,rb_intern("has_key?"),1,to)){
    //If @vertices includes this vertex then look up the vertex number
    vid = NUM2INT(rb_hash_aref(object_h,to));
  } else {
    rb_raise(cIGraphError, "Unknown vertex in edge array. Use add_vertices first");
  }
  igraph_vector_push_back(&edge_v,vid);

  code = igraph_add_edges(graph,&edge_v,0);

  igraph_vector_destroy(&edge_v);

  return INT2NUM(code);

}

VALUE cIGraph_add_vertex(VALUE self, VALUE v){

  igraph_t *graph;
  VALUE vertex;
  VALUE object_h;
  VALUE id_h;
  int vertex_n;
  int code = 0;
  int length;

  object_h = rb_iv_get(self,"@object_ids");
  id_h     = rb_iv_get(self,"@id_objects");
  length   = 1;
  vertex_n = NUM2INT(rb_funcall(object_h,rb_intern("length"),0));

  Data_Get_Struct(self, igraph_t, graph);

  //Loop through objects in vertex array
  if(rb_funcall(object_h,rb_intern("has_key?"),1,v)){
    //If @vertices includes this vertex then raise an error
    //Silently ignore
    //rb_raise(cIGraphError, "Vertex already added to graph");
    length--;
  } else {
    //otherwise add a new entry to Hash
    rb_hash_aset(object_h,v,INT2NUM(vertex_n));
    rb_hash_aset(id_h,    INT2NUM(vertex_n),v);
    vertex_n++;
  }

  if(length != 0)
    code = igraph_add_vertices(graph,length,0);

  return INT2NUM(code);

}
