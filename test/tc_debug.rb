require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_adj
    graph = IGraph.new(['A','B','C','D'],true)
    assert_equal ['B'], graph.adjacent_vertices('A',IGraph::ALL)
  end
end
 
