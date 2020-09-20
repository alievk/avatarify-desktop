#ifndef FACERECOGNITION_H
#define FACERECOGNITION_H


#include <QtGui/QImage>
#include <QDebug>
#include <dlib/pixel.h>
#include <dlib/matrix.h>
#include <dlib/image_processing/shape_predictor.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/serialize.h>
#include <dlib/image_saver/image_saver.h>

class DlibShapePredictor {
public:
    explicit DlibShapePredictor(QString weightsPath);

    static std::vector<dlib::point> detect(const QImage &image);

    static dlib::shape_predictor shapePredictor;

private:
    static const int downscaleFactor = 4;
    static dlib::frontal_face_detector detector;

    static void convert(const QImage &src, dlib::matrix<dlib::rgb_pixel> &dst);

    static std::vector<dlib::point> detectFace(const dlib::matrix<dlib::rgb_pixel> &img);
};


#endif //FACERECOGNITION_H
