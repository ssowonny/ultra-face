require 'rubygems'
require 'rake'
require 'rake/testtask'
require "rake/extensiontask"
require File.expand_path('../lib/ultra-face/version', __FILE__)

Rake::TestTask.new(:test) do |test|
  test.libs << 'lib' << 'test'
  test.pattern = 'test/**/*_test.rb'
end

spec = Gem::Specification.load('ultra-face.gemspec')
Rake::ExtensionTask.new('ultra_face_detector', spec)

namespace :test do
  Rake::TestTask.new(:lint) do |test|
    test.libs << 'lib' << 'test'
    test.pattern = 'test/test_active_model_lint.rb'
  end

  task :all => ['test', 'test:lint']
end

task :default => 'test:all'

desc 'Builds the gem'
task :build do
  sh "gem build ultra-face.gemspec"
end

desc 'Builds and installs the gem'
task :install => :build do
  sh "gem install ultra-face-#{UltraFace::VERSION}"
end

desc 'Tags version, pushes to remote, and pushes gem'
task :release => :build do
  sh "git tag v#{UltraFace::VERSION}"
  sh "git push origin master"
  sh "git push origin v#{UltraFace::VERSION}"
  sh "gem push ultra-face-#{UltraFace::VERSION}.gem"
end
