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
    assert_in_delta 0.25, mod,  0.15
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

    groups,split,eigenvec,eigenval = 
      g.community_leading_eigenvector_step([['A','B','C','D','E','F']],0)
    assert_equal [['A','B','C'],['D','E','F']], groups
    assert split
    assert_in_delta 0.433, eigenvec[0], 0.02
    assert_in_delta 1.730, eigenval,    0.02

  end

  def test_random_walk
    g = IGraph.new(['A','B','B','C','A','C','C','D','D','E','E','F','D','F'],false)
    merges,modularity = g.community_walktrap([],10)
    groups = g.community_to_membership(merges,4,6)
    assert_equal [['A','B','C'],['D','E','F']], groups.sort
    assert_in_delta 0.19, modularity[3], 0.1
  end

  def test_comm_edge_betweenness

    g = IGraph.new(['A','B','B','C','A','C','C','D','D','E','E','F','D','F'],false)
    merges,result,edge_betw,bridges = g.community_edge_betweenness(false)    
    groups = g.community_to_membership(merges,4,6)
    assert_equal [['A','B','C'],['D','E','F']], groups.sort
    assert_equal 3, result[0]
    assert_equal 9, edge_betw[0]
    assert_equal 7, bridges[0]

    merges,bridges = g.community_eb_get_merges(result)
    groups = g.community_to_membership(merges,4,6)
    assert_equal [['A','B','C'],['D','E','F']], groups.sort
    assert_equal 7, bridges[0]    
    
  end

  def test_fastgreedy
    g = IGraph.new(['A','B','B','C','A','C','C','D','D','E','E','F','D','F'],false)
    merges,mod = g.community_fastgreedy
    groups = g.community_to_membership(merges,4,6)
    assert_equal [['A','B','C'],['D','E','F']], groups.sort
    assert_in_delta 0.19, mod[3], 0.1
  end

end
