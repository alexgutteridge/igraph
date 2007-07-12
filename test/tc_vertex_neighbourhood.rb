require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_neighbourhood_size
    graph = IGraph.new(['A','B','C','D'],true)
    assert_equal [1], graph.neighbourhood_size(['A'],0,IGraph::ALL)
    assert_equal [2], graph.neighbourhood_size(['A'],1,IGraph::ALL)
  end
  def test_neighbourhood
    graph = IGraph.new(['A','B','B','C','C','D'],true) 
    assert_equal [['A']],         graph.neighborhood(['A'],0,IGraph::ALL)
    assert_equal [['A','B']],     graph.neighborhood(['A'],1,IGraph::ALL)
    assert_equal [['A','B','C']], graph.neighborhood(['A'],2,IGraph::ALL)
  end
end
