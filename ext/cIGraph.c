#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

//Classes
VALUE cIGraph;
VALUE cIGraphError;

void cIGraph_free(void *p){
  igraph_destroy(p);
  free(p);
}

void cIGraph_mark(void *p){
  rb_gc_mark(((VALUE*)((igraph_t*)p)->attr)[0]);
  rb_gc_mark(((VALUE*)((igraph_t*)p)->attr)[1]);
  rb_gc_mark(((VALUE*)((igraph_t*)p)->attr)[2]);
}

VALUE cIGraph_alloc(VALUE klass){

  igraph_t *graph = malloc(sizeof(igraph_t));
  VALUE obj;

  igraph_empty(graph, 0, 1);

  obj = Data_Wrap_Struct(klass, cIGraph_mark, cIGraph_free, graph);

  return obj;
  
}

/* Document-method: initialize_copy
 *
 * Internal method for copying IGraph objects.
 */
VALUE cIGraph_init_copy(VALUE copy, VALUE orig){

  igraph_t *orig_graph;
  igraph_t *copy_graph;

  if (copy == orig)
    return copy;

  if(TYPE(orig) != T_DATA || RDATA(orig)->dfree != (RUBY_DATA_FUNC)cIGraph_free){
    rb_raise(rb_eTypeError, "Wrong argument type.");
  }
  
  Data_Get_Struct(copy, igraph_t, copy_graph); 
  Data_Get_Struct(orig, igraph_t, orig_graph);

  IGRAPH_CHECK(igraph_copy(copy_graph,orig_graph));

  return copy;

}

/* call-seq:
 *   IGraph.new(edges,directed) -> IGraph
 *
 * Creates a new IGraph graph with the edges specified in the edges Array.
 * The first two elements define the first edge (the order is from,to for 
 * directed graphs). The next two define the second edge and so on. The 
 * Array should contain an even number of elements. Graph elements can be
 * any ruby object.
 *
 * The boolean value directed specifies whether a directed or undirected
 * graph is created.
 *
 * Example:
 *
 *   IGraph.new([1,2,3,4],true)
 *
 * Creates a graph with four vertices. Vertex 1 is connected to vertex 2.
 * Vertex 3 is connected to vertex 4.
 */

VALUE cIGraph_initialize(int argc, VALUE *argv, VALUE self){

  igraph_t *graph;
  igraph_vector_t edge_v;
  VALUE vertex;
  VALUE directed;
  VALUE edges;
  VALUE attrs;
  VALUE v_ary;
  int vertex_n = 0;
  int current_vertex_id;
  int i;

  igraph_vector_ptr_t vertex_attr;
  igraph_vector_ptr_t edge_attr;

  igraph_i_attribute_record_t v_attr_rec;
  v_attr_rec.name  = "__RUBY__";
  v_attr_rec.type  = IGRAPH_ATTRIBUTE_PY_OBJECT;
  v_attr_rec.value = (void*)rb_ary_new();

  igraph_i_attribute_record_t e_attr_rec;
  e_attr_rec.name  = "__RUBY__";
  e_attr_rec.type  = IGRAPH_ATTRIBUTE_PY_OBJECT;
  e_attr_rec.value = (void*)rb_ary_new();

  rb_scan_args(argc,argv,"12", &edges, &directed, &attrs);

  //Initialize edge vector
  IGRAPH_FINALLY(igraph_vector_destroy,&edge_v);
  IGRAPH_FINALLY(igraph_vector_ptr_destroy,&vertex_attr);
  IGRAPH_FINALLY(igraph_vector_ptr_destroy,&edge_attr);

  IGRAPH_CHECK(igraph_vector_init_int(&edge_v,0));

  IGRAPH_CHECK(igraph_vector_ptr_init(&vertex_attr,0));
  IGRAPH_CHECK(igraph_vector_ptr_init(&edge_attr,0));

  Data_Get_Struct(self, igraph_t, graph);

  v_ary = rb_ary_new();

  if(!directed)
    IGRAPH_CHECK(igraph_to_undirected(graph,IGRAPH_TO_UNDIRECTED_COLLAPSE));

  //Loop through objects in edge Array
  for (i=0; i<RARRAY(edges)->len; i++) {
    vertex = RARRAY(edges)->ptr[i];
    if(rb_ary_includes(v_ary,vertex)){
      //If @vertices includes this vertex then look up the vertex number
      current_vertex_id = NUM2INT(rb_funcall(v_ary,rb_intern("index"),1,vertex));
    } else {
      //Otherwise add to the list of vertices
      rb_ary_push(v_ary,vertex);
      current_vertex_id = vertex_n;
      vertex_n++;
      
      //Add object to list of vertex attributes
      rb_ary_push((VALUE)v_attr_rec.value,vertex);
      
    }
    IGRAPH_CHECK(igraph_vector_push_back(&edge_v,current_vertex_id));
    if (i % 2){
      if (attrs != Qnil){
	rb_ary_push((VALUE)e_attr_rec.value,RARRAY(attrs)->ptr[i/2]);
      } else {
	rb_ary_push((VALUE)e_attr_rec.value,Qnil);
      }
    }
  }

  IGRAPH_CHECK(igraph_vector_ptr_push_back(&vertex_attr, &v_attr_rec));
  IGRAPH_CHECK(igraph_vector_ptr_push_back(&edge_attr,   &e_attr_rec));

  if(igraph_vector_size(&edge_v) > 0){
    IGRAPH_CHECK(igraph_add_vertices(graph,vertex_n,&vertex_attr));
    IGRAPH_CHECK(igraph_add_edges(graph,&edge_v,&edge_attr));
  }

  igraph_vector_destroy(&edge_v);
  igraph_vector_ptr_destroy(&vertex_attr);
  igraph_vector_ptr_destroy(&edge_attr);

  IGRAPH_FINALLY_CLEAN(3);

  return self;

}

