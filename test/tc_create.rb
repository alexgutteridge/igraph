require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase

  def test_graph
    assert_nothing_raised{IGraph.new([],true)}
    assert_nothing_raised{IGraph.new(['A','B','C','D'],true)}
  end

end
