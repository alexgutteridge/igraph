require 'hoe'

$LOAD_PATH.unshift("./lib")
$LOAD_PATH.unshift("./ext")

hoe = Hoe.new("igraph",'0.1') do |p|
  
  p.author = "Alex Gutteridge"
  p.email = "alexg@kuicr.kyoto-u.ac.jp"
  p.url = "http://web.kuicr.kyoto-u.ac.jp/~alexg/igraph/"
  
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

  task :setup_rb_package => [:clean, :package, :build_manual] do
    
    package_dir = "#{p.name}-#{p.version}"
    cp("setup.rb","pkg/#{package_dir}")
    cp("manual.pdf","pkg/#{package_dir}")
    
    Dir.chdir("pkg")
    system("tar -czf #{p.name}-#{p.version}.tgz #{package_dir}")
    Dir.chdir("..")

  end

end
  
hoe.spec.dependencies.delete_if{|dep| dep.name == "hoe"}

desc "Uses extconf.rb and make to build the extension"
task :build_extension => ['ext/igraph.so']
SRC = FileList['ext/*.c'] + FileList['ext/*.h']
file 'ext/igraph.so' => SRC do
  Dir.chdir('ext')
  system("ruby extconf.rb --with-igraph-dir=$IGRAPH_HOME")
  system("make")
  Dir.chdir('..')
end

task :test => [:build_extension]

desc "Build PDF manual"
task :build_manual => ["manual.pdf"]
file "manual.pdf" => ["manual.tex"] do
  out = 'Rerun'
  while out.match(/Rerun/)
    out = `pdflatex manual.tex`
  end  
end

task :build_manual_clean => [:build_manual] do
  system("rm manual.{aux,log,out,toc}")
end
