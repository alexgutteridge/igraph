#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   graph[u,v] -> Object
 * 
 * Returns the object associated with the edge connecting vertices u and v.
 * Aliased to graph.get_edge_attr(u,v)
 */
VALUE cIGraph_get_edge_attr(VALUE self, VALUE from, VALUE to){

  int idx;
  igraph_t *graph;
  VALUE e_ary;

  Data_Get_Struct(self, igraph_t, graph);
  e_ary = ((VALUE*)graph->attr)[1];

  idx = NUM2INT(cIGraph_get_eid(self, from, to, 1));
  return rb_ary_entry(e_ary,idx);

}

/* call-seq:
 *   graph[u,v] = w
 * 
 * Sets the object associated with the edge connecting vertices u and v.
 * Aliased to graph.set_edge_attr(u,v)
 */
VALUE cIGraph_set_edge_attr(VALUE self, VALUE from, VALUE to, VALUE attr){

  int idx;
  igraph_t *graph;
  VALUE e_ary;

  Data_Get_Struct(self, igraph_t, graph);
  e_ary = ((VALUE*)graph->attr)[1];

  idx = NUM2INT(cIGraph_get_eid(self, from, to, 1));
  rb_ary_store(e_ary,idx,attr);

  return Qtrue;

}

/* call-seq:
 *   graph.attributes -> Hash
 * 
 * Returns the graph attributes. This is usually only used when reading in
 * graphs from GraphML format. Feel free to use instance variables on the
 * graph object if you like (they won't get copied though!).
 */
VALUE cIGraph_graph_attributes(VALUE self){

  igraph_t *graph;
  VALUE hsh;

  Data_Get_Struct(self, igraph_t, graph);
  hsh = ((VALUE*)graph->attr)[2];

  return hsh;

}

igraph_attribute_table_t cIGraph_attribute_table = {
  cIGraph_attribute_init,
  cIGraph_attribute_destroy,
  cIGraph_attribute_copy,
  cIGraph_attribute_add_vertices,
  cIGraph_attribute_delete_vertices,
  cIGraph_attribute_add_edges,
  cIGraph_attribute_delete_edges,
  cIGraph_attribute_permute_edges,
  cIGraph_attribute_get_info,
  cIGraph_attribute_has_attr,
  cIGraph_attribute_get_type,
  cIGraph_get_numeric_graph_attr,
  cIGraph_get_string_graph_attr,
  cIGraph_get_numeric_vertex_attr,
  cIGraph_get_string_vertex_attr,
  cIGraph_get_numeric_edge_attr,
  cIGraph_get_string_edge_attr,
};

int cIGraph_attribute_init(igraph_t *graph, igraph_vector_ptr_t *attr) {

  VALUE* attrs;
  int i;
  VALUE key;
  VALUE value;

  attrs = (VALUE*)calloc(3, sizeof(VALUE));

  if(!attrs)
    IGRAPH_ERROR("Error allocating Arrays\n", IGRAPH_ENOMEM);

  //[0] is vertex array, [1] is edge array, [2] is graph attr
  attrs[0] = rb_ary_new();
  attrs[1] = rb_ary_new();
  attrs[2] = rb_hash_new();

  if(attr){
    for(i=0;i<igraph_vector_ptr_size(attr);i++){
      igraph_i_attribute_record_t *attr_rec;
      char *s;
      attr_rec = VECTOR(*attr)[i];
      key = rb_str_new2(attr_rec->name);
      switch (attr_rec->type) {
      case IGRAPH_ATTRIBUTE_NUMERIC:
	value=rb_float_new((double)VECTOR(*(igraph_vector_t*)attr_rec->value)[0]);
	break;
      case IGRAPH_ATTRIBUTE_STRING:
	igraph_strvector_get((igraph_strvector_t*)attr_rec->value, 0, &s);
	value=rb_str_new2(s);
	break;
      default:
	IGRAPH_WARNING("unsupported attribute type (not string and not numeric)");
	value=0;
	break;
      }
      if (value){
	rb_hash_aset((VALUE)attrs[2],key,value);
      }
    }
  }
 
  graph->attr = attrs;

  return IGRAPH_SUCCESS;
}

