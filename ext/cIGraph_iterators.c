#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

VALUE cIGraph_each_vertex(VALUE self){

  igraph_t *graph;
  igraph_vs_t vs;
  igraph_vit_t vit;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vs_all(&vs);
  igraph_vit_create(graph, vs, &vit);

  while(!IGRAPH_VIT_END(vit)) {
    rb_yield(cIGraph_get_vertex_object(self,IGRAPH_VIT_GET(vit)));
    IGRAPH_VIT_NEXT(vit);
  }

  igraph_vit_destroy(&vit);
  igraph_vs_destroy(&vs);  

  return Qnil;

}

VALUE cIGraph_each_edge(VALUE self, VALUE order){

  igraph_t *graph;
  igraph_es_t es;
  igraph_eit_t eit;
  igraph_edgeorder_type_t order_t = NUM2INT(order);

  igraph_integer_t from;
  igraph_integer_t to;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_es_all(&es,order_t);
  igraph_eit_create(graph, es, &eit);

  while(!IGRAPH_EIT_END(eit)) {
    igraph_edge(graph,IGRAPH_EIT_GET(eit),&from,&to);
    rb_yield(rb_ary_new3(2,
			 cIGraph_get_vertex_object(self, from),
			 cIGraph_get_vertex_object(self, to)));
    IGRAPH_EIT_NEXT(eit);
  }

  igraph_eit_destroy(&eit);
  igraph_es_destroy(&es);    

  return Qnil;

}

VALUE cIGraph_each_edge_eid(VALUE self, VALUE order){

  igraph_t *graph;
  igraph_es_t es;
  igraph_eit_t eit;
  igraph_edgeorder_type_t order_t = NUM2INT(order);

  Data_Get_Struct(self, igraph_t, graph);

  igraph_es_all(&es,order_t);
  igraph_eit_create(graph, es, &eit);

  while(!IGRAPH_EIT_END(eit)) {
    rb_yield(INT2NUM(IGRAPH_EIT_GET(eit)));
    IGRAPH_EIT_NEXT(eit);
  }

  igraph_eit_destroy(&eit);
  igraph_es_destroy(&es);    

  return Qnil;

}
