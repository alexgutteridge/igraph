require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_each_vertex
    graph = IGraph.new(['A','B','C','D'],true)
    assert_nothing_raised do
      graph.each_vertex do |v|
      end
    end
    assert_nothing_raised do
      graph.each do |v|
      end
    end
  end

  def test_each_edge
    graph = IGraph.new(['A','B','C','D'],true)
    assert_nothing_raised do
      graph.each_edge(IGraph::EDGEORDER_ID) do |v,w|
      end
    end
    assert_nothing_raised do
      graph.each_edge_eid(IGraph::EDGEORDER_ID) do |v|
      end
    end   
    edges = []
    graph.each_edge(IGraph::EDGEORDER_ID){|v,w| edges.push([v,w])}
    assert_equal [['A','B'],['C','D']], edges
    edges = []
    graph.each_edge_eid(IGraph::EDGEORDER_ID){|v| edges.push(v)}
    assert_equal [0,1], edges    
  end

  def test_enumerable
    graph = IGraph.new(['A','B','C','D'],true)
    assert graph.all?{|v| v.kind_of? String}
    assert graph.any?{|v| v == 'B'}
    assert_equal ['A','B','C','D'], graph.collect{|v| v}
    assert graph.detect(Proc.new{true}){|v| }
    assert_equal ['A'], graph.find_all{|v| v  < 'B'}
  end

end
