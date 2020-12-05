#ifndef AMPLITUDELOGGER_H
#define AMPLITUDELOGGER_H

#include <QString>
#include <QNetworkAccessManager>

class AmplitudeLogger {
public:
    AmplitudeLogger() = delete;
    static QNetworkAccessManager* log(const QString& eventType_);
    static void setApiKey(const QString& apiKey_);
    static void setApiUrl(const QString& apiUrl_);
    static void setUserId(const QString& userId_);
private:
    static auto constexpr DEFAULT_API_URL = "https://api.amplitude.com/2/httpapi";
    static QString apiKey;
    static QString apiUrl;
    static QString userId;
    static QString ip;
    static QString getHardwareAddress();
    static QString getIp();
};

#endif // AMPLITUDELOGGER_H
