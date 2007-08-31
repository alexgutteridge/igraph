require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_laplacian
    g = IGraph.new(['A','B','C','D'],false)
    m = g.laplacian(false)
    assert_equal m.length, g.vcount
    m = g.laplacian(true)
    assert_equal m.length, g.vcount
  end
end
