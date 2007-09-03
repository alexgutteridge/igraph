require 'test/unit'
require 'igraph'

class TestGraph < Test::Unit::TestCase
   def test_density
     g = IGraph.new(['A','B','C','D'],true)
     assert_equal 0.125, g.density(true)
   end
   def test_simplify
     g = IGraph.new(['A','B','A','B','C','D','A','A'],true)
     assert_equal 4, g.ecount
     g.simplify(true,false)
     assert_equal 3, g.ecount
     g.simplify(true,true)
     assert_equal 2, g.ecount
   end
   def test_reciprocity
     g = IGraph.new(['A','B','C','D','B','A'],true)
     assert_equal 0.5, g.reciprocity(true)
   end
   def test_bibcoupling
     g = IGraph.new(['A','B','C','D','D','B'],true)
     assert_equal [[0,0,0,1]], g.bibcoupling(['A'])     
   end
   def test_cocitation
     g = IGraph.new(['A','B','C','D','A','D'],true)
     assert_equal [[0,0,0,1]], g.cocitation(['B'])     
   end
   def test_get_adjacency
     g = IGraph.new(['A','B','C','D'],true)
     assert_equal [[0,1,0,0],[0,0,0,0],[0,0,0,1],[0,0,0,0]], g.get_adjacency(1)
   end
 end
 
