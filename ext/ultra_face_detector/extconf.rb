require "mkmf"
extension_name = 'ultra_face_detector'
abort "ERROR: CMake is required to build the vendored libgit2" if !find_executable('cmake')

Dir.chdir(File.expand_path("libflandmark", File.dirname(__FILE__))) do
  Dir.mkdir("build") if !Dir.exists?("build")

  Dir.chdir("build") do
    system("cmake .. -DBUILD_CLAR=OFF -DBUILD_SHARED_LIBS=OFF")
    system("cmake --build .")
    system("cp libflandmark_static.a ../../")
  end
end
$libs += format(LIBARG, 'flandmark_static')
$libs += " #{`pkg-config --cflags --libs opencv`.strip}"

idefault = ['/usr/local/include/opencv', "/usr/local/include"]
ldefault = File.dirname(__FILE__)
dir_config(extension_name, idefault, ldefault)
create_makefile(extension_name)
