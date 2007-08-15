#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.subcomponent(v,mode) -> Array
 *
 * Returns an Array of vertices that are in the same component as the vertex v.
 * mode defines the type of the component for directed graphs, possible 
 * values: IGraph::OUT: the set of vertices reachable from the vertex, 
 * IGraph::IN the set of vertices from which the vertex is reachable, 
 * IGraph::ALL 	the graph is considered as an undirected graph. Note that 
 * this is not the same as the union of the previous two. 
 */
VALUE cIgraph_subcomponent(VALUE self, VALUE v, VALUE mode){

  igraph_t *graph;
  igraph_neimode_t pmode = NUM2INT(mode);
  igraph_vector_t neis;
  int i;
  VALUE component = rb_ary_new();

  igraph_vector_init_int(&neis,0);

  Data_Get_Struct(self, igraph_t, graph);

  igraph_subcomponent(graph, &neis, cIGraph_get_vertex_id(self,from), pmode);

  for(i=0;i<igraph_vector_size(&neis);i++){
    rb_ary_push(component,cIGraph_get_vertex_object(self,VECTOR(neis)[i]));
  }

  igraph_vector_destroy(&neis);

  return component;  

}
