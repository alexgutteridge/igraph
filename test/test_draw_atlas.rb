require 'igraph'
require 'cairo'

gs = []
vs = []
ARGV[0].to_i.times do |n|
  gs << IGraph.atlas(n+1)
  vs += gs[-1].vertices
end

ls = gs.map{|g| g.layout_fruchterman_reingold(10,1,1,2,1,false)}

layout = IGraph.layout_merge_dla(gs,ls).to_a

format = Cairo::FORMAT_ARGB32
width = 1000
height = 1000

surface = Cairo::ImageSurface.new(format, width, height)
cr = Cairo::Context.new(surface)

# fill background with white
cr.set_source_rgba(1.0, 1.0, 1.0, 0.8)
cr.paint

max_x = layout.map{|a| a[0]}.max
min_x = layout.map{|a| a[0]}.min
max_y = layout.map{|a| a[1]}.max
min_y = layout.map{|a| a[1]}.min

x_var = max_x - min_x
y_var = max_y - min_y

max_var = [x_var,y_var].max

layout.each_with_index do |a,i|
  x,y = *a

  x = (x - min_x)/max_var
  y = (y - min_y)/max_var
  x *= (width)
  y *= (height)

  layout[i] = [x,y]

  puts "#{x} #{y}"

end

vn = 1
gi = 0
g  = gs[gi]

layout.each_with_index do |a,i|

  sub_layout = layout[start..finish]

  v = vs[i]
  x,y = *a
  if vn > g.vcount
    gi += 1
    g = gs[gi]
    vn = 1
  end

  puts "Looking for: " + v.inspect
  puts "In: #{g.to_a.inspect}"
  puts "Graph number: #{gi}"
  puts "Vertex number: #{vn}"

  vn += 1

  cr.set_source_rgba(0,0,0,0.5)
  cr.circle(x,y,10).fill

  g.adjacent_vertices(v,IGraph::OUT).each do |w|
    cr.move_to(x,y)
    wx,wy = *layout[vs.index(w)]
    cr.line_to(wx,wy)
    cr.stroke
  end

end

cr.target.write_to_png("test.png")
