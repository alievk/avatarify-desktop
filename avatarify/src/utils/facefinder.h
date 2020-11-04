#ifndef FACEFINDER_H
#define FACEFINDER_H

#include <QObject>

class FaceFinder : public QObject
{
    Q_OBJECT
public:
    explicit FaceFinder(QObject *parent = nullptr);
    Q_INVOKABLE QVector<qreal> findFace(QString fileUrl);

Q_SIGNALS:

};

#endif // FACEFINDER_H
