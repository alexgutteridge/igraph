#include "igraph.h"
#include "ruby.h"
#include "cIGraph.h"

/* call-seq:
 *   igraph.motifs_randesu(size,cut)
 *
 */
VALUE cIGraph_motifs_randesu(VALUE self, VALUE size, VALUE cuts){

  igraph_t *graph;
  igraph_vector_t cutsv;
  igraph_vector_t res;
  int i;
  VALUE hist = rb_ary_new();

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_init(&res,0);

  //Convert an array of vertices to a vector of vertex ids
  igraph_vector_init(&cutsv,0);
  for(i=0;i<RARRAY(cuts)->len;i++){
    igraph_vector_push_back(&cutsv,NUM2DBL(RARRAY(cuts)->ptr[i]));
  }

  igraph_motifs_randesu(graph,&res,NUM2INT(size),&cutsv);

  for(i=0; i<igraph_vector_size(&res); i++){
    rb_ary_push(hist,INT2NUM(VECTOR(res)[i]));
  }

  igraph_vector_destroy(&cutsv);
  igraph_vector_destroy(&res);
 
  return hist;

}

/* call-seq:
 *   igraph.motifs_randesu_no(size,cut)
 *
 */
VALUE cIGraph_motifs_randesu_no(VALUE self, VALUE size, VALUE cuts){

  igraph_t *graph;
  igraph_vector_t cutsv;
  igraph_integer_t res;
  int i;

  Data_Get_Struct(self, igraph_t, graph);

  //Convert an array of vertices to a vector of vertex ids
  igraph_vector_init(&cutsv,0);
  for(i=0;i<RARRAY(cuts)->len;i++){
    igraph_vector_push_back(&cutsv,NUM2DBL(RARRAY(cuts)->ptr[i]));
  }

  igraph_motifs_randesu_no(graph,&res,NUM2INT(size),&cutsv);

  igraph_vector_destroy(&cutsv);
 
  return INT2NUM(res);

}

/* call-seq:
 *   igraph.motifs_randesu_estimate(size,cut,samplen,samplev)
 *
 */
VALUE cIGraph_motifs_randesu_estimate(VALUE self, VALUE size, VALUE cuts, 
				      VALUE samplen, VALUE samplev){

  igraph_t *graph;
  igraph_vector_t cutsv;
  igraph_vector_t vidv;
  igraph_integer_t res;
  int i;

  if(samplev != Qnil){
    igraph_vector_init(&vidv,0);
    //Convert an array of vertices to a vector of vertex ids
    igraph_vector_init_int(&vidv,0);
    cIGraph_vertex_arr_to_id_vec(self,samplev,&vidv);
   }

  Data_Get_Struct(self, igraph_t, graph);

  igraph_vector_init(&cutsv,0);
  for(i=0;i<RARRAY(cuts)->len;i++){
    igraph_vector_push_back(&cutsv,NUM2DBL(RARRAY(cuts)->ptr[i]));
  }

  if(samplev == Qnil){
    igraph_motifs_randesu_estimate(graph,&res,NUM2INT(size),
				   &cutsv,NUM2INT(samplen),NULL);
  } else {
    igraph_motifs_randesu_estimate(graph,&res,NUM2INT(size),
				   &cutsv,NUM2INT(samplen),&vidv);
  }

  igraph_vector_destroy(&cutsv);
  if(samplev != Qnil){
    igraph_vector_destroy(&vidv);
  }
  
  return INT2NUM(res);

}
