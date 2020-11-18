#include "versionchecker.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QRegularExpression>

VersionChecker::VersionChecker(QObject *parent) : QObject(parent) { }

void VersionChecker::checkForUpdates() {
    qDebug() << "VersionChecker::checkForUpdates";
    htmlGet({UPDATE_INFO_URL}, [this](const QString &body){
        qDebug() << "update info received";
        QRegularExpression reVersion(VERSION_REGEXP);
        QRegularExpressionMatch versionMatch = reVersion.match(body);
        if (versionMatch.hasMatch()) {
            auto v1 = versionMatch.captured(1).toInt();
            auto v2 = versionMatch.captured(2).toInt();
            auto v3 = versionMatch.captured(3).toInt();
            auto v4 = versionMatch.captured(4).toInt();
            qDebug() << v1 << v2 << v3 << v4;
            auto update = false;
            if (VERSION_MAJOR < v1) {
                update = true;
            } else if (VERSION_MINOR < v2) {
                update = true;
            } else if (VERSION_BUILD < v3) {
                update = true;
            } else if (VERSION_REVISION < v4 && v4 >= 5) {
                update = true;
            }
            if (update) {
                QRegularExpression reUrl(URL_REGEXP);
                QRegularExpressionMatch urlMatch = reUrl.match(body);
                if (urlMatch.hasMatch()) {
                    m_updateUrl = urlMatch.captured(1);
                    qDebug() << "update available: " << m_updateUrl;
                    m_updateAvailable = true;
                    m_updateVersion = QString("%1.%2.%3.%4").arg(QString::number(v1), QString::number(v2), QString::number(v3), QString::number(v4));
                    Q_EMIT this->updateUrlChanged();
                    Q_EMIT this->updateVersionChanged();
                    Q_EMIT this->updateAvailableChanged();
                }
            }
        }
    });
}

bool VersionChecker::updateAvailable() const {
    return m_updateAvailable;
}

QString VersionChecker::updateUrl() const {
    return m_updateUrl;
}

QString VersionChecker::updateVersion() const {
    return m_updateVersion;
}

void VersionChecker::htmlGet(const QUrl &url, const std::function<void(const QString&)> &fun) {
    QScopedPointer<QNetworkAccessManager> manager(new QNetworkAccessManager);
    QNetworkReply *response = manager->get(QNetworkRequest(QUrl(url)));
    QObject::connect(response, &QNetworkReply::finished, [response, fun]{
        response->deleteLater();
        response->manager()->deleteLater();
        if (response->error() != QNetworkReply::NoError) return;
        auto const contentType =
                response->header(QNetworkRequest::ContentTypeHeader).toString();
        static QRegularExpression re("charset=([!-~]+)");
        auto const match = re.match(contentType);
        if (!match.hasMatch() || 0 != match.captured(1).compare("utf-8", Qt::CaseInsensitive)) {
            qWarning() << "Content charsets other than utf-8 are not implemented yet:" << contentType;
            return;
        }
        auto const html = QString::fromUtf8(response->readAll());
        fun(html);
    }) && manager.take();
}
