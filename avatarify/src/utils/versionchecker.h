#ifndef VERSIONCHECKER_H
#define VERSIONCHECKER_H

#include <QObject>

class VersionChecker : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool updateAvailable READ updateAvailable NOTIFY updateAvailableChanged)
    Q_PROPERTY(QString updateUrl READ updateUrl NOTIFY updateUrlChanged)
    Q_PROPERTY(QString updateVersion READ updateVersion NOTIFY updateVersionChanged);
public:
    explicit VersionChecker(QObject *parent = nullptr);
    Q_INVOKABLE void checkForUpdates();
    bool updateAvailable() const;
    QString updateUrl() const;
    QString updateVersion() const;

Q_SIGNALS:
    void updateAvailableChanged();
    void updateUrlChanged();
    void updateVersionChanged();

private:
    static void htmlGet(const QUrl &url, const std::function<void(const QString&)> &fun);
    bool m_updateAvailable{false};
    QString m_updateUrl;
    QString m_updateVersion;
    static auto constexpr UPDATE_INFO_URL = "https://avatarify.ai/desktop-version";
    static auto constexpr VERSION_REGEXP = "version=\"(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)\"";
    static auto constexpr URL_REGEXP = "url=\"(https?:\\/\\/(www\\.)?[-a-zA-Z0-9@:%._\\+~#=]{1,256}\\.[a-zA-Z0-9()]{1,6}\\b([-a-zA-Z0-9()@:%_\\+.~#?&//=]*))\"";
};

#endif // VERSIONCHECKER_H
