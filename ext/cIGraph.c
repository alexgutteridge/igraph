#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

//Classes
VALUE cIGraph;
VALUE cIGraphError;

void cIGraph_free(void *p){
  igraph_destroy(p);
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

  igraph_copy(copy_graph,orig_graph);

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
  igraph_vector_init_int(&edge_v,0);

  igraph_vector_ptr_init(&vertex_attr,0);
  igraph_vector_ptr_init(&edge_attr,0);

  Data_Get_Struct(self, igraph_t, graph);

  v_ary = rb_ary_new();

  if(!directed)
    igraph_to_undirected(graph,IGRAPH_TO_UNDIRECTED_COLLAPSE);

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
    igraph_vector_push_back(&edge_v,current_vertex_id);
    if (i % 2){
      if (attrs != Qnil){
	rb_ary_push((VALUE)e_attr_rec.value,RARRAY(attrs)->ptr[i/2]);
      } else {
	rb_ary_push((VALUE)e_attr_rec.value,Qnil);
      }
    }
  }

  igraph_vector_ptr_push_back(&vertex_attr, &v_attr_rec);
  igraph_vector_ptr_push_back(&edge_attr,   &e_attr_rec);

  if(igraph_vector_size(&edge_v) > 0){
    igraph_add_vertices(graph,vertex_n,&vertex_attr);
    igraph_add_edges(graph,&edge_v,&edge_attr);
  }

  igraph_vector_destroy(&edge_v);
  igraph_vector_ptr_destroy(&vertex_attr);
  igraph_vector_ptr_destroy(&edge_attr);

  return self;

}

/* Interface to the iGraph[http://cneurocvs.rmki.kfki.hu/igraph/] library
 * for graph and network computation. See IGraph#new for how to create a
 * graph and get started.
 */

