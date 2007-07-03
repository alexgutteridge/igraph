#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.each_vertex{|v| } -> nil
 *
 * Iterates through all the vertices in the graph. Also accessible via 
 * IGraph#each which is used by Enumerable to provide other methods.
 *
 * Example:
 *
 *  g = IGraph.new([1,2,3,4],true)
 *  g.each do |v|
 *    puts v
 *  end
 *  g.find_all{|v| v > 2} #returns [3,4]
 *
 */

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
/* call-seq:
 *   graph.each_edge(mode){|v,w| } -> nil
 *
 * Iterates through all the edges in the graph. mode specifies the order the
 * edges are returned: IGRAPH_EDGEORDER_ID, edge id order. 
 * IGRAPH_EDGEORDER_FROM, vertex id order, the id of the source vertex 
 * counts for directed graphs. The order of the adjacent edges of a given 
 * vertex is arbitrary. IGRAPH_EDGEORDER_TO, vertex id order, the id of the 
 * target vertex counts for directed graphs. The order of the adjacent edges 
 * of a given vertex is arbitrary. For undirected graph the latter two are 
 * the same.
 *
 * Example:
 *
 *  g = IGraph.new([1,2,3,4],true)
 *  g.each_edge(IGraph::EDGORDER_ID) do |v,w|
 *    puts "#{v} -> #{w}"
 *  end
 *
 */
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

/* call-seq:
 *   graph.each_edge_eid(mode){|id| } -> nil
 *
 * Iterates through all the edges in the graph. mode specifies the order the
 * edges are returned: IGRAPH_EDGEORDER_ID, edge id order. 
 * IGRAPH_EDGEORDER_FROM, vertex id order, the id of the source vertex 
 * counts for directed graphs. The order of the adjacent edges of a given 
 * vertex is arbitrary. IGRAPH_EDGEORDER_TO, vertex id order, the id of the 
 * target vertex counts for directed graphs. The order of the adjacent edges 
 * of a given vertex is arbitrary. For undirected graph the latter two are 
 * the same.
 *
 * Example:
 *
 *  g = IGraph.new([1,2,3,4],true)
 *  g.each_edge(IGraph::EDGORDER_ID) do |id|
 *    puts id
 *  end
 *
 */
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
