require 'igraph'
require 'kgml'
require 'benchmark'

xmlfiles = Dir.glob(ARGV[0])
pathway = 
  DirectedBipartiteMetabolicPathway.new(File.open(xmlfiles.shift).read)
pathway = xmlfiles.inject(pathway){|p,file|
  p + DirectedBipartiteMetabolicPathway.new(File.open(file).read)
}

igraph = IGraph.new([],0,true);

pathway.reactions.each do |reaction|
  reaction_name = reaction.name.gsub(/rn:/,'')
  unless igraph.include?(reaction_name)
    igraph.add_vertices([reaction_name])
  end
  reaction.substrates.each do |s_name|
    s_name.gsub!(/cpd:/,'')
    unless igraph.include?(s_name)
      igraph.add_vertices([s_name])
    end 
    igraph.add_edges([s_name,reaction_name])
    if reaction.type == 'reversible'
      igraph.add_edges([reaction_name,s_name])
    end
  end
  reaction.products.each do |p_name|
    p_name.gsub!(/cpd:/,'')
    unless igraph.include?(p_name)
      igraph.add_vertices([p_name])
    end
    igraph.add_edges([reaction_name,p_name])
    if reaction.type == 'reversible'
      igraph.add_edges([p_name,reaction_name])
    end
  end
end

include Benchmark

graph = pathway.graph

bm(6) do |x|
  x.report("igraph"){ 
    100.times{
      igraph.get_shortest_paths("R02740",["C00074"])
    }
  }
  x.report("hashgraph"){ 
    100.times{
      h = {}
      graph.dijkstra_shortest_paths({
                                      :root   => "R02740",
                                      :target => "C00074",
                                      :predecessor => h
                                    })
      graph.route("C00074",h)
    }
  }
end
