#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

VALUE cIGraph_grg_game(VALUE self, VALUE nodes, VALUE radius, VALUE torus){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_grg_game(graph, NUM2INT(nodes), NUM2DBL(radius), 
		  torus == Qtrue ? 1: 0);

  return new_graph;

}

VALUE cIGraph_barabasi_game(VALUE self, VALUE nodes, VALUE m, VALUE outpref, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_barabasi_game(graph, NUM2INT(nodes), NUM2INT(m), NULL, 
		       outpref == Qtrue ? 1: 0, directed == Qtrue ? 1: 0);

  return new_graph;

}

VALUE cIGraph_nonlinear_barabasi_game(VALUE self, VALUE nodes, VALUE power, VALUE m, VALUE outpref, VALUE zeroappeal, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_nonlinear_barabasi_game(graph, NUM2INT(nodes), NUM2DBL(power),
				  NUM2INT(m), NULL, 
				  outpref == Qtrue ? 1: 0,
				  NUM2DBL(zeroappeal),
				  directed == Qtrue ? 1: 0);

  return new_graph;

}

VALUE cIGraph_erdos_renyi_game(VALUE self, VALUE type, VALUE nodes, VALUE mp, VALUE directed, VALUE loops){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_erdos_renyi_game(graph, NUM2INT(type), NUM2INT(nodes),
			  NUM2DBL(mp), 
			  directed == Qtrue ? 1: 0,
			  loops == Qtrue ? 1: 0);

  return new_graph;

}

VALUE cIGraph_watts_strogatz_game(VALUE self, VALUE dim, VALUE size, VALUE nei, VALUE p){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_watts_strogatz_game(graph, NUM2INT(dim), NUM2INT(size),
			     NUM2INT(nei), NUM2DBL(p));

  return new_graph;

}

VALUE cIGraph_degree_sequence_game(VALUE self, VALUE out_deg, VALUE in_deg){

  igraph_t *graph;
  VALUE new_graph;
  igraph_vector_t out_degv;
  igraph_vector_t in_degv;
  int i;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_vector_init(&out_degv,0);
  igraph_vector_init(&in_degv,0);

  for(i=0;i<RARRAY(out_deg)->len;i++){
    igraph_vector_push_back(&out_degv,NUM2INT(RARRAY(out_deg)->ptr[i]));
  }
  for(i=0;i<RARRAY(in_deg)->len;i++){
    igraph_vector_push_back(&in_degv,NUM2INT(RARRAY(in_deg)->ptr[i]));
  }  

  igraph_destroy(graph);
  igraph_degree_sequence_game(graph, &out_degv, &in_degv, 0);

  igraph_vector_destroy(&out_degv);
  igraph_vector_destroy(&in_degv);

  return new_graph;

}

VALUE cIGraph_growing_random_game(VALUE self, VALUE n, VALUE m, VALUE directed, VALUE citation){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_growing_random_game(graph, NUM2INT(n), NUM2INT(m),
				 directed == Qtrue ? 1: 0,
				 citation == Qtrue ? 1: 0);

  return new_graph;

}

VALUE cIGraph_callaway_traits_game(VALUE self, VALUE nodes, VALUE types, VALUE e_per_step, VALUE type_dist, VALUE pref_matrix, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;
  igraph_vector_t type_distv;
  igraph_matrix_t *pref_matrixm;
  int i;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  Data_Get_Struct(pref_matrix, igraph_matrix_t, pref_matrixm);

  igraph_vector_init(&type_distv,0);

  for(i=0;i<RARRAY(type_dist)->len;i++){
    igraph_vector_push_back(&type_distv,NUM2DBL(RARRAY(type_dist)->ptr[i]));
  }
  
  igraph_destroy(graph);
  igraph_callaway_traits_game(graph, NUM2INT(nodes), NUM2INT(types),
			      NUM2INT(e_per_step), &type_distv,
			      pref_matrixm,
			      directed == Qtrue ? 1: 0);

  igraph_vector_destroy(&type_distv);

  return new_graph;
  

}

VALUE cIGraph_establishment_game(VALUE self, VALUE nodes, VALUE types, VALUE k, VALUE type_dist, VALUE pref_matrix, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;
  igraph_vector_t type_distv;
  igraph_matrix_t *pref_matrixm;
  int i;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  Data_Get_Struct(pref_matrix, igraph_matrix_t, pref_matrixm);

  igraph_vector_init(&type_distv,0);

  for(i=0;i<RARRAY(type_dist)->len;i++){
    igraph_vector_push_back(&type_distv,NUM2DBL(RARRAY(type_dist)->ptr[i]));
  }
  
  igraph_destroy(graph);
  igraph_establishment_game(graph, NUM2INT(nodes), NUM2INT(types),
			      NUM2INT(k), &type_distv,
			      pref_matrixm,
			      directed == Qtrue ? 1: 0);

  igraph_vector_destroy(&type_distv);

  return new_graph;
  

}

