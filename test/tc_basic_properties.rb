require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_are_connected
    graph = IGraph.new(['A','B','B','C','C','D'],true)
    assert graph.are_connected('A','B')
    assert !(graph.are_connected('A','C'))
  end
end
