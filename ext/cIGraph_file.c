#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

#ifdef __APPLE__
#else
/* call-seq:
 *   IGraph::FileRead.read_graph_edgelist(file,mode) -> IGraph
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
 *   IGraph::FileRead.read_graph_ncol(file,predefnames,names,weights,directed) -> IGraph
 *
 * Reads a .ncol file used by LGL, also useful for creating graphs from 
 * 'named' (and optionally weighted) edge lists.
 *
 * This format is used by the Large Graph Layout program 
 * (http://bioinformatics.icmb.utexas.edu/lgl/), and it is simply a symbolic 
 * weighted edge list. It is a simple text file with one edge per line. An 
 * edge is defined by two symbolic vertex names separated by whitespace. 
 * (The symbolic vertex names themselves cannot contain whitespace. They 
 * might follow by an optional number, this will be the weight of the edge; 
 * the number can be negative and can be in scientific notation. If there is 
 * no weight specified to an edge it is assumed to be zero.
 *
 * The resulting graph is always undirected. LGL cannot deal with files which 
 * contain multiple or loop edges, this is however not checked here, as 
 * igraph is happy with these. 
 *
 * file: A File or IO object to read from.
 *
 * predefnames: Array of the symbolic names of the vertices in the file.
 * If empty then vertex ids will be assigned to vertex names in the order of 
 * their appearence in the .ncol file.
 *
 * names: Logical value, if TRUE the symbolic names of the vertices will be 
 * added to the graph as a vertex attribute called 'name'.
 *
 * weights: Logical value, if TRUE the weights of the edges is added to the 
 * graph as an edge attribute called 'weight'.
 *
 * directed: Whether to create a directed graph. As this format was originally
 * used only for undirected graphs there is no information in the file about 
 * the directedness of the graph. Set this parameter to IGRAPH_DIRECTED or 
 * IGRAPH_UNDIRECTED to create a directed or undirected graph. 
 */
VALUE cIGraph_read_graph_ncol(VALUE self, VALUE file, VALUE predefnames, VALUE names, VALUE weights, VALUE directed){

  VALUE string;
  FILE *stream;
  VALUE new_graph;
  VALUE v_ary;
  VALUE e_ary;
  VALUE new_ary;

  igraph_t *graph;
  igraph_strvector_t names_vec;
  igraph_bool_t directed_b = 0;
  igraph_bool_t weights_b  = 0;
  igraph_bool_t names_b    = 0;

  int i;

  if(directed)
    directed_b = 1;

  if(names)
    names_b = 1;

  if(weights)
    weights_b = 1;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  igraph_strvector_init(&names_vec,RARRAY(predefnames)->len);

  for(i=0;i<RARRAY(predefnames)->len;i++){
    igraph_strvector_set(&names_vec, i, RSTRING(RARRAY(predefnames)->ptr[i])->ptr);
  }

  string = rb_funcall(file, rb_intern("read"), 0);
  stream = fmemopen(RSTRING(string)->ptr,RSTRING(string)->len, "r");

  if (RARRAY(predefnames)->len == 0){
    igraph_read_graph_ncol(graph, stream, NULL, names_b, weights_b, directed_b);
  } else {
    igraph_read_graph_ncol(graph, stream, &names_vec, names_b, weights_b, directed_b);
  }

  fclose(stream);

  //Convert the Hash of names to Strings instead
  if(names){
    v_ary = ((VALUE*)graph->attr)[0];
    new_ary = rb_ary_new();
    for(i=0;i<RARRAY(v_ary)->len;i++){
      rb_ary_push(new_ary, rb_hash_aref(RARRAY(v_ary)->ptr[i], rb_str_new2("name")));
    }
    ((VALUE*)graph->attr)[0] = new_ary;
  }
  //Convert the Hash of weights to floats instead
  if(weights){
    e_ary = ((VALUE*)graph->attr)[1];
    new_ary = rb_ary_new();
    for(i=0;i<RARRAY(e_ary)->len;i++){
      rb_ary_push(new_ary, rb_hash_aref(RARRAY(e_ary)->ptr[i], rb_str_new2("weight")));
    }
    ((VALUE*)graph->attr)[1] = new_ary;
  }

  igraph_strvector_destroy(&names_vec);

  return new_graph;

}

