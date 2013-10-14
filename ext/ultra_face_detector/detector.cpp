#include "detector.h"
#include <stdio.h>

namespace Ultra {
  Detector::FacialData Detector::FacialDataNotFound;

  Detector::Detector(const char* flandmarkDataPath, const char* faceDataPath) {
    // Initialize data
    _model = flandmark_init(flandmarkDataPath);
    _faceCascade = (CvHaarClassifierCascade*)cvLoad(faceDataPath, 0, 0, 0);
    _storage = cvCreateMemStorage(0);
  }

  Detector::~Detector() {
    cvReleaseMemStorage(&_storage);
    cvReleaseHaarClassifierCascade(&_faceCascade);
    flandmark_free(_model);
  }

  Detector::FacialData Detector::detectFacialData(char* buffer, unsigned int size) {
    // Clear memory storage
    cvClearMemStorage( _storage );

    FacialData data;
    IplImage* image = NULL, * gray = NULL;

    try {
      CvMat mat = cvMat(1, size, CV_32FC2, static_cast<void *>(buffer));
      image = cvDecodeImage(&mat, -1);
      if( !image ) {
        return FacialDataNotFound;
      }

      gray = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
      cvCvtColor(image, gray, CV_BGR2GRAY);

      CvSeq* rects = detectFaceRect(gray);
      if( !rects ) {
        cvReleaseImage(&image);
        cvReleaseImage(&gray);
        return FacialDataNotFound;
      }

      // Pick the largest face
      int facesCount = rects->total;
      int maxSize = 0;
      CvRect* face = NULL;
      for( int i = 0; i < facesCount; ++i ) {
        CvRect* r = (CvRect*)cvGetSeqElem(rects, i);
        int size = r->width * r->height;
        if( maxSize < size ) {
          maxSize = size;
          face = r;
        }
      }

      if( !face ) {
        cvReleaseImage(&image);
        cvReleaseImage(&gray);
        return FacialDataNotFound;
      }

      // Detect Face data
      int bounds[4];
      bounds[0] = face->x;
      bounds[1] = face->y;
      bounds[2] = face->x + face->width;
      bounds[3] = face->y + face->height;

      double *landmarks = (double*)malloc(2 * _model->data.options.M * sizeof(double));
      flandmark_detect(gray, bounds, _model, landmarks);

      data.imageSize = cvSize(image->width, image->height);
      data.faceBounds = *face;
      data.faceCenter   = cvPoint(landmarks[0], landmarks[1]);
      data.canthusRL    = cvPoint(landmarks[2], landmarks[3]);
      data.canthusLR    = cvPoint(landmarks[4], landmarks[5]);
      data.mouthCornerR = cvPoint(landmarks[6], landmarks[7]);
      data.mouthCornerL = cvPoint(landmarks[8], landmarks[9]);
      data.canthusRR    = cvPoint(landmarks[10], landmarks[11]);
      data.canthusLL    = cvPoint(landmarks[12], landmarks[13]);
      data.nose         = cvPoint(landmarks[14], landmarks[15]);

      free(landmarks);
      cvReleaseImage(&image);
      cvReleaseImage(&gray);
    } catch(...) {

      if(image) {
        cvReleaseImage(&image);
      }
      if(gray) {
        cvReleaseImage(&gray);
      }
      return FacialDataNotFound;
    }
    return data;
  }

  CvRect Detector::detectFaceRect(char* buffer, unsigned int size, CvSize* imageSizeRef) {

    // Clear memory storage
    cvClearMemStorage( _storage );
    IplImage* image = NULL, * gray = NULL;
    CvRect rect = CvRectZero;

    do {
      try {
        CvMat mat = cvMat(1, size, CV_32FC2, static_cast<void *>(buffer));
        image = cvDecodeImage(&mat, -1);
        if( !image ) {
          break;
        }

        // Setup the image size
        if( imageSizeRef ) {
          imageSizeRef->width = image->width;
          imageSizeRef->height = image->height;
        }

        gray = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
        cvCvtColor(image, gray, CV_BGR2GRAY);

        CvSeq* rects = detectFaceRect(gray);
        if( !rects ) {
          break;
        }

        // Pick the largest face
        int maxSize = 0;
        int facesCount = rects->total;
        for( int i = 0; i < facesCount; ++i ) {
          CvRect* r = (CvRect*)cvGetSeqElem(rects, i);
          int size = r->width * r->height;
          if( maxSize < size ) {
            maxSize = size;
            rect = *r;
          }
        }

      } catch(...) {
        rect = CvRectZero;
      }
    } while(false);

    // Release memory
    if(image) {
      cvReleaseImage(&image);
    }

    if(gray) {
      cvReleaseImage(&gray);
    }

    return rect;
  }

  CvSeq* Detector::detectFaceRect(IplImage* image) {
    float searchScaleFactor = 1.1f;
    int flags = CV_HAAR_DO_CANNY_PRUNING;
    CvSize minFeatureSize = cvSize(40, 40);

    return cvHaarDetectObjects(image, _faceCascade, _storage, searchScaleFactor, 2, flags, minFeatureSize);
  }

  // FACE DATA
  // ---------

  bool Detector::FacialData::operator==(const struct __FacialData& faceData) {
    return memcmp(this, &faceData, sizeof(this)) == 0;
  }

  bool Detector::FacialData::operator!=(const struct __FacialData& faceData) {
    return !(*this==faceData);
  }

}
