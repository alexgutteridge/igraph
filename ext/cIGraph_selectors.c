#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.vertices -> Array
 *
 * Returns an Array containing all the vertices in the graph. Also aliased
 * to IGraph#all_vertices
 *
 * Example:
 *
 *  g = IGraph.new([1,2,3,4],true)
 *  g.vertices #returns [1,2,3,4]
 *
 */
VALUE cIGraph_all_v(VALUE self){
  igraph_t *graph;  

  Data_Get_Struct(self, igraph_t, graph);
  return rb_funcall(rb_iv_get(self,"@object_ids"),rb_intern("keys"),0);

}

/* call-seq:
 *   graph.adjacent_vertices(v,mode) -> Array
 *
 * Returns an Array containing all the vertices in the graph that are 
 * adjacent to vertex v. mode decides the type of the neighborhood for 
 * directed graphs. Possible values: IGRAPH_OUT, all vertices to which 
 * there is a directed edge from vid. IGRAPH_IN, all vertices from which 
 * there is a directed edge from vid. IGRAPH_ALL, all vertices to which 
 * or from which there is a directed edge from/to vid. This parameter is 
 * ignored for undirected graphs.
 *
 * Example:
 *
 *  g = IGraph.new([1,2,3,4],true)
 *  g.adjacent_vertices(1,IGraph::ALL) #returns [2]
 *
 */
VALUE cIGraph_adj_v(VALUE self, VALUE v, VALUE mode){

  igraph_t *graph;  
  igraph_integer_t pnode;
  VALUE adjacent = rb_ary_new();
  igraph_neimode_t pmode = NUM2INT(mode);  
  igraph_vs_t vs;
  igraph_vit_t vit;

  Data_Get_Struct(self, igraph_t, graph);

  pnode = cIGraph_get_vertex_id(self,v);

  igraph_vs_adj(&vs,pnode,pmode);
  igraph_vit_create(graph, vs, &vit);

  while(!IGRAPH_VIT_END(vit)) {
    rb_ary_push(adjacent,cIGraph_get_vertex_object(self,IGRAPH_VIT_GET(vit)));
    IGRAPH_VIT_NEXT(vit);
  }

  igraph_vit_destroy(&vit);
  igraph_vs_destroy(&vs);  

  return adjacent;

}

VALUE cIGraph_nonadj_v(VALUE self, VALUE v, VALUE mode){

  return Qnil;

}

VALUE cIGraph_all_e(VALUE self, VALUE mode){

  return Qnil;

}

VALUE cIGraph_adj_e(VALUE self, VALUE v, VALUE mode){

  return Qnil;

}

VALUE cIGraph_nonadj_e(VALUE self, VALUE v, VALUE mode){

  return Qnil;

}

