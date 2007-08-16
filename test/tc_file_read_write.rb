require 'test/unit'
require 'igraph'
require 'stringio'

class TestGraph < Test::Unit::TestCase
  def test_edgelist_read
    g = nil
    assert_nothing_raised{
      g = IGraph.read_graph_edgelist(StringIO.new("0 1 2 3"),true)
    }
    assert_instance_of IGraph, g
    assert_equal 4, g.vcount
    assert g.are_connected?(0,1)
  end

  def test_edgelist_write
    g = IGraph.new([0,1,2,3])
    s = StringIO.new("")
    str = g.write_graph_edgelist(s)
    s.rewind
    assert_equal "0 1\n2 3\n", s.read
  end

  def test_graphml_read
    g = nil
    g = IGraph.read_graph_graphml(StringIO.new(Graphml),0)
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
    str = g.write_graph_graphml(s)
    s.rewind
    assert_equal Graphml_out, s.read
  end

  def test_pajek_read_write
    g = nil
    g = IGraph.read_graph_pajek(StringIO.new(Pajek),0)
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

end
