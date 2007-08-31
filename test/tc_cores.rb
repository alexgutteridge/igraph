require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_coreness
    g = IGraph.new(['A','B','C','D'],true)
    c = g.coreness(IGraph::ALL)
    assert_equal c.length, g.vcount
  end
end
