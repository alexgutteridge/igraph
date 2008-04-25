require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
   def test_cliques
     g = IGraph.new([1,2,3,4],false)
     assert_equal [[1,2],[3,4]], g.cliques(2,0)
   end
   def test_largest_cliques
     g = IGraph.new(['A','B','C','D','A','E','B','E'],false)
     assert_equal [['A','B','E']], g.largest_cliques()
   end
   def test_maximal_cliques
     g = IGraph.new(['A','B','C','D','A','E','B','E'],false)
     assert_equal [['A','B','E'],['C','D']], g.maximal_cliques()
   end
   def test_clique_number
     g = IGraph.new(['A','B','C','D','A','E','B','E'],false)
     assert_equal 3, g.clique_number
   end
 end
