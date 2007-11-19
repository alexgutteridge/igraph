== Introduction

IGraph is a Ruby extension for interfacing with the C igraph library (http://cneurocvs.rmki.kfki.hu/igraph/). igraph is a library for creating and manipulating graphs with a particular emphasis on network analysis functions.

IGraph is currently in alpha status and although the basic graph creation and manipulation functions are implemented the API should be considered subject to change. The main documentation can be found at http://igraph.rubyforge.org/igraph/.

All bug reports, feature requests and patches are welcome. Please email alexg (at) kuicr.kyoto-u.ac.jp or use the rubyforge forums: http://rubyforge.org/forum/?group_id=3943

== Installation

A working igraph library installation is required. Please see the igraph homepage (http://cneurocvs.rmki.kfki.hu/igraph/) for details on installing igraph.

IGraph is only available as a gem. The installation requires the location of your igraph headers and library to compile the extension. For example, under Ubuntu linux the following command succesfully compiles and installs (you may need to be root):

  gem install igraph -- --with-igraph-include=/usr/local/include/igraph

== Documentation

Graph objects are represented in the IGraph class. See the IGraph class documentation for a list of available methods available to build and query graph objects. The graph generators will use simple Integers as the vertices, but you can use any object as a vertex and edges can also be annotated with any kind of object.

Note that many functions require 'mode' constants to tell igraph how you want certain calculations to be made. The constants are listed under the IGraph documentation and should also be made explicit in each methods documentation.

Here are three examples which translate the C tutorial programs from the igraph documentation (http://cneurocvs.rmki.kfki.hu/igraph/doc/html/igraph-tutorial.html).

= 1.

  require 'igraph'

  g = IGraph::GenerateRandom.erdos_renyi_game(IGraph::ERDOS_RENYI_GNP, 
					      1000, 5.0/1000,
					      false, false)

  d = g.diameter(false,true).size-1
  puts "Diameter of a random graph with average degree 5: #{d}"

= 2.

  require 'igraph'

  graph = IGraph::Generate.lattice([30,30],false,false,true)
  puts "Average path (lattice): #{graph.average_path_length(false,true)}"

  graph.add_edges(Array.new(20){rand(graph.vcount)})
  puts "Average path (randomised): #{graph.average_path_length(false,true)}"

= 3.

  require 'igraph'

  edges = [
    0,  1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8,
    0, 10, 0,11, 0,12, 0,13, 0,17, 0,19, 0,21, 0,31,
    1,  2, 1, 3, 1, 7, 1,13, 1,17, 1,19, 1,21, 1,30,
    2,  3, 2, 7, 2,27, 2,28, 2,32, 2, 9, 2, 8, 2,13,
    3,  7, 3,12, 3,13, 4, 6, 4,10, 5, 6, 5,10, 5,16,
    6, 16, 8,32, 8,32, 8,33, 9,33,13,33,14,32,14,33,
    15,32,15,33,18,32,18,33,19,33,20,32,20,33,
    22,32,22,33,23,25,23,27,23,32,23,33,23,29,
    24,25,24,27,24,31,25,31,26,29,26,33,27,33,
    28,31,28,33,29,32,29,33,30,32,30,33,31,32,31,33,
    32,33
  ]

  g = IGraph.new(edges,false)
  vs = g.vertices

  max = vs.zip(g.degree(vs,IGraph::ALL,true)).max{|a,b| a[1] <=> b[1]}
  puts "Maximum degree is      #{sprintf("%10i",max[1])}, vertex #{max[0]}."

  max = vs.zip(g.closeness(vs,IGraph::ALL)).max{|a,b| a[1] <=> b[1]}
  puts "Maximum closeness is   #{sprintf("%10f",max[1])}, vertex  #{max[0]}."

  max = vs.zip(g.betweenness(vs,IGraph::ALL)).max{|a,b| a[1] <=> b[1]}
  puts "Maximum betweenness is #{sprintf("%10f",max[1])}, vertex  #{max[0]}."

== License

Copyright (C) 2006 Alex Gutteridge

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
