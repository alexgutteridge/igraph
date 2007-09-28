require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_isomorphic
    g = IGraph.new([1,2,3,4],false)
    h = IGraph.new([5,6,7,8],false)
    assert_equal true, g.isomorphic(h)
    assert_equal true, h.isomorphic(g)
  end
  def test_isomorphic_vf2
    g = IGraph.new([1,2,3,4],false)
    h = IGraph.new([5,6,7,8],false)
    assert_equal true, g.isomorphic_vf2(h)
    assert_equal true, h.isomorphic_vf2(g)
  end
  def test_isoclass
    g = IGraph.new([1,2,3,4],false)
    h = IGraph.new([5,6,7,8],false)
    assert g.isoclass >= 0 and g.isoclass <= 11
    assert_equal h.isoclass, g.isoclass
  end
  def test_isoclass_subgraph
    g = IGraph.new([1,2,3,4],false)
    assert g.isoclass_subgraph([1,2,3]) >= 0 and g.isoclass <= 4
    assert_equal g.isoclass_subgraph([1,2,3]), g.isoclass_subgraph([2,3,4])
  end
  def test_igraph_isoclass_create
    g = IGraph.new([1,2,3,4],false)
    h = IGraph.isoclass_create(4,g.isoclass,false)
    assert_equal g.isoclass, h.isoclass
  end
end
