#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

igraph_integer_t cIGraph_get_vertex_id(VALUE graph, VALUE v){

  //VALUE vertex_h;

  //vertex_h = rb_iv_get(graph,"@object_ids");

  VALUE vertex_array = ((VALUE*)graph->attr)[0];

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
  VALUE i;

  tmp = rb_check_array_type(va);

  if(NIL_P(tmp))
    rb_raise(cIGraphError, "Array expected\n");
    
  //Initialize edge vector
  igraph_vector_init_int(nv,0);
  for (i=0; i<RARRAY(va)->len; i++) {
    vertex = RARRAY(va)->ptr[i];
    igraph_vector_push_back(nv,cIGraph_get_vertex_id(graph, vertex));
  }

  return 0;

}

VALUE cIGraph_include(VALUE self, VALUE v){
  VALUE vertex_h = rb_iv_get(self,"@object_ids");
  return rb_funcall(vertex_h,rb_intern("has_key?"),1,v);
}

VALUE cIGraph_create_derived_graph(VALUE old_graph, igraph_t *new_graph){

  VALUE new_graph_obj;

  //Wrap new graph object
  new_graph_obj = Data_Wrap_Struct(cIGraph, 0, cIGraph_free, new_graph);

  //Go through hashes of old graph and copy across
  //are vertex ids the same? If not we're bolloxed
  rb_iv_set(new_graph_obj,"@object_ids",rb_iv_get(old_graph,"@object_ids"));
  rb_iv_set(new_graph_obj,"@id_objects",rb_iv_get(old_graph,"@id_objects"));

  return new_graph_obj;

}
