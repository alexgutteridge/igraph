require 'test/unit'
require 'igraph'

Infinity = 1.0/0

class TestGraph < Test::Unit::TestCase
   def test_dijkstra
     g = IGraph.new([1,2,3,4],false)
     assert_equal [[0,1.5,Infinity,Infinity]], g.dijkstra_shortest_paths([1],[1.5,2.5],IGraph::OUT)
   end

   def test_get_dijkstra_shortest_paths
    graph = IGraph.new(['A','B','C','D','B','D','A','C'],false)
    weights2 = [6,5,6,5]
    weights = [1,5,1,5]
    m = graph.get_dijkstra_shortest_paths('A',['D'],weights,IGraph::ALL)
    assert_equal ['A','B','D'], m[0]
    m = graph.get_dijkstra_shortest_paths('A',['D'],weights2,IGraph::ALL)
    assert_equal ['A','C','D'], m[0]
    #assert_equal [],        m[1]
  end

end
