#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   IGraph::GenerateRandom.grg_game(nodes,radius,torus) -> IGraph
 *
 * Generating geometric random graphs. A geometric random graph is created by 
 * dropping points (=vertices) randomly to the unit square and then 
 * connecting all those pairs which are less than radius apart in Euclidean 
 * norm.
 *
 * nodes: The number of vertices in the graph.
 *
 * radius: The radius within which the vertices will be connected.
 * torus: Logical constant, if true periodic boundary conditions will be used,
 * ie. the vertices are assumed to be on a torus instead of a square. 
 */
VALUE cIGraph_grg_game(VALUE self, VALUE nodes, VALUE radius, VALUE torus){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_grg_game(graph, NUM2INT(nodes), NUM2DBL(radius), 
		  torus == Qtrue ? 1: 0, NULL, NULL);

  return new_graph;

}

/* call-seq:
 *   IGraph::GenerateRandom.barabasi_game(n,m,outpref,directed) -> IGraph
 *
 * Generates a graph based on the Barabási-Albert model.
 *
 * n: The number of vertices in the graph.
 * m: The number of outgoing edges generated for each vertex.
 *
 * outpref: Boolean, if true not only the in- but also the out-degree of a 
 * vertex increases its citation probability. Ie. the citation probability is 
 * determined by the total degree of the vertices.
 *
 * directed: Boolean, whether to generate a directed graph.
 */
VALUE cIGraph_barabasi_game(VALUE self, VALUE nodes, VALUE m, VALUE outpref, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_barabasi_game(graph, NUM2INT(nodes), NUM2INT(m), NULL, 
		       outpref == Qtrue ? 1: 0, directed == Qtrue ? 1: 0);

  return new_graph;

}

/* call-seq:
 *   IGraph::GenerateRandom.nonlinear_barabasi_game(n,m,outpref,directed) -> IGraph
 *
 * Generates graph with non-linear preferential attachment.
 *
 * This function is very similar to barabasi_game(), only in this game the 
 * probability that a new vertex attaches to a given old vertex is not 
 * proportional to the degree of the old node, but some power of the degree 
 * of the old node.
 *
 * More precisely the attachment probability is the degree to the power of 
 * power plus zeroappeal.
 *
 * This function might generate graphs with multiple edges if the value of m 
 * is at least two. You can call simplify() to get rid of the multiple 
 * edges. 
 *
 * n: The number of vertices in the generated graph.
 *
 * power: The power of the preferential attachment.
 *
 * m: The number of edges to generate in each time step.
 *
 * outpref: Logical constant, if TRUE then the preferential attachment is 
 * based on the total degree of the nodes instead of the in-degree.
 *
 * zeroappeal: Positive number, the attachment probability for vertices with 
 * degree zero.
 *
 * directed: Logical constant, whether to generate a directed graph. 
 */
VALUE cIGraph_nonlinear_barabasi_game(VALUE self, VALUE nodes, VALUE power, VALUE m, VALUE outpref, VALUE zeroappeal, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_nonlinear_barabasi_game(graph, NUM2INT(nodes), NUM2DBL(power),
				  NUM2INT(m), NULL, 
				  outpref == Qtrue ? 1: 0,
				  NUM2DBL(zeroappeal),
				  directed == Qtrue ? 1: 0);

  return new_graph;

}

/* call-seq:
 *   IGraph::GenerateRandom.erdos_renyi_game(type,n,p_or_m,directed,loops) -> IGraph
 *
 * Generates a random (Erdos-Renyi) graph.
 *
 * type: The type of the random graph, possible values:
 *
 * IGraph::ERDOS_RENYI_GNM - G(n,m) graph, m edges are selected uniformly 
 * randomly in a graph with n vertices.
 *
 * IGraph::ERDOS_RENYI_GNP - G(n,p) graph, every possible edge is included in 
 * the graph with probability p.
 *
 * n: The number of vertices in the graph.
 *
 * p_or_m: This is the p parameter for G(n,p) graphs and the m parameter 
 * for G(n,m) graphs.
 *
 * directed: Logical, whether to generate a directed graph.
 *
 * loops: Logical, whether to generate loops (self) edges.
 */
VALUE cIGraph_erdos_renyi_game(VALUE self, VALUE type, VALUE nodes, VALUE mp, VALUE directed, VALUE loops){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_erdos_renyi_game(graph, NUM2INT(type), NUM2INT(nodes),
			  NUM2DBL(mp), 
			  directed == Qtrue ? 1: 0,
			  loops == Qtrue ? 1: 0);

  return new_graph;

}

