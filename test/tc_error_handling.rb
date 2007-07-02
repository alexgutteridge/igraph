require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_edge_fail
    assert_raise IGraphError do
      IGraph.new([1,2,3],true)
    end
  end
end
