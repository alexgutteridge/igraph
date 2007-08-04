#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

//Classes
VALUE cIGraph;
VALUE cIGraphError;

void cIGraph_free(void *p){
  igraph_destroy(p);
}

VALUE cIGraph_alloc(VALUE klass){

  igraph_t *graph = malloc(sizeof(igraph_t));
  VALUE obj;

  igraph_empty(graph, 0, 1);

  obj = Data_Wrap_Struct(klass, 0, cIGraph_free, graph);

  return obj;
  
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
  VALUE object_h;
  VALUE id_h;
  VALUE directed;
  VALUE edges;
  VALUE attrs;
  int vertex_n = 0;
  int current_vertex_id;
  int i;

  igraph_vector_ptr_t vertex_attr;

  rb_scan_args(argc,argv,"12", &edges, &directed, &attrs);

    igraph_set_error_handler(cIGraph_error_handler);
  igraph_set_warning_handler(cIGraph_warning_handler);  

  //New hash for mapping vertex objects to floats used by iGraph
  object_h = rb_iv_set(self,"@object_ids",rb_hash_new());
  id_h     = rb_iv_set(self,"@id_objects",rb_hash_new());

  rb_iv_set(self,"@edge_attrs",rb_hash_new());

  //Initialize edge vector
  igraph_vector_init_int(&edge_v,0);
  igraph_vector_ptr_init(&vertex_attr,0);

  Data_Get_Struct(self, igraph_t, graph);

  if(!directed)
    igraph_to_undirected(graph,IGRAPH_TO_UNDIRECTED_COLLAPSE);

  //Loop through objects in edge Array
  for (i=0; i<RARRAY(edges)->len; i++) {
    vertex = RARRAY(edges)->ptr[i];
    if(rb_funcall(object_h,rb_intern("has_key?"),1,vertex)){
      //If @vertices includes this vertex then look up the vertex number
      current_vertex_id = NUM2INT(rb_hash_aref(object_h,vertex));
    } else {
      //otherwise add a new entadd_vertry to Hash
      rb_hash_aset(object_h,vertex,INT2NUM(vertex_n));
      rb_hash_aset(id_h,    INT2NUM(vertex_n),vertex);
      current_vertex_id = vertex_n;
      
      igraph_vector_ptr_push_back(&vertex_attr,(void*)RARRAY(edges)->ptr[i]);

      vertex_n++;
      
    }
    igraph_vector_push_back(&edge_v,current_vertex_id);
  }

  if(igraph_vector_size(&edge_v) > 0){
    igraph_add_vertices(graph,vertex_n,&vertex_attr);
    igraph_add_edges(graph,&edge_v,0);
  }

  //if(attrs != Qnil){
  //for (i=0; i<RARRAY(attrs)->len; i++) {
  //  cIGraph_set_edge_attr(self,
  //		    RARRAY(edges)->ptr[i*2],
  //		    RARRAY(edges)->ptr[(i*2)+1],
  //		    RARRAY(attrs)->ptr[i]);
  //}
  //}

  igraph_vector_destroy(&edge_v);
  igraph_vector_ptr_destroy(&vertex_attr);

  return self;

}

/* Interface to the iGraph[http://cneurocvs.rmki.kfki.hu/igraph/] library
 * for graph and network computation. See IGraph#new for how to create a
 * graph and get started.
 */

