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

/* call-seq:
 *   graph.layout_circle -> IGraphMatrix
 * 
 * Returns a layout with nodes laid out around a circle.
 */
VALUE cIGraph_layout_circle(VALUE self){

  igraph_t *graph;
  igraph_matrix_t *res = malloc(sizeof(igraph_matrix_t));

  Data_Get_Struct(self, igraph_t, graph);
 
  igraph_matrix_init(res,0,0);
  igraph_layout_circle(graph,res);

  return Data_Wrap_Struct(cIGraphMatrix, 0, cIGraph_matrix_free, res);

}

/* call-seq:
 *   graph.layout_fruchterman_reingold -> IGraphMatrix
 * 
 * Places the vertices on a plane according to the Fruchterman-Reingold 
 * algorithm.
 *
 * This is a force-directed layout, see Fruchterman, T.M.J. and Reingold, 
 * E.M.: Graph Drawing by Force-directed Placement. Software -- Practice and 
 * Experience, 21/11, 1129--1164, 1991.
 */
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

/* call-seq:
 *   graph.layout_kamada_kawai -> IGraphMatrix
 * 
 * Places the vertices on a plane according the Kamada-Kawai algorithm.
 *
 * This is a force directed layout, see Kamada, T. and Kawai, S.: An 
 * Algorithm for Drawing General Undirected Graphs. Information Processing 
 * Letters, 31/1, 7--15, 1989.
 */
VALUE cIGraph_layout_kamada_kawai(VALUE self,
				  VALUE niter,
				  VALUE sigma,
				  VALUE initemp,
				  VALUE coolexp,
				  VALUE kkconst){

  igraph_t *graph;
  igraph_matrix_t *res = malloc(sizeof(igraph_matrix_t));

  Data_Get_Struct(self, igraph_t, graph);
 
  igraph_matrix_init(res,0,0);
  igraph_layout_kamada_kawai(graph,res,
			     NUM2INT(niter),
			     NUM2DBL(sigma),
			     NUM2DBL(initemp),
			     NUM2DBL(coolexp),
			     NUM2DBL(kkconst));

  return Data_Wrap_Struct(cIGraphMatrix, 0, cIGraph_matrix_free, res);

}

/* call-seq:
 *   graph.layout_reingold_tilford -> IGraphMatrix
 * 
 * Reingold-Tilford layout for tree graphs
 *
 * Arranges the nodes in a tree where the given node is used as the root. 
 * The tree is directed downwards and the parents are centered above its 
 * children. For the exact algorithm, see:
 *
 * Reingold, E and Tilford, J: Tidier drawing of trees. IEEE Trans. Softw. 
 * Eng., SE-7(2):223--228, 1981
 *
 * If the given graph is not a tree, a breadth-first search is executed 
 * first to obtain a possible spanning tree.
 */
VALUE cIGraph_layout_reingold_tilford(VALUE self,
				      VALUE root){		

  igraph_t *graph;
  igraph_matrix_t *res = malloc(sizeof(igraph_matrix_t));

  Data_Get_Struct(self, igraph_t, graph);
 
  igraph_matrix_init(res,0,0);
  igraph_layout_reingold_tilford(graph,res,
				 cIGraph_get_vertex_id(self, root));
  
  return Data_Wrap_Struct(cIGraphMatrix, 0, cIGraph_matrix_free, res);

}

/* call-seq:
 *   graph.layout_reingold_tilford_circular -> IGraphMatrix
 * 
 * Reingold-Tilford layout for tree graphs, drawn in a circular style
 *
 * Arranges the nodes in a tree where the given node is used as the root. 
 * The tree is directed downwards and the parents are centered above its 
 * children. For the exact algorithm, see:
 *
 * Reingold, E and Tilford, J: Tidier drawing of trees. IEEE Trans. Softw. 
 * Eng., SE-7(2):223--228, 1981
 *
 * If the given graph is not a tree, a breadth-first search is executed 
 * first to obtain a possible spanning tree.
 */
VALUE cIGraph_layout_reingold_tilford_circular(VALUE self,
					       VALUE root){		

  igraph_t *graph;
  igraph_matrix_t *res = malloc(sizeof(igraph_matrix_t));

  Data_Get_Struct(self, igraph_t, graph);
 
  igraph_matrix_init(res,0,0);
  igraph_layout_reingold_tilford_circular(graph,res,
					  cIGraph_get_vertex_id(self, root));
  
  return Data_Wrap_Struct(cIGraphMatrix, 0, cIGraph_matrix_free, res);

}

/* call-seq:
 *   graph.layout_grid_fruchterman_reingold -> IGraphMatrix
 * 
 * Places the vertices on a plane according to the Fruchterman-Reingold 
 * algorithm.
 *
 * This is a force-directed layout, see Fruchterman, T.M.J. and Reingold, 
 * E.M.: Graph Drawing by Force-directed Placement. Software -- Practice and 
 * Experience, 21/11, 1129--1164, 1991.
 */
VALUE cIGraph_layout_grid_fruchterman_reingold(VALUE self,
					       VALUE niter,
					       VALUE maxdelta,
					       VALUE area,
					       VALUE coolexp,
					       VALUE repulserad,
					       VALUE cellsize,
					       VALUE use_seed){

  igraph_t *graph;
  igraph_matrix_t *res = malloc(sizeof(igraph_matrix_t));

  Data_Get_Struct(self, igraph_t, graph);
 
  igraph_matrix_init(res,0,0);
  igraph_layout_grid_fruchterman_reingold(graph,res,
					  NUM2INT(niter),
					  NUM2DBL(maxdelta),
					  NUM2DBL(area),
					  NUM2DBL(coolexp),
					  NUM2DBL(repulserad),
					  NUM2DBL(cellsize),
					  use_seed == Qtrue ? 1: 0);

  return Data_Wrap_Struct(cIGraphMatrix, 0, cIGraph_matrix_free, res);

}

/* call-seq:
 *   graph.layout_lgl -> IGraphMatrix
 * 
 * Places the vertices on a plane according to the Fruchterman-Reingold 
 * algorithm.
 *
 * This is a force-directed layout, see Fruchterman, T.M.J. and Reingold, 
 * E.M.: Graph Drawing by Force-directed Placement. Software -- Practice and 
 * Experience, 21/11, 1129--1164, 1991.
 */
VALUE cIGraph_layout_lgl(VALUE self,
			 VALUE maxit,
			 VALUE maxdelta,
			 VALUE area,
			 VALUE coolexp,
			 VALUE repulserad,
			 VALUE cellsize,
			 VALUE proot){

  igraph_t *graph;
  igraph_matrix_t *res = malloc(sizeof(igraph_matrix_t));

  Data_Get_Struct(self, igraph_t, graph);
 
  igraph_matrix_init(res,0,0);
  igraph_layout_lgl(graph,res,
		    NUM2INT(maxit),
		    NUM2DBL(maxdelta),
		    NUM2DBL(area),
		    NUM2DBL(coolexp),
		    NUM2DBL(repulserad),
		    NUM2DBL(cellsize),
		    cIGraph_get_vertex_id(self, proot));

  return Data_Wrap_Struct(cIGraphMatrix, 0, cIGraph_matrix_free, res);

}

