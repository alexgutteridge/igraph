require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
   def test_add_edges
    graph = IGraph.new(['A','B','C','D'],true)
    graph.add_edges(['A','C'])
    assert_equal [2], graph.degree(['A'],IGraph::ALL,true)
  end

  def test_add_vertices
    graph = IGraph.new(['A','B','C','D'],true)
    assert_equal 4, graph.vcount
    graph.add_vertices(['E','F','G','H'])
    assert_equal 8, graph.vcount
    assert_equal [0], graph.degree(['E'],IGraph::ALL,true)
  end

  def test_add_to_empty_graph
    graph = IGraph.new([],true)
    assert_equal 0, graph.vcount
    graph.add_vertices(['A','B'])
    assert_equal 2, graph.vcount
    graph.add_edges(['A','B'])
    assert_equal 1, graph.ecount
  end
end
