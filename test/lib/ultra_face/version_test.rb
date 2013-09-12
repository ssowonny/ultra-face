require_relative '../../test_helper'

class UltraFace::VersionTest < MiniTest::Unit::TestCase
  def test_should_check_version
    assert UltraFace::VERSION
  end
end
