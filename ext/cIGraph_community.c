#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph.modularity(groups) -> Float
 *
 * Calculate the modularity of a graph with respect to some vertex types. 
 * The modularity of a graph with respect to some division (or vertex types) 
 * measures how good the division is, or how separated are the different 
 * vertex types from each other.
 *
 */

VALUE cIGraph_modularity(VALUE self, VALUE groups){

  igraph_t *graph;
  igraph_real_t value;
  igraph_vector_t membership;

  VALUE group;

  int i,j;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_init(&membership,igraph_vcount(graph));

  for(i=0;i<RARRAY(groups)->len;i++){
    group = RARRAY(groups)->ptr[i];
    for(j=0;j<RARRAY(group)->len;j++){
      igraph_vector_set(&membership,
			cIGraph_get_vertex_id(self,RARRAY(group)->ptr[j]),i);
    }
  }

  igraph_modularity(graph,&membership,&value,NULL);
  
  igraph_vector_destroy(&membership);

  return rb_float_new(value);

}

/* call-seq:
 *   graph.community_to_membership(merge,steps) -> Array
 *
 * Create membership vector from community structure dendrogram This function 
 * creates a membership vector from a community structure dendrogram. 
 * A membership vector contains for each vertex the id of its graph 
 * component, the graph components are numbered from zero, see the same 
 * argument of igraph_clusters() for an example of a membership vector.
 *
 */

VALUE cIGraph_community_to_membership(VALUE self, VALUE merge, VALUE steps, VALUE nodes){

  igraph_t *graph;
  igraph_matrix_t *merges;

  igraph_vector_t membership;

  VALUE groups;

  int i,groupid,max_groupid;

  Data_Get_Struct(self, igraph_t, graph);
  Data_Get_Struct(merge, igraph_matrix_t, merges);

  igraph_vector_init(&membership,0);

  igraph_community_to_membership(merges,NUM2INT(nodes),NUM2INT(steps),&membership,NULL);

  max_groupid = 0;
  for(i=0;i<igraph_vector_size(&membership);i++){
    if(VECTOR(membership)[i] > max_groupid)
      max_groupid = VECTOR(membership)[i];
  }
    
  groups = rb_ary_new();
  for(i=0;i<max_groupid+1;i++){
    rb_ary_push(groups,rb_ary_new());
  }

  for(i=0;i<igraph_vector_size(&membership);i++){
    groupid = VECTOR(membership)[i];
    if(RARRAY(groups)->ptr[groupid] == Qnil){
      RARRAY(groups)->ptr[groupid] = rb_ary_new();
    }
    rb_ary_push(RARRAY(groups)->ptr[groupid],
		cIGraph_get_vertex_object(self, i));
  }

  igraph_vector_destroy(&membership);

  return groups;

}

/* call-seq:
 *   graph.community_spinglass(weights,spins,parupdate,starttemp,stoptemp,coolfact,update_rule,gamma) -> Array
 *
 * Community detection based on statistical mechanics This function 
 * implements the community structure detection algorithm proposed by Joerg 
 * Reichardt and Stefan Bornholdt. The algorithm is described in their 
 * paper: Statistical Mechanics of Community Detection.
 *
 */

