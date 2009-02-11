#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   IGraph::Generate.adjacency(matrix,mode) -> IGraph
 *
 * Creates a graph object from an adjacency matrix.
 * 
 * matrix should be given as an IGraphMatrix object. mode controls how the
 * matrix is interpreted:
 *
 * IGraph::ADJ_DIRECTED - The graph will be directed and an element 
 * gives the number of edges between two vertex.
 *
 * IGraph::ADJ_UNDIRECTED - This is the same as 
 * IGraph::ADJ_MAX, for convenience.
 *
 * IGraph::ADJ_MAX - Undirected graph will be created and the 
 * number of edges between vertex i and j is max(A(i,j), A(j,i)).
 *
 * IGraph::ADJ_MIN - Undirected graph will be created with 
 * min(A(i,j), A(j,i)) edges between vertex i and j.
 *
 * IGraph::ADJ_PLUS - Undirected graph will be created with 
 * A(i,j)+A(j,i) edges between vertex i and j.
 *
 * IGraph::ADJ_UPPER - Undirected graph will be created, only the 
 * upper right triangle (including the diagonal) is used for the number of 
 * edges.
 *
 * IGraph::ADJ_LOWER - Undirected graph will be created, only the 
 * lower left triangle (including th * e diagonal) is used for creating the 
 * edges. 
 */
VALUE cIGraph_adjacency(VALUE self, VALUE matrix, VALUE mode){

  igraph_t *graph;
  igraph_matrix_t *matrixp;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  Data_Get_Struct(matrix, igraph_matrix_t, matrixp);

  igraph_destroy(graph);
  igraph_adjacency(graph, matrixp, NUM2INT(mode));

  return new_graph;

}

/* call-seq:
 *   IGraph::Generate.star(n,mode,center) -> IGraph
 *
 * Creates a star graph, every vertex connects only to the center.
 *
 * The number of vertices should be given in n. mode gives the type of the 
 * star graph to create. Possible values:
 *
 * IGraph::STAR_OUT - directed star graph, edges point from the center to the 
 * other vertices.
 *
 * IGraph::STAR_IN - directed star graph, edges point to the center from the 
 * other vertices.
 *
 * IGraph::STAR_UNDIRECTED - an undirected star graph is created.
 *
 * The id of the vertex which will be the center of the graph is given by 
 * center  
 */
VALUE cIGraph_star(VALUE self, VALUE n, VALUE mode, VALUE center){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_star(graph, NUM2INT(n), NUM2INT(mode), NUM2INT(center));

  return new_graph;  

}

/* call-seq:
 *   IGraph::Generate.lattice(dim,directed,mutual,circular) -> IGraph
 *
 * Creates most kind of lattices.
 *
 * The dimensions of the lattice should be given as an Array of Fixnums in dim
 *
 * directed: Boolean, whether to create a directed graph. The direction of 
 * the edges is determined by the generation algorithm and is unlikely to 
 * suit you, so this isn't a very useful option.
 *
 * mutual: Boolean, if the graph is directed this gives whether to create 
 * all connections as mutual.
 *
 * circular: Boolean, defines whether the generated lattice is periodic. 
 */
VALUE cIGraph_lattice(VALUE self, VALUE dim, VALUE directed, VALUE mutual, VALUE circular){

  igraph_t *graph;
  VALUE new_graph;
  igraph_vector_t dimvector;
  int i;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_vector_init(&dimvector,0);
  for(i=0; i<RARRAY_LEN(dim); i++){
    igraph_vector_push_back(&dimvector,NUM2INT(RARRAY_PTR(dim)[i])); 
  }

  igraph_destroy(graph);
  igraph_lattice(graph, &dimvector, 0, 
		 directed == Qtrue ? 1 : 0, 
		 mutual   == Qtrue ? 1 : 0, 
		 circular == Qtrue ? 1 : 0);

  igraph_vector_destroy(&dimvector);

  return new_graph;  

}

/* call-seq:
 *   IGraph::Generate.ring(n,directed,mutual,circular) -> IGraph
 *
 * Creates a ring graph, a one dimensional lattice.
 *
 * n: The number of vertices in the ring.
 *
 * directed: Logical, whether to create a directed ring.
 *
 * mutual: Logical, whether to create mutual edges in a directed ring. It is 
 * ignored for undirected graphs.
 *
 * circular: Logical, if false, the ring will be open (this is not a real 
 * ring actually). 
 */
VALUE cIGraph_ring(VALUE self, VALUE n, VALUE directed, VALUE mutual, VALUE circular){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_ring(graph, NUM2INT(n), 
	      directed == Qtrue ? 1 : 0, 
	      mutual   == Qtrue ? 1 : 0, 
	      circular == Qtrue ? 1 : 0);

  return new_graph;  

}

