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
