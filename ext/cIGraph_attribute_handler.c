#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph[u,v] -> Object
 * 
 * Returns the object associated with the edge connecting vertices u and v.
 * Aliased to graph.get_edge_attr(u,v)
 */
VALUE cIGraph_get_edge_attr(VALUE self, VALUE from, VALUE to){

  int idx;
  igraph_t *graph;
  VALUE e_ary;

  Data_Get_Struct(self, igraph_t, graph);
  e_ary = ((VALUE*)graph->attr)[1];

  idx = NUM2INT(cIGraph_get_eid(self, from, to, 1));
  return rb_ary_entry(e_ary,idx);

}

/* call-seq:
 *   graph[u,v] = w
 * 
 * Sets the object associated with the edge connecting vertices u and v.
 * Aliased to graph.set_edge_attr(u,v)
 */
VALUE cIGraph_set_edge_attr(VALUE self, VALUE from, VALUE to, VALUE attr){

  int idx;
  igraph_t *graph;
  VALUE e_ary;

  Data_Get_Struct(self, igraph_t, graph);
  e_ary = ((VALUE*)graph->attr)[1];

  idx = NUM2INT(cIGraph_get_eid(self, from, to, 1));
  rb_ary_store(e_ary,idx,attr);

  return Qtrue;

}

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

  VALUE vertex_array = ((VALUE*)from->attr)[0];
  VALUE edge_array   = ((VALUE*)from->attr)[1];

  VALUE* attrs = (VALUE*)calloc(2, sizeof(VALUE));

  attrs[0] = rb_ary_dup(vertex_array);
  attrs[1] = rb_ary_dup(edge_array);

  to->attr = attrs;  

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
 
 int i;
 VALUE vertex_array = ((VALUE*)graph->attr)[0];
 VALUE edge_array   = ((VALUE*)graph->attr)[1];

 VALUE n_v_ary = rb_ary_new();
 VALUE n_e_ary = rb_ary_new();

 for(i=0;i<igraph_vector_size(vidx);i++){
   if(VECTOR(*vidx)[i] != 0)
     rb_ary_store(n_v_ary,VECTOR(*vidx)[i]-1,rb_ary_entry(vertex_array,i));
 }
 for(i=0;i<igraph_vector_size(eidx);i++){
   if(VECTOR(*eidx)[i] != 0)
     rb_ary_store(n_e_ary,VECTOR(*eidx)[i]-1,rb_ary_entry(edge_array,i));
 }
  
 ((VALUE*)graph->attr)[0] = n_v_ary;
 ((VALUE*)graph->attr)[1] = n_e_ary;

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

  int i;
  VALUE edge_array = ((VALUE*)graph->attr)[1];
  VALUE n_e_ary = rb_ary_new();

  for(i=0;i<igraph_vector_size(idx);i++){
   if(VECTOR(*idx)[i] != 0)
     rb_ary_store(n_e_ary,VECTOR(*idx)[i]-1,rb_ary_entry(edge_array,i));
  }

  ((VALUE*)graph->attr)[1] = n_e_ary;
  
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
