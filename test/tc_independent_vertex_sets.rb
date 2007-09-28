require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_independent_vertex_sets
    g = IGraph.new([1,2,3,4],true)
    assert_equal [[1,3],[1,4],[2,3],[2,4]], g.independent_vertex_sets(2,0)
  end
  def test_largest_independent_vertex_sets
    g = IGraph.new([1,2,3,4,1,5],true)
    assert_equal [[2,3,5],[2,4,5]], g.largest_independent_vertex_sets
  end
  def test_maximal_independent_vertex_sets
    g = IGraph.new([1,2,3,4],true)
    assert_equal [[1,3],[1,4],[2,3],[2,4]], g.maximal_independent_vertex_sets
  end
  def test_independence_number
    g = IGraph.new([1,2,3,4],true)
    assert_equal 2, g.independence_number
  end
end
