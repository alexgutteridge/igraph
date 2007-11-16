#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.layout_random -> IGraphMatrix
 * 
 * Returns a random layout in 3D.
 */
VALUE cIGraph_layout_random_3d(VALUE self){

  igraph_t *graph;
  igraph_matrix_t *res = malloc(sizeof(igraph_matrix_t));
  
  Data_Get_Struct(self, igraph_t, graph);
  
  igraph_matrix_init(res,0,0);
  igraph_layout_random_3d(graph,res);

  return Data_Wrap_Struct(cIGraphMatrix, 0, cIGraph_matrix_free, res);

}

/* call-seq:
 *   graph.layout_sphere -> IGraphMatrix
 *
 * Places vertices (more or less) uniformly on a sphere.
 *
 * The algorithm was described in the following paper: Distributing many 
 * points on a sphere by E.B. Saff and A.B.J. Kuijlaars, Mathematical 
 * Intelligencer 19.1 (1997) 5--11.  
 */
VALUE cIGraph_layout_sphere(VALUE self){

  igraph_t *graph;
  igraph_matrix_t *res = malloc(sizeof(igraph_matrix_t));
  
  Data_Get_Struct(self, igraph_t, graph);
  
  igraph_matrix_init(res,0,0);
  igraph_layout_sphere(graph,res);

  return Data_Wrap_Struct(cIGraphMatrix, 0, cIGraph_matrix_free, res);

}

/* call-seq:
 *   graph.layout_fruchterman_reingold_3d(niter,maxdelta,volume,coolexp,repulserad) -> IGraphMatrix
 *
 * This is the 3D version of the force based Fruchterman-Reingold layout.
 *
 * niter: The number of iterations to do.
 *
 * maxdelta: The maximum distance to move a vertex in an iteration.
 *
 * volume: The volume parameter of the algorithm.
 *
 * coolexp: The cooling exponent of the simulated annealing.
 *
 * repulserad: Determines the radius at which vertex-vertex repulsion 
 * cancels out attraction of adjacent vertices.
 */
VALUE cIGraph_layout_fruchterman_reingold_3d(VALUE self,
					     VALUE niter,
					     VALUE maxdelta,
					     VALUE volume,
					     VALUE coolexp,
					     VALUE repulserad){

  igraph_t *graph;
  igraph_matrix_t *res = malloc(sizeof(igraph_matrix_t));

  Data_Get_Struct(self, igraph_t, graph);
 
  igraph_matrix_init(res,0,0);
  igraph_layout_fruchterman_reingold_3d(graph,res,
					NUM2INT(niter),
					NUM2DBL(maxdelta),
					NUM2DBL(volume),
					NUM2DBL(coolexp),
					NUM2DBL(repulserad),
					1);

  return Data_Wrap_Struct(cIGraphMatrix, 0, cIGraph_matrix_free, res);

}

/* call-seq:
 *   graph.layout_kamada_kawai_3d -> IGraphMatrix
 * 
 * Places the vertices on a plane according the Kamada-Kawai algorithm.
 *
 * This is a force directed layout, see Kamada, T. and Kawai, S.: An 
 * Algorithm for Drawing General Undirected Graphs. Information Processing 
 * Letters, 31/1, 7--15, 1989.
 */
VALUE cIGraph_layout_kamada_kawai_3d(VALUE self,
				     VALUE niter,
				     VALUE sigma,
				     VALUE initemp,
				     VALUE coolexp,
				     VALUE kkconst){

  igraph_t *graph;
  igraph_matrix_t *res = malloc(sizeof(igraph_matrix_t));

  Data_Get_Struct(self, igraph_t, graph);
 
  igraph_matrix_init(res,0,0);
  igraph_layout_kamada_kawai_3d(graph,res,
				NUM2INT(niter),
				NUM2DBL(sigma),
				NUM2DBL(initemp),
				NUM2DBL(coolexp),
				NUM2DBL(kkconst));

  return Data_Wrap_Struct(cIGraphMatrix, 0, cIGraph_matrix_free, res);

}