VALUE cIGraph_community_spinglass(VALUE self, VALUE weights, VALUE spins, VALUE parupdate, VALUE starttemp, VALUE stoptemp, VALUE coolfact, VALUE update_rule, VALUE gamma){

  igraph_t *graph;

  igraph_vector_t weights_vec;
  igraph_vector_t membership;
  igraph_real_t temperature;
  igraph_real_t modularity;
  igraph_bool_t parupdate_b;

  VALUE group;
  VALUE groups;
  VALUE res;

  VALUE str;

  int i,groupid,max_groupid;

  if(parupdate)
    parupdate_b = 1;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_init(&membership,0);

  igraph_vector_init(&weights_vec,RARRAY(weights)->len);
  for(i=0;i<RARRAY(weights)->len;i++){
    VECTOR(weights_vec)[i] = NUM2DBL(RARRAY(weights)->ptr[i]);
  }

  igraph_community_spinglass(graph,
			     igraph_vector_size(&weights_vec) > 0 ? &weights_vec : NULL,
			     &modularity,&temperature,
			     &membership,NULL,NUM2INT(spins),parupdate_b,
			     NUM2DBL(starttemp),NUM2DBL(stoptemp),
			     NUM2DBL(coolfact),NUM2INT(update_rule),
			     NUM2DBL(gamma));
  
  max_groupid = 0;
  for(i=0;i<igraph_vector_size(&membership);i++){
    if(VECTOR(membership)[i] > max_groupid)
      max_groupid = VECTOR(membership)[i];
  }
    
  groups = rb_ary_new();
  for(i=0;i<max_groupid+1;i++){
    rb_ary_push(groups,rb_ary_new());
  }

  for(i=0;i<igraph_vector_size(&membership);i++){

    groupid = VECTOR(membership)[i];

    if(groupid == -1)
      groupid = 0;

    group = RARRAY(groups)->ptr[groupid];
    rb_ary_push(group,cIGraph_get_vertex_object(self, i));
  }

  res = rb_ary_new3(3,groups,
		    rb_float_new(modularity),
		    rb_float_new(temperature));

  igraph_vector_destroy(&membership);
  igraph_vector_destroy(&weights_vec);

  return res;

}

/* call-seq:
 *   graph.community_spinglass_single(weights,vertex,spins,update_rule,gamma) -> Array
 *
 * Community detection based on statistical mechanics This function 
 * implements the community structure detection algorithm proposed by Joerg 
 * Reichardt and Stefan Bornholdt. The algorithm is described in their 
 * paper: Statistical Mechanics of Community Detection.
 *
 */

VALUE cIGraph_community_spinglass_single(VALUE self, VALUE weights, VALUE vertex, VALUE spins, VALUE update_rule, VALUE gamma){

  igraph_t *graph;

  igraph_vector_t weights_vec;
  igraph_vector_t community;
  igraph_real_t cohesion;
  igraph_real_t adhesion;

  VALUE group;
  VALUE res;

  int i;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_init(&community,0);

  igraph_vector_init(&weights_vec,RARRAY(weights)->len);
  for(i=0;i<RARRAY(weights)->len;i++){
    VECTOR(weights_vec)[i] = NUM2DBL(RARRAY(weights)->ptr[i]);
  }

  igraph_community_spinglass_single(graph,
				    igraph_vector_size(&weights_vec) > 0 ? &weights_vec : NULL,
				    cIGraph_get_vertex_id(self, vertex),   
				    &community, &cohesion, &adhesion,
				    NULL, NULL,
				    NUM2INT(spins),NUM2INT(update_rule),
				    NUM2DBL(gamma));
  
  group = rb_ary_new();

  for(i=0;i<igraph_vector_size(&community);i++){
    rb_ary_push(group,cIGraph_get_vertex_object(self, i));
  }

  res = rb_ary_new3(3,group,
		    rb_float_new(cohesion),
		    rb_float_new(adhesion));

  igraph_vector_destroy(&community);
  igraph_vector_destroy(&weights_vec);

  return res;

}

/* call-seq:
 *   graph.community_leading_eigenvector(steps) -> Array
 *
 * Newman's leading eigenvector method for detecting community structure. 
 * This is the proper implementation of the recursive, divisive algorithm: 
 * each split is done by maximizing the modularity regarding the original 
 * network, see MEJ Newman: Finding community structure in networks using 
 * the eigenvectors of matrices, arXiv:physics/0605087.
 *
 */