/* call-seq:
 *   graph.write_graph_ncol(file,names,weights) -> Integer
 *
 * Writes the graph to a file in .ncol format
 *
 * .ncol is a format used by LGL, see igraph_read_graph_ncol() for details.
 *
 * Note that having multiple or loop edges in an .ncol file breaks the LGL 
 * software but igraph does not check for this condition. 
 *
 * file: The file object to write to, it should be writable.
 *
 * names: The name of the vertex attribute, if symbolic names are to be 
 * written to the file.
 *
 * weights: The name of the edge attribute, if they are also written to the 
 * file.
 */
VALUE cIGraph_write_graph_ncol(VALUE self, VALUE file, VALUE names, VALUE weights){

  char *buf;
  size_t size;
  FILE *stream;
  igraph_t *graph;
  int e, i;

  VALUE v_ary;
  VALUE e_ary;
  VALUE new_v_ary;
  VALUE new_e_ary;

  VALUE vertex_h;
  VALUE edge_h;

  char *weights_b = "0";
  char *names_b   = "0";

  if(names)
    names_b = "name";
  
  if(weights)
    weights_b = "weight";

  Data_Get_Struct(self, igraph_t, graph);

  //Convert each object to it's String representation and each edge to
  //it's Float
  if(names){
    v_ary = ((VALUE*)graph->attr)[0];
    new_v_ary = rb_ary_new();
    for(i=0;i<RARRAY(v_ary)->len;i++){
      vertex_h = rb_hash_new();
      rb_hash_aset(vertex_h, rb_str_new2("name"), StringValue(RARRAY(v_ary)->ptr[i]));
      rb_ary_push(new_v_ary, vertex_h);
    }
    ((VALUE*)graph->attr)[0] = new_v_ary;
  }
  if(weights){
    e_ary = ((VALUE*)graph->attr)[1];
    new_e_ary = rb_ary_new();
    for(i=0;i<RARRAY(e_ary)->len;i++){
      edge_h = rb_hash_new();
      rb_hash_aset(edge_h, rb_str_new2("weight"), rb_funcall(RARRAY(e_ary)->ptr[i],rb_intern("to_f"),0));
     rb_ary_push(new_e_ary, edge_h);
    }
    ((VALUE*)graph->attr)[1] = new_e_ary;
  }

  stream = open_memstream(&buf,&size);
  e = igraph_write_graph_ncol(graph, stream, names_b, weights_b);
  fflush(stream);
  rb_funcall(file, rb_intern("write"), 1, rb_str_new(buf,size));
  fclose(stream);

  //Replace initital vertices and edges
  if(names){
    ((VALUE*)graph->attr)[0] = v_ary;
  }
  if(weights){
    ((VALUE*)graph->attr)[0] = e_ary;    
  }  

  return e;

}

/* call-seq:
 *   IGraph::FileRead.read_graph_lgl(file,predefnames,names,weights) -> IGraph
 *
 * Reads a graph from an .lgl file
 *
 * The .lgl format is used by the Large Graph Layout visualization software 
 * (http://bioinformatics.icmb.utexas.edu/lgl/), it can describe undirected 
 * optionally weighted graphs. From the LGL manual:
 *
 * The second format is the LGL file format ( .lgl file suffix). This is yet 
 * another graph file format that tries to be as stingy as possible with 
 * space, yet keeping the edge file in a human readable (not binary) format. 
 * The format itself is like the following:
 *
 *  # vertex1name
 *    vertex2name [optionalWeight]
 *    vertex3name [optionalWeight] 
 *
 * Here, the first vertex of an edge is preceded with a pound sign '#'. 
 * Then each vertex that shares an edge with that vertex is listed one per 
 * line on subsequent lines.
 *
 * LGL cannot handle loop and multiple edges or directed graphs, but in 
 * igraph it is not an error to have multiple and loop edges.
 *
 * file: A File or IO object to read from.
 *
 * names: Logical value, if TRUE the symbolic names of the vertices will be 
 * added to the graph as a vertex attribute called “name”.
 *
 * weights: Logical value, if TRUE the weights of the edges is added to the 
 * graph as an edge attribute called “weight”. 
 */
