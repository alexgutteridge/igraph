require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
   def test_graph_size
    assert_equal 4, IGraph.new([1,2,3,4],true).vcount
    assert_equal 2, IGraph.new([1,2,3,4],true).ecount
  end

  def test_eid_get_edge
    assert_nothing_raised do 
      IGraph.new(['A','B','C','D'],true).get_eid('A','B')
    end
    graph = IGraph.new(['A','B','C','D'],true)
    eid1  = graph.get_eid('A','B')
    eid2  = graph.get_eid('C','D') 
    assert_equal ['A','B'], graph.edge(eid1)
    assert_equal ['C','D'], graph.edge(eid2);
    assert_not_equal eid1,eid2
  end

  def test_neighbours
    assert_nothing_raised do
      IGraph.new(['A','B','C','D'],true).neighbors('A',IGraph::ALL)
    end
    graph = IGraph.new(['A','B','C','D'],true)
    assert_equal ['B'], graph.neighbors('A',IGraph::ALL)
    assert_equal ['D'], graph.neighbors('C',IGraph::ALL)    
  end

  def test_adjacent
    assert_nothing_raised do
      IGraph.new(['A','B','C','D'],true).adjacent('A',IGraph::ALL)
    end
    graph = IGraph.new(['A','B','C','D'],true)
    eid1  = graph.get_eid('A','B')
    eid2  = graph.adjacent('A',IGraph::ALL)[0]
    assert_equal eid1, eid2
  end

  def test_directed
    assert IGraph.new(['A','B','C','D'],true).is_directed?
    assert !(IGraph.new(['A','B','C','D'],false).is_directed?)
  end

  def test_degree
    graph = IGraph.new(['A','B','C','D'],true)
    assert_equal [1],  graph.degree(['A'],    IGraph::ALL,true)
    assert_equal [1,1],graph.degree(['A','B'],IGraph::ALL,true)
    assert_raises IGraphError do 
      graph.degree('A',IGraph::ALL,true)
    end
  end
end
