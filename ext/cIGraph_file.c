#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   IGraph.read_graph_edgelist(file,mode) -> IGraph
 *
 *  Reads an edge list from a File (or any IO) and creates a graph.
 *
 * This format is simply a series of even number integers separated by
 * whitespace. The one edge (ie. two integers) per line format is thus not 
 * required (but recommended for readability). Edges of directed graphs are 
 * assumed to be in from, to order. 
 */
VALUE cIGraph_read_graph_edgelist(VALUE self, VALUE file, VALUE directed){

  VALUE string;
  FILE *stream;
  VALUE new_graph;
  VALUE v_ary;
  igraph_t *graph;
  igraph_bool_t directed_b = 0;

  igraph_vs_t vs;
  igraph_vit_t vit;

  int vid;

  if(directed)
    directed_b = 1;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  string = rb_funcall(file, rb_intern("read"), 0);
  stream = fmemopen(RSTRING(string)->ptr,RSTRING(string)->len, "r");

  igraph_read_graph_edgelist(graph, stream, 0, directed_b);

  fclose(stream);

  igraph_vs_all(&vs);
  igraph_vit_create(graph, vs, &vit);

  v_ary =  ((VALUE*)graph->attr)[0];

  while (!IGRAPH_VIT_END(vit)) {
    vid = IGRAPH_VIT_GET(vit);
    rb_ary_push(v_ary,INT2NUM(vid));
    IGRAPH_VIT_NEXT(vit);
  }
  
  igraph_vit_destroy(&vit);
  igraph_vs_destroy(&vs);

  return new_graph;

}

/* call-seq:
 *   graph.write_graph_edgelist(file) -> Integer
 *
 *  Writes an edge list to an IO
 *
 * This format is simply a series of even number integers separated by
 * whitespace. The one edge (ie. two integers) per line format is thus not 
 * required (but recommended for readability). Edges of directed graphs are 
 * assumed to be in from, to order. 
 */
VALUE cIGraph_write_graph_edgelist(VALUE self, VALUE file){

  char *buf;
  size_t size;
  FILE *stream;
  igraph_t *graph;
  int e;

  Data_Get_Struct(self, igraph_t, graph);

  stream = open_memstream(&buf,&size);
  e = igraph_write_graph_edgelist(graph, stream);
  fflush(stream);

  rb_funcall(file, rb_intern("write"), 1, rb_str_new(buf,size));

  fclose(stream);

  return e;

}

/* call-seq:
 *   IGraph.read_graph_graphml(file,index) -> IGraph
 *
 * Reads a graph from a GraphML file specified as the File object file.
 *
 * GraphML is an XML-based file format for representing various types of 
 * graphs. Currently only the most basic import functionality is implemented 
 * in igraph: it can read GraphML files without nested graphs and hyperedges. 
 *
 * If the GraphML file contains more than one graph, the one specified by 
 * this index will be loaded. Indices start from zero, so supply zero here 
 * if your GraphML file contains only a single graph.
 */
VALUE cIGraph_read_graph_graphml(VALUE self, VALUE file, VALUE index){

  VALUE string;
  FILE *stream;
  VALUE new_graph;
  igraph_t *graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  string = rb_funcall(file, rb_intern("read"), 0);
  stream = fmemopen(RSTRING(string)->ptr,RSTRING(string)->len, "r");

  igraph_read_graph_graphml(graph, stream, NUM2INT(index));

  fclose(stream);

  return new_graph;  

}

/* call-seq:
 *   graph.write_graph_graphml(file) -> Integer
 *
 * Writes the graph to a File in GraphML format
 *
 * GraphML is an XML-based file format for representing various types of 
 * graphs. See the GraphML Primer 
 * (http://graphml.graphdrawing.org/primer/graphml-primer.html) for detailed 
 * format description. 
 */
VALUE cIGraph_write_graph_graphml(VALUE self, VALUE file){

  char *buf;
  size_t size;
  FILE *stream;
  igraph_t *graph;
  int e;

  Data_Get_Struct(self, igraph_t, graph);

  stream = open_memstream(&buf,&size);
  e = igraph_write_graph_graphml(graph, stream);
  fflush(stream);

  rb_funcall(file, rb_intern("write"), 1, rb_str_new(buf,size));

  fclose(stream);

  return e;

}

/* call-seq:
 *   IGraph.read_graph_pajek(file) -> IGraph
 *
 * Reads a file in Pajek format
 *
 * Only a subset of the Pajek format is implemented. This is partially 
 * because this format is not very well documented, but also because 
 * igraph does not support some Pajek features, like multigraphs.
 */
VALUE cIGraph_read_graph_pajek(VALUE self, VALUE file){

  VALUE string;
  FILE *stream;
  VALUE new_graph;
  igraph_t *graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  string = rb_funcall(file, rb_intern("read"), 0);
  stream = fmemopen(RSTRING(string)->ptr,RSTRING(string)->len, "r");

  igraph_read_graph_pajek(graph, stream);

  fclose(stream);

  return new_graph;  

}

/* call-seq:
 *   graph.write_graph_pajek(file) -> Integer
 *
 * Writes a graph to a file in Pajek format.
 */
VALUE cIGraph_write_graph_pajek(VALUE self, VALUE file){

  char *buf;
  size_t size;
  FILE *stream;
  igraph_t *graph;
  int e;

  Data_Get_Struct(self, igraph_t, graph);

  stream = open_memstream(&buf,&size);
  e = igraph_write_graph_pajek(graph, stream);
  fflush(stream);

  rb_funcall(file, rb_intern("write"), 1, rb_str_new(buf,size));

  fclose(stream);

  return e;

}
