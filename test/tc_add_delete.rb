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
    assert_nothing_raised do 
      graph.add_vertices(['H','I'])
    end
    assert_equal 9, graph.vcount    
  end

  def test_add_to_empty_graph
    graph = IGraph.new([],true)
    assert_equal 0, graph.vcount
    graph.add_vertices(['A','B'])
    assert_equal 2, graph.vcount
    graph.add_edges(['A','B'])
    assert_equal 1, graph.ecount
  end

  def test_add_edge
    graph = IGraph.new(['A','B','C','D'],true)
    assert_nothing_raised do 
      graph.add_edge('A','D')
    end
    assert_equal [2], graph.degree(['A'],IGraph::ALL,true)
  end

  def test_add_vertex
    graph = IGraph.new(['A','B','C','D'],true)
    assert_nothing_raised do 
      graph.add_vertex('E')
    end
    assert_equal 5, graph.vcount
    assert_equal [0], graph.degree(['E'],IGraph::ALL,true)
  end

  def test_delete_vertex
    graph = IGraph.new(['A','B','C','D'],true,[1,2])
    assert_nothing_raised do 
      graph.delete_vertex('A')
    end
    assert_equal false, graph.include?('A')
    assert_equal 3, graph.vcount
    assert_equal 1, graph.ecount
  end

  def test_delete_edge
    graph = IGraph.new(['A','B','C','D'],true)
    assert_equal true, graph.are_connected?('A','B')
    assert_nothing_raised do 
      graph.delete_edge('A','B')
    end
    assert_equal false, graph.are_connected?('A','B')
  end

  def test_delete_edge_attr
    graph = IGraph.new(['A','B','C','D'],true,[1,2])
    assert_equal 1, graph['A','B']
    assert_nothing_raised do 
      graph.delete_edge('A','B')
    end
    assert_equal false, graph.are_connected?('A','B')
    assert_equal 2, graph['C','D']
  end

  def test_delete_missing_edge
    graph = IGraph.new(['A','B','C','D'],true)
    assert_equal true, graph.are_connected?('A','B')
    assert_raises(IGraphError) do 
      graph.delete_edge('A','C')
    end
  end

end