/* Interface to the iGraph[http://cneurocvs.rmki.kfki.hu/igraph/] library
 * for graph and network computation.
 *
 * Basic graph operations are defined on the IGraph class itself, but many
 * other operations are defined in Modules included into IGraph:
 *
 * - Deterministic graph generation: IGraph::Generate
 * - Random graph generation: IGraph::GenerateRandom
 * - Graph randomisation: IGraph::Randomise
 * - Shortest paths: IGraph::ShortestPaths
 * - Vertex neighborhoods: IGraph::Neighborhood
 * - Graph components: IGraph::Components
 * - Closeness centrality calculations: IGraph::Closeness
 * - Spanning tree: IGraph::Spanning
 * - Transitivity calculations: IGraph::Transitivity
 * - Spectral properties: IGraph::Spectral
 * - Coreness: IGraph::KCores
 * - Other operations: IGraph::OtherOperations
 * - Clique calculations: IGraph::Cliques
 * - Independent vertex sets: IGraph::IndependentVertexSets
 * - Graph isomorphism: IGraph::Isomorphism
 * - Motifs: IGraph::Motifs
 * - Sorting: IGraph::Sorting
 * - File read/write: IGraph::FileRead, IGraph::FileWrite
 * - Graph layout algorithms: IGraph::Layout
 * - Minimum cuts: IGraph::MinimumCuts
 * - Connectivity: IGraph::Connectivity
 * - Community: IGraph::Community
 *
 * Some methods return (or require as a paramter) an IGraphMatrix object. This
 * class wraps the igraph C matrix type.
 */