/* call-seq:
 *   IGraph::GenerateRandom.watts_strogatz_game(dim,size,nei,p) -> IGraph
 *
 * The Watts-Strogatz small-world model This function generates a graph 
 * according to the Watts-Strogatz model of small-world networks. The graph 
 * is obtained by creating a circular undirected lattice and then rewire the 
 * edges randomly with a constant probability.
 *
 * dim: The dimension of the lattice.
 *
 * size: The size of the lattice along each dimension.
 *
 * nei: The size of the neighborhood for each vertex.
 *
 * p: The rewiring probability. A real number between zero and one (inclusive).
 */
VALUE cIGraph_watts_strogatz_game(VALUE self, VALUE dim, VALUE size, VALUE nei, VALUE p){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_watts_strogatz_game(graph, NUM2INT(dim), NUM2INT(size),
			     NUM2INT(nei), NUM2DBL(p));

  return new_graph;

}

/* call-seq:
 *   IGraph::GenerateRandom.degree_sequence_game(out_deg,in_deg) -> IGraph
 *
 * Generates a random graph with a given degree sequence
 *
 * out_deg: The degree sequence for an undirected graph (if in_seq is of 
 * length zero), or the out-degree sequence of a directed graph (if in_deq is 
 * not of length zero.
 *
 * in_deg: It is either a zero-length Array or the in-degree sequence.
 */
VALUE cIGraph_degree_sequence_game(VALUE self, VALUE out_deg, VALUE in_deg){

  igraph_t *graph;
  VALUE new_graph;
  igraph_vector_t out_degv;
  igraph_vector_t in_degv;
  int i;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_vector_init(&out_degv,0);
  igraph_vector_init(&in_degv,0);

  for(i=0;i<RARRAY(out_deg)->len;i++){
    igraph_vector_push_back(&out_degv,NUM2INT(RARRAY(out_deg)->ptr[i]));
  }
  for(i=0;i<RARRAY(in_deg)->len;i++){
    igraph_vector_push_back(&in_degv,NUM2INT(RARRAY(in_deg)->ptr[i]));
  }  

  igraph_destroy(graph);
  igraph_degree_sequence_game(graph, &out_degv, &in_degv, 0);

  igraph_vector_destroy(&out_degv);
  igraph_vector_destroy(&in_degv);

  return new_graph;

}

/* call-seq:
 *   IGraph::GenerateRandom.growing_random_game(n,m,directed,citation) -> IGraph
 *
 * Generates a growing random graph.
 *
 * This function simulates a growing random graph. In each discrete time 
 * step a new vertex is added and a number of new edges are also added. 
 * These graphs are known to be different from standard (not growing) random 
 * graphs.
 *
 * n: The number of vertices in the graph.
 *
 * m: The number of edges to add in a time step (ie. after adding a vertex).
 *
 * directed: Boolean, whether to generate a directed graph.
 *
 * citation: Boolean, if TRUE, the edges always originate from the most 
 * recently added vertex. 
 */
VALUE cIGraph_growing_random_game(VALUE self, VALUE n, VALUE m, VALUE directed, VALUE citation){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_growing_random_game(graph, NUM2INT(n), NUM2INT(m),
				 directed == Qtrue ? 1: 0,
				 citation == Qtrue ? 1: 0);

  return new_graph;

}

/* call-seq:
 *   IGraph::GenerateRandom.callaway_traits_game(nodes,types,edges_per_step,type_dist,pref_matrix,directed) -> IGraph
 *
 * This function simulates a growing network with vertex types. The different 
 * types of vertices prefer to connect other types of vertices with a given 
 * probability.
 *
 * The simulation goes like this: in each discrete time step a new vertex is 
 * added to the graph. The type of this vertex is generated based on 
 * type_dist. Then two vertices are selected uniformly randomly from the 
 * graph. The probability that they will be connected depends on the types 
 * of these vertices and is taken from pref_matrix. Then another two vertices 
 * are selected and this is repeated edges_per_step times in each time step.
 *
 * nodes: The number of nodes in the graph.
 *
 * types: Number of node types.
 *
 * edges_per_step: The number of edges to be add per time step.
 *
 * type_dist: Array giving the distribution of the vertex types.
 *
 * pref_matrix: IGraphMatrix giving the connection probabilities for the 
 * vertex types.
 *
 * directed: Logical, whether to generate a directed graph. 
 */
