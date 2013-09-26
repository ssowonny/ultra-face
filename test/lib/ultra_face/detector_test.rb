require_relative '../../test_helper'

class UltraFace::DetectorTest < MiniTest::Test
  def setup
    UltraFace.setup do |config|
    end
  end

  def test_should_detect_face
    path = File.expand_path('../../../fixtures/face.jpg', __FILE__)
    result = UltraFace::Detector.detect_face(path)
    assert result, 'Face should be detected'
    assert_equal 250, result[:image_size][:width]
  end
end
