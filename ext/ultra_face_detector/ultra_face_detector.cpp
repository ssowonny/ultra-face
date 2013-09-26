#include <ruby.h>
#include <highgui.h>
#include <cv.h>
#include "libflandmark/flandmark_detector.h"
#include "detector.h"

typedef VALUE (ruby_method)(...);

Ultra::Detector* detector = NULL;

VALUE setup(VALUE self, VALUE flandmarkPath, VALUE haarcascadePath ) {
  // Initialize Detector
  detector = new Ultra::Detector(RSTRING_PTR(flandmarkPath), RSTRING_PTR(haarcascadePath));

  return Qnil;
}

VALUE detect_face_data(VALUE self, VALUE buffer) {
  Ultra::Detector::FacialData data = detector->detect( RSTRING_PTR(buffer), RSTRING_LEN(buffer) );

  if( data == Ultra::Detector::FacialDataNotFound ) {
    return Qnil;
  }

  // Image Size
  VALUE imageSize = rb_hash_new();
  rb_hash_aset(imageSize, ID2SYM(rb_intern("width")), INT2NUM(data.imageSize.width));
  rb_hash_aset(imageSize, ID2SYM(rb_intern("height")), INT2NUM(data.imageSize.height));

  // Face Bounds
  VALUE faceBounds = rb_hash_new();
  rb_hash_aset(faceBounds, ID2SYM(rb_intern("x")), INT2NUM(data.faceBounds.x));
  rb_hash_aset(faceBounds, ID2SYM(rb_intern("y")), INT2NUM(data.faceBounds.y));
  rb_hash_aset(faceBounds, ID2SYM(rb_intern("width")), INT2NUM(data.faceBounds.width));
  rb_hash_aset(faceBounds, ID2SYM(rb_intern("height")), INT2NUM(data.faceBounds.height));

  // Face Center
  VALUE faceCenter = rb_hash_new();
  rb_hash_aset(faceCenter, ID2SYM(rb_intern("x")), INT2NUM(data.faceCenter.x));
  rb_hash_aset(faceCenter, ID2SYM(rb_intern("y")), INT2NUM(data.faceCenter.y));

  // Canthus Right (Right to left)
  VALUE canthusRR = rb_ary_new();
  rb_ary_push(canthusRR, INT2NUM(data.canthusRR.x));
  rb_ary_push(canthusRR, INT2NUM(data.canthusRR.y));
  VALUE canthusRL = rb_ary_new();
  rb_ary_push(canthusRL, INT2NUM(data.canthusRL.x));
  rb_ary_push(canthusRL, INT2NUM(data.canthusRL.y));
  VALUE canthusRight = rb_ary_new();
  rb_ary_push(canthusRight, canthusRR);
  rb_ary_push(canthusRight, canthusRL);

  // Canthus Left (Right to left)
  VALUE canthusLR = rb_ary_new();
  rb_ary_push(canthusLR, INT2NUM(data.canthusLR.x));
  rb_ary_push(canthusLR, INT2NUM(data.canthusLR.y));
  VALUE canthusLL = rb_ary_new();
  rb_ary_push(canthusLL, INT2NUM(data.canthusLL.x));
  rb_ary_push(canthusLL, INT2NUM(data.canthusLL.y));
  VALUE canthusLeft = rb_ary_new();
  rb_ary_push(canthusLeft, canthusLR);
  rb_ary_push(canthusLeft, canthusLL);

  // Mouth Corner
  VALUE mouthCornerR = rb_ary_new();
  rb_ary_push(mouthCornerR, INT2NUM(data.mouthCornerR.x));
  rb_ary_push(mouthCornerR, INT2NUM(data.mouthCornerR.y));
  VALUE mouthCornerL = rb_ary_new();
  rb_ary_push(mouthCornerL, INT2NUM(data.mouthCornerL.x));
  rb_ary_push(mouthCornerL, INT2NUM(data.mouthCornerL.y));
  VALUE mouthCorner = rb_ary_new();
  rb_ary_push(mouthCorner, mouthCornerR);
  rb_ary_push(mouthCorner, mouthCornerL);

  // Nose
  VALUE nose = rb_hash_new();
  rb_hash_aset(nose, ID2SYM(rb_intern("x")), INT2NUM(data.nose.x));
  rb_hash_aset(nose, ID2SYM(rb_intern("y")), INT2NUM(data.nose.y));

  // Result
  VALUE result = rb_hash_new();
  rb_hash_aset(result, ID2SYM(rb_intern("image_size")), imageSize);
  rb_hash_aset(result, ID2SYM(rb_intern("face_bounds")), faceBounds);
  rb_hash_aset(result, ID2SYM(rb_intern("face_center")), faceCenter);
  rb_hash_aset(result, ID2SYM(rb_intern("canthus_right")), canthusRight);
  rb_hash_aset(result, ID2SYM(rb_intern("canthus_left")), canthusLeft);
  rb_hash_aset(result, ID2SYM(rb_intern("mouth_corner")), mouthCorner);
  rb_hash_aset(result, ID2SYM(rb_intern("nose")), nose);

  return result;
}

extern "C"
void Init_ultra_face_detector(void) {
  VALUE ultra_module = rb_define_module("UltraFace");
  VALUE detector_module = rb_define_module_under(ultra_module, "Detector");
  // Detect method
  rb_define_singleton_method(detector_module, "detect_face_data", (ruby_method*) &detect_face_data, 1);

  // Setup method
  rb_define_singleton_method(detector_module, "setup", (ruby_method*) &setup, 2);
}
