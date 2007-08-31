require 'test/unit'
require 'igraph'

class Array
  def mean
    total = 0.0
    self.each do |i|
      total += i
    end
    total / self.length.to_f
  end
end

class TestGraph < Test::Unit::TestCase
  def test_thrash
    g = IGraph.new([],true)
    v = 100
    v.times do |x|
      g.add_vertex(x)
    end
    200.times do |x|
      g.add_edge(rand(v/2),rand(v/2))
    end
    200.times do |x|
      g.add_edge(rand(v/2)+v/2,rand(v/2)+v/2)
    end    
    assert_equal v, g.vcount
    h,i = g.decompose(IGraph::WEAK,-1,2)

    p h.vcount
    p i.vcount

    p h.degree(h.to_a[20..30],IGraph::ALL,false).mean
    p h.degree(h.to_a[10..20],IGraph::ALL,false).mean

    p h.betweenness(h.to_a[20..30],true).mean
    p h.betweenness(h.to_a[10..20],true).mean

  end
end
 