VALUE cIGraph_preference_game(VALUE self, VALUE nodes, VALUE types, VALUE type_dist, VALUE pref_matrix, VALUE directed, VALUE loops){

  igraph_t *graph;
  VALUE new_graph;
  igraph_vector_t type_distv;
  igraph_matrix_t *pref_matrixm;
  int i;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  Data_Get_Struct(pref_matrix, igraph_matrix_t, pref_matrixm);

  igraph_vector_init(&type_distv,0);

  for(i=0;i<RARRAY(type_dist)->len;i++){
    igraph_vector_push_back(&type_distv,NUM2DBL(RARRAY(type_dist)->ptr[i]));
  }
  
  igraph_destroy(graph);
  igraph_preference_game(graph, NUM2INT(nodes), NUM2INT(types),
			    &type_distv,
			    pref_matrixm,
			    NULL,
			    directed == Qtrue ? 1: 0,
			    loops == Qtrue ? 1 : 0);

  igraph_vector_destroy(&type_distv);

  return new_graph;
  

}

VALUE cIGraph_asymmetric_preference_game(VALUE self, VALUE nodes, VALUE types, VALUE type_dist_matrix, VALUE pref_matrix, VALUE loops){

  igraph_t *graph;
  VALUE new_graph;
  igraph_matrix_t *type_dist_matrixm;
  igraph_matrix_t *pref_matrixm;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  Data_Get_Struct(pref_matrix, igraph_matrix_t, pref_matrixm);
  Data_Get_Struct(type_dist_matrix, igraph_matrix_t, type_dist_matrixm);

  igraph_destroy(graph);
  igraph_asymmetric_preference_game(graph, NUM2INT(nodes), NUM2INT(types),
				    type_dist_matrixm,
				    pref_matrixm,
				    NULL, NULL,
				    loops == Qtrue ? 1 : 0);

  return new_graph;

}

VALUE cIGraph_recent_degree_game(VALUE self, VALUE n, VALUE power, VALUE window, VALUE m, VALUE outpref, VALUE zero_appeal, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_recent_degree_game(graph, NUM2INT(n), NUM2DBL(power),
			    NUM2INT(window), NUM2INT(m),
			    NULL,
			    outpref == Qtrue ? 1: 0,
			    NUM2DBL(zero_appeal),
			    directed == Qtrue ? 1: 0);

  return new_graph;

}

VALUE cIGraph_barabasi_aging_game(VALUE self, VALUE nodes, VALUE m, VALUE outpref, VALUE pa_exp, VALUE aging_exp, VALUE aging_bin, VALUE zero_deg_appeal, VALUE zero_age_appeal, VALUE deg_coef, VALUE age_coef, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_barabasi_aging_game(graph, NUM2INT(nodes), NUM2INT(m),
			     NULL,
			     outpref == Qtrue ? 1: 0,
			     NUM2DBL(pa_exp), NUM2DBL(aging_exp),
			     NUM2INT(aging_bin),
			     NUM2DBL(zero_deg_appeal), 
			     NUM2DBL(zero_age_appeal),
			     NUM2DBL(deg_coef),
			     NUM2DBL(age_coef),
			     directed == Qtrue ? 1: 0);

  return new_graph;

}

VALUE cIGraph_recent_degree_aging_game(VALUE self, VALUE nodes, VALUE m, VALUE outpref, VALUE pa_exp, VALUE aging_exp, VALUE aging_bin, VALUE time_window, VALUE zero_appeal, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_recent_degree_aging_game(graph, NUM2INT(nodes), NUM2INT(m),
				  NULL,
				  outpref == Qtrue ? 1: 0,
				  NUM2DBL(pa_exp), NUM2DBL(aging_exp),
				  NUM2INT(aging_bin),
				  NUM2INT(time_window),
				  NUM2DBL(zero_appeal),
				  directed == Qtrue ? 1: 0);

  return new_graph;

}

VALUE cIGraph_cited_type_game(VALUE self, VALUE nodes, VALUE types, VALUE pref, VALUE e_per_s, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;
  igraph_vector_t type_distv;
  igraph_vector_t prefv;
  int i;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_vector_init(&type_distv,0);
  igraph_vector_init(&prefv,0);

  for(i=0;i<RARRAY(types)->len;i++){
    igraph_vector_push_back(&type_distv,NUM2DBL(RARRAY(types)->ptr[i]));
  }
  for(i=0;i<RARRAY(pref)->len;i++){
    igraph_vector_push_back(&prefv,NUM2DBL(RARRAY(pref)->ptr[i]));
  }
    
  igraph_destroy(graph);
  igraph_cited_type_game(graph, NUM2INT(nodes),
			 &type_distv,
			 &prefv,
			 NUM2INT(e_per_s),
			 directed == Qtrue ? 1: 0);

  igraph_vector_destroy(&type_distv);
  igraph_vector_destroy(&prefv);

  return new_graph;
  

}

VALUE cIGraph_citing_cited_type_game(VALUE self, VALUE nodes, VALUE types, VALUE pref, VALUE e_per_s, VALUE directed){

  igraph_t *graph;
  VALUE new_graph;
  igraph_vector_t typev;
  igraph_matrix_t *prefm;
  int i;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);
  Data_Get_Struct(pref, igraph_matrix_t, prefm);

  igraph_vector_init(&typev,0);

  for(i=0;i<RARRAY(types)->len;i++){
    igraph_vector_push_back(&typev,NUM2INT(RARRAY(types)->ptr[i]));
  }

  printf("ok\n");
    
  igraph_destroy(graph);
  igraph_citing_cited_type_game(graph, NUM2INT(nodes),
				&typev,
				prefm,
				NUM2INT(e_per_s),
				directed == Qtrue ? 1: 0);

  printf("death\n");

  igraph_vector_destroy(&typev);

  return new_graph;
  

}