VALUE cIGraph_callaway_traits_game(VALUE self, VALUE nodes, VALUE types, VALUE e_per_step, VALUE type_dist, VALUE pref_matrix, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;
  igraph_vector_t type_distv;
  igraph_matrix_t *pref_matrixm;
  int i;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  Data_Get_Struct(pref_matrix, igraph_matrix_t, pref_matrixm);

  igraph_vector_init(&type_distv,0);

  for(i=0;i<RARRAY(type_dist)->len;i++){
    igraph_vector_push_back(&type_distv,NUM2DBL(RARRAY(type_dist)->ptr[i]));
  }
  
  igraph_destroy(graph);
  igraph_callaway_traits_game(graph, NUM2INT(nodes), NUM2INT(types),
			      NUM2INT(e_per_step), &type_distv,
			      pref_matrixm,
			      directed == Qtrue ? 1: 0);

  igraph_vector_destroy(&type_distv);

  return new_graph;
  

}

/* call-seq:
 *   IGraph::GenerateRandom.establishment_game(nodes,types,k,type_dist,pref_matrix,directed) -> IGraph
 *
 * Generates a graph with a simple growing model with vertex types
 *
 * The simulation goes like this: a single vertex is added at each time step. 
 * This new vertex tries to connect to k vertices in the graph. The 
 * probability that such a connection is realized depends on the types of the 
 * vertices involved.
 *
 * nodes: The number of vertices in the graph.
 *
 * types: The number of vertex types.
 *
 * k: The number of connections tried in each time step.
 *
 * type_dist: Array giving the distribution of vertex types.
 *
 * pref_matrix: IGraphMatrix giving the connection probabilities for 
 * different vertex types.
 *
 * directed: Logical, whether to generate a directed graph. 
 */
VALUE cIGraph_establishment_game(VALUE self, VALUE nodes, VALUE types, VALUE k, VALUE type_dist, VALUE pref_matrix, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;
  igraph_vector_t type_distv;
  igraph_matrix_t *pref_matrixm;
  int i;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  Data_Get_Struct(pref_matrix, igraph_matrix_t, pref_matrixm);

  igraph_vector_init(&type_distv,0);

  for(i=0;i<RARRAY(type_dist)->len;i++){
    igraph_vector_push_back(&type_distv,NUM2DBL(RARRAY(type_dist)->ptr[i]));
  }
  
  igraph_destroy(graph);
  igraph_establishment_game(graph, NUM2INT(nodes), NUM2INT(types),
			      NUM2INT(k), &type_distv,
			      pref_matrixm,
			      directed == Qtrue ? 1: 0);

  igraph_vector_destroy(&type_distv);

  return new_graph;
  

}

/* call-seq:
 *   IGraph::GenerateRandom.preference_game(nodes,types,type_dist,pref_matrixdirected,loops) -> IGraph
 *
 * Generates a graph with vertex types and connection preferences
 *
 * This is practically the nongrowing variant of igraph_establishment_game. 
 * A given number of vertices are generated. Every vertex is assigned to a 
 * vertex type according to the given type probabilities. Finally, every 
 * vertex pair is evaluated and an edge is created between them with a 
 * probability depending on the types of the vertices involved.
 *
 * nodes: The number of vertices in the graph.
 *
 * types: The number of vertex types.
 *
 * type_dist: Vector giving the distribution of vertex types.
 *
 * pref_matrix: IGraphMatrix giving the connection probabilities for 
 * different vertex types.
 *
 * directed: Logical, whether to generate a directed graph. If undirected 
 * graphs are requested, only the lower left triangle of the preference 
 * matrix is considered.
 *
 * loops: Logical, whether loop edges are allowed. 
 */
