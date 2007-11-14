require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_modularity
    g = IGraph.new(['A','B','B','C','A','C','C','D','D','E','E','F','D','F'])
    assert_in_delta 0.357, g.modularity([['A','B','C'],['D','E','F']]), 0.001
  end
  def test_spinglass
    g = IGraph.new(['A','B','B','C','A','C','C','D','D','E','E','F','D','F'])
    groups,mod,temp = g.community_spinglass([],25,false,1,0.01,0.99,IGraph::SPINCOMM_UPDATE_SIMPLE,1.0)
    assert_in_delta 0.357, mod,  0.001
    assert_in_delta 0.200, temp, 0.100
    assert_equal [['A','B','C','D','E','F']], groups
    commun,coh,adh  = g.community_spinglass_single([],'A',25,IGraph::SPINCOMM_UPDATE_SIMPLE,1.0)
    assert_in_delta 1.25, coh,  0.001
    assert_in_delta(-2.5, adh, 0.100)
    assert_equal ['A','B','C'], commun    
  end
  def test_eigen
    g = IGraph.new(['A','B','B','C','A','C','C','D','D','E','E','F','D','F'],false)
    groups,merges = g.community_leading_eigenvector(6)
    assert_equal [['A','B','C'],['D','E','F']], groups
    assert_equal [[0,1]], merges.to_a
    groups,merges = g.community_leading_eigenvector_naive(6)
    assert_equal [['A','B','C'],['D','E','F']], groups
    assert_equal [[0,1]], merges.to_a

    groups,split,eigenvec,eigenval = g.community_leading_eigenvector_step([['A','B','C','D','E','F']],0)

  end
end
