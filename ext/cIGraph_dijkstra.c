#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.dijkstra_shortest_paths(varray,weights,mode) -> Array
 *
 * Calculates the length of the shortest paths from each of the vertices in
 * the varray Array to all of the other vertices in the graph given a set of 
 * edge weights given in the weights Array. The result
 * is returned as an Array of Array. Each top-level Array contains the results
 * for a vertex in the varray Array. Each entry in the Array is the path length
 * to another vertex in the graph in vertex order (the order the vertices were
 * added to the graph. (This should probalby be changed to give a Hash of Hash
 * to allow easier look up.)
 */
VALUE cIGraph_dijkstra_shortest_paths(VALUE self, VALUE from, VALUE weights, VALUE mode){

  igraph_t *graph;
  igraph_vs_t vids;
  igraph_vector_t vidv;
  igraph_vector_t wghts;
  igraph_neimode_t pmode = NUM2INT(mode);
  igraph_matrix_t res;
  int i;
  int j;
  VALUE row;
  VALUE path_length;
  VALUE matrix = rb_ary_new();
  int n_row;
  int n_col;

  Data_Get_Struct(self, igraph_t, graph);

  n_row = NUM2INT(rb_funcall(from,rb_intern("length"),0));
  n_col = igraph_vcount(graph);

  //matrix to hold the results of the calculations
  igraph_matrix_init(&res,n_row,n_col);

  igraph_vector_init(&wghts,RARRAY(weights)->len);

  for(i=0;i<RARRAY(weights)->len;i++){
    VECTOR(wghts)[i] = NUM2DBL(RARRAY(weights)->ptr[i]);
  }

  //Convert an array of vertices to a vector of vertex ids
  igraph_vector_init_int(&vidv,0);
  cIGraph_vertex_arr_to_id_vec(self,from,&vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&vids,&vidv);

  igraph_dijkstra_shortest_paths(graph,&res,vids,&wghts,pmode);

  for(i=0; i<igraph_matrix_nrow(&res); i++){
    row = rb_ary_new();
    rb_ary_push(matrix,row);
    for(j=0; j<igraph_matrix_ncol(&res); j++){
      path_length = MATRIX(res,i,j) == n_col ? Qnil : rb_float_new(MATRIX(res,i,j));
      rb_ary_push(row,path_length);
    }
  }

  igraph_vector_destroy(&vidv);
  igraph_matrix_destroy(&res);
  igraph_vs_destroy(&vids);

  return matrix;

}

/* call-seq:
 *   graph.get_shortest_paths(from,to_array,mode) -> Array
 *
 * Calculates the paths from the vertex specified as from to each vertex in the
 * to_array Array. Returns an Array of Arrays. Each top level Array represents
 * a path and each entry in each Array is a vertex on the path. mode
 * represents the type of shortest paths to be calculated: IGraph::OUT
 * the outgoing paths are calculated. IGraph::IN the incoming paths are 
 * calculated. IGraph::ALL the directed graph is considered as an undirected 
 * one for the computation. 
 */
VALUE cIGraph_get_dijkstra_shortest_paths(VALUE self, VALUE from, VALUE to, VALUE mode){

  igraph_t *graph;

  igraph_integer_t from_vid;
  igraph_vs_t to_vids;
  igraph_vector_t to_vidv;

  igraph_neimode_t pmode = NUM2INT(mode);

  igraph_vector_ptr_t res;
  igraph_vector_t *path_v;

  int i;
  int j;
  VALUE path;
  VALUE matrix = rb_ary_new();
  int n_paths;

  Data_Get_Struct(self, igraph_t, graph);

  n_paths = RARRAY(to)->len;

  //vector to hold the results of the calculations
  igraph_vector_ptr_init(&res,0);
  for(i=0;i<n_paths;i++){
    path_v = malloc(sizeof(igraph_vector_t));
    igraph_vector_init(path_v,0);
    igraph_vector_ptr_push_back(&res,path_v);
  }

  //Convert an array of vertices to a vector of vertex ids
  igraph_vector_init_int(&to_vidv,0);
  cIGraph_vertex_arr_to_id_vec(self,to,&to_vidv);
  //create vertex selector from the vecotr of ids
  igraph_vs_vector(&to_vids,&to_vidv);

  //The id of the vertex from where we are counting
  from_vid = cIGraph_get_vertex_id(self, from);

  igraph_get_shortest_paths(graph,&res,from_vid,to_vids,pmode);

  for(i=0; i<n_paths; i++){
    path = rb_ary_new();
    rb_ary_push(matrix,path);
    path_v = VECTOR(res)[i];
    for(j=0; j<igraph_vector_size(VECTOR(res)[i]); j++){
      rb_ary_push(path,cIGraph_get_vertex_object(self,VECTOR(*path_v)[j]));
    }
  }

  for(i=0;i<n_paths;i++){
    igraph_vector_destroy(VECTOR(res)[i]);
    free(VECTOR(res)[i]);
  }

  igraph_vector_destroy(&to_vidv);
  igraph_vector_ptr_destroy(&res);
  igraph_vs_destroy(&to_vids);

  return matrix;

}

int igraph_dijkstra_shortest_paths(const igraph_t *graph, 
				   igraph_matrix_t *res, 
				   const igraph_vs_t from, 
				   const igraph_vector_t *wghts,
				   igraph_neimode_t mode) {

  long int no_of_nodes=igraph_vcount(graph);
  long int no_of_from;
  igraph_real_t *shortest;
  igraph_real_t min,alt;

  int i, j, uj, included;
  igraph_integer_t eid, u,v;
  igraph_vector_t q;
  igraph_vit_t fromvit;
  igraph_vector_t neis;

  IGRAPH_CHECK(igraph_vit_create(graph, from, &fromvit));
  IGRAPH_FINALLY(igraph_vit_destroy, &fromvit);

  no_of_from=IGRAPH_VIT_SIZE(fromvit);

  if (mode != IGRAPH_OUT && mode != IGRAPH_IN && 
      mode != IGRAPH_ALL) {
    IGRAPH_ERROR("Invalid mode argument", IGRAPH_EINVMODE);
  }
  shortest=calloc(no_of_nodes, sizeof(igraph_real_t));
  if (shortest==0) {
    IGRAPH_ERROR("shortest paths failed", IGRAPH_ENOMEM);
  }
  IGRAPH_FINALLY(free, shortest);

  IGRAPH_CHECK(igraph_matrix_resize(res, no_of_from, no_of_nodes));
  igraph_matrix_null(res);

  for (IGRAPH_VIT_RESET(fromvit), i=0; 
       !IGRAPH_VIT_END(fromvit); 
       IGRAPH_VIT_NEXT(fromvit), i++) {

     //Start shortest and previous
    for(j=0;j<no_of_nodes;j++){
      shortest[j] = INFINITY;
      //memset(previous,NAN,     no_of_nodes);
    }

    shortest[(int)IGRAPH_VIT_GET(fromvit)] = 0;
    igraph_vector_init_seq(&q,0,no_of_nodes-1);

    while(igraph_vector_size(&q) != 0){

      min = INFINITY;
      u = no_of_nodes;
      uj = igraph_vector_size(&q);
      for(j=0;j<igraph_vector_size(&q);j++){
	v = VECTOR(q)[j];
	if(shortest[(int)v] < min){
	  min = shortest[(int)v];
	  u = v;
	  uj = j;
	}
      }
      
      if(min == INFINITY)
	break;

      igraph_vector_remove(&q,uj);

      igraph_vector_init(&neis,0);
      igraph_neighbors(graph,&neis,u,mode);

      for(j=0;j<igraph_vector_size(&neis);j++){

	v = VECTOR(neis)[j];

	//v must be in Q
	included = 0;
	for(j=0;j<igraph_vector_size(&q);j++){
	  if(v == VECTOR(q)[j]){
	    included = 1;
	    break;
	  }
	}
	
	if(!included)
	  continue;
	
  	igraph_get_eid(graph,&eid,u,v,1);

	alt = shortest[(int)u] + VECTOR(*wghts)[(int)eid];

	if(alt < shortest[(int)v]){
	  shortest[(int)v] = alt;
	}
      }
      igraph_vector_destroy(&neis);
    }

    for(j=0;j<no_of_nodes;j++){
      MATRIX(*res,i,j) = shortest[j];
    }

    igraph_vector_destroy(&q);

  }

  /* Clean */
  free(shortest);
  igraph_vit_destroy(&fromvit);
  IGRAPH_FINALLY_CLEAN(2);

  return 0;
}
