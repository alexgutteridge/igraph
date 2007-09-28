require 'igraph'

g = IGraph.new(['A','B','A','C','A','D','C','D'],false)

adj = g.get_adjacency(2)
g.vcount.times do |k|
  adj[k][k] = 1
end

dis = Array.new(4){|i| Array.new(4)}

dis.each_with_index do |row,i|
  row.each_with_index do |cell,j|
    cij = 0
    g.vcount.times do |k|
      puts "#{i} #{j} #{k}"
      cij += adj[i][k] * adj[j][k]
    end
    dis[i][j] = 10 - cij
  end
end

p adj
p dis
