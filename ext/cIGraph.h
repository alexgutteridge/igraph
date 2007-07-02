//Classes
VALUE cIGraph;
VALUE cIGraphError;

//Error and warning handling functions
void cIGraph_error_handler(const char *reason, const char *file,
			   int line, int igraph_errno);
void cIGraph_warning_handler(const char *reason, const char *file,
                             int line, int igraph_errno);

//IGraph specific utility functions
igraph_integer_t cIGraph_get_vertex_id(VALUE graph, VALUE v);
VALUE cIGraph_get_vertex_object(VALUE graph, igraph_integer_t n);
int cIGraph_vertex_arr_to_id_vec(VALUE graph, VALUE va, igraph_vector_t *nv);
VALUE cIGraph_include(VALUE self, VALUE v);

//IGraph allocation, destruction and intialization
void Init_igraph(void);
void cIGraph_free(void *p);
VALUE cIGraph_alloc(VALUE klass);
VALUE cIGraph_initialize(VALUE self, VALUE edges, VALUE directed);

//Basic query operations
VALUE cIGraph_vcount     (VALUE self);
VALUE cIGraph_ecount     (VALUE self);
VALUE cIGraph_edge       (VALUE self, VALUE eid);
VALUE cIGraph_get_eid    (VALUE self, VALUE from, VALUE to, VALUE directed);
VALUE cIGraph_neighbors  (VALUE self, VALUE v, VALUE mode);
VALUE cIGraph_adjacent   (VALUE self, VALUE v, VALUE mode);
VALUE cIGraph_is_directed(VALUE self);
VALUE cIGraph_degree     (VALUE self, VALUE v, VALUE mode, VALUE loops);

//Adding and eleting vertices and edges
VALUE cIGraph_add_edges      (VALUE self, VALUE edges);
VALUE cIGraph_add_vertices   (VALUE self, VALUE vs);
VALUE cIGraph_delete_edges   (VALUE self, VALUE edges);
VALUE cIGraph_delete_vertices(VALUE self, VALUE vs);

//Basic properties
VALUE cIGraph_are_connected(VALUE self, VALUE from, VALUE to);

//Shortest Path Related Functions
VALUE cIGraph_shortest_paths    (VALUE self, VALUE from, VALUE mode);
VALUE cIGraph_get_shortest_paths(VALUE self, VALUE from, VALUE to, VALUE mode);
