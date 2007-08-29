require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_shortest_paths
    graph = IGraph.new(['A','B','C','D'],true)
    m = graph.shortest_paths(['A'],IGraph::ALL)
    assert_equal 1, m[0][1]
    assert_equal 0, m[0][0]
    assert_equal graph.vcount, m[0].size
    assert_equal nil, m[0][2]
  end

  def test_get_shortest_paths
    graph = IGraph.new(['A','B','C','D'],true)    
    m = graph.get_shortest_paths('A',['B'],IGraph::ALL)
    assert_equal ['A','B'], m[0]
    #assert_equal [],        m[1]
  end

  def test_get_all_shortest_paths
    graph = IGraph.new(['A','B','A','C','B','D','C','D'],true)    
    m = graph.get_all_shortest_paths('A',['D'],IGraph::ALL)
    assert_equal 2, m.length
  end

  def test_average_path_length
    graph = IGraph.new(['A','B','A','C','B','D','C','D'],true)    
    m = graph.average_path_length(true,true)
    assert_equal 1.2, m
  end

  def test_diameter_girth
    graph = IGraph.new(['A','B','A','C','B','D'],true)    
    m = graph.diameter(true,true)
    assert_equal 3, m.length
    #assert_raises IGraphError do 
    #  graph.girth
    #end
    graph = IGraph.new(['A','B','A','C','B','D','C','D'],true)    
    assert_equal 4, graph.girth.length
  end

end
