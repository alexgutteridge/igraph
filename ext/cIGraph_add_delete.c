#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.add_edges(edges) -> Fixnum
 *
 * Adds the edges in the edges Array to the graph. Edges are specified as an
 * Array in the same way as with IGraph#new.
 *
 * Returns 0 on success.
 *
 * Example:
 *
 *  g = IGraph.new([1,2,3,4],true)
 *  g.add_edges([1,4,2,3])
 *
 * Adds an extra directed edge to this trivial graph. Note that vertices
 * must have already been added to the graph before they can be used in
 * an edge (throws a IGraphError otherwise).
 */
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

/* call-seq:
 *   graph.add_vertices(vs) -> Fixnum
 *
 * Adds the vertices in the vs Array to the graph.
 *
 * Returns 0 on success.
 *
 * Example:
 *
 *  g = IGraph.new([1,2,3,4],true)
 *  g.add_vertices([5,6,7,8])
 *
 * Adds 4 extra vertices to this graph. IGraph will silently ignore attempts
 * to add a vertex twice to the same graph.
 */
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

/* call-seq:
 *   graph.add_edge(from,to) -> Fixnum
 *
 * Adds an edge between the two vertices given. This is just a convinience
 * method, it is possible to use IGraph#add_edges to add a single edge as well
 *
 * Returns 0 on success.
 *
 * Example:
 *
 *  g = IGraph.new([1,2,3,4],true)
 *  g.add_edge(1,4)
 *
 * Note that vertices must have already been added to the graph before 
 * they can be used in an edge (throws a IGraphError otherwise).
 */
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

/* call-seq:
 *   graph.add_vertex(v) -> Fixnum
 *
 * Adds the vertex to the graph. This is just a convinience method it is
 * also possible to use IGraph#add_vertices to add a single vertex.
 *
 * Returns 0 on success.
 *
 * Example:
 *
 *  g = IGraph.new([1,2,3,4],true)
 *  g.add_vertex(5)
 *
 * Adds 1 extra vertex to this graph. IGraph will silently ignore attempts
 * to add a vertex twice to the same graph.
 */
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
