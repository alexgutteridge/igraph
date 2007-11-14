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
