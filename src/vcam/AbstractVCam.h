#ifndef VCAM_H
#define VCAM_H


#include <QImage>

class AbstractVCam : public QObject {
Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled)
public:
    bool enabled() const;

    void setEnabled(bool enabled);

    virtual void present(const QImage &generatedFrame) = 0;

private:
    bool m_enabled = false;
};


#endif //VCAM_H