void Init_igraph(){

  igraph_i_set_attribute_table(&cIGraph_attribute_table);
  igraph_set_error_handler(cIGraph_error_handler);
  igraph_set_warning_handler(cIGraph_warning_handler);  

  cIGraph      = rb_define_class("IGraph",      rb_cObject);
  cIGraphError = rb_define_class("IGraphError", rb_eRuntimeError);

  rb_define_alloc_func(cIGraph, cIGraph_alloc);
  rb_define_method(cIGraph, "initialize",      cIGraph_initialize, -1);
  rb_define_method(cIGraph, "initialize_copy", cIGraph_init_copy,   1);

  rb_include_module(cIGraph, rb_mEnumerable);

  rb_define_const(cIGraph, "VERSION", rb_str_new2("0.3"));

  rb_define_const(cIGraph, "EDGEORDER_ID",   INT2NUM(1));
  rb_define_const(cIGraph, "EDGEORDER_FROM", INT2NUM(2));
  rb_define_const(cIGraph, "EDGEORDER_TO",   INT2NUM(3));

  rb_define_const(cIGraph, "OUT",   INT2NUM(1));
  rb_define_const(cIGraph, "IN",    INT2NUM(2));
  rb_define_const(cIGraph, "ALL",   INT2NUM(3));
  rb_define_const(cIGraph, "TOTAL", INT2NUM(4));

  rb_define_const(cIGraph, "WEAK",   INT2NUM(1));
  rb_define_const(cIGraph, "STRONG", INT2NUM(2));  

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

  rb_define_method(cIGraph, "shortest_paths",         cIGraph_shortest_paths,         2); /* in cIGraph_shortest_paths.c */
  rb_define_method(cIGraph, "get_shortest_paths",     cIGraph_get_shortest_paths,     3); /* in cIGraph_shortest_paths.c */
  rb_define_method(cIGraph, "get_all_shortest_paths", cIGraph_get_all_shortest_paths, 3); /* in cIGraph_shortest_paths.c */  
  rb_define_method(cIGraph, "average_path_length",    cIGraph_average_path_length,    2); /* in cIGraph_shortest_paths.c */  
  rb_define_method(cIGraph, "diameter",               cIGraph_diameter,               2); /* in cIGraph_shortest_paths.c */
  rb_define_method(cIGraph, "girth",                  cIGraph_girth,                  0); /* in cIGraph_shortest_paths.c */

  rb_define_method(cIGraph, "neighbourhood_size",   cIGraph_neighborhood_size,   3); /* in cIGraph_vertex_neighbourhood.c */
  rb_define_method(cIGraph, "neighbourhood",        cIGraph_neighborhood,        3); /* in cIGraph_vertex_neighbourhood.c */
  rb_define_method(cIGraph, "neighbourhood_graphs", cIGraph_neighborhood_graphs, 3); /* in cIGraph_vertex_neighbourhood.c */
  rb_define_alias (cIGraph, "neighborhood_size", "neighbourhood_size");
  rb_define_alias (cIGraph, "neighborhood", "neighbourhood");
  rb_define_alias (cIGraph, "neighborhood_graphs", "neighbourhood_graphs");

  rb_define_method(cIGraph, "subcomponent", cIGraph_subcomponent, 2); /* in cIGraph_components.c */
  rb_define_method(cIGraph, "subgraph",     cIGraph_subgraph,     1); /* in cIGraph_components.c */
  rb_define_method(cIGraph, "clusters",     cIGraph_clusters,     1); /* in cIGraph_components.c */
  rb_define_method(cIGraph, "decompose",    cIGraph_decompose,   -1); /* in cIGraph_components.c */

  rb_define_method(cIGraph, "closeness",        cIGraph_closeness,        2); /* in cIGraph_centrality.c */
  rb_define_method(cIGraph, "betweenness",      cIGraph_betweenness,      2); /* in cIGraph_centrality.c */
  rb_define_method(cIGraph, "edge_betweenness", cIGraph_edge_betweenness, 1); /* in cIGraph_centrality.c */
  rb_define_method(cIGraph, "pagerank",         cIGraph_pagerank,         5); /* in cIGraph_centrality.c */  
  rb_define_method(cIGraph, "constraint",       cIGraph_constraint,      -1); /* in cIGraph_centrality.c */  
  rb_define_method(cIGraph, "maxdegree",        cIGraph_maxdegree,        3); /* in cIGraph_centrality.c */    

  rb_define_method(cIGraph, "topological_sorting", cIGraph_topological_sorting, 1); /* in cIGraph_topological_sort.c */

  rb_define_singleton_method(cIGraph, "read_graph_edgelist", cIGraph_read_graph_edgelist, 2); /* in cIGraph_file.c */
  rb_define_singleton_method(cIGraph, "read_graph_graphml", cIGraph_read_graph_graphml, 2);   /* in cIGraph_file.c */  
  rb_define_singleton_method(cIGraph, "read_graph_pajek", cIGraph_read_graph_pajek, 2);   /* in cIGraph_file.c */  
  rb_define_method(cIGraph, "write_graph_edgelist", cIGraph_write_graph_edgelist, 1);         /* in cIGraph_file.c */
  rb_define_method(cIGraph, "write_graph_graphml", cIGraph_write_graph_graphml,   1);         /* in cIGraph_file.c */  
  rb_define_method(cIGraph, "write_graph_pajek", cIGraph_write_graph_pajek, 1);         /* in cIGraph_file.c */

  rb_define_method(cIGraph, "layout_random", cIGraph_layout_random, 0);
  rb_define_method(cIGraph, "layout_circle", cIGraph_layout_circle, 0);
  rb_define_method(cIGraph, "layout_fruchterman_reingold", cIGraph_layout_fruchterman_reingold, 6);

  //Matrix class
  cIGraphMatrix = rb_define_class("IGraphMatrix", rb_cObject);

  rb_define_alloc_func(cIGraphMatrix, cIGraph_matrix_alloc);
  rb_define_method(cIGraphMatrix, "initialize",      cIGraph_matrix_initialize, -1);
  rb_define_method(cIGraphMatrix, "initialize_copy", cIGraph_matrix_init_copy,   1);
  //rb_define_singleton_method(cIGraphMatrix, "[]",    cIGraph_matrix_initialize, -1);
  rb_include_module(cIGraphMatrix, rb_mEnumerable);
  rb_define_method (cIGraphMatrix, "each", cIGraph_matrix_each,0);
 
  rb_define_method(cIGraphMatrix, "[]",   cIGraph_matrix_get,  2);
  rb_define_method(cIGraphMatrix, "[]=",  cIGraph_matrix_set,  3);
  rb_define_method(cIGraphMatrix, "size", cIGraph_matrix_size, 0);
  rb_define_method(cIGraphMatrix, "nrow", cIGraph_matrix_nrow, 0);
  rb_define_method(cIGraphMatrix, "ncol", cIGraph_matrix_ncol, 0);
  rb_define_method(cIGraphMatrix, "max",  cIGraph_matrix_max,  0);

  rb_define_method(cIGraphMatrix, "*", cIGraph_matrix_multiply, 1);

  rb_define_method(cIGraphMatrix, "to_a", cIGraph_matrix_toa, 0);

}