/* Destruction */
void cIGraph_attribute_destroy(igraph_t *graph) {
  VALUE *attrs = (VALUE*)graph->attr;
  free(attrs);
  return;
}

/* Copying */

int replace_i(VALUE key, VALUE val, VALUE hash){
    if (key != Qundef) {
        rb_hash_aset(hash, key, val);
    }

    //return ST_CONTINUE;
    return 0;
}


int cIGraph_attribute_copy(igraph_t *to, const igraph_t *from) {

#ifdef DEBUG
  printf("Entering cIGraph_attribute_copy\n");
#endif

  VALUE* attrs;
  VALUE vertex_array = ((VALUE*)from->attr)[0];
  VALUE edge_array   = ((VALUE*)from->attr)[1];
  VALUE graph_attr   = ((VALUE*)from->attr)[2];

  attrs = ALLOC_N(VALUE, 3);

  attrs[0] = rb_ary_dup(vertex_array);
  attrs[1] = rb_ary_dup(edge_array);
  attrs[2] = rb_hash_new();

  rb_hash_foreach(graph_attr, replace_i, attrs[2]);

  to->attr = attrs;  

#ifdef DEBUG
  printf("Leaving cIGraph_attribute_copy\n");
#endif

  return IGRAPH_SUCCESS;
}

/* Adding vertices */
int cIGraph_attribute_add_vertices(igraph_t *graph, long int nv, igraph_vector_ptr_t *attr) {

#ifdef DEBUG
  printf("Entering cIGraph_attribute_add_vertices\n");
#endif

  int i,j;
  VALUE vertex_array = ((VALUE*)graph->attr)[0];
  VALUE values;

  if(attr){
    if(igraph_vector_ptr_size(attr) > 0 && ((igraph_i_attribute_record_t*)VECTOR(*attr)[0])->type == IGRAPH_ATTRIBUTE_PY_OBJECT){

      values = (VALUE)((igraph_i_attribute_record_t*)VECTOR(*attr)[0])->value;
      Check_Type(values, T_ARRAY);
      for(i=0;i<RARRAY(values)->len;i++){
      rb_ary_push(vertex_array, RARRAY(values)->ptr[i]);
      }
      //Otherwise read each attriute into hashes and use those
    } else {
      for(i=0;i<nv;i++){
	
	VALUE record;
	igraph_i_attribute_record_t *attr_rec;
	char *s;
	record = rb_hash_new();

	//For when no attributes are given
	if(igraph_vector_ptr_size(attr) == 0){
	  record = INT2NUM(i+1);
	}
	
	for (j=0; j<igraph_vector_ptr_size(attr); j++) {
	  VALUE key;
	  VALUE value;
	  //Add key value pair
	  attr_rec = VECTOR(*attr)[j];
	  key = rb_str_new2(attr_rec->name);
	  switch (attr_rec->type) {
	  case IGRAPH_ATTRIBUTE_NUMERIC:
	    value=rb_float_new((double)VECTOR(*(igraph_vector_t*)attr_rec->value)[i]);
	    break;
	  case IGRAPH_ATTRIBUTE_STRING:
	    igraph_strvector_get((igraph_strvector_t*)attr_rec->value, i, &s);
	    value=rb_str_new2(s);
	    break;
	  default:
	    IGRAPH_WARNING("unsupported attribute type (not string and not numeric)");
	    value=Qnil;
	    break;
	  }
	  rb_hash_aset(record,key,value);
	}
	rb_ary_push(vertex_array,record);
      }
    }
  }
 
#ifdef DEBUG
  printf("Leaving cIGraph_attribute_add_vertices\n");
#endif

  return IGRAPH_SUCCESS;
}

