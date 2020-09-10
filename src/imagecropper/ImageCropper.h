#ifndef IMAGECROPPER_H
#define IMAGECROPPER_H

#include "ImageCropperPrivate.h"
#include "CursorPosition.h"

#include <QWidget>

class ImageCropper : public QWidget {
Q_OBJECT

public:
    explicit ImageCropper(QWidget *parent = nullptr);

    ~ImageCropper() override;

public Q_SLOTS:

    // Установить изображение для обрезки
    void setImage(const QPixmap &_image);

    // Установить цвет фона виджета обрезки
    void setBackgroundColor(const QColor &_backgroundColor);

    // Установить цвет рамки области обрезки
    void setCroppingRectBorderColor(const QColor &_borderColor);

    // Установить пропорции области выделения
    void setProportion(const QSizeF &_proportion);

    // Использовать фиксированные пропорции области виделения
    void setProportionFixed(bool _isFixed);

public:
    // Обрезать изображение
    QPixmap cropImage();

protected:
    void paintEvent(QPaintEvent *_event) override;

    void mousePressEvent(QMouseEvent *_event) override;

    void mouseMoveEvent(QMouseEvent *_event) override;

    void mouseReleaseEvent(QMouseEvent *_event) override;

private:
    // Определение местоположения курсора над виджетом
    static CursorPosition cursorPosition(const QRectF &_cropRect, const QPointF &_mousePosition);

    // Обновить иконку курсора соответствующую местоположению мыши
    void updateCursorIcon(const QPointF &_mousePosition);

    // Получить размер виджета после его изменения мышью
    // --------
    // Контракты:
    // 1. Метод должен вызываться, только при зажатой кнопке мыши
    //    (т.е. при перемещении или изменении размера виджета)
    // --------
    // В случае неудачи возвращает null-прямоугольник
    QRectF calculateGeometry(
            const QRectF &_sourceGeometry,
            CursorPosition _cursorPosition,
            const QPointF &_mouseDelta
    );

    // Получить размер виджета после его изменения мышью
    // Метод изменяет виджет не сохраняя начальных пропорций сторон
    // ------
    // Контракты:
    // 1. Метод должен вызываться, только при зажатой кнопке мыши
    //    (т.е. при перемещении или изменении размера виджета)
    static QRectF calculateGeometryWithCustomProportions(
            const QRectF &_sourceGeometry,
            CursorPosition _cursorPosition,
            const QPointF &_mouseDelta
    );

    // Получить размер виджета после его изменения мышью
    // Метод изменяет виджет сохраняя начальные пропорции сторон
    // ------
    // Контракты:
    // 1. Метод должен вызываться, только при зажатой кнопке мыши
    //    (т.е. при перемещении или изменении размера виджета)
    static QRectF calculateGeometryWithFixedProportions(const QRectF &_sourceGeometry,
                                                        CursorPosition _cursorPosition,
                                                        const QPointF &_mouseDelta,
                                                        const QSizeF &_deltas
    );

private:
    // Private data implementation
    ImageCropperPrivate *pimpl;
};

#endif // IMAGECROPPER_H
