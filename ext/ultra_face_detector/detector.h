#pragma once

#include "libflandmark/flandmark_detector.h"
#include <highgui.h>

#define CvRectZero cvRect(0,0,0,0)

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

    Detector(const char* flandmarkDataPath, const char* faceDataPath);
    ~Detector();

    FacialData detectFacialData(char* buffer, unsigned int size);
    CvRect detectFaceRect(char* buffer, unsigned int size, CvSize* imageSizeRef = NULL);
    CvSeq* detectFaceRect(IplImage* image);

    protected:

    FLANDMARK_Model* _model;
    CvHaarClassifierCascade* _faceCascade;
    CvMemStorage* _storage;

  };
}
