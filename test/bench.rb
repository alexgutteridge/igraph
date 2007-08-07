require 'igraph'
require 'gratr/import'
require 'benchmark'

gratr_g  = Digraph[1,2,2,3,3,4]
igraph_g = IGraph.new([1,2,2,3,3,4])
graph_g   =

include Benchmark

bm(6) do |x|
  x.report("igraph"){ 
    50000.times{
      igraph_g.topological_sorting(IGraph::OUT)
    }
  }
  x.report("gratr"){ 
    50000.times{
      gratr_g.topsort
    }
  }
end
