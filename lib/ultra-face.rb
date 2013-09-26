require 'ultra-face/version'
require 'ultra-face/base'
require 'ultra-face/detector'

module UltraFace
  def self.setup
    yield self
    Detector.setup(@@flandmark_data_path, @@haarcascade_data_path)
  end

  attr_accessor :flandmark_data_path
  @@flandmark_data_path = File.expand_path("../../vendor/flandmark_model.dat", __FILE__)

  attr_accessor :haarcascade_data_path
  @@haarcascade_data_path = File.expand_path("../../vendor/haarcascade_frontalface_alt.xml", __FILE__)
end
