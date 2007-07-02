require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_shortest_paths
    graph = IGraph.new(['A','B','C','D'],true)
    m = graph.shortest_paths(['A'],IGraph::IGRAPH_ALL)
    assert_equal 1, m[0][1]
    assert_equal 0, m[0][0]
    assert_equal graph.vcount, m[0].size
    assert_equal graph.vcount, m[0][2]
  end

  def test_get_shortest_paths
    graph = IGraph.new(['A','B','C','D'],true)
    m = graph.get_shortest_paths('A',['B','C'],IGraph::IGRAPH_ALL)
    assert_equal ['A','B'], m[0]
    assert_equal [],        m[1]
  end

end