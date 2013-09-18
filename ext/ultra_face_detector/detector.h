#pragma once

#include "libflandmark/flandmark_detector.h"
#include <highgui.h>

namespace Ultra {
  class Detector {

    public:

    typedef struct __FaceData {
      CvRect bound;

      CvPoint faceCenter;
      CvPoint canthusRL;
      CvPoint canthusLR;
      CvPoint mouthCornerR;
      CvPoint mouthCornerL;
      CvPoint canthusRR;
      CvPoint canthusLL;
      CvPoint nose;

      public:
      bool operator==(const struct __FaceData& faceData);
      bool operator!=(const struct __FaceData& faceData);
    } FaceData;
    static FaceData FaceDataNotFound;

    Detector();
    ~Detector();

    FaceData detect(char* buffer, unsigned int size);

    CvSeq* detectFaceRect(IplImage* image);

    protected:

    FLANDMARK_Model* _model;
    CvHaarClassifierCascade* _faceCascade;
    CvMemStorage* _storage;

  };
}
