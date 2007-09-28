require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
  def test_motifs_randesu
    g = IGraph.new(['A','B','C','D','A','C'],false)
    hist = g.motifs_randesu(3,[0,0,0])
    assert_equal [0,0,2,0], hist
  end
  def test_motifs_randesu_no
    g = IGraph.new(['A','B','C','D','A','C'],false)
    assert_equal 2, g.motifs_randesu_no(3,[0,0,0])    
  end
  def test_motifs_randesu_estimate
    g = IGraph.new(['A','B','C','D','A','C'],false)
    assert_equal 2, g.motifs_randesu_estimate(3,[0,0,0],4,nil)
    assert_equal 2, g.motifs_randesu_estimate(3,[0,0,0],0,['A','B','C','D'])
  end 
end
