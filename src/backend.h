#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <qqml.h>

class BackEnd : public QObject {
Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)

public:
    explicit BackEnd(QObject *parent = nullptr);

    QString userName();

    void setUserName(const QString &userName);

signals:

    void userNameChanged();

private:
    QString m_userName = "zhopa";
};

#endif // BACKEND_H
