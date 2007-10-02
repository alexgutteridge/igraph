require 'igraph'
require 'cairo'

g = IGraph.barabasi_game(100,3,false,false)

vertices = g.to_a
layout   = g.send(ARGV.shift.to_sym,*ARGV.map{|a| eval(a)}).to_a

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

layout.each_with_index do |a,i|

  v = vertices[i]
  x,y = *a

  cr.set_source_rgba(rand,rand,rand,0.5)
  cr.circle(x,y,1).fill

  g.adjacent_vertices(v,IGraph::OUT).each do |w|
    cr.move_to(x,y)
    wx,wy = *layout[vertices.index(w)]
    cr.line_to(wx,wy)
    cr.stroke
  end

end

cr.target.write_to_png("test.png")