VALUE cIGraph_read_graph_lgl(VALUE self, VALUE file, VALUE names, VALUE weights){

  VALUE string;
  FILE *stream;
  VALUE new_graph;
  VALUE v_ary;
  VALUE e_ary;
  VALUE new_ary;

  igraph_t *graph;
  igraph_bool_t weights_b  = 0;
  igraph_bool_t names_b    = 0;

  int i;

  if(names)
    names_b = 1;

  if(weights)
    weights_b = 1;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  string = rb_funcall(file, rb_intern("read"), 0);
  stream = fmemopen(RSTRING(string)->ptr,RSTRING(string)->len, "r");

  igraph_read_graph_lgl(graph, stream, names_b, weights_b);

  fclose(stream);

  //Convert the Hash of names to Strings instead
  if(names){
    v_ary = ((VALUE*)graph->attr)[0];
    new_ary = rb_ary_new();
    for(i=0;i<RARRAY(v_ary)->len;i++){
      rb_ary_push(new_ary, rb_hash_aref(RARRAY(v_ary)->ptr[i], rb_str_new2("name")));
    }
    ((VALUE*)graph->attr)[0] = new_ary;
  }
  //Convert the Hash of weights to floats instead
  if(weights){
    e_ary = ((VALUE*)graph->attr)[1];
    new_ary = rb_ary_new();
    for(i=0;i<RARRAY(e_ary)->len;i++){
      rb_ary_push(new_ary, rb_hash_aref(RARRAY(e_ary)->ptr[i], rb_str_new2("weight")));
    }
    ((VALUE*)graph->attr)[1] = new_ary;
  }

  return new_graph;

}

/* call-seq:
 *   graph.write_graph_lgl(file,names,weights,isolates) -> Integer
 *
 * Writes the graph to a file in .lgl format
 *
 * .lgl is a format used by LGL, see read_graph_lgl() for details.
 *
 * Note that having multiple or loop edges in an .lgl file breaks the LGL 
 * software but igraph does not check for this condition. 
 *
 * file: The File object to write to, it should be writable.
 *
 * names: The name of the vertex attribute, if symbolic names are written to 
 * the file.
 *
 * weights: The name of the edge attribute, if they are also written to the 
 * file.
 * 
 * isolates: Logical, if TRUE isolated vertices are also written to the file. 
 * If FALSE they will be omitted. 
 */
VALUE cIGraph_write_graph_lgl(VALUE self, VALUE file, VALUE names, VALUE weights, VALUE isolates){

  char *buf;
  size_t size;
  FILE *stream;
  igraph_t *graph;
  int e, i;

  VALUE v_ary;
  VALUE e_ary;
  VALUE new_v_ary;
  VALUE new_e_ary;

  VALUE vertex_h;
  VALUE edge_h;

  char *weights_b = "0";
  char *names_b   = "0";

  igraph_bool_t isolates_b;

  if(names)
    names_b = "name";
  
  if(weights)
    weights_b = "weight";

  if(isolates)
    isolates_b = 1;

  Data_Get_Struct(self, igraph_t, graph);

  //Convert each object to it's String representation and each edge to
  //it's Float
  if(names){
    v_ary = ((VALUE*)graph->attr)[0];
    new_v_ary = rb_ary_new();
    for(i=0;i<RARRAY(v_ary)->len;i++){
      vertex_h = rb_hash_new();
      rb_hash_aset(vertex_h, rb_str_new2("name"), StringValue(RARRAY(v_ary)->ptr[i]));
      rb_ary_push(new_v_ary, vertex_h);
    }
    ((VALUE*)graph->attr)[0] = new_v_ary;
  }
  if(weights){
    e_ary = ((VALUE*)graph->attr)[1];
    new_e_ary = rb_ary_new();
    for(i=0;i<RARRAY(e_ary)->len;i++){
      edge_h = rb_hash_new();
      rb_hash_aset(edge_h, rb_str_new2("weight"), rb_funcall(RARRAY(e_ary)->ptr[i],rb_intern("to_f"),0));
     rb_ary_push(new_e_ary, edge_h);
    }
    ((VALUE*)graph->attr)[1] = new_e_ary;
  }

  stream = open_memstream(&buf,&size);
  e = igraph_write_graph_lgl(graph, stream, names_b, weights_b, isolates);
  fflush(stream);
  rb_funcall(file, rb_intern("write"), 1, rb_str_new(buf,size));
  fclose(stream);

  //Replace initital vertices and edges
  if(names){
    ((VALUE*)graph->attr)[0] = v_ary;
  }
  if(weights){
    ((VALUE*)graph->attr)[0] = e_ary;    
  }  

  return e;

}