VALUE cIGraph_preference_game(VALUE self, VALUE nodes, VALUE types, VALUE type_dist, VALUE pref_matrix, VALUE directed, VALUE loops){

  igraph_t *graph;
  VALUE new_graph;
  igraph_vector_t type_distv;
  igraph_matrix_t *pref_matrixm;
  int i;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  Data_Get_Struct(pref_matrix, igraph_matrix_t, pref_matrixm);

  igraph_vector_init(&type_distv,0);

  for(i=0;i<RARRAY(type_dist)->len;i++){
    igraph_vector_push_back(&type_distv,NUM2DBL(RARRAY(type_dist)->ptr[i]));
  }
  
  igraph_destroy(graph);
  igraph_preference_game(graph, NUM2INT(nodes), NUM2INT(types),
			    &type_distv,
			    pref_matrixm,
			    NULL,
			    directed == Qtrue ? 1: 0,
			    loops == Qtrue ? 1 : 0);

  igraph_vector_destroy(&type_distv);

  return new_graph;
  

}

/* call-seq:
 *   IGraph::GenerateRandom.asymmetric_preference_game(nodes,types,type_dist_matrix,pref_matrix,loops) -> IGraph
 *
 * Generates a graph with asymmetric vertex types and connection preferences
 *
 * This is the asymmetric variant of preference_game() . A given number of 
 * vertices are generated. Every vertex is assigned to an "incoming" and an 
 * "outgoing" vertex type according to the given joint type probabilities. 
 * Finally, every vertex pair is evaluated and a directed edge is created 
 * between them with a probability depending on the "outgoing" type of the 
 * source vertex and the "incoming" type of the target vertex. 
 *
 * nodes: The number of vertices in the graph.
 *
 * types: The number of vertex types.
 *
 * type_dist_matrix: IGraphMatrix giving the joint distribution of vertex 
 * types.
 *
 * pref_matrix: IGraphMatrix giving the connection probabilities for different 
 * vertex types.
 *
 * loops: Logical, whether loop edges are allowed. 
 */
VALUE cIGraph_asymmetric_preference_game(VALUE self, VALUE nodes, VALUE types, VALUE type_dist_matrix, VALUE pref_matrix, VALUE loops){

  igraph_t *graph;
  VALUE new_graph;
  igraph_matrix_t *type_dist_matrixm;
  igraph_matrix_t *pref_matrixm;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  Data_Get_Struct(pref_matrix, igraph_matrix_t, pref_matrixm);
  Data_Get_Struct(type_dist_matrix, igraph_matrix_t, type_dist_matrixm);

  igraph_destroy(graph);
  igraph_asymmetric_preference_game(graph, NUM2INT(nodes), NUM2INT(types),
				    type_dist_matrixm,
				    pref_matrixm,
				    NULL, NULL,
				    loops == Qtrue ? 1 : 0);

  return new_graph;

}

/* call-seq:
 *   IGraph::GenerateRandom.recent_degree_game(n,power,window,m,outseq,outpref,zero_appeal,directed) -> IGraph
 *
 * Stochastic graph generator based on the number of adjacent edges a node 
 * has gained recently
 *
 * n: The number of vertices in the graph, this is the same as the number of 
 * time steps.
 *
 * power: The exponent, the probability that a node gains a new edge is 
 * proportional to the number of edges it has gained recently (in the last 
 * window time steps) to power.
 *
 * window: Integer constant, the size of the time window to use to count the 
 * number of recent edges.
 *
 * m: Integer constant, the number of edges to add per time step
 *
 * outpref: Logical constant, if true the edges originated by a vertex also 
 * count as recent adjacent edges. It is false in most cases.
 * 
 * zero_appeal: Constant giving the attractiveness of the vertices which 
 * haven't gained any edge recently.

 * directed: Logical constant, whether to generate a directed graph. 
 */
VALUE cIGraph_recent_degree_game(VALUE self, VALUE n, VALUE power, VALUE window, VALUE m, VALUE outpref, VALUE zero_appeal, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_recent_degree_game(graph, NUM2INT(n), NUM2DBL(power),
			    NUM2INT(window), NUM2INT(m),
			    NULL,
			    outpref == Qtrue ? 1: 0,
			    NUM2DBL(zero_appeal),
			    directed == Qtrue ? 1: 0);

  return new_graph;

}

