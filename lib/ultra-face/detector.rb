require File.expand_path('../../ultra_face_detector', __FILE__)

module UltraFace
  module Detector

    def self.detect_face path
      file = File.open path, 'rb'
      data = file.read
      file.close

      detect_face_data data
    end
  end
end