void Init_igraph(){

  cIGraph = rb_define_class("IGraph", rb_cObject);
  cIGraphError = rb_define_class("IGraphError", rb_eRuntimeError);

  rb_define_alloc_func(cIGraph, cIGraph_alloc);
  rb_define_method(cIGraph, "initialize", cIGraph_initialize, -1);

  //rb_define_method(cIGraph, "[]",            cIGraph_get_edge_attr, 2);
  //rb_define_method(cIGraph, "[]=",           cIGraph_set_edge_attr, 3);
  //rb_define_method(cIGraph, "get_edge_attr", cIGraph_get_edge_attr, 2);
  //rb_define_method(cIGraph, "set_edge_attr", cIGraph_set_edge_attr, 3);

  rb_define_method(cIGraph, "each_vertex",   cIGraph_each_vertex,  0); /* in cIGraph_iterators.c */
  rb_define_method(cIGraph, "each_edge",     cIGraph_each_edge,    1); /* in cIGraph_iterators.c */
  rb_define_method(cIGraph, "each_edge_eid", cIGraph_each_edge_eid,1); /* in cIGraph_iterators.c */ 

  rb_define_const(cIGraph, "EDGEORDER_ID",   INT2NUM(1));
  rb_define_const(cIGraph, "EDGEORDER_FROM", INT2NUM(2));
  rb_define_const(cIGraph, "EDGEORDER_TO",   INT2NUM(3));

  rb_define_method(cIGraph, "each",        cIGraph_each_vertex, 0);
  rb_include_module(cIGraph, rb_mEnumerable);

  rb_define_method(cIGraph, "include?", cIGraph_include, 1);

  rb_define_method(cIGraph, "all_vertices",         cIGraph_all_v,    0);
  rb_define_method(cIGraph, "vertices",             cIGraph_all_v,    0); /* in cIGraph_selectors.c */
  rb_define_method(cIGraph, "adjacent_vertices",    cIGraph_adj_v,    2); /* in cIGraph_selectors.c */
  rb_define_method(cIGraph, "nonadjacent_vertices", cIGraph_nonadj_v, 2); /* in cIGraph_selectors.c */

  rb_define_method(cIGraph, "all_edges",         cIGraph_all_e,    1);
  rb_define_method(cIGraph, "edges",             cIGraph_all_e,    1);
  rb_define_method(cIGraph, "adjacent_edges",    cIGraph_adj_e,    2);
  rb_define_method(cIGraph, "nonadjacent_edges", cIGraph_nonadj_e, 2);  

  rb_define_method(cIGraph, "vcount", cIGraph_vcount, 0); /* in cIGraph_basic_query.c */
  rb_define_method(cIGraph, "ecount", cIGraph_ecount, 0); /* in cIGraph_basic_query.c */

  rb_define_method(cIGraph, "edge",    cIGraph_edge, 1); /* in cIGraph_basic_query.c */
  rb_define_method(cIGraph, "get_eid", cIGraph_get_eid, 2); /* in cIGraph_basic_query.c */

  rb_define_method(cIGraph, "neighbors",  cIGraph_neighbors,2);
  rb_define_method(cIGraph, "neighbours", cIGraph_neighbors,2); /* in cIGraph_basic_query.c */
  rb_define_method(cIGraph, "adjacent",   cIGraph_adjacent,2); /* in cIGraph_basic_query.c */

  rb_define_const(cIGraph, "OUT",   INT2NUM(1));
  rb_define_const(cIGraph, "IN",    INT2NUM(2));
  rb_define_const(cIGraph, "ALL",   INT2NUM(3));
  rb_define_const(cIGraph, "TOTAL", INT2NUM(4));
  
  rb_define_method(cIGraph, "is_directed",  cIGraph_is_directed,0);
  rb_define_method(cIGraph, "is_directed?", cIGraph_is_directed,0); /* in cIGraph_basic_query.c */ 

  rb_define_method(cIGraph, "degree", cIGraph_degree,3); /* in cIGraph_basic_query.c */

  rb_define_method(cIGraph, "add_edges",    cIGraph_add_edges,    -1); /* in cIGraph_add_delete.c */
  rb_define_method(cIGraph, "add_vertices", cIGraph_add_vertices, 1); /* in cIGraph_add_delete.c */

  rb_define_method(cIGraph, "add_edge",   cIGraph_add_edge,   -1); /* in cIGraph_add_delete.c */
  rb_define_method(cIGraph, "add_vertex", cIGraph_add_vertex, 1); /* in cIGraph_add_delete.c */

  rb_define_method(cIGraph, "delete_edge", cIGraph_delete_edge, 2); /* in cIGraph_add_delete.c */

  rb_define_method(cIGraph, "are_connected",  cIGraph_are_connected,2);
  rb_define_method(cIGraph, "are_connected?", cIGraph_are_connected,2); /* in cIGraph_basic_properties.c */  

  rb_define_method(cIGraph, "shortest_paths",         cIGraph_shortest_paths,2); /* in cIGraph_shortest_paths.c */
  rb_define_method(cIGraph, "get_shortest_paths",     cIGraph_get_shortest_paths,3); /* in cIGraph_shortest_paths.c */
  rb_define_method(cIGraph, "get_all_shortest_paths", cIGraph_get_all_shortest_paths,3); /* in cIGraph_shortest_paths.c */  
  rb_define_method(cIGraph, "average_path_length",    cIGraph_average_path_length,2); /* in cIGraph_shortest_paths.c */  
  rb_define_method(cIGraph, "diameter",               cIGraph_diameter,2); /* in cIGraph_shortest_paths.c */
  rb_define_method(cIGraph, "girth",                  cIGraph_girth,0); /* in cIGraph_shortest_paths.c */


  rb_define_method(cIGraph, "neighbourhood_size",   cIGraph_neighborhood_size, 3);  /* in cIGraph_vertex_neighbourhood.c */
  rb_define_method(cIGraph, "neighborhood_size",    cIGraph_neighborhood_size, 3);  
  rb_define_method(cIGraph, "neighbourhood",        cIGraph_neighborhood, 3); /* in cIGraph_vertex_neighbourhood.c */
  rb_define_method(cIGraph, "neighborhood",         cIGraph_neighborhood, 3);
  rb_define_method(cIGraph, "neighbourhood_graphs", cIGraph_neighborhood_graphs, 3);  /* in cIGraph_vertex_neighbourhood.c */
  rb_define_method(cIGraph, "neighborhood_graphs",  cIGraph_neighborhood_graphs, 3);

  rb_define_method(cIGraph, "topological_sorting", cIGraph_topological_sorting, 1); /* in cIGraph_topological_sort.c */

  rb_define_singleton_method(cIGraph, "read_graph_edgelist", cIGraph_read_graph_edgelist, 2); /* in cIGraph_file.c */
  rb_define_method(cIGraph, "write_graph_edgelist", cIGraph_write_graph_edgelist, 1); /* in cIGraph_file.c */

  igraph_i_set_attribute_table(&cIGraph_attribute_table);

}
