#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.subcomponent(v,mode) -> Array
 *
 * Returns an Array of vertices that are in the same component as the vertex v.
 * mode defines the type of the component for directed graphs, possible 
 * values: IGraph::OUT: the set of vertices reachable from the vertex, 
 * IGraph::IN the set of vertices from which the vertex is reachable, 
 * IGraph::ALL 	the graph is considered as an undirected graph. Note that 
 * this is not the same as the union of the previous two. 
 */
VALUE cIGraph_subcomponent(VALUE self, VALUE v, VALUE mode){

  igraph_t *graph;
  igraph_neimode_t pmode = NUM2INT(mode);
  igraph_vector_t neis;
  int i;
  VALUE component = rb_ary_new();

  igraph_vector_init_int(&neis,0);

  Data_Get_Struct(self, igraph_t, graph);

  igraph_subcomponent(graph, &neis, cIGraph_get_vertex_id(self,v), pmode);

  for(i=0;i<igraph_vector_size(&neis);i++){
    rb_ary_push(component,cIGraph_get_vertex_object(self,VECTOR(neis)[i]));
  }

  igraph_vector_destroy(&neis);

  return component;  

}

/* call-seq:
 *   graph.subgraph(vs) -> IGraph
 *
 * Returns an IGraph object containing the vertices defined in the Array vs.
 */
VALUE cIGraph_subgraph(VALUE self, VALUE vs){

  igraph_t *graph;
  igraph_t *n_graph = malloc(sizeof(igraph_t));
  igraph_vs_t vids;
  igraph_vector_t vidv;

  VALUE n_graph_obj;

  Data_Get_Struct(self, igraph_t, graph);

  //Convert an array of vertices to a vector of vertex ids
  igraph_vector_init_int(&vidv,0);
  cIGraph_vertex_arr_to_id_vec(self,vs,&vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&vids,&vidv);

  igraph_subgraph(graph,n_graph,vids);

  n_graph_obj = Data_Wrap_Struct(cIGraph, cIGraph_mark, cIGraph_free, n_graph);

  igraph_vector_destroy(&vidv);
  igraph_vs_destroy(&vids);

  return n_graph_obj;

}

/* call-seq:
 *   graph.clusters(mode) -> Array
 *
 * Calculates the (weakly or strongly) connected components in a graph.
 * Returns an Array of Arrays of vertices. Each sub-Array is a graph component
 */
VALUE cIGraph_clusters(VALUE self, VALUE mode){

  igraph_t *graph;
  igraph_vector_t membership;
  igraph_integer_t no;
  VALUE components;
  VALUE v,c;
  int i;

  igraph_vector_init_int(&membership,0);
  
  Data_Get_Struct(self, igraph_t, graph); 
  
  igraph_clusters(graph, &membership, NULL, &no, NUM2INT(mode));

  components = rb_ary_new();
  for(i=0;i<no;i++){
    rb_ary_push(components,rb_ary_new());
  }

  for(i=0;i<igraph_vector_size(&membership);i++){
    v = cIGraph_get_vertex_object(self, i);
    c = rb_ary_entry(components,VECTOR(membership)[i]);
    rb_ary_push(c,v);
  }

  igraph_vector_destroy(&membership);

  return components;

}

/* call-seq:
 *    graph.decompose(mode,maxcomp=-1,minelem=1) -> Array
 * 
 * Create separate graph for each component of a graph. Returns an Array
 * of new IGraph object. mode specifies whether weakly and strongly connected 
 * components are returned. Right now only the former is implemented. maxcomp
 * limits the number of components returned. Leave at the default -1 to return
 * all components. minelements specifies the minimum number of vertices a
 * component should contain before it is returned. Default 1 returns all 
 * components.
 */
VALUE cIGraph_decompose(int argc, VALUE *argv, VALUE self){

  igraph_t *graph;
  igraph_t *n_graph;
  igraph_vector_ptr_t components;
  VALUE mode,maxcomp, minelem, components_a;
  VALUE n_graph_obj;
  int i;

  rb_scan_args(argc,argv,"12", &mode, &maxcomp, &minelem);

  if(maxcomp == Qnil)
    maxcomp = INT2NUM(-1);
  if(minelem == Qnil)
    minelem = INT2NUM(1);

  igraph_vector_ptr_init(&components,0);
  
  Data_Get_Struct(self, igraph_t, graph); 
  
  igraph_decompose(graph, &components, NUM2INT(mode), NUM2INT(maxcomp), NUM2INT(minelem));

  components_a = rb_ary_new();

  for(i=0; i<igraph_vector_ptr_size(&components); i++){
    n_graph = VECTOR(components)[i];
    n_graph_obj = Data_Wrap_Struct(cIGraph, cIGraph_mark, cIGraph_free, n_graph);
    rb_ary_push(components_a,n_graph_obj);
  } 

  igraph_vector_ptr_destroy(&components);

  return components_a;

}
