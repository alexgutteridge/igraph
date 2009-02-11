#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.closeness(vs,mode) -> Array
 *
 * Returns an Array of closeness centrality measures for the vertices given in
 * the vs Array. mode defines the type of shortest paths used for the 
 * calculation
 */
VALUE cIGraph_closeness(VALUE self, VALUE vs, VALUE mode){

  igraph_t *graph;
  igraph_vs_t vids;
  igraph_vector_t vidv;
  igraph_neimode_t pmode = NUM2INT(mode);
  igraph_vector_t res;
  int i;
  VALUE closeness = rb_ary_new();

  //vector to hold the results of the degree calculations
  igraph_vector_init_int(&res,0);

  Data_Get_Struct(self, igraph_t, graph);

  //Convert an array of vertices to a vector of vertex ids
  igraph_vector_init_int(&vidv,0);
  cIGraph_vertex_arr_to_id_vec(self,vs,&vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&vids,&vidv);

  igraph_closeness(graph,&res,vids,pmode);

  for(i=0;i<igraph_vector_size(&res);i++){
    rb_ary_push(closeness,rb_float_new(VECTOR(res)[i]));
  }

  igraph_vector_destroy(&vidv);
  igraph_vector_destroy(&res);
  igraph_vs_destroy(&vids);

  return closeness;

}

/* call-seq:
 *   graph.betweenness(vs,mode) -> Array
 *
 * Returns an Array of betweenness centrality measures for the vertices given 
 * in the vs Array. mode defines whether directed paths or considered for 
 * directed graphs.
 */
VALUE cIGraph_betweenness(VALUE self, VALUE vs, VALUE directed){

  igraph_t *graph;
  igraph_vs_t vids;
  igraph_vector_t vidv;
  igraph_bool_t dir = 0;
  igraph_vector_t res;
  int i;
  VALUE betweenness = rb_ary_new();

  if(directed == Qtrue)
    dir = 1;

  //vector to hold the results of the degree calculations
  IGRAPH_FINALLY(igraph_vector_destroy, &res);
  IGRAPH_FINALLY(igraph_vector_destroy, &vidv);
  IGRAPH_FINALLY(igraph_vs_destroy,&vids);
   
  IGRAPH_CHECK(igraph_vector_init(&res,0));

  Data_Get_Struct(self, igraph_t, graph);

  //Convert an array of vertices to a vector of vertex ids
  IGRAPH_CHECK(igraph_vector_init_int(&vidv,0));
  cIGraph_vertex_arr_to_id_vec(self,vs,&vidv);
  //create vertex selector from the vecotr of ids
  IGRAPH_CHECK(igraph_vs_vector(&vids,&vidv));

  IGRAPH_CHECK(igraph_betweenness(graph,&res,vids,dir));

  for(i=0;i<igraph_vector_size(&res);i++){
    rb_ary_push(betweenness,rb_float_new((float)VECTOR(res)[i]));
  }

  igraph_vector_destroy(&vidv);
  igraph_vector_destroy(&res);
  igraph_vs_destroy(&vids);

  IGRAPH_FINALLY_CLEAN(3);

  return betweenness;

}

/* call-seq:
 *   graph.edge_betweenness(mode) -> Array
 *
 * Returns an Array of betweenness centrality measures for the edges 
 * in the graph. mode defines whether directed paths or considered for 
 * directed graphs.
 */
VALUE cIGraph_edge_betweenness(VALUE self, VALUE directed){

  igraph_t *graph;
  igraph_bool_t dir = 0;
  igraph_vector_t res;
  int i;
  VALUE betweenness = rb_ary_new();

  if(directed == Qtrue)
    dir = 1;

  //vector to hold the results of the degree calculations
  igraph_vector_init_int(&res,0);

  Data_Get_Struct(self, igraph_t, graph);

  igraph_edge_betweenness(graph,&res,dir);

  for(i=0;i<igraph_vector_size(&res);i++){
    rb_ary_push(betweenness,INT2NUM((int)VECTOR(res)[i]));
  }

  igraph_vector_destroy(&res);

  return betweenness;

}

/* call-seq:
 *   graph.pagerank(vs,mode,niter,eps,damping) -> Array
 *
 * Returns an Array of PageRank measures for the vertices 
 * in the graph. mode defines whether directed paths or considered for 
 * directed graphs.
 */
