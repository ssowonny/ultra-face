require_relative '../../test_helper'

class UltraFace::VersionTest < MiniTest::Test
  def test_should_check_version
    assert UltraFace::VERSION
  end
end
