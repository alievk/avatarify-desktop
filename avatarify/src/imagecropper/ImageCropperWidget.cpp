#include "ImageCropperWidget.h"

#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>


ImageCropperWidget::ImageCropperWidget(QWidget *_parent) : QWidget(_parent) {
    // Настраиваем виджет обрезки изображений
    m_imageCropper = new ImageCropper(this);
    m_imageCropper->resize(256, 256);
    m_imageCropper->setProportion(QSize(1, 1));
    m_imageCropper->setImage(QPixmap(":/img.jpg"));
    m_imageCropper->setBackgroundColor(Qt::lightGray);
    m_imageCropper->setCroppingRectBorderColor(Qt::magenta);
    // Использовать фиксированные пропорции
    auto *fixedProportionsCombo = new QCheckBox("Fix proportion to (1x1)", this);
    connect(fixedProportionsCombo, SIGNAL(toggled(bool)), m_imageCropper, SLOT(setProportionFixed(bool)));
    // Кадрировать выделенную область
    auto *cropBtn = new QPushButton("Crop", this);
    connect(cropBtn, SIGNAL(clicked()), this, SLOT(crop()));
    // Область для вставки обрезанного изображения
    m_croppedImage = new QLabel(this);
    // Настраиваем отображение
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_imageCropper);
    layout->addWidget(fixedProportionsCombo);
    layout->addWidget(cropBtn);
    layout->addWidget(m_croppedImage);
}

void ImageCropperWidget::crop() {
    m_croppedImage->setPixmap(m_imageCropper->cropImage());
}