/* Deleting vertices */
void cIGraph_attribute_delete_vertices(igraph_t *graph,
				       const igraph_vector_t *eidx,
				       const igraph_vector_t *vidx) {
 
#ifdef DEBUG
  printf("Entering cIGraph_attribute_delete_vertices\n");
#endif

 int i;
 VALUE vertex_array = ((VALUE*)graph->attr)[0];
 VALUE edge_array   = ((VALUE*)graph->attr)[1];

 VALUE n_v_ary = rb_ary_new();
 VALUE n_e_ary = rb_ary_new();

 for(i=0;i<igraph_vector_size(vidx);i++){
   if(VECTOR(*vidx)[i] != 0)
     rb_ary_store(n_v_ary,VECTOR(*vidx)[i]-1,rb_ary_entry(vertex_array,i));
 }
 for(i=0;i<igraph_vector_size(eidx);i++){
   if(VECTOR(*eidx)[i] != 0)
     rb_ary_store(n_e_ary,VECTOR(*eidx)[i]-1,rb_ary_entry(edge_array,i));
 }
  
 ((VALUE*)graph->attr)[0] = n_v_ary;
 ((VALUE*)graph->attr)[1] = n_e_ary;

#ifdef DEBUG
  printf("Leaving cIGraph_attribute_delete_vertices\n");
#endif

  return;
}

/* Adding edges */
int cIGraph_attribute_add_edges(igraph_t *graph, 
				const igraph_vector_t *edges, 
				igraph_vector_ptr_t *attr) {

#ifdef DEBUG
  printf("Entering cIGraph_attribute_add_edges\n");
#endif

  int i,j;
  VALUE edge_array = ((VALUE*)graph->attr)[1];
  VALUE values;

  if(attr){
    //If the only record is of type PY_OBJ then use the values as attributes
    if(((igraph_i_attribute_record_t*)VECTOR(*attr)[0])->type == IGRAPH_ATTRIBUTE_PY_OBJECT){
      values = (VALUE)((igraph_i_attribute_record_t*)VECTOR(*attr)[0])->value;
      Check_Type(values, T_ARRAY);
      for(i=0;i<RARRAY(values)->len;i++){
	rb_ary_push(edge_array, RARRAY(values)->ptr[i]);
      }
      //Otherwise read each attriute into hashes and use those
    } else {
      for(i=0;i<igraph_vector_size(edges)/2;i++){
	VALUE record;
	igraph_i_attribute_record_t *attr_rec;
	char *s;
	record = rb_hash_new();
	for (j=0; j<igraph_vector_ptr_size(attr); j++) {
	  VALUE key;
	  VALUE value;
	  //Add key value pair
	  attr_rec = VECTOR(*attr)[j];
	  key = rb_str_new2(attr_rec->name);
	  switch (attr_rec->type) {
	  case IGRAPH_ATTRIBUTE_NUMERIC:
	    value=rb_float_new((double)VECTOR(*(igraph_vector_t*)attr_rec->value)[i]);
	    break;
	  case IGRAPH_ATTRIBUTE_STRING:
	    igraph_strvector_get((igraph_strvector_t*)attr_rec->value, i, &s);
	    value=rb_str_new2(s);
	    break;
	  default:
	    IGRAPH_WARNING("unsupported attribute type (not string and not numeric)");
	    value=Qnil;
	    break;
	  }
	  rb_hash_aset(record,key,value);
	}
	rb_ary_push(edge_array,record);
      }
    }
  }

#ifdef DEBUG
  printf("Leaving cIGraph_attribute_add_edges\n");
#endif

  return IGRAPH_SUCCESS;
}

/* Deleting edges */
void cIGraph_attribute_delete_edges(igraph_t *graph, const igraph_vector_t *idx) {

#ifdef DEBUG
  printf("Entering cIGraph_attribute_delete_edges\n");
#endif

  int i;
  VALUE edge_array = ((VALUE*)graph->attr)[1];
  VALUE n_e_ary = rb_ary_new();

  for(i=0;i<igraph_vector_size(idx);i++){
   if(VECTOR(*idx)[i] != 0)
     rb_ary_store(n_e_ary,VECTOR(*idx)[i]-1,rb_ary_entry(edge_array,i));
  }

  ((VALUE*)graph->attr)[1] = n_e_ary;

#ifdef DEBUG
  printf("Leaving cIGraph_attribute_delete_edges\n");
#endif
  
  return;  
}

/* Permuting edges */
int cIGraph_attribute_permute_edges(igraph_t *graph,
				    const igraph_vector_t *idx) {

#ifdef DEBUG
  printf("Entering cIGraph_attribute_permute_edges\n");
#endif

  int i;
  VALUE edge_array = ((VALUE*)graph->attr)[1];
  VALUE n_e_ary = rb_ary_new();

  for(i=0;i<igraph_vector_size(idx);i++){
    rb_ary_push(n_e_ary,rb_ary_entry(edge_array,VECTOR(*idx)[i]));
  }

  ((VALUE*)graph->attr)[1] = n_e_ary;

#ifdef DEBUG
  printf("Leaving cIGraph_attribute_permute_edges\n");
#endif
  
  return 0;
}

