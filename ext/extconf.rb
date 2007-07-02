require 'mkmf'
dir_config('igraph')
unless have_library("igraph")
  $stderr.puts "\nERROR: Cannot find the iGraph library, aborting."
  exit 1
end
unless have_header("igraph.h")
  $stderr.puts "\nERROR: Cannot find the iGraph header, aborting."
  exit 1
end
  
create_makefile("igraph")
