#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

igraph_attribute_table_t cIGraph_attribute_table = {
  cIGraph_attribute_init,
  cIGraph_attribute_destroy,
  cIGraph_attribute_copy,
  cIGraph_attribute_add_vertices,
  cIGraph_attribute_delete_vertices,
  cIGraph_attribute_add_edges,
  cIGraph_attribute_delete_edges,
  cIGraph_attribute_permute_edges,
  cIGraph_attribute_get_info,
  cIGraph_attribute_has_attr,
  cIGraph_attribute_get_type,
  cIGraph_get_numeric_graph_attr,
  cIGraph_get_string_graph_attr,
  cIGraph_get_numeric_vertex_attr,
  cIGraph_get_string_vertex_attr,
  cIGraph_get_numeric_edge_attr,
  cIGraph_get_string_edge_attr,
};

int cIGraph_attribute_init(igraph_t *graph, igraph_vector_ptr_t *attr) {

  VALUE* attrs;

  attrs = (VALUE*)calloc(2, sizeof(VALUE));

  if(!attrs)
    IGRAPH_ERROR("Error allocating Arrays\n", IGRAPH_ENOMEM);

  //[0] is vertex array, [1] is edge array
  attrs[0] = rb_ary_new();
  attrs[1] = rb_ary_new();

  graph->attr = attrs;

  return IGRAPH_SUCCESS;
}

/* Destruction */
void cIGraph_attribute_destroy(igraph_t *graph) {
  free(graph->attr);
  return;
}

/* Copying */
int cIGraph_attribute_copy(igraph_t *to, const igraph_t *from) {
  return IGRAPH_SUCCESS;
}

/* Adding vertices */
int cIGraph_attribute_add_vertices(igraph_t *graph, long int nv, igraph_vector_ptr_t *attr) {

  int i;
  VALUE vertex_array = ((VALUE*)graph->attr)[0];

  if(attr){
    for(i=0;i<nv;i++){
      rb_ary_push(vertex_array,(VALUE)VECTOR(*attr)[i]);
    }
  }

  return IGRAPH_SUCCESS;
}

/* Deleting vertices */
void cIGraph_attribute_delete_vertices(igraph_t *graph,
				       const igraph_vector_t *eidx,
				       const igraph_vector_t *vidx) {
  

  return;
}

/* Adding edges */
int cIGraph_attribute_add_edges(igraph_t *graph, 
				const igraph_vector_t *edges, 
				igraph_vector_ptr_t *attr) {

  int i;
  VALUE edge_array = ((VALUE*)graph->attr)[1];

  if(attr){
    for(i=0;i<igraph_vector_size(edges)/2;i++){
      rb_ary_push(edge_array,(VALUE)VECTOR(*attr)[i]);
    }
  }
  
  return IGRAPH_SUCCESS;
}

/* Deleting edges */
void cIGraph_attribute_delete_edges(igraph_t *graph, const igraph_vector_t *idx) {
  return;
}

/* Permuting edges */
int cIGraph_attribute_permute_edges(igraph_t *graph,
						  const igraph_vector_t *idx) {   return 0;
}

/* Getting attribute names and types */
int cIGraph_attribute_get_info(const igraph_t *graph,
					     igraph_strvector_t *gnames,
					     igraph_vector_t *gtypes,
					     igraph_strvector_t *vnames,
					     igraph_vector_t *vtypes,
					     igraph_strvector_t *enames,
					     igraph_vector_t *etypes) {
  return 0;
}

/* Checks whether the graph has a graph/vertex/edge attribute with the given name */
igraph_bool_t cIGraph_attribute_has_attr(const igraph_t *graph,
					   igraph_attribute_elemtype_t type,
					   const char* name) {
  return 0;
}

/* Returns the type of a given attribute */
int cIGraph_attribute_get_type(const igraph_t *graph,
				 igraph_attribute_type_t *type,
				 igraph_attribute_elemtype_t elemtype,
				 const char *name) {
  return 0;
}

/* Getting numeric graph attributes */
int cIGraph_get_numeric_graph_attr(const igraph_t *graph,
					  const char *name, igraph_vector_t *value) {
  return 0;
}

/* Getting string graph attributes */
int cIGraph_get_string_graph_attr(const igraph_t *graph,
					 const char *name, igraph_strvector_t *value) {
  return 0;
}

/* Getting numeric vertex attributes */
int cIGraph_get_numeric_vertex_attr(const igraph_t *graph,
					   const char *name,
					   igraph_vs_t vs,
					   igraph_vector_t *value) {
  return 0;
}

/* Getting string vertex attributes */
int cIGraph_get_string_vertex_attr(const igraph_t *graph,
					  const char *name,
					  igraph_vs_t vs,
					  igraph_strvector_t *value) {
  return 0;
}

/* Getting numeric edge attributes */
int cIGraph_get_numeric_edge_attr(const igraph_t *graph,
					 const char *name,
					 igraph_es_t es,
					 igraph_vector_t *value) {
  return 0;
}

/* Getting string edge attributes */
int cIGraph_get_string_edge_attr(const igraph_t *graph,
					const char *name,
					igraph_es_t es,
					igraph_strvector_t *value) {
  return 0;
}
