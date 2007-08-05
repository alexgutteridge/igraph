#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

igraph_integer_t cIGraph_get_vertex_id(VALUE graph, VALUE v){

  VALUE v_ary;
  VALUE idx;
  igraph_t *igraph;

  Data_Get_Struct(graph, igraph_t, igraph);
  v_ary = ((VALUE*)igraph->attr)[0];

  idx   = rb_funcall(v_ary,rb_intern("index"),1,v);

  if(idx)
    return NUM2INT(idx);

  return -1;

}

VALUE cIGraph_get_vertex_object(VALUE graph, igraph_integer_t n){

  VALUE v_ary;
  VALUE obj;
  igraph_t *igraph;

  Data_Get_Struct(graph, igraph_t, igraph);
  v_ary = ((VALUE*)igraph->attr)[0];

  obj = rb_ary_entry(v_ary,n);

  return obj;

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

  VALUE v_ary;
  igraph_t *igraph;

  Data_Get_Struct(self, igraph_t, igraph);
  v_ary = ((VALUE*)igraph->attr)[0];

  return rb_ary_includes(v_ary,v);
}
