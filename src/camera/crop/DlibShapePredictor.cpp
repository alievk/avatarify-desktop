#include "DlibShapePredictor.h"

dlib::frontal_face_detector DlibShapePredictor::detector = dlib::get_frontal_face_detector();

dlib::shape_predictor DlibShapePredictor::shapePredictor;


DlibShapePredictor::DlibShapePredictor(QString weightsPath) {
    dlib::deserialize(weightsPath.toStdString()) >> shapePredictor;
}

std::vector<dlib::point> DlibShapePredictor::detect(const QImage &image) {
    QImage smallImg = image.scaled(1280 / downscaleFactor, 720 / downscaleFactor);
    smallImg.save("./out_qt.bmp");
    dlib::matrix<dlib::rgb_pixel> img;
    convert(smallImg, img);
    std::vector<dlib::point> points = detectFace(img);
    for (auto & point : points) {
        point *= downscaleFactor;
    }
    return points;
}

std::vector<dlib::point> DlibShapePredictor::detectFace(const dlib::matrix<dlib::rgb_pixel> &img) {
    std::vector<dlib::point> points;

    auto detected = detector(img);
    if (detected.empty()) {
        qDebug() << "no face detected";
        return points;
    }
    if (detected.size() > 1) {
        qDebug() << "more than one face detected; choose #0";
    }
    dlib::full_object_detection shape = shapePredictor(img, detected[0]);
    for (unsigned long i = 0; i < shape.num_parts(); ++i) {
        points.push_back(shape.part(i));
    }
    return points;
}


void DlibShapePredictor::convert(const QImage &src, dlib::matrix<dlib::rgb_pixel> &dst) {
    dst.set_size(src.height(), src.width());
    auto dstIt = dst.begin();
    int i = 0;
    const uint8_t *st = src.bits();
    while (dstIt != dst.end() && i < src.width() * src.height()) {
        dstIt->red = st[3 * i];
        dstIt->green = st[3 * i + 1];
        dstIt->blue = st[3 * i + 2];

        ++dstIt;
        ++i;
    }
}


