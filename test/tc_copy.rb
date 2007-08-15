require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_copy
    g = IGraph.new(['A','B','C','D'],true,[1,2]);
    h = g.dup
    assert g.vcount == h.vcount
    assert g['A','B'] == h['A','B']
    h['A','B'] = g['A','B'] + 1
    assert g['A','B'] != h['A','B']
  end
end