void Init_igraph(){

  //Modules
  VALUE cIGraph_generate;
  VALUE cIGraph_genrandom;
  VALUE cIGraph_connectivity;
  VALUE cIGraph_mincuts;
  VALUE cIGraph_layout;
  VALUE cIGraph_clique;
  VALUE cIGraph_indyver;
  VALUE cIGraph_isomor;
  VALUE cIGraph_motifs;
  VALUE cIGraph_sorting;
  VALUE cIGraph_filewrite;
  VALUE cIGraph_fileread;
  VALUE cIGraph_community;
  VALUE cIGraph_shortestpaths;
  VALUE cIGraph_neighborhoodm;
  VALUE cIGraph_components;
  VALUE cIGraph_closenessm;
  VALUE cIGraph_spanning;
  VALUE cIGraph_transitivitym;
  VALUE cIGraph_spectral;
  VALUE cIGraph_kcore;
  VALUE cIGraph_otherop;
  VALUE cIGraph_randomise;

  igraph_i_set_attribute_table(&cIGraph_attribute_table);
  igraph_set_error_handler(cIGraph_error_handler);
  igraph_set_warning_handler(cIGraph_warning_handler);  

  cIGraph      = rb_define_class("IGraph",      rb_cObject);
  cIGraphError = rb_define_class("IGraphError", rb_eRuntimeError);

  rb_define_alloc_func(cIGraph, cIGraph_alloc);
  rb_define_method(cIGraph, "initialize",      cIGraph_initialize, -1);
  rb_define_method(cIGraph, "initialize_copy", cIGraph_init_copy,   1);

  rb_include_module(cIGraph, rb_mEnumerable);

  
  /* Functions for deterministically generating graphs. */
  cIGraph_generate = rb_define_module_under(cIGraph, "Generate");
  rb_include_module(cIGraph, cIGraph_generate);       

  rb_define_singleton_method(cIGraph_generate, "adjacency", cIGraph_adjacency, 2); /* in cIGraph_generators_deterministic.c */
  rb_define_singleton_method(cIGraph_generate, "star", cIGraph_star, 3); /* in cIGraph_generators_deterministic.c */ 
  rb_define_singleton_method(cIGraph_generate, "lattice", cIGraph_lattice, 4); /* in cIGraph_generators_deterministic.c */ 
  rb_define_singleton_method(cIGraph_generate, "ring", cIGraph_ring, 4); /* in cIGraph_generators_deterministic.c */ 
  rb_define_singleton_method(cIGraph_generate, "tree", cIGraph_tree, 3); /* in cIGraph_generators_deterministic.c */ 
  rb_define_singleton_method(cIGraph_generate, "full", cIGraph_full, 3); /* in cIGraph_generators_deterministic.c */ 
  rb_define_singleton_method(cIGraph_generate, "atlas", cIGraph_atlas, 1); /* in cIGraph_generators_deterministic.c */ 
  rb_define_singleton_method(cIGraph_generate, "extended_chordal_ring", cIGraph_extended_chordal_ring, 2); /* in cIGraph_generators_deterministic.c */ 

  /* Functions for randomly generating graphs. */
  cIGraph_genrandom = rb_define_module_under(cIGraph, "GenerateRandom");
  rb_include_module(cIGraph, cIGraph_genrandom);       

  rb_define_singleton_method(cIGraph_genrandom, "grg_game", cIGraph_grg_game, 3); /* in cIGraph_generators_random.c */
  rb_define_singleton_method(cIGraph_genrandom, "barabasi_game", cIGraph_barabasi_game, 4); /* in cIGraph_generators_random.c */ 
  rb_define_singleton_method(cIGraph_genrandom, "nonlinear_barabasi_game", cIGraph_nonlinear_barabasi_game, 6); /* in cIGraph_generators_random.c */ 
  rb_define_singleton_method(cIGraph_genrandom, "erdos_renyi_game", cIGraph_erdos_renyi_game, 5); /* in cIGraph_generators_random.c */ 
  rb_define_singleton_method(cIGraph_genrandom, "watts_strogatz_game", cIGraph_watts_strogatz_game, 4); /* in cIGraph_generators_random.c */
  rb_define_singleton_method(cIGraph_genrandom, "degree_sequence_game", cIGraph_degree_sequence_game, 2); /* in cIGraph_generators_random.c */
  rb_define_singleton_method(cIGraph_genrandom, "growing_random_game", cIGraph_growing_random_game, 4); /* in cIGraph_generators_random.c */
  rb_define_singleton_method(cIGraph_genrandom, "callaway_traits_game", cIGraph_callaway_traits_game, 6); /* in cIGraph_generators_random.c */
  rb_define_singleton_method(cIGraph_genrandom, "establishment_game", cIGraph_establishment_game, 6); /* in cIGraph_generators_random.c */
  rb_define_singleton_method(cIGraph_genrandom, "preference_game", cIGraph_preference_game, 6); /* in cIGraph_generators_random.c */
  rb_define_singleton_method(cIGraph_genrandom, "asymmetric_preference_game", cIGraph_asymmetric_preference_game, 5); /* in cIGraph_generators_random.c */
  rb_define_singleton_method(cIGraph_genrandom, "recent_degree_game", cIGraph_recent_degree_game, 7); /* in cIGraph_generators_random.c */
  rb_define_singleton_method(cIGraph_genrandom, "barabasi_aging_game", cIGraph_barabasi_aging_game, 11); /* in cIGraph_generators_random.c */
  rb_define_singleton_method(cIGraph_genrandom, "recent_degree_aging_game", cIGraph_recent_degree_aging_game, 9); /* in cIGraph_generators_random.c */
  rb_define_singleton_method(cIGraph_genrandom, "cited_type_game", cIGraph_cited_type_game, 5); /* in cIGraph_generators_random.c */
  rb_define_singleton_method(cIGraph_genrandom, "citing_cited_type_game", cIGraph_citing_cited_type_game, 5); /* in cIGraph_generators_random.c */

  rb_define_method(cIGraph, "[]",            cIGraph_get_edge_attr, 2); /* in cIGraph_attribute_handler.c */
  rb_define_method(cIGraph, "[]=",           cIGraph_set_edge_attr, 3); /* in cIGraph_attribute_handler.c */
  rb_define_alias (cIGraph, "get_edge_attr", "[]");
  rb_define_alias (cIGraph, "set_edge_attr", "[]=");

  rb_define_method(cIGraph, "attributes", cIGraph_graph_attributes, 0); /* in cIGraph_attribute_handler.c */

  rb_define_method(cIGraph, "each_vertex",   cIGraph_each_vertex,  0); /* in cIGraph_iterators.c */
  rb_define_method(cIGraph, "each_edge",     cIGraph_each_edge,    1); /* in cIGraph_iterators.c */
  rb_define_method(cIGraph, "each_edge_eid", cIGraph_each_edge_eid,1); /* in cIGraph_iterators.c */ 
  rb_define_alias (cIGraph, "each", "each_vertex");

  rb_define_method(cIGraph, "vertices",             cIGraph_all_v,    0); /* in cIGraph_selectors.c */
  rb_define_method(cIGraph, "adjacent_vertices",    cIGraph_adj_v,    2); /* in cIGraph_selectors.c */
  rb_define_method(cIGraph, "nonadjacent_vertices", cIGraph_nonadj_v, 2); /* in cIGraph_selectors.c */
  rb_define_alias (cIGraph, "all_vertices", "vertices");

  rb_define_method(cIGraph, "edges",                cIGraph_all_e,    1); /* in cIGraph_selectors.c */
  rb_define_method(cIGraph, "adjacent_edges",       cIGraph_adj_e,    2); /* in cIGraph_selectors.c */
  rb_define_alias (cIGraph, "all_edges", "edges");

  rb_define_method(cIGraph, "vcount",       cIGraph_vcount,      0); /* in cIGraph_basic_query.c */
  rb_define_method(cIGraph, "ecount",       cIGraph_ecount,      0); /* in cIGraph_basic_query.c */
  rb_define_method(cIGraph, "edge",         cIGraph_edge,        1); /* in cIGraph_basic_query.c */
  rb_define_method(cIGraph, "get_eid",      cIGraph_get_eid,     2); /* in cIGraph_basic_query.c */
  rb_define_method(cIGraph, "neighbours",   cIGraph_neighbors,   2); /* in cIGraph_basic_query.c */
  rb_define_method(cIGraph, "adjacent",     cIGraph_adjacent,    2); /* in cIGraph_basic_query.c */
  rb_define_method(cIGraph, "degree",       cIGraph_degree,      3); /* in cIGraph_basic_query.c */
  rb_define_method(cIGraph, "is_directed?", cIGraph_is_directed, 0); /* in cIGraph_basic_query.c */ 
  rb_define_alias (cIGraph, "is_directed",  "is_directed?");
  rb_define_alias (cIGraph, "neighbors",    "neighbours");

  rb_define_method(cIGraph, "add_edges",     cIGraph_add_edges,    -1); /* in cIGraph_add_delete.c */
  rb_define_method(cIGraph, "add_vertices",  cIGraph_add_vertices,  1); /* in cIGraph_add_delete.c */
  rb_define_method(cIGraph, "add_edge",      cIGraph_add_edge,     -1); /* in cIGraph_add_delete.c */
  rb_define_method(cIGraph, "add_vertex",    cIGraph_add_vertex,    1); /* in cIGraph_add_delete.c */
  rb_define_method(cIGraph, "delete_edge",   cIGraph_delete_edge,   2); /* in cIGraph_add_delete.c */
  rb_define_method(cIGraph, "delete_vertex", cIGraph_delete_vertex, 1); /* in cIGraph_add_delete.c */

  rb_define_method(cIGraph, "are_connected",  cIGraph_are_connected,2); /* in cIGraph_basic_properties.c */  
  rb_define_alias (cIGraph, "are_connected?", "are_connected");

  rb_define_method(cIGraph, "to_directed",   cIGraph_to_directed,   1); /* in cIGraph_direction.c */
  rb_define_method(cIGraph, "to_undirected", cIGraph_to_undirected, 1); /* in cIGraph_direction.c */  

  /* These methods randomise a graph by rewiring the edges. */
  cIGraph_randomise = rb_define_module_under(cIGraph, "Randomise");
  rb_include_module(cIGraph, cIGraph_randomise);       

  rb_define_method(cIGraph_randomise, "rewire_edges", cIGraph_rewire_edges, 1); /* in cIGraph_randomisation.c */
  rb_define_method(cIGraph_randomise, "rewire", cIGraph_rewire, 1); /* in cIGraph_randomisation.c */

  /* Functions for calculating the shortest path through a graph */
  cIGraph_shortestpaths = rb_define_module_under(cIGraph, "ShortestPaths");
  rb_include_module(cIGraph, cIGraph_shortestpaths);     

  rb_define_method(cIGraph_shortestpaths, "shortest_paths",         cIGraph_shortest_paths,         2); /* in cIGraph_shortest_paths.c */
  rb_define_method(cIGraph_shortestpaths, "get_shortest_paths",     cIGraph_get_shortest_paths,     3); /* in cIGraph_shortest_paths.c */
  rb_define_method(cIGraph_shortestpaths, "get_all_shortest_paths", cIGraph_get_all_shortest_paths, 3); /* in cIGraph_shortest_paths.c */  
  rb_define_method(cIGraph_shortestpaths, "average_path_length",    cIGraph_average_path_length,    2); /* in cIGraph_shortest_paths.c */  
  rb_define_method(cIGraph_shortestpaths, "diameter",               cIGraph_diameter,               2); /* in cIGraph_shortest_paths.c */
  rb_define_method(cIGraph_shortestpaths, "girth",                  cIGraph_girth,                  0); /* in cIGraph_shortest_paths.c */

  rb_define_method(cIGraph_shortestpaths, "dijkstra_shortest_paths", cIGraph_dijkstra_shortest_paths, 3); /* in cIGraph_dijkstra.c */

  /* Functions for querying the neighborhood of vertices */
  cIGraph_neighborhoodm = rb_define_module_under(cIGraph, "Neighborhood");
  rb_include_module(cIGraph, cIGraph_neighborhoodm);     

  rb_define_method(cIGraph_neighborhoodm, "neighbourhood_size",   cIGraph_neighborhood_size,   3); /* in cIGraph_vertex_neighbourhood.c */
  rb_define_method(cIGraph_neighborhoodm, "neighbourhood",        cIGraph_neighborhood,        3); /* in cIGraph_vertex_neighbourhood.c */
  rb_define_method(cIGraph_neighborhoodm, "neighbourhood_graphs", cIGraph_neighborhood_graphs, 3); /* in cIGraph_vertex_neighbourhood.c */
  rb_define_alias (cIGraph_neighborhoodm, "neighborhood_size", "neighbourhood_size");
  rb_define_alias (cIGraph_neighborhoodm, "neighborhood", "neighbourhood");
  rb_define_alias (cIGraph_neighborhoodm, "neighborhood_graphs", "neighbourhood_graphs");
  rb_define_method(cIGraph_neighborhoodm, "connect_neighborhood", cIGraph_connect_neighborhood, 2); /* in cIGraph_generators_deterministic.c */

  /* Functions for splitting the graph into components */
  cIGraph_components = rb_define_module_under(cIGraph, "Components");
  rb_include_module(cIGraph, cIGraph_components);     

  rb_define_method(cIGraph_components, "subcomponent", cIGraph_subcomponent, 2); /* in cIGraph_components.c */
  rb_define_method(cIGraph_components, "subgraph",     cIGraph_subgraph,     1); /* in cIGraph_components.c */
  rb_define_method(cIGraph_components, "clusters",     cIGraph_clusters,     1); /* in cIGraph_components.c */
  rb_define_method(cIGraph_components, "decompose",    cIGraph_decompose,   -1); /* in cIGraph_components.c */

  /* Graph centrality functions */
  cIGraph_closenessm = rb_define_module_under(cIGraph, "Closeness");
  rb_include_module(cIGraph, cIGraph_closenessm);     

  rb_define_method(cIGraph_closenessm, "closeness",        cIGraph_closeness,        2); /* in cIGraph_centrality.c */
  rb_define_method(cIGraph_closenessm, "betweenness",      cIGraph_betweenness,      2); /* in cIGraph_centrality.c */
  rb_define_method(cIGraph_closenessm, "edge_betweenness", cIGraph_edge_betweenness, 1); /* in cIGraph_centrality.c */
  rb_define_method(cIGraph_closenessm, "pagerank",         cIGraph_pagerank,         5); /* in cIGraph_centrality.c */  
  rb_define_method(cIGraph_closenessm, "constraint",       cIGraph_constraint,      -1); /* in cIGraph_centrality.c */  
  rb_define_method(cIGraph_closenessm, "maxdegree",        cIGraph_maxdegree,        3); /* in cIGraph_centrality.c */    

  /* Minimum spanning tree functions */
  cIGraph_spanning = rb_define_module_under(cIGraph, "Spanning");
  rb_include_module(cIGraph, cIGraph_spanning);     

  rb_define_method(cIGraph_spanning, "minimum_spanning_tree_unweighted", cIGraph_minimum_spanning_tree_unweighted, 0); /* in cIGraph_spanning.c */
  rb_define_method(cIGraph_spanning, "minimum_spanning_tree_prim",       cIGraph_minimum_spanning_tree_prim,       1); /* in cIGraph_spanning.c */
  
  /* Graph transitivity functions */
  cIGraph_transitivitym = rb_define_module_under(cIGraph, "Transitivity");
  rb_include_module(cIGraph, cIGraph_transitivitym);     

  rb_define_method(cIGraph_transitivitym, "transitivity",          cIGraph_transitivity,          0); /* in cIGraph_transitivity.c */
  rb_define_method(cIGraph_transitivitym, "transitivity_local",    cIGraph_transitivity_local,    1); /* in cIGraph_transitivity.c */
  rb_define_method(cIGraph_transitivitym, "transitivity_avglocal", cIGraph_transitivity_avglocal, 0); /* in cIGraph_transitivity.c */

  /* Functions for the Laplacian matrix. */
  cIGraph_spectral = rb_define_module_under(cIGraph, "Spectral");
  rb_include_module(cIGraph, cIGraph_spectral);     

  rb_define_method(cIGraph_spectral, "laplacian", cIGraph_laplacian, 1); /* in cIGraph_spectral.c */

  /* Functions for finding the coreness of a graph */
  cIGraph_kcore = rb_define_module_under(cIGraph, "KCores");
  rb_include_module(cIGraph, cIGraph_kcore);     

  rb_define_method(cIGraph_kcore, "coreness", cIGraph_coreness, 1); /* in cIGraph_kcores.c */

  /* Other general graph operations */
  cIGraph_otherop = rb_define_module_under(cIGraph, "OtherOperations");
  rb_include_module(cIGraph, cIGraph_otherop);   

  rb_define_method(cIGraph_otherop, "density",       cIGraph_density,       1); /* in cIGraph_other_ops.c */
  rb_define_method(cIGraph_otherop, "simplify",      cIGraph_simplify,      2); /* in cIGraph_other_ops.c */
  rb_define_method(cIGraph_otherop, "reciprocity",   cIGraph_reciprocity,   1); /* in cIGraph_other_ops.c */
  rb_define_method(cIGraph_otherop, "bibcoupling",   cIGraph_bibcoupling,   1); /* in cIGraph_other_ops.c */
  rb_define_method(cIGraph_otherop, "cocitation",    cIGraph_cocitation,    1); /* in cIGraph_other_ops.c */
  rb_define_method(cIGraph_otherop, "get_adjacency", cIGraph_get_adjacency, 1); /* in cIGraph_other_ops.c */
  
  /* Clique finding functions */
  cIGraph_clique = rb_define_module_under(cIGraph, "Cliques");
  rb_include_module(cIGraph, cIGraph_clique);   

  rb_define_method(cIGraph_clique, "cliques",         cIGraph_cliques,         2); /* in cIGraph_cliques.c */
  rb_define_method(cIGraph_clique, "largest_cliques", cIGraph_largest_cliques, 0); /* in cIGraph_cliques.c */ 
  rb_define_method(cIGraph_clique, "maximal_cliques", cIGraph_maximal_cliques, 0); /* in cIGraph_cliques.c */ 
  rb_define_method(cIGraph_clique, "clique_number",   cIGraph_clique_number,   0); /* in cIGraph_cliques.c */ 

  /* Independent vertex set finding functions */
  cIGraph_indyver = rb_define_module_under(cIGraph, "IndependentVertexSets");
  rb_include_module(cIGraph, cIGraph_indyver);  

  rb_define_method(cIGraph_indyver, "independent_vertex_sets", cIGraph_independent_vertex_sets, 2); /* in cIGraph_independent_vertex_sets.c */
  rb_define_method(cIGraph_indyver, "largest_independent_vertex_sets", cIGraph_largest_independent_vertex_sets, 0); /* in cIGraph_independent_vertex_sets.c */
  rb_define_method(cIGraph_indyver, "maximal_independent_vertex_sets", cIGraph_maximal_independent_vertex_sets, 0); /* in cIGraph_independent_vertex_sets.c */
  rb_define_method(cIGraph_indyver, "independence_number", cIGraph_independence_number, 0); /* in cIGraph_independent_vertex_sets.c */

  /* Functions for isomorphism and isoclasses */
  cIGraph_isomor = rb_define_module_under(cIGraph, "Isomorphism");
  rb_include_module(cIGraph, cIGraph_isomor);  

  rb_define_method(cIGraph_isomor, "isomorphic",     cIGraph_isomorphic,     1); /* in cIGraph_isomorphism.c */
  rb_define_method(cIGraph_isomor, "isomorphic_vf2", cIGraph_isomorphic_vf2, 1); /* in cIGraph_isomorphism.c */
  rb_define_method(cIGraph_isomor, "isoclass", cIGraph_isoclass, 0); /* in cIGraph_isomorphism.c */
  rb_define_method(cIGraph_isomor, "isoclass_subgraph", cIGraph_isoclass_subgraph, 1); /* in cIGraph_isomorphism.c */
  rb_define_singleton_method(cIGraph_generate, "isoclass_create", cIGraph_isoclass_create, 3); /* in cIGraph_isomorphism.c */

  /* Motif finding functions */
  cIGraph_motifs = rb_define_module_under(cIGraph, "Motifs");
  rb_include_module(cIGraph, cIGraph_motifs);  

  rb_define_method(cIGraph_motifs, "motifs_randesu",          cIGraph_motifs_randesu,          2); /* in cIGraph_motif.c */ 
  rb_define_method(cIGraph_motifs, "motifs_randesu_no",       cIGraph_motifs_randesu_no,       2); /* in cIGraph_motif.c */ 
  rb_define_method(cIGraph_motifs, "motifs_randesu_estimate", cIGraph_motifs_randesu_estimate, 4); /* in cIGraph_motif.c */ 

  /* Graph sorting functions. */
  cIGraph_sorting = rb_define_module_under(cIGraph, "Sorting");
  rb_include_module(cIGraph, cIGraph_sorting);  

  rb_define_method(cIGraph_sorting, "topological_sorting", cIGraph_topological_sorting, 1); /* in cIGraph_topological_sort.c */

  /* Functions for reading graphs from files */
  cIGraph_fileread = rb_define_module_under(cIGraph, "FileRead");
  rb_include_module(cIGraph, cIGraph_fileread);  

  rb_define_singleton_method(cIGraph_fileread, "read_graph_edgelist", cIGraph_read_graph_edgelist, 2); /* in cIGraph_file.c */
  rb_define_singleton_method(cIGraph_fileread, "read_graph_graphml",  cIGraph_read_graph_graphml, 2);  /* in cIGraph_file.c */  
  rb_define_singleton_method(cIGraph_fileread, "read_graph_ncol",     cIGraph_read_graph_ncol, 5);     /* in cIGraph_file.c */ 
  rb_define_singleton_method(cIGraph_fileread, "read_graph_lgl",      cIGraph_read_graph_lgl,  3);     /* in cIGraph_file.c */ 
  rb_define_singleton_method(cIGraph_fileread, "read_graph_dimacs",   cIGraph_read_graph_dimacs, 2);     /* in cIGraph_file.c */ 
  rb_define_singleton_method(cIGraph_fileread, "read_graph_graphdb",  cIGraph_read_graph_graphdb, 2);     /* in cIGraph_file.c */ 
  rb_define_singleton_method(cIGraph_fileread, "read_graph_gml",      cIGraph_read_graph_gml,  1);     /* in cIGraph_file.c */ 
  rb_define_singleton_method(cIGraph_fileread, "read_graph_pajek",    cIGraph_read_graph_pajek, 2);    /* in cIGraph_file.c */
  
  /* Functions for writing graphs to files */
  cIGraph_filewrite = rb_define_module_under(cIGraph, "FileWrite");
  rb_include_module(cIGraph, cIGraph_filewrite);

  rb_define_method(cIGraph_filewrite, "write_graph_edgelist", cIGraph_write_graph_edgelist, 1);  /* in cIGraph_file.c */
  rb_define_method(cIGraph_filewrite, "write_graph_graphml",  cIGraph_write_graph_graphml,   1); /* in cIGraph_file.c */  
  rb_define_method(cIGraph_filewrite, "write_graph_gml",      cIGraph_write_graph_gml,    1); /* in cIGraph_file.c */  
  rb_define_method(cIGraph_filewrite, "write_graph_ncol",     cIGraph_write_graph_ncol,   3);    /* in cIGraph_file.c */    
  rb_define_method(cIGraph_filewrite, "write_graph_lgl",      cIGraph_write_graph_lgl,   4);    /* in cIGraph_file.c */ 
  rb_define_method(cIGraph_filewrite, "write_graph_dimacs",   cIGraph_write_graph_dimacs, 4);    /* in cIGraph_file.c */ 
  rb_define_method(cIGraph_filewrite, "write_graph_pajek",    cIGraph_write_graph_pajek, 1);     /* in cIGraph_file.c */

  /* Graph layout functions */
  cIGraph_layout = rb_define_module_under(cIGraph, "Layout");
  rb_include_module(cIGraph, cIGraph_layout);

  rb_define_method(cIGraph_layout, "layout_random",                    cIGraph_layout_random,                        0); /* in cIGraph_layout.c */
  rb_define_method(cIGraph_layout, "layout_circle",                    cIGraph_layout_circle,                        0); /* in cIGraph_layout.c */
  rb_define_method(cIGraph_layout, "layout_fruchterman_reingold",      cIGraph_layout_fruchterman_reingold,          6); /* in cIGraph_layout.c */
  rb_define_method(cIGraph_layout, "layout_kamada_kawai",              cIGraph_layout_kamada_kawai,                  5); /* in cIGraph_layout.c */
  rb_define_method(cIGraph_layout, "layout_reingold_tilford",          cIGraph_layout_reingold_tilford,              1); /* in cIGraph_layout.c */
  rb_define_method(cIGraph_layout, "layout_reingold_tilford_circular", cIGraph_layout_reingold_tilford_circular, 1); /* in cIGraph_layout.c */
  rb_define_method(cIGraph_layout, "layout_grid_fruchterman_reingold", cIGraph_layout_grid_fruchterman_reingold,     7); /* in cIGraph_layout.c */
  rb_define_method(cIGraph_layout, "layout_lgl",                       cIGraph_layout_lgl,                           7); /* in cIGraph_layout.c */

  rb_define_method(cIGraph_layout, "layout_random_3d",               cIGraph_layout_random_3d,               0); /* in cIGraph_layout3d.c */
  rb_define_method(cIGraph_layout, "layout_sphere",                  cIGraph_layout_sphere,                  0); /* in cIGraph_layout3d.c */
  rb_define_method(cIGraph_layout, "layout_fruchterman_reingold_3d", cIGraph_layout_fruchterman_reingold_3d, 5); /* in cIGraph_layout3d.c */
  rb_define_method(cIGraph_layout, "layout_kamada_kawai_3d",         cIGraph_layout_kamada_kawai_3d,         5); /* in cIGraph_layout3d.c */

  rb_define_singleton_method(cIGraph_layout, "layout_merge_dla", cIGraph_layout_merge_dla, 2); /* in cIGraph_layout.c */

  /* Minimum cuts related functions */
  cIGraph_mincuts = rb_define_module_under(cIGraph, "MinimumCuts");
  rb_include_module(cIGraph, cIGraph_mincuts);

  rb_define_method(cIGraph_mincuts, "maxflow_value",   cIGraph_maxflow_value,   3); /* in cIGraph_min_cuts.c */ 
  rb_define_method(cIGraph_mincuts, "st_mincut_value", cIGraph_st_mincut_value, 3); /* in cIGraph_min_cuts.c */  
  rb_define_method(cIGraph_mincuts, "mincut_value",    cIGraph_mincut_value,    1); /* in cIGraph_min_cuts.c */
  rb_define_method(cIGraph_mincuts, "mincut",          cIGraph_mincut,          1); /* in cIGraph_min_cuts.c */

  /* Vertex and edge connectivity functions */
  cIGraph_connectivity = rb_define_module_under(cIGraph, "Connectivity");
  rb_include_module(cIGraph, cIGraph_connectivity);

  rb_define_method(cIGraph_connectivity, "st_edge_connectivity",   cIGraph_st_edge_connectivity,   2); /* in cIGraph_connectivity.c */   
  rb_define_method(cIGraph_connectivity, "edge_connectivity",      cIGraph_edge_connectivity,      0); /* in cIGraph_connectivity.c */   
  rb_define_method(cIGraph_connectivity, "st_vertex_connectivity", cIGraph_st_vertex_connectivity, 3); /* in cIGraph_connectivity.c */   
  rb_define_method(cIGraph_connectivity, "vertex_connectivity",    cIGraph_vertex_connectivity,    0); /* in cIGraph_connectivity.c */   
  rb_define_method(cIGraph_connectivity, "edge_disjoint_paths",    cIGraph_edge_disjoint_paths,    2); /* in cIGraph_connectivity.c */ 
  rb_define_method(cIGraph_connectivity, "vertex_disjoint_paths",  cIGraph_vertex_disjoint_paths,  2); /* in cIGraph_connectivity.c */ 
  rb_define_method(cIGraph_connectivity, "adhesion",               cIGraph_adhesion,               0); /* in cIGraph_connectivity.c */    
  rb_define_method(cIGraph_connectivity, "cohesion",               cIGraph_cohesion,               0); /* in cIGraph_connectivity.c */   

  /* Community and modularity related functions */
  cIGraph_community = rb_define_module_under(cIGraph, "Community");
  rb_include_module(cIGraph, cIGraph_community);
  
  rb_define_method(cIGraph_community, "modularity",   cIGraph_modularity,   1); /* in cIGraph_community.c */
  rb_define_method(cIGraph_community, "community_to_membership", cIGraph_community_to_membership, 2);  /* in cIGraph_community.c */
  rb_define_method(cIGraph_community, "community_spinglass", cIGraph_community_spinglass, 8);  /* in cIGraph_community.c */
  rb_define_method(cIGraph_community, "community_spinglass_single", cIGraph_community_spinglass_single, 5);  /* in cIGraph_community.c */
  rb_define_method(cIGraph_community, "community_leading_eigenvector", cIGraph_community_leading_eigenvector, 1);  /* in cIGraph_community.c */      
  rb_define_method(cIGraph_community, "community_leading_eigenvector_naive", cIGraph_community_leading_eigenvector_naive, 1);  /* in cIGraph_community.c */
  rb_define_method(cIGraph_community, "community_leading_eigenvector_step", cIGraph_community_leading_eigenvector_step, 2);  /* in cIGraph_community.c */       rb_define_method(cIGraph_community, "community_walktrap", cIGraph_community_walktrap, 2);  /* in cIGraph_community.c */      
  rb_define_method(cIGraph_community, "community_edge_betweenness", cIGraph_community_edge_betweenness, 1);  /* in cIGraph_community.c */  
  rb_define_method(cIGraph_community, "community_eb_get_merges", cIGraph_community_eb_get_merges, 1);  /* in cIGraph_community.c */  
  rb_define_method(cIGraph_community, "community_fastgreedy", cIGraph_community_fastgreedy, 0);  /* in cIGraph_community.c */  

  rb_define_const(cIGraph, "VERSION", rb_str_new2("0.9.0"));

  rb_define_const(cIGraph, "EDGEORDER_ID",   INT2NUM(1));
  rb_define_const(cIGraph, "EDGEORDER_FROM", INT2NUM(2));
  rb_define_const(cIGraph, "EDGEORDER_TO",   INT2NUM(3));

  rb_define_const(cIGraph, "OUT",   INT2NUM(1));
  rb_define_const(cIGraph, "IN",    INT2NUM(2));
  rb_define_const(cIGraph, "ALL",   INT2NUM(3));
  rb_define_const(cIGraph, "TOTAL", INT2NUM(4));

  rb_define_const(cIGraph_components, "WEAK",   INT2NUM(1));
  rb_define_const(cIGraph_components, "STRONG", INT2NUM(2));  

  rb_define_const(cIGraph, "ARBITRARY", INT2NUM(0));
  rb_define_const(cIGraph, "MUTUAL",    INT2NUM(1));
  rb_define_const(cIGraph, "EACH",      INT2NUM(0));
  rb_define_const(cIGraph, "COLLAPSE",  INT2NUM(1));

  rb_define_const(cIGraph_otherop, "GET_ADJACENCY_UPPER", INT2NUM(0));
  rb_define_const(cIGraph_otherop, "GET_ADJACENCY_LOWER", INT2NUM(1));
  rb_define_const(cIGraph_otherop, "GET_ADJACENCY_BOTH",  INT2NUM(2));  

  rb_define_const(cIGraph, "ERDOS_RENYI_GNP", INT2NUM(0));
  rb_define_const(cIGraph, "ERDOS_RENYI_GNM", INT2NUM(1));

  rb_define_const(cIGraph_generate, "ADJ_DIRECTED",   INT2NUM(0));
  rb_define_const(cIGraph_generate, "ADJ_UNDIRECTED", INT2NUM(1));
  rb_define_const(cIGraph_generate, "ADJ_MAX",        INT2NUM(2));
  rb_define_const(cIGraph_generate, "ADJ_MIN",        INT2NUM(3));
  rb_define_const(cIGraph_generate, "ADJ_PLUS",       INT2NUM(4));
  rb_define_const(cIGraph_generate, "ADJ_UPPER",      INT2NUM(5));
  rb_define_const(cIGraph_generate, "ADJ_LOWER",      INT2NUM(6));

  rb_define_const(cIGraph_generate, "STAR_OUT",        INT2NUM(0));
  rb_define_const(cIGraph_generate, "STAR_IN",         INT2NUM(1));
  rb_define_const(cIGraph_generate, "STAR_UNDIRECTED", INT2NUM(2));

  rb_define_const(cIGraph_generate, "TREE_OUT",        INT2NUM(0));
  rb_define_const(cIGraph_generate, "TREE_IN",         INT2NUM(1));
  rb_define_const(cIGraph_generate, "TREE_UNDIRECTED", INT2NUM(2));

  rb_define_const(cIGraph_connectivity, "VCONN_NEI_ERROR",    INT2NUM(0));
  rb_define_const(cIGraph_connectivity, "VCONN_NEI_INFINITY", INT2NUM(1));
  rb_define_const(cIGraph_connectivity, "VCONN_NEI_IGNORE",   INT2NUM(2));  

  rb_define_const(cIGraph_community, "SPINCOMM_UPDATE_SIMPLE", INT2NUM(0));
  rb_define_const(cIGraph_community, "SPINCOMM_UPDATE_CONFIG", INT2NUM(1));  

  /* This class wraps the igraph matrix type. It can be created from and 
   * converted to an Array of Ruby Arrays.
   */
  cIGraphMatrix = rb_define_class("IGraphMatrix", rb_cObject);

  rb_define_alloc_func(cIGraphMatrix, cIGraph_matrix_alloc);
  rb_define_method(cIGraphMatrix, "initialize",      cIGraph_matrix_initialize, -1); /* in cIGraph_matrix.c */
  rb_define_method(cIGraphMatrix, "initialize_copy", cIGraph_matrix_init_copy,   1); /* in cIGraph_matrix.c */
  //rb_define_singleton_method(cIGraphMatrix, "[]",    cIGraph_matrix_initialize, -1);
  rb_include_module(cIGraphMatrix, rb_mEnumerable);
  rb_define_method (cIGraphMatrix, "each", cIGraph_matrix_each,0); /* in cIGraph_matrix.c */
 
  rb_define_method(cIGraphMatrix, "[]",   cIGraph_matrix_get,  2); /* in cIGraph_matrix.c */
  rb_define_method(cIGraphMatrix, "[]=",  cIGraph_matrix_set,  3); /* in cIGraph_matrix.c */
  rb_define_method(cIGraphMatrix, "size", cIGraph_matrix_size, 0); /* in cIGraph_matrix.c */
  rb_define_method(cIGraphMatrix, "nrow", cIGraph_matrix_nrow, 0); /* in cIGraph_matrix.c */
  rb_define_method(cIGraphMatrix, "ncol", cIGraph_matrix_ncol, 0); /* in cIGraph_matrix.c */
  rb_define_method(cIGraphMatrix, "max",  cIGraph_matrix_max,  0); /* in cIGraph_matrix.c */

  rb_define_method(cIGraphMatrix, "*", cIGraph_matrix_multiply, 1); /* in cIGraph_matrix.c */

  rb_define_method(cIGraphMatrix, "to_a", cIGraph_matrix_toa, 0); /* in cIGraph_matrix.c */

}