/* call-seq:
 *   IGraph::FileRead.read_graph_dimacs(file,directed) -> IGraph
 *
 * This function reads the DIMACS file format, more specifically the version 
 * for network flow problems, see the files at 
 * ftp://dimacs.rutgers.edu/pub/netflow/general-info/
 *
 * This is a line-oriented text file (ASCII) format. The first character of 
 * each line defines the type of the line. If the first character is c the 
 * line is a comment line and it is ignored. There is one problem line ( p in 
 * the file, it must appear before any node and arc descriptor lines. The 
 * problem line has three fields separated by spaces: the problem type 
 * ( min , max or asn ), the number of vertices and number of edges in the 
 * graph. Exactly two node identification lines are expected ( n ), one for 
 * the source, one for the target vertex. These have two fields: the id of 
 * the vertex and the type of the vertex, either s (=source) or t (=target). 
 * Arc lines start with a and have three fields: the source vertex, the 
 * target vertex and the edge capacity.
 *
 * Vertex ids are numbered from 1. The source, target vertices and edge 
 * capacities are added as attributes of the graph. 
 * I.e: g.attributes['source'].
 *
 * file: The File to read from.
 *
 * directed: Boolean, whether to create a directed graph. 
 */
VALUE cIGraph_read_graph_dimacs(VALUE self, VALUE file, VALUE directed){

  VALUE string;
  FILE *stream;
  VALUE new_graph;

  igraph_integer_t source;
  igraph_integer_t target;
  igraph_vector_t capacity;

  igraph_t *graph;
  igraph_bool_t directed_b = 0;

  igraph_vs_t vs;
  igraph_vit_t vit;

  VALUE v_ary;
  VALUE g_hsh;

  int i;
  int vid;

  if(directed)
    directed_b = 1;

  igraph_vector_init(&capacity, 0);

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  string = rb_funcall(file, rb_intern("read"), 0);
  stream = fmemopen(RSTRING(string)->ptr,RSTRING(string)->len, "r");

  igraph_read_graph_dimacs(graph, stream, &source, &target, &capacity, directed_b);

  fclose(stream);

  igraph_vs_all(&vs);
  igraph_vit_create(graph, vs, &vit);

  //Push Integer onto vertex array
  v_ary = ((VALUE*)graph->attr)[0];

  while (!IGRAPH_VIT_END(vit)) {
    vid = IGRAPH_VIT_GET(vit);
    rb_ary_push(v_ary,INT2NUM(vid));
    IGRAPH_VIT_NEXT(vit);
  }

  g_hsh = ((VALUE*)graph->attr)[2];
  
  rb_hash_aset(g_hsh, rb_str_new2("source"),   INT2NUM(source));
  rb_hash_aset(g_hsh, rb_str_new2("target"),   INT2NUM(target));
  rb_hash_aset(g_hsh, rb_str_new2("capacity"), rb_ary_new());

  for(i=0;i<igraph_vector_size(&capacity);i++){
    rb_ary_push(rb_hash_aref(g_hsh, rb_str_new2("capacity")), rb_float_new(VECTOR(capacity)[i]));
  }

  igraph_vit_destroy(&vit);
  igraph_vs_destroy(&vs);

  return new_graph;

}

