#include "facefinder.h"

#include <QDebug>
#include <QImage>
#include <QUrl>
#include <dlib/image_processing/frontal_face_detector.h>

FaceFinder::FaceFinder(QObject *parent) : QObject(parent) { }

QVector<qreal> FaceFinder::findFace(QString fileUrl) {
    static dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
    QImage image(QUrl(fileUrl).toLocalFile());
    image = image.scaled(800, 800, Qt::KeepAspectRatio);
    if (image.format() != QImage::Format_RGB888) {
        image = image.convertToFormat(QImage::Format_RGB888);
    }

    dlib::matrix<dlib::rgb_pixel> img;
    img.set_size(image.height(), image.width());
    auto dstIt = img.begin();
    int i = 0;
    auto total = image.width() * image.height();
    while (dstIt != img.end() && i < total) {
        auto color = QColor(image.pixel(i % image.width(), i / image.width()));
        dstIt->red = color.red();
        dstIt->green = color.green();
        dstIt->blue = color.blue();
        ++dstIt;
        ++i;
    }

    QVector<qreal> retVal;
    auto detected = detector(img);
    if (detected.empty()) {
        qDebug() << "no face detected";
        return retVal;
    }
    if (detected.size() > 1) {
        qDebug() << "more than one face detected; choose #0";
    }
    if (detected.size()) {
        retVal.push_back(static_cast<double>(detected[0].left()) / image.width());
        retVal.push_back(static_cast<double>(detected[0].top()) / image.height());
        retVal.push_back(static_cast<double>(detected[0].right()) / image.width());
        retVal.push_back(static_cast<double>(detected[0].bottom()) / image.height());
    }
    return retVal;
}