VALUE keys_to_strvec(VALUE data, VALUE arr){

  VALUE key = rb_ary_entry(data, 0);
  VALUE val = rb_ary_entry(data, 1);
  
  VALUE rb_names = rb_ary_entry(arr, 0);
  VALUE rb_types = rb_ary_entry(arr, 1);
  
  VALUE str = StringValue(key);
  
  rb_ary_push(rb_names,str);
  
  if(TYPE(val) == T_STRING){
    rb_ary_push(rb_types,INT2FIX(IGRAPH_ATTRIBUTE_STRING));
  } else if (TYPE(val) == T_FLOAT || TYPE(val) == T_FIXNUM){
    rb_ary_push(rb_types,INT2FIX(IGRAPH_ATTRIBUTE_NUMERIC));
  } else {
    rb_ary_push(rb_types,INT2FIX(IGRAPH_ATTRIBUTE_PY_OBJECT));
  }

  return data;

}

/* Getting attribute names and types */
int cIGraph_attribute_get_info(const igraph_t *graph,
			       igraph_strvector_t *gnames,
			       igraph_vector_t *gtypes,
			       igraph_strvector_t *vnames,
			       igraph_vector_t *vtypes,
			       igraph_strvector_t *enames,
			       igraph_vector_t *etypes) {

  
#ifdef DEBUG
  printf("Entering cIGraph_attribute_get_info\n");
#endif

  igraph_strvector_t *names[3] = { vnames, enames, gnames };
  igraph_vector_t    *types[3] = { vtypes, etypes, gtypes };

  long int i,j;

  for (i=0; i<3; i++) {

    igraph_strvector_t *n = names[i];
    igraph_vector_t *t = types[i];

    VALUE rb_names = rb_ary_new();
    VALUE rb_types = rb_ary_new();

    VALUE obj_hash;

    //Graph attributes are different
    if (i != 2){

      VALUE store = ((VALUE*)graph->attr)[i];
      VALUE obj   = RARRAY(store)->ptr[0];

      obj_hash = Qnil;
      if(rb_funcall(obj, rb_intern("respond_to?"), 1, rb_str_new2("to_hash")) == Qtrue){
	obj_hash = rb_funcall(obj, rb_intern("to_hash"), 0);
      }
    } else {
      obj_hash = ((VALUE*)graph->attr)[2];
    }

    if(!NIL_P(obj_hash)){
      //Take the keys of the hash, convert to strings and put in vector n
      rb_iterate(rb_each, obj_hash, keys_to_strvec, rb_ary_new3(2,rb_names,rb_types));
    }

    //Push names onto n and types onto t
    for(j=0;j<RARRAY(rb_names)->len;j++){
      igraph_strvector_add(n, RSTRING(RARRAY(rb_names)->ptr[j])->ptr);
      igraph_vector_push_back(t, NUM2INT(RARRAY(rb_types)->ptr[j]));
    }

  }

#ifdef DEBUG
  printf("Leaving cIGraph_attribute_get_info\n");
#endif

  return 0;

}

/* Checks whether the graph has a graph/vertex/edge attribute with the given name */
igraph_bool_t cIGraph_attribute_has_attr(const igraph_t *graph,
					 igraph_attribute_elemtype_t type,
					 const char* name) {

#ifdef DEBUG
  printf("Entering cIGraph_attribute_has_attr\n");
#endif

  long int attrnum;
  igraph_bool_t res = 0;
  VALUE obj;

  switch (type) {
  case IGRAPH_ATTRIBUTE_GRAPH: attrnum=0; break;
  case IGRAPH_ATTRIBUTE_VERTEX: attrnum=1; break;
  case IGRAPH_ATTRIBUTE_EDGE: attrnum=2; break;
  default: return 0; break;
  }

  obj = ((VALUE*)graph->attr)[attrnum];
  if (attrnum != 2)
    obj = RARRAY(obj)->ptr[0];

  if(rb_funcall(obj,rb_intern("include?"), 1, rb_str_new2(name))){
    res = 1;
  }

#ifdef DEBUG
  printf("Leaving cIGraph_attribute_has_attr\n");
#endif

  return res;
}