/* call-seq:
 *   IGraph::GenerateRandom.barabasi_aging_game(nodes,m,outpref,pa_exp,aging_exp,aging_bin,zero_deg_appeal,zero_age_appeal,deg_coef,age_coef,directed) -> IGraph
 *
 * Preferential attachment with aging of vertices.
 *
 * In this game, the probability that a node gains a new edge is given by 
 * its (in-)degree (k) and age (l). This probability has a degree dependent 
 * component multiplied by an age dependent component. The degree dependent 
 * part is: deg_coef times k to the power of pa_exp plus zero_deg_appeal; and 
 * the age dependent part is age_coef times l to the power of aging_exp plus 
 * zero_age_appeal.
 *
 * The age is based on the number of vertices in the network and the 
 * aging_bin argument: vertices grew one unit older after each aging_bin 
 * vertices added to the network.
 *
 * nodes: The number of vertices in the graph.
 *
 * m: The number of edges to add in each time step.
 *
 * outpref: Logical constant, whether the edges initiated by a vertex 
 * contribute to the probability to gain a new edge.
 *
 * pa_exp: The exponent of the preferential attachment, a small positive 
 * number usually, the value 1 yields the classic linear preferential 
 * attachment.
 *
 * aging_exp: The exponent of the aging, this is a negative number usually.
 *
 * aging_bin: Integer constant, the number of vertices to add before vertices 
 * in the network grew one unit older.
 *
 * zero_deg_appeal: The degree dependent part of the attractiveness of the 
 * zero degree vertices.
 *
 * zero_age_appeal: The age dependent part of the attractiveness of the 
 * vertices of age zero. This parameter is usually zero.
 *
 * deg_coef: The coefficient for the degree.
 *
 * age_coef: The coefficient for the age.
 *
 * directed: Logical constant, whether to generate a directed graph. 
 */
VALUE cIGraph_barabasi_aging_game(VALUE self, VALUE nodes, VALUE m, VALUE outpref, VALUE pa_exp, VALUE aging_exp, VALUE aging_bin, VALUE zero_deg_appeal, VALUE zero_age_appeal, VALUE deg_coef, VALUE age_coef, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_barabasi_aging_game(graph, NUM2INT(nodes), NUM2INT(m),
			     NULL,
			     outpref == Qtrue ? 1: 0,
			     NUM2DBL(pa_exp), NUM2DBL(aging_exp),
			     NUM2INT(aging_bin),
			     NUM2DBL(zero_deg_appeal), 
			     NUM2DBL(zero_age_appeal),
			     NUM2DBL(deg_coef),
			     NUM2DBL(age_coef),
			     directed == Qtrue ? 1: 0);

  return new_graph;

}

/* call-seq:
 *   IGraph::GenerateRandom.recent_degree_aging_game(nodes,m,outpref,pa_exp,aging_exp,aging_bin,time_window,zero_appeal,directed) -> IGraph
 *
 * Preferential attachment based on the number of edges gained recently, with 
 * aging of vertices
 *
 * This game is very similar to igraph_barabasi_aging_game(), except that 
 * instead of the total number of adjacent edges the number of edges gained 
 * in the last time_window time steps are counted.
 *
 * The degree dependent part of the attractiveness is given by k to the power 
 * of pa_exp plus zero_appeal; the age dependent part is l to the power to 
 * aging_exp. k is the number of edges gained in the last time_window time 
 * steps, l is the age of the vertex. 
 *
 * nodes: The number of vertices in the graph.
 *
 * m: The number of edges to add in each time step.
 *
 * outpref: Logical constant, whether the edges initiated by a vertex 
 * contribute to the probability to gain a new edge.
 *
 * pa_exp: The exponent of the preferential attachment, a small positive 
 * number usually, the value 1 yields the classic linear preferential 
 * attachment.
 *
 * aging_exp: The exponent of the aging, this is a negative number usually.
 *
 * aging_bin: Integer constant, the number of vertices to add before vertices 
 * in the network grew one unit older.
 *
 * zero_appeal: The degree dependent part of the attractiveness of the 
 * zero degree vertices.
 *
 * time_window: The time window to use to count the number of adjacent edges 
 * for the vertices.
 *
 * directed: Logical constant, whether to generate a directed graph. 
 */
VALUE cIGraph_recent_degree_aging_game(VALUE self, VALUE nodes, VALUE m, VALUE outpref, VALUE pa_exp, VALUE aging_exp, VALUE aging_bin, VALUE time_window, VALUE zero_appeal, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_recent_degree_aging_game(graph, NUM2INT(nodes), NUM2INT(m),
				  NULL,
				  outpref == Qtrue ? 1: 0,
				  NUM2DBL(pa_exp), NUM2DBL(aging_exp),
				  NUM2INT(aging_bin),
				  NUM2INT(time_window),
				  NUM2DBL(zero_appeal),
				  directed == Qtrue ? 1: 0);

  return new_graph;

}

