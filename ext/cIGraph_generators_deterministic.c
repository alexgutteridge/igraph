#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

VALUE cIGraph_adjacency(VALUE self, VALUE matrix, VALUE mode){

  igraph_t *graph;
  igraph_matrix_t *matrixp;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  Data_Get_Struct(matrix, igraph_matrix_t, matrixp);

  igraph_destroy(graph);
  igraph_adjacency(graph, matrixp, NUM2INT(mode));

  return new_graph;

}

VALUE cIGraph_star(VALUE self, VALUE n, VALUE mode, VALUE center){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_star(graph, NUM2INT(n), NUM2INT(mode), NUM2INT(center));

  return new_graph;  

}

VALUE cIGraph_lattice(VALUE self, VALUE dim, VALUE directed, VALUE mutual, VALUE circular){

  igraph_t *graph;
  VALUE new_graph;
  igraph_vector_t dimvector;
  int i;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_vector_init(&dimvector,0);
  for(i=0; i<RARRAY(dim)->len; i++){
    igraph_vector_push_back(&dimvector,NUM2INT(RARRAY(dim)->ptr[i])); 
  }

  igraph_destroy(graph);
  igraph_lattice(graph, &dimvector, 0, 
		 directed == Qtrue ? 1 : 0, 
		 mutual   == Qtrue ? 1 : 0, 
		 circular == Qtrue ? 1 : 0);

  igraph_vector_destroy(&dimvector);

  return new_graph;  

}

VALUE cIGraph_ring(VALUE self, VALUE n, VALUE directed, VALUE mutual, VALUE circular){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_ring(graph, NUM2INT(n), 
	      directed == Qtrue ? 1 : 0, 
	      mutual   == Qtrue ? 1 : 0, 
	      circular == Qtrue ? 1 : 0);

  return new_graph;  

}

VALUE cIGraph_tree(VALUE self, VALUE n, VALUE children, VALUE type){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_tree(graph, NUM2INT(n), NUM2INT(children), NUM2INT(type));

  return new_graph;  

}

VALUE cIGraph_full(VALUE self, VALUE n, VALUE directed, VALUE loops){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_full(graph, NUM2INT(n), 
	      directed == Qtrue ? 1 : 0, 
	      loops    == Qtrue ? 1 : 0);

  return new_graph;  

}

VALUE cIGraph_atlas(VALUE self, VALUE n){

  igraph_t *graph;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_destroy(graph);
  igraph_atlas(graph, NUM2INT(n));

  return new_graph;  

}

VALUE cIGraph_extended_chordal_ring(VALUE self, VALUE n, VALUE matrix){

  igraph_t *graph;
  igraph_matrix_t *matrixp;
  VALUE new_graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  Data_Get_Struct(matrix, igraph_matrix_t, matrixp);

  igraph_destroy(graph);
  igraph_extended_chordal_ring(graph, NUM2INT(n), matrixp);

  return new_graph;

}

VALUE cIGraph_connect_neighborhood(VALUE self, VALUE order, VALUE mode){

  igraph_t *graph;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_connect_neighborhood(graph, NUM2INT(order), NUM2INT(mode));

  return Qnil;  

}
