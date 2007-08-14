require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_get_attributes
    graph = IGraph.new(['A','B','C','D'],true,[1,2])
    assert_equal 2, graph.get_edge_attr('C','D')
    assert_equal 2, graph['C','D']
  end
  def test_set_attributes
    graph = IGraph.new(['A','B','C','D'],true,[1,2])
    graph.set_edge_attr('C','D',3)
    assert_equal 3, graph.get_edge_attr('C','D')
    graph['C','D'] = 4
    assert_equal 4, graph['C','D']
  end
  def test_new_edges
    graph = IGraph.new(['A','B','C','D'],true,[1,2])
    graph.add_edges(['A','C'],[3])
    assert_equal 3, graph['A','C']
    graph.add_edge('A','D',4)
    assert_equal 4, graph['A','D']
  end
  def test_graph_attr
    graph = IGraph.new(['A','B','C','D'],true,[1,2])
    graph.attributes['test'] = 1
    assert_equal 1, graph.attributes['test']
  end
end
