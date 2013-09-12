# -*- encoding: utf-8 -*-
$:.push File.expand_path("../lib", __FILE__)
require "ultra_face/version"

Gem::Specification.new do |s|
  s.name        = 'ultra-face'
  s.version     = UltraFace::VERSION
  s.date        = '2013-09-12'
  s.summary     = "Detect Facial Features"
  s.description = "Detect Facial Features"
  s.authors     = ["Sungwon Lee"]
  s.email       = 'ssowonny@ultracaption.net'
  s.homepage    = 'http://rubygems.org/gems/ultra_face'
  s.license     = 'MIT'

  s.files         = `git ls-files`.split("\n")
  s.test_files    = `git ls-files -- {test,spec,features}/*`.split("\n")
  s.executables   = `git ls-files -- bin/*`.split("\n").map{ |f| File.basename(f) }
  s.require_paths = ['lib']
end
