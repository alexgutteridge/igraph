#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.topological_sorting(mode) -> Array
 *
 * Calculate a possible topological sorting of the graph. A topological 
 * sorting of a directed acyclic graph is a linear ordering of its nodes 
 * where each node comes before all nodes to which it has edges. Every DAG 
 * has at least one topological sort, and may have many. This function 
 * returns a possible topological sort among them. If the graph is not 
 * acyclic (it has at least one cycle), a partial topological sort is 
 * returned and a warning is issued. mode specifies how to use the direction 
 * of the edges. For IGRAPH_OUT, the sorting order ensures that each node 
 * comes before all nodes to which it has edges, so nodes with no incoming 
 * edges go first. For IGRAPH_IN, it is quite the opposite: each node comes 
 * before all nodes from which it receives edges. Nodes with no outgoing 
 * edges go first. 
 */
VALUE cIGraph_topological_sorting(VALUE self, VALUE mode){

  igraph_t *graph;
  igraph_vector_t res;
  igraph_neimode_t pmode = NUM2INT(mode);
  VALUE result = rb_ary_new();
  int i;

  igraph_vector_init_int(&res,0);  

  Data_Get_Struct(self, igraph_t, graph);

  igraph_topological_sorting(graph, &res, pmode);

  for(i=0;i<igraph_vector_size(&res);i++){
    rb_ary_push(result,cIGraph_get_vertex_object(self,VECTOR(res)[i]));
  }

  igraph_vector_destroy(&res);

  return result;

}
