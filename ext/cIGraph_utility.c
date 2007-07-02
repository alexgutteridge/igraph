#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

igraph_integer_t cIGraph_get_vertex_id(VALUE graph, VALUE v){

  VALUE vertex_h;

  vertex_h = rb_iv_get(graph,"@object_ids");

  if(rb_funcall(vertex_h,rb_intern("has_key?"),1,v))
    return NUM2INT(rb_hash_aref(vertex_h,v));

  return -1;

}

VALUE cIGraph_get_vertex_object(VALUE graph, igraph_integer_t n){

  VALUE vertex_h;

  vertex_h = rb_iv_get(graph,"@id_objects");

  if(rb_funcall(vertex_h,rb_intern("has_key?"),1,INT2NUM(n)))
    return rb_hash_aref(vertex_h,INT2NUM(n));

  return Qnil;

}

int cIGraph_vertex_arr_to_id_vec(VALUE graph, VALUE va, igraph_vector_t *nv){

  VALUE vertex;
  VALUE tmp;

  tmp = rb_check_array_type(va);

  if(NIL_P(tmp))
    rb_raise(cIGraphError, "Array expected\n");
    
  //Initialize edge vector
  igraph_vector_init_int(nv,0);
  vertex = rb_ary_shift(va);
  while(vertex != Qnil){
    igraph_vector_push_back(nv,cIGraph_get_vertex_id(graph, vertex));
    vertex = rb_ary_shift(va);
  }

  return 0;

}

VALUE cIGraph_include(VALUE self, VALUE v){
  VALUE vertex_h = rb_iv_get(self,"@object_ids");
  return rb_funcall(vertex_h,rb_intern("has_key?"),1,v);
}
