require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_grg
    g = IGraph::GenerateRandom.grg_game(10,0.1,false)
    assert_equal 10, g.vertices.size
  end
  def test_barabasi
    g = IGraph::GenerateRandom.barabasi_game(10,3,false,false)
    assert_equal 10, g.vertices.size
  end
  def test_nonlinear_barabasi
    g = IGraph::GenerateRandom.nonlinear_barabasi_game(10,1.9,3,false,0.1,false)
    assert_equal 10, g.vertices.size
  end 
  def test_erdos_renyi
    g = IGraph::GenerateRandom.erdos_renyi_game(IGraph::ERDOS_RENYI_GNP,10,0.5,false,false)
    assert_instance_of IGraph, g    
    g = IGraph::GenerateRandom.erdos_renyi_game(IGraph::ERDOS_RENYI_GNM,10,0.5,false,false)
    assert_instance_of IGraph, g
  end
  def test_watts_strogatz
    g = IGraph::GenerateRandom.watts_strogatz_game(10,1,2,0.6)
    assert_instance_of IGraph, g
  end
  def test_degree_sequence_game
    g = IGraph::GenerateRandom.degree_sequence_game([1,2,3],[1,2,3])
    assert_instance_of IGraph, g
  end
  def test_growing_random_game
    assert_instance_of IGraph, IGraph::GenerateRandom.growing_random_game(10,2,true,true)
  end
  def test_callaway_traits_game
    assert_instance_of IGraph, 
    IGraph::GenerateRandom.callaway_traits_game(30,4,2,[0.25,0.25,0.25,0.25],
                                IGraphMatrix.new([0,0.5,0.25,0.25],
                                                 [0.5,0,0.25,0.25],
                                                 [0.5,0.25,0,0.25],
                                                 [0.5,0.25,0.25,0]),true)
  end    
  def test_establishment_game
    assert_instance_of IGraph, 
    IGraph::GenerateRandom.establishment_game(30,4,2,[0.25,0.25,0.25,0.25],
                              IGraphMatrix.new([0,0.5,0.25,0.25],
                                               [0.5,0,0.25,0.25],
                                               [0.5,0.25,0,0.25],
                                               [0.5,0.25,0.25,0]),true)
  end    
  def test_preference_game
    assert_instance_of IGraph, 
    IGraph::GenerateRandom.preference_game(30,4,[0.25,0.25,0.25,0.25],
                           IGraphMatrix.new([0,0.5,0.25,0.25],
                                            [0.5,0,0.25,0.25],
                                            [0.5,0.25,0,0.25],
                                            [0.5,0.25,0.25,0]),true,false)
  end      
  def test_asymmetric_preference_game
    assert_instance_of IGraph, 
    IGraph::GenerateRandom.asymmetric_preference_game(30,4,
                                      IGraphMatrix.new([0,0.5,0.25,0.25],
                                                       [0.5,0,0.25,0.25],
                                                       [0.5,0.25,0,0.25],
                                                       [0.5,0.25,0.25,0]),
                                      IGraphMatrix.new([0,0.5,0.25,0.25],
                                                       [0.5,0,0.25,0.25],
                                                       [0.5,0.25,0,0.25],
                                                       [0.5,0.25,0.25,0]),
                                      false)
  end        
  def test_recent_degree_game
    assert_instance_of IGraph, 
    IGraph::GenerateRandom.recent_degree_game(30,2,4,5,false,0.1,true)
  end          
  def test_barabasi_aging_game
    assert_instance_of IGraph, 
    IGraph::GenerateRandom.barabasi_aging_game(30,2,true,0.9,-0.5,3,0.1,0.1,2,2,true)
  end 
  def test_recent_degree_aging_game
    assert_instance_of IGraph, 
    IGraph::GenerateRandom.recent_degree_aging_game(30,2,true,0.9,-0.5,3,4,0.1,true)
  end   
  def test_cited_type_game
    assert_instance_of IGraph, 
    IGraph::GenerateRandom.cited_type_game(10,(0..9).to_a,
                           Array.new(5,0.5)+Array.new(5,0.2),
                           2,true)
  end
  def test_citing_cited_type_Game
  #  assert_instance_of IGraph, 
  #  IGraph::GenerateRandom.citing_cited_type_game(4,(0..3).to_a,
  #                                IGraphMatrix.new([0,0.5,0.25,0.25],
  #                                                 [0.5,0,0.25,0.25],
  #                                                 [0.5,0.25,0,0.25],
  #                                                 [0.5,0.25,0.25,0]),
  #                                1,
  #                                true)
  end
  
end
