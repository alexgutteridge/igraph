== Introduction

IGraph

IGraph

IGraph

== Installation

A working igraph library installation is required.

An IGraph gem is available as well as a package using setup.rb. In each case the installation requires the location of your igraph library to compile the extension. If you download the setup.rb package use these incantations:

  ruby setup.rb config -- --with-igraph-dir=$R_HOME
  ruby setup.rb setup
  ruby setup.rb install

Using gems it is almost the same:

  gem install igraph -- --with-igraph-dir=$R_HOME

== Documentation

Here is a very quick and simple example:

 require 'igraph'

 graph = IGraph.new([],true)

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
