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
VALUE cIGraph_create_derived_graph(VALUE old_graph, igraph_t *new_graph);

//IGraph allocation, destruction and intialization
void Init_igraph(void);
void cIGraph_free(void *p);
VALUE cIGraph_alloc(VALUE klass);
VALUE cIGraph_initialize(int argc, VALUE *argv, VALUE self);

//Attribute accessors
VALUE cIGraph_get_edge_attr(VALUE self, VALUE from, VALUE to);
VALUE cIGraph_set_edge_attr(VALUE self, VALUE from, VALUE to, VALUE attr);

//Iterators
VALUE cIGraph_each_vertex  (VALUE self);
VALUE cIGraph_each_edge    (VALUE self, VALUE order);
VALUE cIGraph_each_edge_eid(VALUE self, VALUE order);

//Selectors
VALUE cIGraph_all_v   (VALUE self);
VALUE cIGraph_adj_v   (VALUE self, VALUE v, VALUE mode);
VALUE cIGraph_nonadj_v(VALUE self, VALUE v, VALUE mode);

VALUE cIGraph_all_e   (VALUE self,          VALUE mode);
VALUE cIGraph_adj_e   (VALUE self, VALUE v, VALUE mode);
VALUE cIGraph_nonadj_e(VALUE self, VALUE v, VALUE mode);

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
VALUE cIGraph_add_edges      (int argc, VALUE *argv, VALUE self);
VALUE cIGraph_add_edge       (int argc, VALUE *argv, VALUE self);
VALUE cIGraph_add_vertices   (VALUE self, VALUE vs);
VALUE cIGraph_delete_edges   (VALUE self, VALUE edges);
VALUE cIGraph_delete_vertices(VALUE self, VALUE vs);
VALUE cIGraph_add_vertex     (VALUE self, VALUE v);

//Basic properties
VALUE cIGraph_are_connected(VALUE self, VALUE from, VALUE to);

//Shortest Path Related Functions
VALUE cIGraph_shortest_paths        (VALUE self, VALUE from, VALUE mode);
VALUE cIGraph_get_shortest_paths    (VALUE self, VALUE from, VALUE to, VALUE mode);
VALUE cIGraph_get_all_shortest_paths(VALUE self, VALUE from, VALUE to, VALUE mode); 
VALUE cIGraph_average_path_length   (VALUE self, VALUE directed, VALUE unconn);
VALUE cIGraph_diameter              (VALUE self, VALUE directed, VALUE unconn);
VALUE cIGraph_girth                 (VALUE self);


//Vertex neighbourhood functions
VALUE cIGraph_neighborhood_size  (VALUE self, VALUE from, VALUE order, VALUE mode);
VALUE cIGraph_neighborhood       (VALUE self, VALUE from, VALUE order, VALUE mode);
VALUE cIGraph_neighborhood_graphs(VALUE self, VALUE from, VALUE order, VALUE mode);