VALUE cIGraph_community_leading_eigenvector(VALUE self, VALUE steps){

  igraph_t *graph;

  igraph_vector_t membership;
  igraph_matrix_t *merges = malloc(sizeof(igraph_matrix_t));
  
igraph_arpack_options_t arpack_opt;
igraph_arpack_options_init(&arpack_opt);

  int i,groupid,max_groupid;

  VALUE groups, group, res;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_matrix_init(merges,0,0); 
  igraph_vector_init(&membership,0);

  igraph_community_leading_eigenvector(graph,merges,&membership,
				       NUM2INT(steps),&arpack_opt);

  max_groupid = 0;
  for(i=0;i<igraph_vector_size(&membership);i++){
    if(VECTOR(membership)[i] > max_groupid)
      max_groupid = VECTOR(membership)[i];
  }

  groups = rb_ary_new();
  for(i=0;i<max_groupid+1;i++){
    rb_ary_push(groups,rb_ary_new());
  }

  for(i=0;i<igraph_vector_size(&membership);i++){

    groupid = VECTOR(membership)[i];

    if(groupid == -1)
      groupid = 0;

    group = RARRAY(groups)->ptr[groupid];
    rb_ary_push(group,cIGraph_get_vertex_object(self, i));
  }

  res = rb_ary_new3(2,groups,
		    Data_Wrap_Struct(cIGraphMatrix, 0, 
				     cIGraph_matrix_free, merges));

  igraph_vector_destroy(&membership);

  return res;

}

/* call-seq:
 *   graph.community_leading_eigenvector_naive(steps) -> Array
 *
 * Newman's leading eigenvector method for detecting community structure. 
 * This is the proper implementation of the recursive, divisive algorithm: 
 * each split is done by maximizing the modularity regarding the original 
 * network, see MEJ Newman: Finding community structure in networks using 
 * the eigenvectors of matrices, arXiv:physics/0605087.
 *
 */

VALUE cIGraph_community_leading_eigenvector_naive(VALUE self, VALUE steps){

  igraph_t *graph;

  igraph_vector_t membership;
  igraph_matrix_t *merges = malloc(sizeof(igraph_matrix_t));

igraph_arpack_options_t arpack_opt;
igraph_arpack_options_init(&arpack_opt);

  int i,groupid,max_groupid;

  VALUE groups, group, res;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_matrix_init(merges,0,0); 
  igraph_vector_init(&membership,0);

  igraph_community_leading_eigenvector_naive(graph,merges,&membership,
					     NUM2INT(steps), &arpack_opt);

  max_groupid = 0;
  for(i=0;i<igraph_vector_size(&membership);i++){
    if(VECTOR(membership)[i] > max_groupid)
      max_groupid = VECTOR(membership)[i];
  }

  groups = rb_ary_new();
  for(i=0;i<max_groupid+1;i++){
    rb_ary_push(groups,rb_ary_new());
  }

  for(i=0;i<igraph_vector_size(&membership);i++){

    groupid = VECTOR(membership)[i];

    if(groupid == -1)
      groupid = 0;

    group = RARRAY(groups)->ptr[groupid];
    rb_ary_push(group,cIGraph_get_vertex_object(self, i));
  }

  res = rb_ary_new3(2,groups,
		    Data_Wrap_Struct(cIGraphMatrix, 0, 
				     cIGraph_matrix_free, merges));

  igraph_vector_destroy(&membership);

  return res;

}

/* call-seq:
 *   graph.community_leading_eigenvector_step(membership,community) -> Array
 *
 * Do one split according to Mark Newman's leading eigenvector community 
 * detection method. See MEJ Newman: Finding community structure in networks 
 * using the eigenvectors of matrices, arXiv:phyisics/0605087 for the details.
 *
 */

