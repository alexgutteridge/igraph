require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_random
    g = IGraph.new([1,2,3,4],true)
    l = g.layout_random
    assert_instance_of IGraphMatrix, l
    assert_equal g.vcount, l.nrow
    assert_equal 2,        l.ncol
  end
  def test_circle
    g = IGraph.new([1,2,3,4],true)
    l = g.layout_circle
    assert_instance_of IGraphMatrix, l
    assert_equal g.vcount, l.nrow
    assert_equal 2,        l.ncol
  end
  def test_fruchterman_reingold
    g = IGraph.new([1,2,3,4],true)
    l = g.layout_fruchterman_reingold(10,1,1,2,1,false)
    assert_instance_of IGraphMatrix, l
    assert_equal g.vcount, l.nrow
    assert_equal 2,        l.ncol
  end
end
