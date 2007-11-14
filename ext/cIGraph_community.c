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

  igraph_modularity(graph,&membership,&value);
  
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

VALUE cIGraph_community_to_membership(VALUE self, VALUE merge, VALUE steps){

  igraph_t *graph;
  igraph_matrix_t *merges;

  igraph_vector_t membership;

  VALUE groups;

  int i,groupid,max_groupid;

  Data_Get_Struct(self, igraph_t, graph);
  Data_Get_Struct(merge, igraph_matrix_t, merges);

  igraph_vector_init(&membership,0);

  igraph_community_to_membership(graph,merges,NUM2INT(steps),&membership,NULL);

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

  int i,groupid,max_groupid;

  VALUE groups, group, res;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_matrix_init(merges,0,0); 
  igraph_vector_init(&membership,0);

  igraph_community_leading_eigenvector(graph,merges,&membership,
				       NUM2INT(steps));

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

  int i,groupid,max_groupid;

  VALUE groups, group, res;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_matrix_init(merges,0,0); 
  igraph_vector_init(&membership,0);

  igraph_community_leading_eigenvector_naive(graph,merges,&membership,
					     NUM2INT(steps));

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

  int i,j,groupid,max_groupid;

  VALUE groups, group, res, eigenvector_a;

  VALUE str;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_init(&membership_vec,igraph_vcount(graph));
  
  for(i=0;i<RARRAY(membership)->len;i++){
    group = RARRAY(membership)->ptr[i];

    str = rb_funcall(group,rb_intern("inspect"),0);
    printf("obj: %s\n",RSTRING(str)->ptr);

    for(j=0;j<RARRAY(group)->len;j++){

      str =  rb_funcall(RARRAY(group)->ptr[j],rb_intern("inspect"),0);
      printf("obj: %s\n",RSTRING(str)->ptr);

      igraph_vector_set(&membership_vec,
			cIGraph_get_vertex_id(self,
					      RARRAY(group)->ptr[j]),i);
    }
  }

  printf("Get here?\n");

  for(i=0;i<igraph_vector_size(&membership_vec);i++){
    printf("%i: %i\n",i,VECTOR(membership_vec)[i]);
  }
  printf("c: %i\n",NUM2INT(community));

  igraph_community_leading_eigenvector_step(graph,&membership_vec,
					    NUM2INT(community),
					    &split,&eigenvector,&eigenvalue);

  printf("Get here\n");

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


