#ifndef POORMANSPROBE_H
#define POORMANSPROBE_H

#include <QAbstractVideoSurface>
#include <QList>
#include <QCamera>


class PoorMansProbe : public QAbstractVideoSurface {
Q_OBJECT

public:
    explicit PoorMansProbe(QObject *parent = nullptr);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const override;

    // Called from QAbstractVideoSurface whenever a new frame is present
    bool present(const QVideoFrame &frame) Q_DECL_OVERRIDE;

    bool setSource(QCamera *newSource);

Q_SIGNALS:

    // Users of this class will get frames via this signal
    void videoFrameProbed(const QVideoFrame &videoFrame);

private:
    QCamera *source;
};

#endif // POORMANSPROBE_H