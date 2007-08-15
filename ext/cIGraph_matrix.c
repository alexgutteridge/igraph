#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

//Classes
VALUE cIGraphMatrix;

void cIGraph_matrix_free(void *p){
  igraph_matrix_destroy(p);
}

VALUE cIGraph_matrix_alloc(VALUE klass){

  igraph_matrix_t *m = malloc(sizeof(igraph_matrix_t));
  VALUE obj;

  igraph_matrix_init(m, 0, 0);

  obj = Data_Wrap_Struct(klass, 0, cIGraph_matrix_free, m);

  return obj;
  
}

/* Document-method: initialize_copy
 *
 * Internal method for copying IGraph objects.
 */
VALUE cIGraph_matrix_init_copy(VALUE copy, VALUE orig){

  igraph_matrix_t *orig_m;
  igraph_matrix_t *copy_m;

  if (copy == orig)
    return copy;

  if(TYPE(orig) != T_DATA || RDATA(orig)->dfree != (RUBY_DATA_FUNC)cIGraph_free){
    rb_raise(rb_eTypeError, "Wrong argument type.");
  }
  
  Data_Get_Struct(copy, igraph_matrix_t, copy_m); 
  Data_Get_Struct(orig, igraph_matrix_t, orig_m);

  igraph_matrix_copy(copy_m,orig_m);

  return copy;

}

/* call-seq:
 *   IGraphMatrix[[x,y,...],...] -> IGraphMatrix
 *
 *   IGraphMatrix[[1,2],[3,4]]
 *
 * Creates a graph with four vertices. Vertex 1 is connected to vertex 2.
 * Vertex 3 is connected to vertex 4.
 */

VALUE cIGraph_matrix_initialize(int argc, VALUE *argv, VALUE self){

  igraph_matrix_t *m;
  VALUE rows;
  int nrows;
  int ncols;
  int i;
  int j;

  rb_scan_args(argc,argv,"0*", &rows);

  Data_Get_Struct(self, igraph_matrix_t, m);

  nrows = RARRAY(rows)->len;
  ncols = RARRAY(RARRAY(rows)->ptr[0])->len;

  igraph_matrix_resize(m, nrows, ncols);

  //Loop through rows
  for (i=0; i<nrows; i++) {
    for (j=0; j<ncols; j++){
      MATRIX(*m,i,j) = NUM2DBL(RARRAY(RARRAY(rows)->ptr[i])->ptr[j]);
    }
  }

  return self;

}

VALUE cIGraph_matrix_get(VALUE self, VALUE i, VALUE j){

  igraph_matrix_t *m;

  Data_Get_Struct(self, igraph_matrix_t, m);
  return rb_float_new(MATRIX(*m,NUM2INT(i),NUM2INT(j)));

}

VALUE cIGraph_matrix_set(VALUE self, VALUE i, VALUE j, VALUE x){

  igraph_matrix_t *m;

  Data_Get_Struct(self, igraph_matrix_t, m);
  MATRIX(*m,NUM2INT(i),NUM2INT(j)) = NUM2DBL(x);
  return x;

}

VALUE cIGraph_matrix_each(VALUE self){

  igraph_matrix_t *m;
  int i;
  int j;

  Data_Get_Struct(self, igraph_matrix_t, m);
  
  for(i=0;i < m->nrow;i++){
    for(j=0;j < m->ncol;j++){
      rb_yield(rb_float_new(MATRIX(*m,i,j)));
    }
  }

  return Qnil;

}

VALUE cIGraph_matrix_size(VALUE self){

  igraph_matrix_t *m;

  Data_Get_Struct(self, igraph_matrix_t, m);
  return LONG2FIX(igraph_matrix_size(m));

}

VALUE cIGraph_matrix_nrow(VALUE self){

  igraph_matrix_t *m;

  Data_Get_Struct(self, igraph_matrix_t, m);
  return LONG2FIX(igraph_matrix_nrow(m));

}

VALUE cIGraph_matrix_ncol(VALUE self){

  igraph_matrix_t *m;

  Data_Get_Struct(self, igraph_matrix_t, m);
  return LONG2FIX(igraph_matrix_ncol(m));

}

VALUE cIGraph_matrix_max(VALUE self){

  igraph_matrix_t *m;

  Data_Get_Struct(self, igraph_matrix_t, m);
  return rb_float_new(igraph_matrix_max(m));

}

VALUE cIGraph_matrix_multiply(VALUE self, VALUE x){

  igraph_matrix_t *m;
  igraph_matrix_t *n = malloc(sizeof(igraph_matrix_t));
  VALUE nobj;

  Data_Get_Struct(self, igraph_matrix_t, m);

  igraph_matrix_copy(n,m);
  igraph_matrix_multiply(n, NUM2DBL(x));

  nobj = Data_Wrap_Struct(cIGraphMatrix, 0, cIGraph_matrix_free, n);

  return nobj;

}

VALUE cIGraph_matrix_toa(VALUE self){

  igraph_matrix_t *m;
  int i;
  int j;
  VALUE a = rb_ary_new();
  VALUE row;

  Data_Get_Struct(self, igraph_matrix_t, m);

  for(i=0;i < m->nrow;i++){
    row = rb_ary_new();
    for(j=0;j < m->ncol;j++){
      rb_ary_push(row,rb_float_new(MATRIX(*m,i,j)));
    }
    rb_ary_push(a,row);
  }

  return a;

}
  