/* Returns the type of a given attribute */
int cIGraph_attribute_get_type(const igraph_t *graph,
			       igraph_attribute_type_t *type,
			       igraph_attribute_elemtype_t elemtype,
			       const char *name) {

#ifdef DEBUG
  printf("Entering cIGraph_attribute_get_type\n");
#endif

  long int attrnum;
  VALUE obj;
  VALUE val;

  switch (elemtype) {
  case IGRAPH_ATTRIBUTE_GRAPH: attrnum=0; break;
  case IGRAPH_ATTRIBUTE_VERTEX: attrnum=1; break;
  case IGRAPH_ATTRIBUTE_EDGE: attrnum=2; break;
  default: return 0; break;
  }

  obj = ((VALUE*)graph->attr)[attrnum];
  if (attrnum != 2)
    obj = RARRAY(obj)->ptr[0];

  if(rb_funcall(obj,rb_intern("includes"), rb_str_new2(name))){
    val = rb_hash_aref(obj,rb_str_new2(name));
    if (TYPE(val) == T_STRING){
      *type = IGRAPH_ATTRIBUTE_STRING;
    } else if (TYPE(val) == T_FIXNUM || TYPE(val) == T_FLOAT){
      *type = IGRAPH_ATTRIBUTE_NUMERIC;
    } else {
      *type = IGRAPH_ATTRIBUTE_PY_OBJECT;
    }
  }  

#ifdef DEBUG
  printf("Leaving cIGraph_attribute_get_type\n");
#endif

  return 0;

}

/* Getting numeric graph attributes */
int cIGraph_get_numeric_graph_attr(const igraph_t *graph,
				   const char *name, igraph_vector_t *value) {

#ifdef DEBUG
  printf("Entering cIGraph_get_numeric_graph_attr\n");
#endif

  VALUE val;

  val = rb_hash_aref(((VALUE*)graph->attr)[2],rb_str_new2(name));
  VECTOR(*value)[0] = NUM2DBL(val);

#ifdef DEBUG
  printf("Leaving cIGraph_get_numeric_graph_attr\n");
#endif

  return 0;
}

/* Getting string graph attributes */
int cIGraph_get_string_graph_attr(const igraph_t *graph,
				  const char *name, igraph_strvector_t *value) {

#ifdef DEBUG
  printf("Entering cIGraph_get_string_graph_attr\n");
#endif

  VALUE val;

  val = rb_hash_aref(((VALUE*)graph->attr)[2],rb_str_new2(name));
  igraph_strvector_set(value,0,RSTRING(val)->ptr);

#ifdef DEBUG
  printf("Leaving cIGraph_get_string_graph_attr\n");
#endif

  return 0;
}

/* Getting numeric vertex attributes */
int cIGraph_get_numeric_vertex_attr(const igraph_t *graph,
				    const char *name,
				    igraph_vs_t vs,
				    igraph_vector_t *value) {

#ifdef DEBUG
  printf("Entering cIGraph_get_numeric_vertex_attr\n");
#endif

  VALUE array = ((VALUE*)graph->attr)[0];
  VALUE val, vertex;
  igraph_vit_t it;
  int i = 0;

  IGRAPH_CHECK(igraph_vit_create(graph, vs, &it));
  IGRAPH_FINALLY(igraph_vit_destroy, &it);
  IGRAPH_CHECK(igraph_vector_resize(value, IGRAPH_VIT_SIZE(it)));

  while(!IGRAPH_VIT_END(it)){
    vertex = RARRAY(array)->ptr[(int)IGRAPH_VIT_GET(it)];
    val = rb_hash_aref(vertex,rb_str_new2(name));

    if(val == Qnil)
      val = rb_float_new(NAN);
      
    VECTOR(*value)[i] = NUM2DBL(val);
    IGRAPH_VIT_NEXT(it);
    i++;
  }

  igraph_vit_destroy(&it);
  IGRAPH_FINALLY_CLEAN(1);  

#ifdef DEBUG
  printf("Leaving cIGraph_get_numeric_vertex_attr\n");
#endif

  return 0;
}

