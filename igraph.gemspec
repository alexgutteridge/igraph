# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = %q{igraph}
  s.version = "0.9.7"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Alex Gutteridge"]
  s.date = %q{2009-02-11}
  s.description = %q{IGraph is a Ruby extension for interfacing with the C igraph library (http://cneurocvs.rmki.kfki.hu/igraph/). igraph is a library for creating and manipulating graphs with a particular emphasis on network analysis functions.}
  s.email = %q{ag357@cam.ac.uk}
  s.extensions = ["ext/extconf.rb"]
  s.extra_rdoc_files = ["README.txt", "History.txt", "License.txt"]
  s.files = ["History.txt", "License.txt", "Manifest.txt", "README.txt", "Rakefile.rb", "ext/cIGraph.c", "ext/cIGraph.h", "ext/cIGraph_add_delete.c", "ext/cIGraph_attribute_handler.c", "ext/cIGraph_basic_properties.c", "ext/cIGraph_basic_query.c", "ext/cIGraph_centrality.c", "ext/cIGraph_cliques.c", "ext/cIGraph_community.c", "ext/cIGraph_components.c", "ext/cIGraph_connectivity.c", "ext/cIGraph_dijkstra.c", "ext/cIGraph_direction.c", "ext/cIGraph_error_handlers.c", "ext/cIGraph_file.c", "ext/cIGraph_generators_deterministic.c", "ext/cIGraph_generators_random.c", "ext/cIGraph_independent_vertex_sets.c", "ext/cIGraph_isomorphism.c", "ext/cIGraph_iterators.c", "ext/cIGraph_kcores.c", "ext/cIGraph_layout.c", "ext/cIGraph_layout3d.c", "ext/cIGraph_matrix.c", "ext/cIGraph_min_cuts.c", "ext/cIGraph_motif.c", "ext/cIGraph_operators.c", "ext/cIGraph_other_ops.c", "ext/cIGraph_randomisation.c", "ext/cIGraph_selectors.c", "ext/cIGraph_shortest_paths.c", "ext/cIGraph_spanning.c", "ext/cIGraph_spectral.c", "ext/cIGraph_topological_sort.c", "ext/cIGraph_transitivity.c", "ext/cIGraph_utility.c", "ext/cIGraph_vertex_neighbourhood.c", "ext/extconf.rb", "test/tc_add_delete.rb", "test/tc_adj_to_distance.rb", "test/tc_attributes.rb", "test/tc_basic_properties.rb", "test/tc_basic_query.rb", "test/tc_centrality.rb", "test/tc_cliques.rb", "test/tc_community.rb", "test/tc_components.rb", "test/tc_connectivity.rb", "test/tc_copy.rb", "test/tc_cores.rb", "test/tc_create.rb", "test/tc_dijkstra.rb", "test/tc_directedness.rb", "test/tc_error_handling.rb", "test/tc_file_read_write.rb", "test/tc_generators_deterministic.rb", "test/tc_generators_random.rb", "test/tc_independent_vertex_sets.rb", "test/tc_isomorphic.rb", "test/tc_iterators.rb", "test/tc_layout.rb", "test/tc_layout3d.rb", "test/tc_matrix.rb", "test/tc_mincuts.rb", "test/tc_motif.rb", "test/tc_other_ops.rb", "test/tc_randomisation.rb", "test/tc_selectors.rb", "test/tc_shortest_paths.rb", "test/tc_spanning.rb", "test/tc_spectral.rb", "test/tc_thrash.rb", "test/tc_topological_sort.rb", "test/tc_transitivity.rb", "test/tc_vertex_neighbourhood.rb", "test/test_all.rb"]
  s.has_rdoc = true
  s.homepage = %q{http://igraph.rubyforge.org/}
  s.rdoc_options = ["--exclude", "test/*", "--main", "README.txt", "--inline-source"]
  s.require_paths = ["test"]
  s.rubyforge_project = %q{igraph}
  s.rubygems_version = %q{1.3.1}
  s.summary = %q{IGraph is a Ruby extension for interfacing with the C igraph library (http://cneurocvs.rmki.kfki.hu/igraph/). igraph is a library for creating and manipulating graphs with a particular emphasis on network analysis functions.}
  s.test_files = ["test/test_all.rb"]

  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 2

    if Gem::Version.new(Gem::RubyGemsVersion) >= Gem::Version.new('1.2.0') then
    else
    end
  else
  end
end
