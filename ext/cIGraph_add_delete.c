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
  VALUE edges;
  VALUE attrs;
  VALUE v_ary;
  int vid;
  int code = 0;
  int i;
  igraph_vector_ptr_t edge_attr;

  rb_scan_args(argc, argv, "11", &edges, &attrs);

  //Initialize edge vector
  igraph_vector_init_int(&edge_v,0);
  igraph_vector_ptr_init(&edge_attr,0);

  Data_Get_Struct(self, igraph_t, graph);

  v_ary = ((VALUE*)graph->attr)[0];

  //Loop through objects in edge Array
  for (i=0; i<RARRAY(edges)->len; i++) {
    vertex = RARRAY(edges)->ptr[i];
    if(rb_ary_includes(v_ary,vertex)){
      vid = cIGraph_get_vertex_id(self, vertex);
    } else {
      rb_raise(cIGraphError, "Unknown vertex in edge array. Use add_vertices first");
    }
    igraph_vector_push_back(&edge_v,vid);
    if (attrs != Qnil && i % 2){
      igraph_vector_ptr_push_back(&edge_attr,(void*)RARRAY(attrs)->ptr[i/2]);
    }
  }

  if(igraph_vector_size(&edge_v) > 0){
    code = igraph_add_edges(graph,&edge_v,&edge_attr);
  }

  igraph_vector_destroy(&edge_v);
  igraph_vector_ptr_destroy(&edge_attr);

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
  VALUE v_ary;
  int code = 0;
  int to_add;
  int i;
  igraph_vector_ptr_t vertex_attr;

  igraph_vector_ptr_init(&vertex_attr,0);

  Data_Get_Struct(self, igraph_t, graph);
  v_ary = ((VALUE*)graph->attr)[0];

  to_add = RARRAY(vs)->len;

  //Loop through objects in vertex array
  for (i=0; i<RARRAY(vs)->len; i++) {
    vertex = RARRAY(vs)->ptr[i];
    if(rb_ary_includes(v_ary,vertex)){
      //Silently ignore duplicated additions
      //rb_raise(cIGraphError, "Vertex already added to graph");
      to_add--;
    } else {
      igraph_vector_ptr_push_back(&vertex_attr,(void*)RARRAY(vs)->ptr[i]);
    }
  }

  code = igraph_add_vertices(graph,to_add,&vertex_attr);

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
  igraph_vector_ptr_t edge_attr;

  int code = 0;

  VALUE v_ary;
  VALUE from;
  VALUE to;
  VALUE attr;

  rb_scan_args(argc, argv, "21", &from, &to, &attr);

  //Initialize edge vector
  igraph_vector_init_int(&edge_v,0);
  igraph_vector_ptr_init(&edge_attr,0);

  Data_Get_Struct(self, igraph_t, graph);

  v_ary = ((VALUE*)graph->attr)[0];

  if(rb_ary_includes(v_ary,from) && rb_ary_includes(v_ary,to)){
    //If graph includes this vertex then look up the vertex number
    igraph_vector_push_back(&edge_v,cIGraph_get_vertex_id(self, from));
    igraph_vector_push_back(&edge_v,cIGraph_get_vertex_id(self, to));
    if (attr != Qnil){
      igraph_vector_ptr_push_back(&edge_attr,(void*)attr);
    }
  } else {
    rb_raise(cIGraphError, "Unknown vertex in edge array. Use add_vertices first");
  }

  igraph_vector_push_back(&edge_v,cIGraph_get_vertex_id(self, from));
  igraph_vector_push_back(&edge_v,cIGraph_get_vertex_id(self, to));

  code = igraph_add_edges(graph,&edge_v,&edge_attr);

  igraph_vector_ptr_destroy(&edge_attr);
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
  igraph_vector_ptr_t vertex_attr;

  int code = 0;

  VALUE v_ary;

  igraph_vector_ptr_init(&vertex_attr,0);

  Data_Get_Struct(self, igraph_t, graph);

  v_ary = ((VALUE*)graph->attr)[0];
  
  //Loop through objects in vertex array
  if(rb_ary_includes(v_ary,v)){
    //rb_raise(cIGraphError, "Vertex already added to graph");
    return code;
  } else {
    igraph_vector_ptr_push_back(&vertex_attr,(void*)v);
  }

  code = igraph_add_vertices(graph,1,&vertex_attr);

  return INT2NUM(code);

}

/* call-seq:
 *   graph.delete_edge(from,to)
 *
 * Deletes the edge connecting the two vertices given.
 */
VALUE cIGraph_delete_edge(VALUE self, VALUE from, VALUE to){

  igraph_t *graph;
  igraph_integer_t eid = 0;
  int from_i;
  int to_i;

  Data_Get_Struct(self, igraph_t, graph);

  from_i = cIGraph_get_vertex_id(self,from);
  to_i   = cIGraph_get_vertex_id(self,to);

  igraph_get_eid(graph,&eid,from_i,to_i,1);

  igraph_delete_edges(graph,igraph_ess_1(eid));

  return Qnil;

}
