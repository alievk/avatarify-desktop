#ifndef FACEFINDER_H
#define FACEFINDER_H

#include <QObject>
#include <dlib/image_processing/frontal_face_detector.h>

class FaceFinder : public QObject
{
    Q_OBJECT
public:
    explicit FaceFinder(QObject *parent = nullptr);
    Q_INVOKABLE QVector<qreal> findFace(QString fileUrl);
    static QVector<qreal> findFace(const QImage &image);

Q_SIGNALS:
private:
    static dlib::frontal_face_detector detector;
};

#endif // FACEFINDER_H