VALUE cIGraph_pagerank(VALUE self, VALUE vs, VALUE directed, VALUE niter, VALUE eps, VALUE damping){

  igraph_t *graph;
  igraph_vs_t vids;
  igraph_vector_t vidv;
  igraph_vector_t res;
  int i;
  VALUE pagerank = rb_ary_new();
  igraph_bool_t dir = 0;

  if(directed == Qtrue)
    dir = 1;

  //vector to hold the results of the degree calculations
  igraph_vector_init_int(&res,0);

  Data_Get_Struct(self, igraph_t, graph);

  //Convert an array of vertices to a vector of vertex ids
  igraph_vector_init_int(&vidv,0);
  cIGraph_vertex_arr_to_id_vec(self,vs,&vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&vids,&vidv);

  igraph_pagerank_old(graph,&res,vids,dir,
		  NUM2INT(niter),NUM2DBL(eps),NUM2DBL(damping),0);

  for(i=0;i<igraph_vector_size(&res);i++){
    rb_ary_push(pagerank,rb_float_new(VECTOR(res)[i]));
  }

  igraph_vector_destroy(&vidv);
  igraph_vector_destroy(&res);
  igraph_vs_destroy(&vids);

  return pagerank;

}

/* call-seq:
 *   graph.constraint(vs,weights) -> Array
 *
 * Returns an Array of constraint measures for the vertices 
 * in the graph. Weights is an Array of weight measures for each edge.
 */
VALUE cIGraph_constraint(int argc, VALUE *argv, VALUE self){

  igraph_t *graph;
  igraph_vs_t vids;
  igraph_vector_t vidv;
  igraph_vector_t res;
  igraph_vector_t wght;
  int i;
  VALUE constraints = rb_ary_new();
  VALUE vs, weights;

  rb_scan_args(argc,argv,"11",&vs, &weights);

  //vector to hold the results of the degree calculations
  IGRAPH_FINALLY(igraph_vector_destroy, &res);
  IGRAPH_FINALLY(igraph_vector_destroy, &wght);
  IGRAPH_FINALLY(igraph_vector_destroy, &vidv);
  IGRAPH_CHECK(igraph_vector_init(&res,0));
  IGRAPH_CHECK(igraph_vector_init(&wght,0));

  Data_Get_Struct(self, igraph_t, graph);

  //Convert an array of vertices to a vector of vertex ids
  IGRAPH_CHECK(igraph_vector_init_int(&vidv,0));
  cIGraph_vertex_arr_to_id_vec(self,vs,&vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&vids,&vidv);

  if(weights == Qnil){
    IGRAPH_CHECK(igraph_constraint(graph,&res,vids,NULL));
  } else {
    for(i=0;i<RARRAY_LEN(weights);i++){
      IGRAPH_CHECK(igraph_vector_push_back(&wght,NUM2DBL(RARRAY_PTR(weights)[i])));
    }
    IGRAPH_CHECK(igraph_constraint(graph,&res,vids,&wght));
  }

  for(i=0;i<igraph_vector_size(&res);i++){
    rb_ary_push(constraints,rb_float_new(VECTOR(res)[i]));
  }

  igraph_vector_destroy(&vidv);
  igraph_vector_destroy(&res);
  igraph_vector_destroy(&wght);
  igraph_vs_destroy(&vids);

  IGRAPH_FINALLY_CLEAN(3);

  return constraints;

}


/* call-seq:
 *   graph.maxdegree(vs,mode,loops) -> Vertex
 *
 * Returns the vertex Object in the vs Array with the largest degree. 
 * mode defines the type 
 * of the degree. IGRAPH_OUT, out-degree, IGRAPH_IN, in-degree, IGRAPH_ALL, 
 * total degree (sum of the in- and out-degree). This parameter is ignored 
 * for undirected graphs. loops is a boolean gives whether the self-loops 
 * should be counted. 
 */
VALUE cIGraph_maxdegree(VALUE self, VALUE vs, VALUE mode, VALUE loops){

  igraph_t *graph;
  igraph_bool_t loop = 0;
  igraph_integer_t res;
  igraph_neimode_t pmode = NUM2INT(mode);
  igraph_vs_t vids;
  igraph_vector_t vidv;

  if(loops == Qtrue)
    loop = 1;

  Data_Get_Struct(self, igraph_t, graph);

  //Convert an array of vertices to a vector of vertex ids
  igraph_vector_init_int(&vidv,0);
  cIGraph_vertex_arr_to_id_vec(self,vs,&vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&vids,&vidv);

  igraph_maxdegree(graph,&res,vids,pmode,loop);

  igraph_vector_destroy(&vidv);
  igraph_vs_destroy(&vids);

  return INT2NUM(res);

}
