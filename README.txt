== Introduction

IGraph is a Ruby extension for using the igraph library ([http://cneurocvs.rmki.kfki.hu/igraph/]). igraph is a library for creating and manipulating graphs with a particular emphasis on network analysis functions.

IGraph is currently pre-alpha status and should be considered unstable. There are functions for building and querying simple graphs as well as shortest path calculations.

All bug reports, feature request and patches are welcome.

== Installation

A working igraph library installation is required.

An IGraph gem is available. The installation requires the location of your igraph headers and library to compile the extension. E.g. under Ubuntu linux:

  gem install igraph -- --with-igraph-include=/usr/local/include/igraph

== Documentation

Here is a very quick and simple example:

 require 'igraph'

 graph = IGraph.new([],true)
 graph.add_edges([1,2,3,4])
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
