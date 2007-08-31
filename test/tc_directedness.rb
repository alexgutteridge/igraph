require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_directed_conv

    g = IGraph.new(['A','B','B','C','C','D'],false)

    g.to_directed(IGraph::ARBITRARY)
    assert g.are_connected?('A','B') || g.are_connected?('B','A')
    assert !(g.are_connected?('A','B') && g.are_connected?('B','A'))

    g = IGraph.new(['A','B','B','C','C','D'],false)

    g.to_directed(IGraph::MUTUAL)
    assert g.are_connected?('A','B') && g.are_connected?('B','A')

  end
  def test_undirected
    g = IGraph.new(['A','B','B','A','B','C','C','D'],true)
    
    g.to_undirected(IGraph::EACH)
    assert_equal 4, g.ecount

    g = IGraph.new(['A','B','B','A','B','C','C','D'],true)
    
    g.to_undirected(IGraph::COLLAPSE)
    assert_equal 3, g.ecount

  end  
end
