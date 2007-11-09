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
  def test_kamada_kawai
    g = IGraph.new([1,2,3,4],true)
    l = g.layout_kamada_kawai(10,1,1,2,1)
    assert_instance_of IGraphMatrix, l
    assert_equal g.vcount, l.nrow
    assert_equal 2,        l.ncol
  end  
  def test_reingold_tilford
    g = IGraph.new([1,2,3,4],true)
    l = g.layout_reingold_tilford(1)
    assert_instance_of IGraphMatrix, l
    assert_equal g.vcount, l.nrow
    assert_equal 2,        l.ncol
  end  
  def test_reingold_tilford_circular
    g = IGraph.new([1,2,3,4],true)
    l = g.layout_reingold_tilford_circular(1)
    assert_instance_of IGraphMatrix, l
    assert_equal g.vcount, l.nrow
    assert_equal 2,        l.ncol
  end   
  def test_grid_fruchterman_reingold
    g = IGraph.new([1,2,3,4],true)
    l = g.layout_grid_fruchterman_reingold(10,1,1,2,1,1,false)
    assert_instance_of IGraphMatrix, l
    assert_equal g.vcount, l.nrow
    assert_equal 2,        l.ncol
  end
  def test_lgl
    g = IGraph.new([1,2,3,4],true)
    l = g.layout_lgl(10,1,1,2,1,1,1)
    assert_instance_of IGraphMatrix, l
    assert_equal g.vcount, l.nrow
    assert_equal 2,        l.ncol
  end
  def test_merge
    g = IGraph.new([1,2,3,4],true)
    l = g.layout_lgl(10,1,1,2,1,1,1)
    h = IGraph.new([1,2,3,4],true)
    m = h.layout_lgl(10,1,1,2,1,1,1)
    f = IGraph.layout_merge_dla([g,h],[l,m])
    assert_instance_of IGraphMatrix, f
    assert_equal g.vcount + h.vcount, f.nrow
    assert_equal 2,                   f.ncol
  end
end