VALUE cIGraph_community_leading_eigenvector_step(VALUE self, VALUE membership, VALUE community){

  igraph_t *graph;

  igraph_vector_t membership_vec;
  igraph_vector_t eigenvector;
  igraph_real_t eigenvalue;
  igraph_bool_t split;

  int i,j,groupid,max_groupid,vid;

igraph_arpack_options_t arpack_opt;
igraph_arpack_options_init(&arpack_opt);

  VALUE groups, group, res, eigenvector_a, obj;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_init(&membership_vec,igraph_vcount(graph));
  igraph_vector_init(&eigenvector,0);

  for(i=0;i<RARRAY(membership)->len;i++){
    group = RARRAY(membership)->ptr[i];

    for(j=0;j<RARRAY(group)->len;j++){

      obj = RARRAY(group)->ptr[j];
      vid = cIGraph_get_vertex_id(self,obj);

      VECTOR(membership_vec)[vid] = i;

    }
  }

  igraph_community_leading_eigenvector_step(graph,&membership_vec,
					    NUM2INT(community),
					    &split,&eigenvector,&eigenvalue,&arpack_opt,NULL);

  max_groupid = 0;
  for(i=0;i<igraph_vector_size(&membership_vec);i++){
    if(VECTOR(membership_vec)[i] > max_groupid)
      max_groupid = VECTOR(membership_vec)[i];
  }

  groups = rb_ary_new();
  for(i=0;i<max_groupid+1;i++){
    rb_ary_push(groups,rb_ary_new());
  }

  for(i=0;i<igraph_vector_size(&membership_vec);i++){

    groupid = VECTOR(membership_vec)[i];

    if(groupid == -1)
      groupid = 0;

    group = RARRAY(groups)->ptr[groupid];
    rb_ary_push(group,cIGraph_get_vertex_object(self, i));
  }

  eigenvector_a = rb_ary_new();
  for(i=0;i<igraph_vector_size(&eigenvector);i++){
    rb_ary_push(eigenvector_a,rb_float_new(VECTOR(eigenvector)[i]));
  }

  res = rb_ary_new3(4,groups,split==0 ? Qfalse : Qtrue,
		    eigenvector_a,rb_float_new(eigenvalue));


  igraph_vector_destroy(&membership_vec);
  igraph_vector_destroy(&eigenvector);

  return res;

}

/* call-seq:
 *   graph.community_walktrap(weights,steps) -> Array
 *
 * This function is the implementation of the Walktrap community finding 
 * algorithm, see Pascal Pons, Matthieu Latapy: Computing communities in 
 * large networks using random walks, http://arxiv.org/abs/physics/0512106
 *
 */

VALUE cIGraph_community_walktrap(VALUE self, VALUE weights, VALUE steps){

  igraph_t *graph;

  igraph_vector_t weights_vec;
  igraph_vector_t modularity;
  igraph_matrix_t *merges = malloc(sizeof(igraph_matrix_t));

  int i;

  VALUE modularity_a, res;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_matrix_init(merges,0,0);
  igraph_vector_init(&weights_vec,0);
  igraph_vector_init(&modularity,0);

  for(i=0;i<RARRAY(weights)->len;i++){
    VECTOR(weights_vec)[i] = NUM2DBL(RARRAY(weights)->ptr[i]);
  }

  igraph_community_walktrap(graph,
			    igraph_vector_size(&weights_vec) > 0 ? &weights_vec : NULL,
			    NUM2INT(steps),merges,&modularity);

  modularity_a = rb_ary_new();
  for(i=0;i<igraph_vector_size(&modularity);i++){
    rb_ary_push(modularity_a,rb_float_new(VECTOR(modularity)[i]));
  }

  res = rb_ary_new3(2,
		    Data_Wrap_Struct(cIGraphMatrix, 0, 
				     cIGraph_matrix_free, merges),
		    modularity_a);

  igraph_vector_destroy(&weights_vec);
  igraph_vector_destroy(&modularity);

  return res;

}

/* call-seq:
 *   graph.community_edge_betweenness(directed) -> Array
 *
 * Community structure detection based on the betweenness of the edges in the 
 * network. The algorithm was invented by M. Girvan and M. Newman, see: 
 * M. Girvan and M. E. J. Newman: Community structure in social and 
 * biological networks, Proc. Nat. Acad. Sci. USA 99, 7821-7826 (2002).
 *
 */

