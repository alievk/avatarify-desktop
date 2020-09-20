#ifndef IMAGECROPPER_PRIVATE_H
#define IMAGECROPPER_PRIVATE_H

#include "CursorPosition.h"

#include <QtCore/QRect>
#include <QtGui/QPixmap>
#include <QtGui/QColor>

namespace {
    const QRect INIT_CROPPING_RECT = QRect();
    const QSizeF INIT_PROPORTION = QSizeF(1.0, 1.0);
}

class ImageCropperPrivate {
public:
    ImageCropperPrivate() :
            imageForCropping(QPixmap()),
            croppingRect(INIT_CROPPING_RECT),
            lastStaticCroppingRect(QRect()),
            cursorPosition(CursorPositionUndefined),
            isMousePressed(false),
            isProportionFixed(false),
            startMousePos(QPoint()),
            proportion(INIT_PROPORTION),
            deltas(INIT_PROPORTION),
            backgroundColor(Qt::black),
            croppingRectBorderColor(Qt::white) {}

public:
    // Изображение для обрезки
    QPixmap imageForCropping;
    // Область обрезки
    QRectF croppingRect;
    // Последняя фиксированная область обрезки
    QRectF lastStaticCroppingRect;
    // Позиция курсора относительно области обрезки
    CursorPosition cursorPosition;
    // Зажата ли левая кнопка мыши
    bool isMousePressed;
    // Фиксировать пропорции области обрезки
    bool isProportionFixed;
    // Начальная позиция курсора при изменении размера области обрезки
    QPointF startMousePos;
    // Пропорции
    QSizeF proportion;
    // Приращения
    // width  - приращение по x
    // height - приращение по y
    QSizeF deltas;
    // Цвет заливки фона под изображением
    QColor backgroundColor;
    // Цвет рамки области обрезки
    QColor croppingRectBorderColor;
};

#endif // IMAGECROPPER_PRIVATE_H
