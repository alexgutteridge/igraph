require 'test/unit'
require 'igraph'
require 'stringio'
require 'rbconfig'
include Config

class TestGraph < Test::Unit::TestCase
  def test_edgelist_read
    g = nil
    if CONFIG['host'] =~ /apple/
      assert_raises(NoMethodError){
        IGraph::FileRead.read_graph_edgelist(StringIO.new("0 1 2 3"),true)
      }
      return
    end
    assert_nothing_raised{
      g = IGraph::FileRead.read_graph_edgelist(StringIO.new("0 1 2 3"),true)
    }
    assert_instance_of IGraph, g
    assert_equal 4, g.vcount
    assert g.are_connected?(0,1)
  end

  def test_edgelist_write
    g = IGraph.new([0,1,2,3])
    s = StringIO.new("")
    if CONFIG['host'] =~ /apple/
      assert_raises(NoMethodError){
        g.write_graph_edgelist(s)
      }
      return
    end
    str = g.write_graph_edgelist(s)
    s.rewind
    assert_equal "0 1\n2 3\n", s.read
  end

  def test_ncol_read
    g = nil
    if CONFIG['host'] =~ /apple/
      assert_raises(NoMethodError){
        IGraph::FileRead.read_graph_ncol(StringIO.new("0 1\n2 3\n"),[],
  				  false,false,false)
      }
      return
    end
    assert_nothing_raised{
      g = IGraph::FileRead.read_graph_ncol(StringIO.new("0 1\n2 3\n"),[],
				  false,false,false)
    }
    assert_instance_of IGraph, g
    assert_equal 4, g.vcount
    assert g.are_connected?(0,1)   

    assert_nothing_raised{
      g = IGraph::FileRead.read_graph_ncol(StringIO.new("A B\nC D\n"),[],
				  true,false,false)
    }
    assert_instance_of IGraph, g
    assert_equal 4, g.vcount
    assert g.are_connected?('A','B')   
    
    assert_nothing_raised{
      g = IGraph::FileRead.read_graph_ncol(StringIO.new("A B 1\nC D 2\n"),[],
				 true,true,false)
    }
    assert_instance_of IGraph, g
    assert_equal 4, g.vcount
    assert g.are_connected?('A','B')       
    assert_equal 1, g['A','B']
  end

  def test_ncol_write
    g = IGraph.new(["A","B","C","D"],true,[1,2])
    s = StringIO.new("")
    if CONFIG['host'] =~ /apple/
       assert_raises(NoMethodError){
         g.write_graph_ncol(s,true,true)
       }
       return
    end
    str = g.write_graph_ncol(s,true,true)
    s.rewind
    assert_equal "A B 1.0\nC D 2.0\n", s.read
  end

  def test_lgl_read
    g = nil
    if CONFIG['host'] =~ /apple/
       assert_raises(NoMethodError){
         IGraph::FileRead.read_graph_lgl(StringIO.new("#A\nB\n#C\nD\n"))
       }
       return
    end
    assert_nothing_raised{
      g = IGraph::FileRead.read_graph_lgl(StringIO.new("#A\nB\n#C\nD\n"),
				 false,false)
    }
    assert_instance_of IGraph, g
    assert_equal 4, g.vcount
    assert g.are_connected?(0,1)   

    assert_nothing_raised{
      g = IGraph::FileRead.read_graph_lgl(StringIO.new("#A\nB 1\n#C\nD 1\n"),
				 true,true)
    }
    assert_instance_of IGraph, g
    assert_equal 4, g.vcount
    assert g.are_connected?('A','B')       
    assert_equal 1, g['A','B']
  end

  def test_lgl_write
    g = IGraph.new(["A","B","C","D"],true,[1,2])
    s = StringIO.new("")
    if CONFIG['host'] =~ /apple/
       assert_raises(NoMethodError){
         g.write_graph_lgl(s,true,true,false)
       }
       return
    end
    str = g.write_graph_lgl(s,true,true,false)
    s.rewind
    assert_equal "# A\nB 1.0\n# C\nD 2.0\n", s.read
  end

  def test_dimacs_read
    s = StringIO.new("c com\np min 4 2\nn 1 s\nn 2 t\na 1 2 1\na 3 4 2\n")
    g = nil
    if CONFIG['host'] =~ /apple/
       assert_raises(NoMethodError){
         IGraph::FileRead.read_graph_dimacs(s,false)
       }
       return
    end
    assert_nothing_raised{
      g = IGraph::FileRead.read_graph_dimacs(s,
				   false)
    }
    assert_instance_of IGraph, g
    assert_equal 4, g.vcount
    assert g.are_connected?(0,1)   
    assert_equal 0, g.attributes['source']
    assert_equal 1, g.attributes['target']
    assert_equal [1,2], g.attributes['capacity']
  end

  def test_dimacs_write
    g = IGraph.new(["A","B","C","D"],true,[1,2])
    s = StringIO.new("")
    if CONFIG['host'] =~ /apple/
       assert_raises(NoMethodError){
         g.write_graph_dimacs(s,0,1,[1,2])
       }
       return
    end
    str = g.write_graph_dimacs(s,0,1,[1,2])
    s.rewind
    assert_equal "c created by igraph\np max 4 2\nn 1 s\nn 2 t\na 1 2 1\na 3 4 2\n", s.read
  end

  def test_graphml_read
    if CONFIG['host'] =~ /apple/
       assert_raises(NoMethodError){
         IGraph::FileRead.read_graph_graphml(StringIO.new(Graphml),0)
       }
       return
    end
    g = nil
    g = IGraph::FileRead.read_graph_graphml(StringIO.new(Graphml),0)
    assert_instance_of IGraph, g
    assert_equal '2006-11-12', g.attributes['date']
    h = g.dup
    assert_equal g.to_a,h.to_a
    assert_equal g.attributes['date'], h.attributes['date']
  end

  def test_graphml_write
    g = IGraph.new([{'id'=>0,'name'=>'a','type' => 4.0},
                    {'id'=>1,'name'=>'b','type' => 5},
                    {'id'=>2,'type' => 6},
                    {'id'=>3,'name'=>'d'}],
                   true,
                   [{'eid'=>'e1'},
                    {'eid'=>'e2'}])
    g.attributes['date'] = 'Friday'
    s = StringIO.new("")
    if CONFIG['host'] =~ /apple/
       assert_raises(NoMethodError){
         g.write_graph_graphml(s)
       }
       return
    end
    str = g.write_graph_graphml(s)
    s.rewind
    assert_equal Graphml_out, s.read
  end

  def test_gml_read
    if CONFIG['host'] =~ /apple/
       assert_raises(NoMethodError){
         IGraph::FileRead.read_graph_gml(StringIO.new(Gml))
       }
       return
    end
    g = IGraph::FileRead.read_graph_gml(StringIO.new(Gml))
    assert_instance_of IGraph, g
  end

  def test_gml_write
    g = IGraph.new([{'id'=>0,'name'=>'a','type' => 4.0},
                    {'id'=>1,'name'=>'b','type' => 5},
                    {'id'=>2,'type' => 6},
                    {'id'=>3,'name'=>'d'}],
                   true,
                   [{'eid'=>'e1'},
                    {'eid'=>'e2'}])
    g.attributes['date'] = 'Friday'
    s = StringIO.new("")
    if CONFIG['host'] =~ /apple/
       assert_raises(NoMethodError){
         g.write_graph_gml(s)
       }
       return
    end
    str = g.write_graph_gml(s)
    s.rewind
    s = s.read
    s = s.split(/\n/)[1..-1].join("\n")
    assert_equal Gml_out, s
  end

  def test_pajek_read_write
    if CONFIG['host'] =~ /apple/
       assert_raises(NoMethodError){
         IGraph::FileRead.read_graph_pajek(StringIO.new(Pajek),0)
       }
       return
    end
    g = nil
    g = IGraph::FileRead.read_graph_pajek(StringIO.new(Pajek),0)
    assert_instance_of IGraph, g
    assert_equal 4, g.vcount
    assert_equal 1, g[4,1]['weight']
    h = g.dup
    s = StringIO.new('')
    h.write_graph_pajek(s)
    s.rewind
    str = s.read
    str.gsub!(/\r/,'')
    #assert_equal Pajek, str
  end

  Graphml = %q{<?xml version="1.0" encoding="UTF-8"?>
<!-- This file was written by the JAVA GraphML Library.-->
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"  xmlns:xsi="http://www.w3
.org/2001/XMLSchema-instance"
xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns http://graphml.graphdr
awing.org/xmlns/1.0/graphml.xsd">
  <key id="d0" for="node" attr.name="color" attr.type="string">yellow</key>
  <key id="d1" for="edge" attr.name="weight" attr.type="double"/>
  <key id="d2" for="graph" attr.name="date" attr.type="string"></key>
  <graph id="G" edgedefault="undirected">
    <data key="d2">2006-11-12</data>
    <node id="n0">
      <data key="d0">green</data>
      <data key="d3">incorrect</data>
      <!-- incorrect attribute key, should issue a warning -->
    </node>
    <node id="n1"/>
    <node id="n2">
      <data key="d0">blue</data>
    </node>
    <node id="n3">
      <data key="d0">red</data>
    </node>
    <node id="n4"/>
    <node id="n5">
      <data key="d0">turquoise</data>
    </node>
    <edge id="e0" source="n0" target="n2">
      <data key="d1">1.0</data>
    </edge>
    <edge id="e1" source="n0" target="n1">
      <data key="d1">1.0</data>
    </edge>
    <edge id="e2" source="n1" target="n3">
      <data key="d1">2.0</data>
    </edge>
    <edge id="e3" source="n3" target="n2"/>
    <edge id="e4" source="n2" target="n4"/>
    <edge id="e5" source="n3" target="n5"/>
    <edge id="e6" source="n5" target="n4">
      <data key="d1">1.1</data>
    </edge>
  </graph>
</graphml>
}


  Graphml_out = %q{<?xml version="1.0" encoding="UTF-8"?>
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
         http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
<!-- Created by igraph -->
  <key id="date" for="graph" attr.name="date" attr.type="string"/>
  <key id="name" for="node" attr.name="name" attr.type="string"/>
  <key id="type" for="node" attr.name="type" attr.type="double"/>
  <key id="id" for="node" attr.name="id" attr.type="double"/>
  <key id="eid" for="edge" attr.name="eid" attr.type="string"/>
  <graph id="G" edgedefault="directed">
    <data key="date">Friday</data>
    <node id="n0">
      <data key="name">a</data>
      <data key="type">4</data>
      <data key="id">0</data>
    </node>
    <node id="n1">
      <data key="name">b</data>
      <data key="type">5</data>
      <data key="id">1</data>
    </node>
    <node id="n2">
      <data key="name"></data>
      <data key="type">6</data>
      <data key="id">2</data>
    </node>
    <node id="n3">
      <data key="name">d</data>
      <data key="id">3</data>
    </node>
    <edge source="n0" target="n1">
      <data key="eid">e1</data>
    </edge>
    <edge source="n2" target="n3">
      <data key="eid">e2</data>
    </edge>
  </graph>
</graphml>
}

  Pajek = %q{*Vertices 4
*Edges
4 1 1
1 2 4
1 3 2
2 3 2
}

  Gml = %q{graph [
    comment "This is a sample graph"
    directed 1
    id 42
    label "Hello, I am a graph"
    node [
        id 1
        label "Node 1"
        thisIsASampleAttribute 42
    ]
    node [
        id 2
        label "node 2"
        thisIsASampleAttribute 43
    ]
    node [
        id 3
        label "node 3"
        thisIsASampleAttribute 44
    ]
    edge [
        source 1
        target 2
        label "Edge from node 1 to node 2"
    ]
    edge [
        source 2
        target 3
        label "Edge from node 2 to node 3"
    ]
    edge [
        source 3
        target 1
        label "Edge from node 3 to node 1"
    ]
]
}

  Gml_out = %q{Version 1
graph
[
  directed 0
  date "Friday"
  node
  [
    id 0
    name "a"
    type 4
  ]
  node
  [
    id 1
    name "b"
    type 5
  ]
  node
  [
    id 2
    name ""
    type 6
  ]
  node
  [
    id 3
    name "d"
    type nan
  ]
  edge
  [
    source 0
    target 1
    eid "e1"
  ]
  edge
  [
    source 2
    target 3
    eid "e2"
  ]
]}

end
