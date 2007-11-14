require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_rewire_edges
    g = IGraph.grg_game(10,0.1,false)
    h = g.rewire_edges(0.5)
    assert_equal 10, h.to_a.size
  end
  def test_rewire
    g = IGraph.grg_game(10,0.1,false)
    h = g.rewire(0.5)
    assert_equal 10, h.to_a.size
  end
end
