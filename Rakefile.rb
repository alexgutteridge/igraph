require 'hoe'

$LOAD_PATH.unshift("./ext")

class IGraph
  VERSION = "0.9.5"
end

begin 
  require 'igraph'
rescue RuntimeError
end

hoe = Hoe.new("igraph",IGraph::VERSION) do |p|
  
  p.author = "Alex Gutteridge"
  p.email = "ag357@cam.ac.uk"
  p.url = "http://igraph.rubyforge.org/"
  
  p.description = p.paragraphs_of("README.txt",1..3)[0]
  p.summary     = p.paragraphs_of("README.txt",1)[0]
  p.changes     = p.paragraphs_of("History.txt",0..1).join("\n\n")
  
  p.clean_globs = ["ext/*.o","ext/*.so","ext/Makefile","ext/mkmf.log","**/*~","email.txt","manual.{aux,log,out,toc,pdf}"]
  
  p.rdoc_pattern = /(^ext\/.*\.c$|^README|^History|^License)/
  
  p.spec_extras = {
    :extensions    => ['ext/extconf.rb'],
    :require_paths => ['test'],
    :has_rdoc      => true,
    :extra_rdoc_files => ["README.txt","History.txt","License.txt"],
    :rdoc_options  => ["--exclude", "test/*", "--main", "README.txt", "--inline-source"]
  }

end
  
hoe.spec.dependencies.delete_if{|dep| dep.name == "hoe"}

IGRAPH = '/usr/local/include/igraph'

desc "Uses extconf.rb and make to build the extension"
task :build_extension => ['ext/igraph.so']
SRC = FileList['ext/*.c'] + FileList['ext/*.h']
file 'ext/igraph.so' => SRC do
  Dir.chdir('ext')
  system("ruby extconf.rb --with-igraph-include=#{IGRAPH}")
  system("make")
  Dir.chdir('..')
end

task :test => [:build_extension]
