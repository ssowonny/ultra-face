#include "detector.h"

namespace Ultra {
  Detector::FaceData Detector::FaceDataNotFound;

  Detector::Detector() {
    // Initialize data
    _storage = cvCreateMemStorage(0);
    _model = flandmark_init("./ext/ultra_face_detector/flandmark_model.dat");
    char faceCascadeFilename[] = "./ext/ultra_face_detector/haarcascade_frontalface_alt.xml";
    _faceCascade = (CvHaarClassifierCascade*)cvLoad(faceCascadeFilename, 0, 0, 0);
  }

  Detector::~Detector() {
    cvReleaseHaarClassifierCascade(&_faceCascade);
    flandmark_free(_model);
    cvReleaseMemStorage(&_storage);
  }

  Detector::FaceData Detector::detect(char* buffer, unsigned int size) {
    FaceData data;

    CvMat mat = cvMat(1, size, CV_32FC2, static_cast<void *>(buffer));
    IplImage* image = cvDecodeImage(&mat, -1);

    IplImage* gray = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
    cvCvtColor(image, gray, CV_BGR2GRAY);

    CvSeq* rects = detectFaceRect(gray);
    if( !rects ) {
      return FaceDataNotFound;
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
      return FaceDataNotFound;
    }

    // Detect Face data
    int bound[4];
    bound[0] = face->x;
    bound[1] = face->y;
    bound[2] = face->x + face->width;
    bound[3] = face->y + face->height;

    double *landmarks = (double*)malloc(2 * _model->data.options.M * sizeof(double));
    flandmark_detect(gray, bound, _model, landmarks);

    data.bound = *face;
    data.faceCenter   = cvPoint(landmarks[0], landmarks[1]);
    data.canthusRL    = cvPoint(landmarks[2], landmarks[3]);
    data.canthusLR    = cvPoint(landmarks[4], landmarks[5]);
    data.mouthCornerR = cvPoint(landmarks[6], landmarks[7]);
    data.mouthCornerL = cvPoint(landmarks[8], landmarks[9]);
    data.canthusRR    = cvPoint(landmarks[10], landmarks[11]);
    data.canthusLL    = cvPoint(landmarks[12], landmarks[13]);
    data.nose         = cvPoint(landmarks[14], landmarks[15]);

    free(landmarks);

    return data;
  }

  CvSeq* Detector::detectFaceRect(IplImage* image) {
    float searchScaleFactor = 1.1f;
    int flags = CV_HAAR_DO_CANNY_PRUNING;
    CvSize minFeatureSize = cvSize(40, 40);

    return cvHaarDetectObjects(image, _faceCascade, _storage, searchScaleFactor, 2, flags, minFeatureSize);
  }

  // FACE DATA
  // ---------

  bool Detector::FaceData::operator==(const struct __FaceData& faceData) {
    return memcmp(this, &faceData, sizeof(this)) == 0;
  }

  bool Detector::FaceData::operator!=(const struct __FaceData& faceData) {
    return !(*this==faceData);
  }

}