/* Getting string vertex attributes */
int cIGraph_get_string_vertex_attr(const igraph_t *graph,
				   const char *name,
				   igraph_vs_t vs,
				   igraph_strvector_t *value) {

#ifdef DEBUG
  printf("Entering cIGraph_get_string_vertex_attr\n");
#endif

  VALUE array = ((VALUE*)graph->attr)[0];
  VALUE val, vertex;
  igraph_vit_t it;
  int i=0;

  IGRAPH_CHECK(igraph_vit_create(graph, vs, &it));
  IGRAPH_FINALLY(igraph_vit_destroy, &it);
  IGRAPH_CHECK(igraph_strvector_resize(value, IGRAPH_VIT_SIZE(it)));

  while(!IGRAPH_VIT_END(it)){
    vertex = RARRAY(array)->ptr[(int)IGRAPH_VIT_GET(it)];
    val = rb_hash_aref(vertex,rb_str_new2(name));

    if(val == Qnil)
      val = rb_str_new2("");

    igraph_strvector_set(value,i,RSTRING(val)->ptr);
    IGRAPH_VIT_NEXT(it);
    i++;
  }	

  igraph_vit_destroy(&it);
  IGRAPH_FINALLY_CLEAN(1);  

#ifdef DEBUG
  printf("Leaving cIGraph_get_string_vertex_attr\n");
#endif

  return 0;
}

/* Getting numeric edge attributes */
int cIGraph_get_numeric_edge_attr(const igraph_t *graph,
				  const char *name,
				  igraph_es_t es,
				  igraph_vector_t *value) {

#ifdef DEBUG
  printf("Entering cIGraph_get_numeric_edge_attr\n");
#endif

  VALUE array = ((VALUE*)graph->attr)[1];
  VALUE val, vertex;
  igraph_eit_t it;
  int i = 0;

  IGRAPH_CHECK(igraph_eit_create(graph, es, &it));
  IGRAPH_FINALLY(igraph_eit_destroy, &it);
  IGRAPH_CHECK(igraph_vector_resize(value, IGRAPH_EIT_SIZE(it)));

  while(!IGRAPH_EIT_END(it)){
    vertex = RARRAY(array)->ptr[(int)IGRAPH_EIT_GET(it)];
    val = rb_hash_aref(vertex,rb_str_new2(name));

    if(val == Qnil)
      val = rb_float_new(NAN);

    VECTOR(*value)[i] = NUM2DBL(val);
    IGRAPH_EIT_NEXT(it);
    i++;
  }

  igraph_eit_destroy(&it);
  IGRAPH_FINALLY_CLEAN(1);  

#ifdef DEBUG
  printf("Leaving cIGraph_get_numeric_edge_attr\n");
#endif

  return 0;
}

/* Getting string edge attributes */
int cIGraph_get_string_edge_attr(const igraph_t *graph,
				 const char *name,
				 igraph_es_t es,
				 igraph_strvector_t *value) {

#ifdef DEBUG
  printf("Entering cIGraph_get_string_edge_attr\n");
#endif

  VALUE array = ((VALUE*)graph->attr)[1];
  VALUE val, vertex;
  igraph_eit_t it;
  int i=0;

  IGRAPH_CHECK(igraph_eit_create(graph, es, &it));
  IGRAPH_FINALLY(igraph_eit_destroy, &it);
  IGRAPH_CHECK(igraph_strvector_resize(value, IGRAPH_EIT_SIZE(it)));

  while(!IGRAPH_EIT_END(it)){
    vertex = RARRAY(array)->ptr[(int)IGRAPH_EIT_GET(it)];
    val = rb_hash_aref(vertex,rb_str_new2(name));
    if(val == Qnil)
      val = rb_str_new2("");
    igraph_strvector_set(value,i,RSTRING(val)->ptr);
    IGRAPH_EIT_NEXT(it);
    i++;
  }	

  igraph_eit_destroy(&it);
  IGRAPH_FINALLY_CLEAN(1);  

#ifdef DEBUG
  printf("Leaving cIGraph_get_string_edge_attr\n");
#endif

  return 0;
}
