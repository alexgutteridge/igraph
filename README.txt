== Introduction

IGraph is a Ruby extension for interfacing with the C igraph library (http://cneurocvs.rmki.kfki.hu/igraph/). igraph is a library for creating and manipulating graphs with a particular emphasis on network analysis functions.

IGraph is currently in alpha status and although the basic graph creation and manipulation functions are implemented the API should be considered subject to change. The main documentation can be found at http://igraph.rubyforge.org/igraph/.

All bug reports, feature requests and patches are welcome. Please email alexg (at) kuicr.kyoto-u.ac.jp or use the rubyforge forums: http://rubyforge.org/forum/?group_id=3943

== Installation

A working igraph library installation is required. Please see the igraph homepage (http://cneurocvs.rmki.kfki.hu/igraph/) for details on installing igraph.

IGraph is only available as a gem. The installation requires the location of your igraph headers and library to compile the extension. For example, under Ubuntu linux the following command succesfully compiles and installs (you may need to be root):

  gem install igraph -- --with-igraph-include=/usr/local/include/igraph

== Documentation

Graph objects are represented in the IGraph class. See the IGraph class documentation for a list of available methods available to build and query graph objects. Any object can be used as a vertex and any edge can be annotated with any kind of object.

Note that many functions require 'mode' constants to tell igraph how you want certain calculations to be made. The constants are listed under the IGraph documentation and should also be made explicit in each methods documentation.

Here is a very quick and simple example:

 require 'igraph'

 #Create an empty directed graph
 graph = IGraph.new([],true)

 #Add two edges (unannotated)
 graph.add_edges([1,2,3,4])

 #Count the number of vertices
 graph.vcount # returns 4

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
