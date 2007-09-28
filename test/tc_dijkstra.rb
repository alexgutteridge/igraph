require 'test/unit'
require 'igraph'

Infinity = 1.0/0

class TestGraph < Test::Unit::TestCase
   def test_dijkstra
     g = IGraph.new([1,2,3,4],false)
     assert_equal [[0,1.5,Infinity,Infinity]], g.dijkstra_shortest_paths([1],[1.5,2.5],IGraph::OUT)
   end
 end