/* call-seq:
 *   graph.write_graph_dimacs(file,source,target,capacity) -> Integer
 *
 * This function writes a graph to an output stream in DIMACS format, 
 * describing a maximum flow problem. See 
 * ftp://dimacs.rutgers.edu/pub/netflow/general-info/
 *
 * This file format is discussed in the documentation of read_graph_dimacs(), 
 * see that for more information.
 *
 * file: IO object to write to. 	
 *
 * source: The source vertex for the maximum flow.
 *
 * target: The target vertex.
 *
 * capacity: Array containing the edge capacity values. 
 */
VALUE cIGraph_write_graph_dimacs(VALUE self, VALUE file, VALUE source, VALUE target, VALUE capacity){

  char *buf;
  size_t size;
  FILE *stream;
  igraph_t *graph;
  int e, i;

  igraph_vector_t capacity_v;

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_init(&capacity_v,0);

  for(i=0;i<RARRAY(capacity)->len;i++){
    igraph_vector_push_back(&capacity_v,NUM2DBL(RARRAY(capacity)->ptr[i]));
  }

  stream = open_memstream(&buf,&size);
  e = igraph_write_graph_dimacs(graph, stream, NUM2INT(source), NUM2INT(target), &capacity_v);
  fflush(stream);
  rb_funcall(file, rb_intern("write"), 1, rb_str_new(buf,size));
  fclose(stream);

  return e;

}

/* call-seq:
 *   IGraph::FileRead.read_graph_graphdb(file,directed) -> IGraph
 *
 * Read a graph in the binary graph database format.
 *
 * file: The IO object to read from.
 *
 * directed: Boolean, whether to create a directed graph. 
 */
VALUE cIGraph_read_graph_graphdb(VALUE self, VALUE file, VALUE directed){

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

  igraph_read_graph_graphdb(graph, stream, directed_b);

  fclose(stream);

  igraph_vs_all(&vs);
  igraph_vit_create(graph, vs, &vit);

  //Push Integer onto vertex array
  v_ary = ((VALUE*)graph->attr)[0];

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
 *   IGraph::FileRead.read_graph_graphml(file,index) -> IGraph
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
 *   IGraph::FileRead.read_graph_gml(file) -> IGraph
 *
 * Reads a graph from a GraphML file.
 *
 * GraphML is an XML-based file format for representing various types of 
 * graphs. Currently only the most basic import functionality is implemented 
 * in igraph: it can read GraphML files without nested graphs and hyperedges.
 *
 * file: IO object to read from
 */
VALUE cIGraph_read_graph_gml(VALUE self, VALUE file){

  VALUE string;
  FILE *stream;
  VALUE new_graph;
  igraph_t *graph;

  new_graph = cIGraph_alloc(cIGraph);
  Data_Get_Struct(new_graph, igraph_t, graph);

  string = rb_funcall(file, rb_intern("read"), 0);
  stream = fmemopen(RSTRING(string)->ptr,RSTRING(string)->len, "r");

  igraph_read_graph_gml(graph, stream);

  fclose(stream);

  return new_graph;  

}

/* call-seq:
 *   graph.write_graph_gml(file) -> IGraph
 *
 * Writes the graph to a file in GraphML format.
 *
 * file: IO object to write to
 */
VALUE cIGraph_write_graph_gml(VALUE self, VALUE file){

  char *buf;
  size_t size;
  FILE *stream;
  igraph_t *graph;
  int e;

  Data_Get_Struct(self, igraph_t, graph);

  stream = open_memstream(&buf,&size);
  e = igraph_write_graph_gml(graph, stream, NULL, 0);
  fflush(stream);

  rb_funcall(file, rb_intern("write"), 1, rb_str_new(buf,size));

  fclose(stream);

  return e;

}

/* call-seq:
 *   IGraph::FileRead.read_graph_pajek(file) -> IGraph
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

  IGRAPH_CHECK(igraph_read_graph_pajek(graph, stream));

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
#endif
