require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_subcomponent
    g = IGraph.new([1,2,3,4])
    assert_equal [1,2], g.subcomponent(1,IGraph::ALL)
  end
  def test_subgraph
    g = IGraph.new([1,2,3,4])
    h = g.subgraph([1,2])
    assert_equal 2, h.vcount
    assert_equal [1,2], h.vertices
  end
  def test_clusters
    g  = IGraph.new([1,2,3,4])
    assert_equal 2, g.clusters(IGraph::WEAK).length
    assert_equal [1,2], g.clusters(IGraph::WEAK)[0]
  end
  def test_decompose
    g = IGraph.new([1,2,3,4])
    assert_equal 2, g.decompose(IGraph::WEAK).length
    assert_equal [1,2], g.decompose(IGraph::WEAK)[0].vertices
  end
end
