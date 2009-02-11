#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.st_edge_connectivity(source,target) -> Integer
 *
 *  Edge connectivity of a pair of vertices
 *
 * The edge connectivity of two vertices (source and target) in a graph is 
 * the minimum number of edges that have to be deleted from the graph to 
 * eliminate all paths from source to target.
 *
 * This function uses the maximum flow algorithm to calculate the edge 
 * connectivity. 
 *
 */

VALUE cIGraph_st_edge_connectivity(VALUE self, VALUE source, VALUE target){

  igraph_t *graph;
  igraph_integer_t from_i;
  igraph_integer_t to_i;
  igraph_integer_t value;
  
  Data_Get_Struct(self, igraph_t, graph);

  from_i = cIGraph_get_vertex_id(self,source);
  to_i   = cIGraph_get_vertex_id(self,target);

  igraph_st_edge_connectivity(graph,&value,from_i,to_i);
  
  return INT2NUM(value);

}

/* call-seq:
 *   graph.edge_connectivity() -> Integer
 *
 * This is the minimum of the edge connectivity over all pairs of vertices 
 * in the graph.
 *
 */

VALUE cIGraph_edge_connectivity(VALUE self){

  igraph_t *graph;
  igraph_integer_t value;
  
  Data_Get_Struct(self, igraph_t, graph);

  igraph_edge_connectivity(graph,&value,1);
  
  return INT2NUM(value);

}

/* call-seq:
 *   graph.st_vertex_connectivity(source,target,neigh) -> Integer
 *
 * The vertex connectivity of two vertices (source and target) is the minimum 
 * number of vertices that have to be deleted to eliminate all paths from 
 * source to target. Directed paths are considered in directed graphs.
 *
 * A constant giving what to do if the two vertices are connected. Possible 
 * values: IGraph::VCONN_NEI_ERROR, stop with an error message, 
 * IGraph::VCONN_INFINITY, return infinity (ie. 1.0/0.0). 
 * IGraph::VCONN_IGNORE, ignore the fact that the two vertices are connected 
 * and calculated the number of vertices needed to aliminate all paths except 
 * for the trivial (direct) paths between source and vertex.
 *
 */

VALUE cIGraph_st_vertex_connectivity(VALUE self, VALUE source, VALUE target, VALUE neighbours){

  igraph_t *graph;
  igraph_integer_t from_i;
  igraph_integer_t to_i;
  igraph_integer_t value;
  
  Data_Get_Struct(self, igraph_t, graph);

  from_i = cIGraph_get_vertex_id(self,source);
  to_i   = cIGraph_get_vertex_id(self,target);

  igraph_st_vertex_connectivity(graph,&value,from_i,to_i,NUM2INT(neighbours));
  
  return INT2NUM(value);

}

/* call-seq:
 *   graph.vertex_connectivity() -> Integer
 *
 * This is the minimum of the vertex connectivity over all pairs of vertices 
 * in the graph.
 *
 */

VALUE cIGraph_vertex_connectivity(VALUE self){

  igraph_t *graph;
  igraph_integer_t value;
  
  Data_Get_Struct(self, igraph_t, graph);

  igraph_vertex_connectivity(graph,&value,1);
  
  return INT2NUM(value);

}

/* call-seq:
 *   graph.edge_disjoint_paths(source,target) -> Integer
 *
 *  The maximum number of edge-disjoint paths between two vertices.
 *
 * A set of paths between two vertices is called edge-disjoint if they do not 
 * share any edges. The maximum number of edge-disjoint paths are calculated 
 * by this function using maximum flow techniques. Directed paths are 
 * considered in directed graphs. 
 *
 */

VALUE cIGraph_edge_disjoint_paths(VALUE self, VALUE source, VALUE target){

  igraph_t *graph;
  igraph_integer_t from_i;
  igraph_integer_t to_i;
  igraph_integer_t value;
  
  Data_Get_Struct(self, igraph_t, graph);

  from_i = cIGraph_get_vertex_id(self,source);
  to_i   = cIGraph_get_vertex_id(self,target);

  igraph_edge_disjoint_paths(graph,&value,from_i,to_i);
  
  return INT2NUM(value);

}

/* call-seq:
 *   graph.vertex_disjoint_paths(source,target) -> Integer
 *
 *  The maximum number of vertex-disjoint paths between two vertices.
 *
 * A set of paths between two vertices is called vertex-disjoint if they do 
 * not share any vertexs. The maximum number of vertex-disjoint paths are 
 * calculated by this function using maximum flow techniques. Directed paths 
 * are considered in directed graphs. 
 *
 */

VALUE cIGraph_vertex_disjoint_paths(VALUE self, VALUE source, VALUE target){

  igraph_t *graph;
  igraph_integer_t from_i;
  igraph_integer_t to_i;
  igraph_integer_t value;
  
  Data_Get_Struct(self, igraph_t, graph);

  from_i = cIGraph_get_vertex_id(self,source);
  to_i   = cIGraph_get_vertex_id(self,target);

  igraph_vertex_disjoint_paths(graph,&value,from_i,to_i);
  
  return INT2NUM(value);

}

/* call-seq:
 *   graph.adhesion() -> Integer
 *
 *  Graph adhesion, this is (almost) the same as edge connectivity.
 *
 * This quantity is defined by White and Harary in The cohesiveness of 
 * blocks in social networks: node connectivity and conditional density, 
 * (Sociological Methodology 31:305--359, 2001) and basically it is the 
 * edge connectivity of the graph with uniform edge weights. 
 *
 */

VALUE cIGraph_adhesion(VALUE self){

  igraph_t *graph;
  igraph_integer_t value;
  
  Data_Get_Struct(self, igraph_t, graph);

  igraph_adhesion(graph,&value,1);
  
  return INT2NUM(value);

}

/* call-seq:
 *   graph.cohesion() -> Integer
 *
 *  Graph cohesion, this is the same as vertex connectivity.
 *
 * This quantity is defined by White and Harary in The cohesiveness of 
 * blocks in social networks: node connectivity and conditional density, 
 * (Sociological Methodology 31:305--359, 2001) and basically it is the 
 * edge connectivity of the graph with uniform edge weights. 
 *
 */

VALUE cIGraph_cohesion(VALUE self){

  igraph_t *graph;
  igraph_integer_t value;
  
  Data_Get_Struct(self, igraph_t, graph);

  igraph_cohesion(graph,&value,1);
  
  return INT2NUM(value);

}

