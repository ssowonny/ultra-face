#include <ruby.h>
#include <stdio.h>
#include <highgui.h>
#include <cv.h>
#include "libflandmark/flandmark_detector.h"
#include "detector.h"

typedef VALUE (ruby_method)(...);

VALUE detect_face_data(VALUE self, VALUE buffer) {
  Ultra::Detector* detector = new Ultra::Detector();
  Ultra::Detector::FaceData data = detector->detect( RSTRING_PTR(buffer), RSTRING_LEN(buffer) );

  if( data == Ultra::Detector::FaceDataNotFound ) {
    return Qnil;
  }

  return INT2NUM(250);
}

extern "C"
void Init_ultra_face_detector(void) {
  VALUE ultra_module = rb_define_module("UltraFace");
  VALUE detector_module = rb_define_module_under(ultra_module, "Detector");
  rb_define_singleton_method(detector_module, "detect_face_data", (ruby_method*) &detect_face_data, 1);
}
