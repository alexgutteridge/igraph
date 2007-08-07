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
  def test_non_adj
    graph = IGraph.new(['A','B','C','D'],true)
    assert_equal ['A','C','D'], graph.nonadjacent_vertices('A',IGraph::ALL)
  end

  def test_e_all
    graph = IGraph.new(['A','B','C','D'],true)
    assert_equal [0,1], graph.edges(IGraph::EDGEORDER_ID)
  end

  def test_e_adj
    graph = IGraph.new(['A','B','C','D'],true,[1,2])
    assert_equal [0], graph.adjacent_edges('B',IGraph::ALL)
  end

end
