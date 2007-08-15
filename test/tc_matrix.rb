require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_matrix
    m = IGraphMatrix.new([1,2],[3,4])
    assert_equal 1, m[0,0]
    assert_equal 3, m[1,0]
  end
  def test_set
    m = IGraphMatrix.new([1,2],[3,4])
    m[0,0] = 6
    assert_equal 6, m[0,0]
  end
  def test_prop
    m = IGraphMatrix.new([1,2],[3,4])
    assert_equal 4, m.size
    assert_equal 2, m.nrow
    assert_equal 2, m.ncol
    assert_equal 4, m.max
  end
  def test_op
    m = IGraphMatrix.new([1,2],[3,4])
    n = m * 2
    assert_equal 1, m[0,0]
    assert_equal 2, n[0,0]
  end
  def test_to_a
    m = IGraphMatrix.new([1,2],[3,4])
    assert_equal [[1,2],[3,4]], m.to_a
  end
end