VALUE cIGraph_community_edge_betweenness(VALUE self, VALUE directed){

  igraph_t *graph;

  igraph_vector_t result_vec;
  igraph_vector_t edge_betw_vec;
  igraph_vector_t bridges_vec;
  igraph_matrix_t *merges = malloc(sizeof(igraph_matrix_t));
  igraph_bool_t   directed_b = 0;

  int i;

  VALUE result_a, edge_betw_a, bridges_a, res;

  if(directed)
    directed_b = 1;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_matrix_init(merges,0,0);
  igraph_vector_init(&result_vec,0);
  igraph_vector_init(&edge_betw_vec,0);
  igraph_vector_init(&bridges_vec,0);

  igraph_community_edge_betweenness(graph,
				    &result_vec,&edge_betw_vec,
				    merges,&bridges_vec,directed_b);

  result_a = rb_ary_new();
  for(i=0;i<igraph_vector_size(&result_vec);i++){
    rb_ary_push(result_a,INT2NUM(VECTOR(result_vec)[i]));
  }
  edge_betw_a = rb_ary_new();
  for(i=0;i<igraph_vector_size(&edge_betw_vec);i++){
    rb_ary_push(edge_betw_a,INT2NUM(VECTOR(edge_betw_vec)[i]));
  }
  bridges_a = rb_ary_new();
  for(i=0;i<igraph_vector_size(&bridges_vec);i++){
    rb_ary_push(bridges_a,INT2NUM(VECTOR(bridges_vec)[i]));
  }

  res = rb_ary_new3(4,
		    Data_Wrap_Struct(cIGraphMatrix, 0, 
				     cIGraph_matrix_free, merges),
		    result_a, edge_betw_a, bridges_a);

  igraph_vector_destroy(&result_vec);
  igraph_vector_destroy(&edge_betw_vec);
  igraph_vector_destroy(&bridges_vec);

  return res;

}

/* call-seq:
 *   graph.community_eb_get_merges(edges) -> Array
 *
 * Calculating the merges, ie. the dendrogram for an edge betweenness 
 * community structure
 *
 */

VALUE cIGraph_community_eb_get_merges(VALUE self, VALUE edges){

  igraph_t *graph;
  igraph_matrix_t *res = malloc(sizeof(igraph_matrix_t));

  igraph_vector_t edges_vec;
  igraph_vector_t bridges_vec;

  VALUE result,bridges_a;

  int i;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_matrix_init(res,0,0);
  igraph_vector_init(&edges_vec,0);
  igraph_vector_init(&bridges_vec,0);

  for(i=0;i<RARRAY(edges)->len;i++){
    igraph_vector_push_back(&edges_vec,NUM2INT(RARRAY(edges)->ptr[i]));
  }

  igraph_community_eb_get_merges(graph,&edges_vec,res,&bridges_vec);

  bridges_a = rb_ary_new();
  for(i=0;i<igraph_vector_size(&bridges_vec);i++){
    rb_ary_push(bridges_a,INT2NUM(VECTOR(bridges_vec)[i]));
  }

  igraph_vector_destroy(&bridges_vec);
  igraph_vector_destroy(&edges_vec);

  result = rb_ary_new3(2,
		       Data_Wrap_Struct(cIGraphMatrix, 0, 
					cIGraph_matrix_free, res),
		       bridges_a);

  return result;

}

/* call-seq:
 *   graph.community_fastgreedy() -> Array
 *
 * Finding community structure by greedy optimization of modularity. 
 * This function implements the fast greedy modularity optimization algorithm 
 * for finding community structure, see A Clauset, MEJ Newman, C Moore: 
 * Finding community structure in very large networks, 
 * http://www.arxiv.org/abs/cond-mat/0408187 for the details.
 *
 */

VALUE cIGraph_community_fastgreedy(VALUE self){

  igraph_t *graph;

  igraph_vector_t modularity;
  igraph_matrix_t *merges = malloc(sizeof(igraph_matrix_t));

  int i;

  VALUE modularity_a, res;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_matrix_init(merges,0,0);
  igraph_vector_init(&modularity,0);

  igraph_community_fastgreedy(graph,NULL,
			      merges,&modularity);

  modularity_a = rb_ary_new();
  for(i=0;i<igraph_vector_size(&modularity);i++){
    rb_ary_push(modularity_a,rb_float_new(VECTOR(modularity)[i]));
  }

  res = rb_ary_new3(2,
		    Data_Wrap_Struct(cIGraphMatrix, 0, 
				     cIGraph_matrix_free, merges),
		    modularity_a);

  igraph_vector_destroy(&modularity);

  return res;

}
