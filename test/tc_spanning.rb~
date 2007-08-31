require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_closeness
    g = IGraph.new(['A','B','B','C','C','D'],true)
    assert_equal [0.75], g.closeness(['B'],IGraph::ALL)
  end
  def test_betweenness
    g = IGraph.new(['A','B','B','C','C','D'],true)
    assert_equal [0,2], g.betweenness(['A','B'],true)
  end
  def test_edge_betweenness
    g = IGraph.new(['A','B','C','D'],true)
    assert_equal [1,1], g.edge_betweenness(true)
  end    
  def test_pagerank
    g = IGraph.new(['A','B','C','D','E','B','F','B'],true)
    assert_equal 67, (g.pagerank(['B'],true,100,0.01,0.8)[0] * 100).to_i
  end 
  def test_constraint
    g = IGraph.new(['A','B','C','D'],true)
    assert_equal [1], g.constraint(['A'])    
    assert_raises IGraphError do
      g.constraint(['A'],[3])    
    end
    assert_equal [1], g.constraint(['A'],[2,3])    
  end
  def test_maxdegree
    g = IGraph.new(['A','B','C','D','A','E','A','F'],true)
    assert_equal 3, g.maxdegree(g.vertices,IGraph::ALL,true)        
  end
end
