require_relative '../../test_helper'

class UltraFace::DetectorTest < MiniTest::Test
  def test_should_detect_face
    path = File.expand_path('../../../fixtures/face.jpg', __FILE__)
    assert UltraFace::Detector.detect_face(path), 'Face should be detected'
  end
end
