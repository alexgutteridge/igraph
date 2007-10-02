require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_grg
    g = IGraph.grg_game(10,0.1,false)
    assert_equal 10, g.vertices.size
  end
  def test_barabasi
    g = IGraph.barabasi_game(10,3,false,false)
    assert_equal 10, g.vertices.size
  end
  def test_nonlinear_barabasi
    g = IGraph.nonlinear_barabasi_game(10,1.9,3,false,0.1,false)
    assert_equal 10, g.vertices.size
  end 
  def test_erdos_renyi
    g = IGraph.erdos_renyi_game(IGraph::ERDOS_RENYI_GNP,10,0.5,false,false)
    assert_equal 10, g.vertices.size    
    g = IGraph.erdos_renyi_game(IGraph::ERDOS_RENYI_GNM,10,0.5,false,false)
    assert_equal 10, g.vertices.size    
  end
  def test_watts_strogatz
    
  end
end
