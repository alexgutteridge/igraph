require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_trans
    g = IGraph.new(['A','B','A','C','A','D','B','C'],true)
    assert_equal 0.6, g.transitivity
  end
  def test_trans_local
    g = IGraph.new(['A','B','A','C','A','D','B','C'],true)
    assert_in_delta 0.33, g.transitivity_local(['A'])[0], 0.01   
  end
  def test_trans_avr_local
    g = IGraph.new(['A','B','A','C','A','D','B','C'],true)
    assert_equal 0.6, g.transitivity
  end
end
