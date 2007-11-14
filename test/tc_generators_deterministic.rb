require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_adjacency
    m = IGraphMatrix.new([0,1,1,0],[1,0,0,0],[1,0,0,1],[0,0,1,0])
     
    g = IGraph.adjacency(m,IGraph::ADJ_MAX)
    assert_equal 4, g.vcount
    assert_equal 3, g.ecount
  end

  def test_star
    g = IGraph.star(10,IGraph::STAR_UNDIRECTED,0)
    assert_equal 10, g.vcount
    assert_equal  9, g.ecount
  end

  def test_lattice
    g = IGraph.lattice([2,2],false,false,false)
    assert_equal 4, g.vcount
    assert_equal 4, g.ecount
  end

  def test_ring
    g = IGraph.ring(10,false,false,false)
    assert_equal 10, g.vcount
    assert_equal 9, g.ecount
  end

  def test_tree
    g = IGraph.tree(13,3,IGraph::TREE_UNDIRECTED)
    assert_equal 13, g.vcount
    assert_equal 12, g.ecount    
  end

  def test_full
    g = IGraph.full(10,false,false)
    assert_equal 10, g.vcount
    assert_equal 45, g.ecount    
  end

  def test_atlas
    g = IGraph.atlas(10)
    assert_equal 4, g.vcount
    assert_equal 2, g.ecount        
  end

  def test_extended_chordal_ring
    g = IGraph.extended_chordal_ring(3,IGraphMatrix.new([1,2,3],[1,2,3],[1,2,3]))
    assert_equal 3, g.vcount
    assert_equal 6, g.ecount        
  end

  def test_connect_neighborhood
    g = IGraph.new([1,2,1,3,3,4],false)
    g.connect_neighborhood(2,IGraph::ALL)
    assert g.are_connected?(2,3)
  end

end
