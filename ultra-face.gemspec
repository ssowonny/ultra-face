# -*- encoding: utf-8 -*-
$:.push File.expand_path("../lib", __FILE__)
require "ultra-face/version"

Gem::Specification.new do |s|
  s.name        = 'ultra-face'
  s.version     = UltraFace::VERSION
  s.date        = '2013-09-12'
  s.summary     = "Detect Facial Features"
  s.description = "Detect Facial Features"
  s.authors     = ["Sungwon Lee"]
  s.email       = 'ssowonny@ultracaption.net'
  s.homepage    = 'http://rubygems.org/gems/ultra-face'
  s.license     = 'MIT'

  s.files       = Dir.glob("ext/**/*.{h,c,cpp,rb,a}") +
                  Dir.glob("lib/**/*.rb") +
                  Dir.glob("vendor/*")
  s.test_files    = `git ls-files -- {test,spec,features}/*`.split("\n")
  s.executables   = `git ls-files -- bin/*`.split("\n").map{ |f| File.basename(f) }
  s.extensions << "ext/ultra_face_detector/extconf.rb"
  s.add_development_dependency "rake-compiler"

  s.require_paths = ['lib']
end
