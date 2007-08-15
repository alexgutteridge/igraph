require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_top_sort
    g = IGraph.new([1,2,2,3,3,4])
    assert_equal [1,2,3,4], g.topological_sorting(IGraph::OUT)
  end
  
end
