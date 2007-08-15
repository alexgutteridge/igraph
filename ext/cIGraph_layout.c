#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.layout_random -> IGraphMatrix
 * 
 * Returns a random layout
 */
VALUE cIGraph_layout_random(VALUE self){

  igraph_t *graph;
  igraph_matrix_t *res = malloc(sizeof(igraph_matrix_t));

  Data_Get_Struct(self, igraph_t, graph);
 
  igraph_matrix_init(res,0,0);
  igraph_layout_random(graph,res);

  return Data_Wrap_Struct(cIGraphMatrix, 0, cIGraph_matrix_free, res);

}

VALUE cIGraph_layout_circle(VALUE self){

  igraph_t *graph;
  igraph_matrix_t *res = malloc(sizeof(igraph_matrix_t));

  Data_Get_Struct(self, igraph_t, graph);
 
  igraph_matrix_init(res,0,0);
  igraph_layout_circle(graph,res);

  return Data_Wrap_Struct(cIGraphMatrix, 0, cIGraph_matrix_free, res);

}

VALUE cIGraph_layout_fruchterman_reingold(VALUE self,
					  VALUE niter,
					  VALUE maxdelta,
					  VALUE area,
					  VALUE coolexp,
					  VALUE repulserad,
					  VALUE use_seed){

  igraph_t *graph;
  igraph_matrix_t *res = malloc(sizeof(igraph_matrix_t));

  Data_Get_Struct(self, igraph_t, graph);
 
  igraph_matrix_init(res,0,0);
  igraph_layout_fruchterman_reingold(graph,res,
				     NUM2INT(niter),
				     NUM2DBL(maxdelta),
				     NUM2DBL(area),
				     NUM2DBL(coolexp),
				     NUM2DBL(repulserad),
				     use_seed == Qtrue ? 1: 0);

  return Data_Wrap_Struct(cIGraphMatrix, 0, cIGraph_matrix_free, res);

}
