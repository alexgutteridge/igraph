require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_minimum_span_tree
    g = IGraph.new(['A','B','B','C','C','D','A','D'],true)
    st = g.minimum_spanning_tree_unweighted()
    assert_kind_of IGraph, st
    assert_equal ['A','B','C','D'], st.to_a.sort
  end
  def test_minimum_span_tree_prim
    g = IGraph.new(['A','B','B','C','C','D','A','D'],true)
    st = g.minimum_spanning_tree_prim([1,1,5,1])
    assert_kind_of IGraph, st
    assert_equal ['A','B','C','D'], st.to_a.sort
    assert  st.are_connected?('A','D')
    assert !st.are_connected?('C','D')
    st = g.minimum_spanning_tree_prim([1,1,1,5])
    assert_kind_of IGraph, st
    assert_equal ['A','B','C','D'], st.to_a.sort
    assert !st.are_connected?('A','D')
    assert  st.are_connected?('C','D')
  end 
end
