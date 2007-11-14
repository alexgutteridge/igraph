require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_maxflow_mincut
    g = IGraph.new([1,2,2,3,3,4])
    assert_equal 3, g.maxflow_value(1,4,[5,4,3])
    assert_equal 3, g.st_mincut_value(1,4,[5,4,3])
  end
  def test_mincut
    g = IGraph.new([1,2,2,3,3,4,2,1,3,2,4,3],true)
    assert_equal 3, g.mincut_value([5,4,3,3,4,5])
    g = IGraph.new([1,2,2,3,1,3,3,4,4,5,4,6,5,6],false)
    val,p1,p2,cut_eid = g.mincut(Array.new(7,1))
    assert_equal 1,       val
    assert_equal [1,2,3], p2.sort
    assert_equal [4,5,6], p1.sort
    assert_equal [3],       cut_eid
  end
end
