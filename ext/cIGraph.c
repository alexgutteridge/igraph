#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

void cIGraph_free(void *p){
  igraph_destroy(p);
}

VALUE cIGraph_alloc(VALUE klass){

  igraph_t *graph = malloc(sizeof(igraph_t));
  VALUE obj;

  igraph_empty(graph, 0, 1);
  obj = Data_Wrap_Struct(klass, 0, cIGraph_free, graph);

  return obj;
  
}

/* call-seq:
 *   IGraph.new(edges,directed) -> graph
 *
 * Creates a new IGraph graph with the edges specified in the edge Array.
 * The first two elements define the first edge (the order is from,to for 
 * directed graphs). The next two define the second edge and so on. The 
 * Array should contain an even number of elements.
 * The boolean value directed specifies whether a directed or undirected
 * graph is created.
 */

VALUE cIGraph_initialize(VALUE self, VALUE edges, VALUE directed){

  igraph_t *graph;
  igraph_vector_t edge_v;
  VALUE vertex;
  VALUE object_h;
  VALUE id_h;
  int vertex_n = 0;
  int current_vertex_id;

  igraph_set_error_handler(cIGraph_error_handler);
  igraph_set_warning_handler(cIGraph_warning_handler);  

  //New hash for mapping vertex objects to floats used by iGraph
  object_h = rb_iv_set(self,"@object_ids",rb_hash_new());
  id_h     = rb_iv_set(self,"@id_objects",rb_hash_new());

  //Initialize edge vector
  igraph_vector_init_int(&edge_v,0);

  Data_Get_Struct(self, igraph_t, graph);
  if(!directed)
    igraph_to_undirected(graph,IGRAPH_TO_UNDIRECTED_COLLAPSE);

  //Loop through objects in edge Array
  vertex = rb_ary_shift(edges);
  while(vertex != Qnil){
    if(rb_funcall(object_h,rb_intern("has_key?"),1,vertex)){
      //If @vertices includes this vertex then look up the vertex number
      current_vertex_id = NUM2INT(rb_hash_aref(object_h,vertex));
    } else {
      //otherwise add a new entry to Hash
      rb_hash_aset(object_h,vertex,INT2NUM(vertex_n));
      rb_hash_aset(id_h,    INT2NUM(vertex_n),vertex);
      current_vertex_id = vertex_n;
      vertex_n++;
    }
    igraph_vector_push_back(&edge_v,current_vertex_id);
    vertex = rb_ary_shift(edges);
  }

  if(igraph_vector_size(&edge_v) > 0){
    igraph_add_vertices(graph,vertex_n,0);
    igraph_add_edges(graph,&edge_v,0);
  }

  igraph_vector_destroy(&edge_v);

  return self;

}

/* Interface to the iGraph[http://cneurocvs.rmki.kfki.hu/igraph/] library
 * for graph computation
 */

void Init_igraph(){
  cIGraph = rb_define_class("IGraph", rb_cObject);
  cIGraphError = rb_define_class("IGraphError", rb_eRuntimeError);

  rb_define_alloc_func(cIGraph, cIGraph_alloc);
  rb_define_method(cIGraph, "initialize", cIGraph_initialize, 2);

  rb_define_method(cIGraph, "each_vertex",   cIGraph_each_vertex,  0);
  rb_define_method(cIGraph, "each_edge",     cIGraph_each_edge,    1);
  rb_define_method(cIGraph, "each_edge_eid", cIGraph_each_edge_eid,1);  

  rb_define_const(cIGraph, "EDGEORDER_ID",   INT2NUM(1));
  rb_define_const(cIGraph, "EDGEORDER_FROM", INT2NUM(2));
  rb_define_const(cIGraph, "EDGEORDER_TO",   INT2NUM(3));

  rb_define_method(cIGraph, "each",        cIGraph_each_vertex, 0);
  rb_include_module(cIGraph, rb_mEnumerable);

  rb_define_method(cIGraph, "include?", cIGraph_include, 1);

  rb_define_method(cIGraph, "vcount", cIGraph_vcount, 0);
  rb_define_method(cIGraph, "ecount", cIGraph_ecount, 0);

  rb_define_method(cIGraph, "edge",    cIGraph_edge, 1);
  rb_define_method(cIGraph, "get_eid", cIGraph_get_eid, 2);

  rb_define_method(cIGraph, "neighbors",  cIGraph_neighbors,2);
  rb_define_method(cIGraph, "neighbours", cIGraph_neighbors,2);
  rb_define_method(cIGraph, "adjacent",   cIGraph_adjacent,2);

  rb_define_const(cIGraph, "OUT",   INT2NUM(1));
  rb_define_const(cIGraph, "IN",    INT2NUM(2));
  rb_define_const(cIGraph, "ALL",   INT2NUM(3));
  rb_define_const(cIGraph, "TOTAL", INT2NUM(4));
  
  rb_define_method(cIGraph, "is_directed",  cIGraph_is_directed,0);
  rb_define_method(cIGraph, "is_directed?", cIGraph_is_directed,0);  

  rb_define_method(cIGraph, "degree", cIGraph_degree,3);

  rb_define_method(cIGraph, "add_edges",    cIGraph_add_edges,1);
  rb_define_method(cIGraph, "add_vertices", cIGraph_add_vertices,1);

  rb_define_method(cIGraph, "are_connected",  cIGraph_are_connected,2);
  rb_define_method(cIGraph, "are_connected?", cIGraph_are_connected,2);  

  rb_define_method(cIGraph, "shortest_paths",     cIGraph_shortest_paths,2);
  rb_define_method(cIGraph, "get_shortest_paths", cIGraph_get_shortest_paths,3);

}
