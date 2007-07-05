require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_select_all
    graph = IGraph.new(['A','B','C','D'],true)
    assert_equal ['A','B','C','D'], graph.all_vertices
    assert_equal ['A','B','C','D'], graph.vertices
  end
  def test_adj
    graph = IGraph.new(['A','B','C','D'],true)
    assert_equal ['B'], graph.adjacent_vertices('A',IGraph::ALL)
  end
end
