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
VALUE cIGraph_add_edges(int argc, VALUE *argv, VALUE self){

  igraph_t *graph;
  igraph_vector_t edge_v;
  VALUE vertex;
  VALUE object_h;
  VALUE edges;
  VALUE attrs;
  int vid;
  int code = 0;
  int i;

  rb_scan_args(argc, argv, "11", &edges, &attrs);

  //Initialize edge vector
  igraph_vector_init_int(&edge_v,0);
  object_h = rb_iv_get(self,"@object_ids");

  Data_Get_Struct(self, igraph_t, graph);

  //Loop through objects in edge Array
  for (i=0; i<RARRAY(edges)->len; i++) {
    vertex = RARRAY(edges)->ptr[i];
    if(rb_funcall(object_h,rb_intern("has_key?"),1,vertex)){
      //If @vertices includes this vertex then look up the vertex number
      vid = NUM2INT(rb_hash_aref(object_h,vertex));
    } else {
      rb_raise(cIGraphError, "Unknown vertex in edge array. Use add_vertices first");
    }
    igraph_vector_push_back(&edge_v,vid);
  }

  if(igraph_vector_size(&edge_v) > 0){
    code = igraph_add_edges(graph,&edge_v,0);
  }

  if(attrs != Qnil){
    for (i=0; i<RARRAY(attrs)->len; i++) {
      cIGraph_set_edge_attr(self,
			    RARRAY(edges)->ptr[i*2],
			    RARRAY(edges)->ptr[(i*2)+1],
			    RARRAY(attrs)->ptr[i]);
    }
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
  int i;

  object_h = rb_iv_get(self,"@object_ids");
  id_h     = rb_iv_get(self,"@id_objects");
  length   = NUM2INT(rb_funcall(vs,      rb_intern("length"),0));
  vertex_n = NUM2INT(rb_funcall(object_h,rb_intern("length"),0));

  Data_Get_Struct(self, igraph_t, graph);

  //Loop through objects in vertex array
  for (i=0; i<RARRAY(vs)->len; i++) {
    vertex = RARRAY(vs)->ptr[i];
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
VALUE cIGraph_add_edge(int argc, VALUE *argv, VALUE self){

  igraph_t *graph;
  igraph_vector_t edge_v;
  VALUE object_h;
  int vid;
  int code = 0;
  VALUE from;
  VALUE to;
  VALUE attr;

  rb_scan_args(argc, argv, "21", &from, &to, &attr);

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

  if(attr != Qnil){
    cIGraph_set_edge_attr(self, from, to, attr);
  }

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
