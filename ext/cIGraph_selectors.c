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
  return ((VALUE*)graph->attr)[0];

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

/* call-seq:
 *   graph.noadjacent_vertices(v,mode) -> Array
 *
 * Returns all non-neighboring vertices of a given vertex (v). The mode 
 * argument controls the type of neighboring vertics not to select. Possible 
 * values: IGRAPH_OUT, all vertices will be selected except those to which 
 * there is a directed edge from vid. IGRAPH_IN, all vertices will be 
 * selected except those from which there is a directed edge to vid. 
 * IGRAPH_ALL, all vertices will be selected except those from or to which 
 * there is a directed edge to or from vid.
 */

VALUE cIGraph_nonadj_v(VALUE self, VALUE v, VALUE mode){

  igraph_t *graph;  
  igraph_integer_t pnode;
  VALUE nonadjacent = rb_ary_new();
  igraph_neimode_t pmode = NUM2INT(mode);  
  igraph_vs_t vs;
  igraph_vit_t vit;

  Data_Get_Struct(self, igraph_t, graph);

  pnode = cIGraph_get_vertex_id(self,v);

  igraph_vs_nonadj(&vs,pnode,pmode);
  igraph_vit_create(graph, vs, &vit);

  while(!IGRAPH_VIT_END(vit)) {
    rb_ary_push(nonadjacent,cIGraph_get_vertex_object(self,IGRAPH_VIT_GET(vit)));
    IGRAPH_VIT_NEXT(vit);
  }

  igraph_vit_destroy(&vit);
  igraph_vs_destroy(&vs);  

  return nonadjacent;  

}

/* call-seq:
 *   graph.edges(mode) -> Array
 *
 * Returns an Array of all edge ids in the graph. The mode argument specifies 
 * the order the eids are returned. Possible values: IGRAPH_EDGEORDER_ID, 
 * edge id order. IGRAPH_EDGEORDER_FROM, vertex id order, the id of the 
 * source vertex counts for directed graphs. The order of the adjacent edges 
 * of a given vertex is arbitrary. IGRAPH_EDGEORDER_TO, vertex id order, the 
 * id of the target vertex counts for directed graphs. The order of the 
 * adjacent edges of a given vertex is arbitrary. For undirected graph the 
 * latter two is the same.
 */
VALUE cIGraph_all_e(VALUE self, VALUE mode){

  igraph_t *graph;
  igraph_es_t es;
  igraph_eit_t eit;
  igraph_edgeorder_type_t pmode = NUM2INT(mode);  
  VALUE edge_ids = rb_ary_new();

  Data_Get_Struct(self, igraph_t, graph);

  igraph_es_all(&es,pmode);
  igraph_eit_create(graph, es, &eit);

  while(!IGRAPH_EIT_END(eit)) {
    rb_ary_push(edge_ids,INT2NUM(IGRAPH_EIT_GET(eit)));
    IGRAPH_EIT_NEXT(eit);
  }

  igraph_eit_destroy(&eit);
  igraph_es_destroy(&es);  

  return edge_ids;  
  

}

/* call-seq:
 *   graph.adjacent_edges(v,mode) -> Array
 *
 * Returns an Array of the eids of the adjacent edges of a vertex (v). The 
 * mode argument gives the type of edges to select. Possible values: 
 * IGRAPH_OUT, outgoing edges IGRAPH_IN, incoming edges IGRAPH_ALL, all edges
 */
VALUE cIGraph_adj_e(VALUE self, VALUE v, VALUE mode){

  igraph_t *graph;  
  igraph_es_t es;
  igraph_eit_t eit;

  VALUE adjacent = rb_ary_new();

  Data_Get_Struct(self, igraph_t, graph);

  igraph_es_none(&es);
  igraph_es_adj(&es,cIGraph_get_vertex_id(self,v),NUM2INT(mode));
  igraph_eit_create(graph, es, &eit);

  while(!IGRAPH_EIT_END(eit)) {
    rb_ary_push(adjacent,INT2NUM(IGRAPH_EIT_GET(eit)));
    IGRAPH_EIT_NEXT(eit);
  }

  igraph_eit_destroy(&eit);
  igraph_es_destroy(&es);  

  return adjacent;

}


