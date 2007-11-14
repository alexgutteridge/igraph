require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_connecitivity
    g = IGraph.new(['A','B','B','C','C','D'],true)
    assert_equal 1, g.st_edge_connectivity('A','B')
    assert_equal 0, g.edge_connectivity
    assert_equal 1, g.st_vertex_connectivity('A','C',IGraph::VCONN_NEI_ERROR)
    assert_equal 0, g.vertex_connectivity
  end
  def test_disjoint
    g = IGraph.new(['A','B','B','C','C','D','A','E','E','D'],true)
    assert_equal 2, g.edge_disjoint_paths('A','D')
    assert_equal 2, g.vertex_disjoint_paths('A','D')
  end
  def test_adhesion
    g = IGraph.new(['A','B','B','C','C','D'],true)
    assert_equal 0, g.adhesion
    assert_equal 0, g.cohesion   
  end
end
