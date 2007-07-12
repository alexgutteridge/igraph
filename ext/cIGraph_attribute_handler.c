#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

igraph_attribute_table_t cIGraph_attribute_table;

int cIGraph_add_edges_attr(igraph_t *graph, const igraph_vector_t *edges, 
			   igraph_vector_ptr_t *attr){
  return 0;
}

int cIGraph_get_numeric_edge_attr(const igraph_t *graph, const char *name,
				  igraph_es_t es,
				  igraph_vector_t *value){
  return 0;
}
int cIGraph_get_string_edge_attr(const igraph_t *graph, const char *name,
				 igraph_es_t es,
				 igraph_strvector_t *value){
  return 0;
}

VALUE cIGraph_get_edge_attr(VALUE self, VALUE from, VALUE to){

  VALUE eid;
  VALUE attr_hash;

  eid = cIGraph_get_eid(self, from, to, 1);

  attr_hash = rb_iv_get(self,"@edge_attrs");
  return rb_hash_aref(attr_hash,eid);
  
}

VALUE cIGraph_set_edge_attr(VALUE self, VALUE from, VALUE to, VALUE attr){

  VALUE eid;
  VALUE attr_hash;

  eid = cIGraph_get_eid(self, from, to, 1);

  attr_hash = rb_iv_get(self,"@edge_attrs");
  rb_hash_aset(attr_hash,eid,attr);

  return Qnil;
			      
}
