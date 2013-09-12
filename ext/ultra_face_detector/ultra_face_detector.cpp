#include <ruby.h>
#include <stdio.h>

VALUE hello_world(...) {
  printf("Hello World\n");
  return Qnil;
}

extern "C"
void Init_ultra_face_detector(void) {
  VALUE ultra_module = rb_define_module("UltraFace");
  VALUE detector_module = rb_define_module_under(ultra_module, "Detector");
  rb_define_singleton_method(detector_module, "hello_world", hello_world, 0);
}
