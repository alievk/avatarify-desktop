#ifndef IMAGECROPPER_WIDGET_H
#define IMAGECROPPER_WIDGET_H

#include <QWidget>
#include <QLabel>
#include "ImageCropper.h"


class ImageCropperWidget : public QWidget {
Q_OBJECT
public:
    explicit ImageCropperWidget(QWidget *_parent = nullptr);

private Q_SLOTS:

    void crop();

private:
    ImageCropper *m_imageCropper;
    QLabel *m_croppedImage;
};

#endif // IMAGECROPPER_WIDGET_H
