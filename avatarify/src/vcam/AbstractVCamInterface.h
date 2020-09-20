#ifndef VCAM_H
#define VCAM_H


#include <QImage>

class AbstractVCamInterface : public QObject {
Q_OBJECT
    Q_PROPERTY(bool available READ available)
    Q_PROPERTY(bool stream READ stream WRITE setStream)
public:
    bool available() const;

    bool stream() const;

    void setStream(bool stream);

    virtual void present(const QImage &generatedFrame) = 0;

protected:
    bool m_available = false;
    bool m_stream = false;
};


#endif //VCAM_H
