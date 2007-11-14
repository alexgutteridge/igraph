require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_random_3d
    g = IGraph.new([1,2,3,4],true)
    l = g.layout_random_3d
    assert_instance_of IGraphMatrix, l
    assert_equal g.vcount, l.nrow
    assert_equal 3,        l.ncol
  end
  def test_sphere
    g = IGraph.new([1,2,3,4],true)
    l = g.layout_sphere
    assert_instance_of IGraphMatrix, l
    assert_equal g.vcount, l.nrow
    assert_equal 3,        l.ncol
  end
  def test_fruchterman_reingold_3d
    g = IGraph.new([1,2,3,4],true)
    l = g.layout_fruchterman_reingold_3d(10,1,1,2,1,false)
    assert_instance_of IGraphMatrix, l
    assert_equal g.vcount, l.nrow
    assert_equal 3,        l.ncol
  end
  def test_kamada_kawai_3d
    g = IGraph.new([1,2,3,4],true)
    l = g.layout_kamada_kawai_3d(10,1,1,2,1)
    assert_instance_of IGraphMatrix, l
    assert_equal g.vcount, l.nrow
    assert_equal 3,        l.ncol
  end  
  
end