/* call-seq:
 *   IGraph::GenerateRandom.cited_type_game(nodes,types,pref,edges_per_step_directed) -> IGraph
 *
 * Function to create a network based on some vertex categories. This function
 * creates a citation network, in each step a single vertex and edges_per_step
 * citating edges are added, nodes with different categories (may) have 
 * different probabilities to get cited, as given by the pref vector.
 *
 * Note that this function might generate networks with multiple edges if 
 * edges_per_step is greater than one. You might want to call 
 * igraph_simplify() on the result to remove multiple edges.
 *
 * nodes: The number of vertices in the network.
 *
 * types: Numeric Array giving the categories of the vertices, so it should 
 * contain nodes non-negative integer numbers. Types are numbered from zero.
 *
 * pref: The attractivity of the different vertex categories in an Array. Its 
 * length should be the maximum element in types plus one (types are numbered
 * from zero).
 *
 * edges_per_step: Integer constant, the number of edges to add in each time 
 * step.
 *
 * directed: Logical constant, whether to create a directed network. 
 */
VALUE cIGraph_cited_type_game(VALUE self, VALUE nodes, VALUE types, VALUE pref, VALUE e_per_s, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;
  igraph_vector_t type_distv;
  igraph_vector_t prefv;
  int i;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_vector_init(&type_distv,0);
  igraph_vector_init(&prefv,0);

  for(i=0;i<RARRAY(types)->len;i++){
    igraph_vector_push_back(&type_distv,NUM2DBL(RARRAY(types)->ptr[i]));
  }
  for(i=0;i<RARRAY(pref)->len;i++){
    igraph_vector_push_back(&prefv,NUM2DBL(RARRAY(pref)->ptr[i]));
  }
    
  igraph_destroy(graph);
  igraph_cited_type_game(graph, NUM2INT(nodes),
			 &type_distv,
			 &prefv,
			 NUM2INT(e_per_s),
			 directed == Qtrue ? 1: 0);

  igraph_vector_destroy(&type_distv);
  igraph_vector_destroy(&prefv);

  return new_graph;
  

}
/* call-seq:
 *   IGraph::GenerateRandom.citing_cited_type_game(nodes,types,pref,edges_per_step_directed) -> IGraph
 *
 * This game is similar to igraph_cited_type_game() but here the category of 
 * the citing vertex is also considered.
 *
 * An evolving citation network is modeled here, a single vertex and its 
 * edges_per_step citation are added in each time step. The odds the a given 
 * vertex is cited by the new vertex depends on the category of both the 
 * citing and the cited vertex and is given in the pref matrix. The categories
 * of the citing vertex correspond to the rows, the categories of the cited 
 * vertex to the columns of this matrix. Ie. the element in row i and column 
 * j gives the probability that a j vertex is cited, if the category of the 
 * citing vertex is i.
 *
 * Note that this function might generate networks with multiple edges if 
 * edges_per_step is greater than one. You might want to call 
 * igraph_simplify() on the result to remove multiple edges. 
 *
 * nodes: The number of vertices in the network.
 *
 * types: A numeric IGraphMatrix of length nodes, containing the categories 
 * of the vertices. The categories are numbered from zero.
 *
 * pref: The preference IGraphMatrix, a square matrix is required, both the 
 * number of rows and columns should be the maximum element in types plus 
 * one (types are numbered from zero). 
 *
 * edges_per_step: Integer constant, the number of edges to add in each time 
 * step.
 *
 * directed: Logical constant, whether to create a directed network.
 */
VALUE cIGraph_citing_cited_type_game(VALUE self, VALUE nodes, VALUE types, VALUE pref, VALUE e_per_s, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;
  igraph_vector_t typev;
  igraph_matrix_t *prefm;
  int i;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);
  Data_Get_Struct(pref, igraph_matrix_t, prefm);

  igraph_vector_init(&typev,0);

  for(i=0;i<RARRAY(types)->len;i++){
    igraph_vector_push_back(&typev,NUM2INT(RARRAY(types)->ptr[i]));
  }

  printf("ok\n");
    
  igraph_destroy(graph);
  igraph_citing_cited_type_game(graph, NUM2INT(nodes),
				&typev,
				prefm,
				NUM2INT(e_per_s),
				directed == Qtrue ? 1: 0);

  printf("death\n");

  igraph_vector_destroy(&typev);

  return new_graph;
  

}
