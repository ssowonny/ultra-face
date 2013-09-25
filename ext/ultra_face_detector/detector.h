#pragma once

#include "libflandmark/flandmark_detector.h"
#include <highgui.h>

namespace Ultra {
  class Detector {

    public:

    typedef struct __FacialData {
      CvSize imageSize;
      CvRect faceBounds;

      CvPoint faceCenter;
      CvPoint canthusRL;
      CvPoint canthusLR;
      CvPoint mouthCornerR;
      CvPoint mouthCornerL;
      CvPoint canthusRR;
      CvPoint canthusLL;
      CvPoint nose;

      public:
      bool operator==(const struct __FacialData& faceData);
      bool operator!=(const struct __FacialData& faceData);
    } FacialData;
    static FacialData FacialDataNotFound;

    Detector();
    ~Detector();

    FacialData detect(char* buffer, unsigned int size);

    CvSeq* detectFaceRect(IplImage* image);

    protected:

    FLANDMARK_Model* _model;
    CvHaarClassifierCascade* _faceCascade;
    CvMemStorage* _storage;

  };
}