/* call-seq:
 *   IGraph::Generate.tree(n,children,type) -> IGraph
 *
 * Creates a tree in which almost all vertices have the same number of 
 * children.
 *
 * n: Integer, the number of vertices in the graph.
 *
 * children: Integer, the number of children of a vertex in the tree.
 *
 * type: Constant, gives whether to create a directed tree, and if this is 
 * the case, also its orientation. Possible values:
 *
 * IGraph::TREE_OUT - directed tree, the edges point from the parents to 
 * their children,
 *
 * IGraph::TREE_IN - directed tree, the edges point from the children to 
 * their parents.
 *
 * IGraph::TREE_UNDIRECTED - undirected tree. 
 */
VALUE cIGraph_tree(VALUE self, VALUE n, VALUE children, VALUE type){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_tree(graph, NUM2INT(n), NUM2INT(children), NUM2INT(type));

  return new_graph;  

}
/* call-seq:
 *   IGraph::Generate.full(n,directed,loops) -> IGraph
 *
 * Creates a full graph (directed or undirected, with or without loops).
 *
 * In a full graph every possible edge is present, every vertex is connected 
 * to every other vertex.
 *
 * n: Integer, the number of vertices in the graph.
 * 
 * directed: Logical, whether to create a directed graph.
 *
 * loops: Logical, whether to include self-edges (loops). 
 */
VALUE cIGraph_full(VALUE self, VALUE n, VALUE directed, VALUE loops){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_full(graph, NUM2INT(n), 
	      directed == Qtrue ? 1 : 0, 
	      loops    == Qtrue ? 1 : 0);

  return new_graph;  

}
/* call-seq:
 *   IGraph::Generate.atlas(number) -> IGraph
 *
 * Create a small graph from the “Graph Atlas”.
 *
 * The number of the graph is given as the parameter. The graphs are listed:
 *
 * 1. in increasing order of number of nodes
 * 2. for a fixed number of nodes, in increasing order of the number of edges
 * 3. for fixed numbers of nodes and edges, in increasing order of the degree 
 * sequence, for example 111223 < 112222;
 * 4. for fixed degree sequence, in increasing number of automorphisms.
 *
 * The data was converted from the networkx software package, see 
 * http://networkx.lanl.gov.
 *
 * See An Atlas of Graphs by Ronald C. Read and Robin J. Wilson, Oxford 
 * University Press, 1998.
 */
VALUE cIGraph_atlas(VALUE self, VALUE n){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_atlas(graph, NUM2INT(n));

  return new_graph;  

}
/* call-seq:
 *   IGraph::Generate.chordal_ring(nodes,w) -> IGraph
 *
 * Create an extended chordal ring. An extended chordal ring is regular graph,
 * each node has the same degree. It can be obtained from a simple ring by 
 * adding some extra edges specified by a matrix. Let p denote the number of 
 * columns in the W matrix. The extra edges of vertex i are added according 
 * to column (i mod p) in W. The number of extra edges is the number of rows 
 * in W: for each row j an edge i->i+w[ij] is added if i+w[ij] is less than 
 * the number of total nodes.
 *
 * See also Kotsis, G: Interconnection Topologies for Parallel Processing 
 * Systems, PARS Mitteilungen 11, 1-6, 1993.
 *
 * nodes: Integer, the number of vertices in the graph. It must be at least 3.
 *
 * w: The matrix specifying the extra edges. The number of columns should 
 * divide the number of total vertices. 
 */
VALUE cIGraph_extended_chordal_ring(VALUE self, VALUE n, VALUE matrix){

  igraph_t *graph;
  igraph_matrix_t *matrixp;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  Data_Get_Struct(matrix, igraph_matrix_t, matrixp);

  igraph_destroy(graph);
  igraph_extended_chordal_ring(graph, NUM2INT(n), matrixp);

  return new_graph;

}

/* call-seq:
 *   g.connect_neighborhood(order,mode) -> nil
 *
 * Connects every vertex to its neighborhood This function adds new edges to 
 * graph. For each vertex vertices reachable by at most order steps and not 
 * yet connected to the vertex a new edge is created.
 *
 * order: Integer, it gives the distance within which the vertices will be 
 * connected to the source vertex.
 *
 * mode: Constant, it specifies how the neighborhood search is performed for 
 * directed graphs. If IGraph::OUT then vertices reachable from the source 
 * vertex will be connected, IGraph::IN is the opposite. If IGRAPH_ALL then 
 * the directed graph is considered as an undirected one.  
 */
VALUE cIGraph_connect_neighborhood(VALUE self, VALUE order, VALUE mode){

  igraph_t *graph;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_connect_neighborhood(graph, NUM2INT(order), NUM2INT(mode));

  return Qnil;  

}
