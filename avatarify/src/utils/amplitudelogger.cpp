#include "amplitudelogger.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QHostAddress>
#include <QNetworkInterface>

QNetworkAccessManager* AmplitudeLogger::log(const QString& eventType_) {
    auto request = QNetworkRequest(QUrl(apiUrl));
    QJsonObject json;
    json.insert("api_key", apiKey);
    QJsonArray events;
    QJsonObject event;
    event.insert("user_id", userId);
    event.insert("event_type", eventType_);
    event.insert("ip", ip);
    event.insert("time", QDateTime::currentMSecsSinceEpoch());
    events.append(event);
    json.insert("events", events);
    QJsonDocument jsonDoc(json);
    auto jsonData = jsonDoc.toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(jsonData.size()));
    auto networkManager = new QNetworkAccessManager;
    QObject::connect(networkManager, &QNetworkAccessManager::finished, [networkManager](QNetworkReply* reply_) {
        reply_->deleteLater();
        networkManager->deleteLater();
        qDebug() << "post request finished";
    });
    networkManager->post(request, jsonData);
    return networkManager;
}

void AmplitudeLogger::setApiKey(const QString &apiKey_) {
    apiKey = apiKey_;
}

void AmplitudeLogger::setApiUrl(const QString &apiUrl_) {
    apiUrl = apiUrl_;
}

void AmplitudeLogger::setUserId(const QString &userId_) {
    userId = userId_;
}

QString AmplitudeLogger::getHardwareAddress() {
    for(const QNetworkInterface& netInterface : QNetworkInterface::allInterfaces()) {
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack)) {
            return netInterface.hardwareAddress();
        }
    }
    return "unknown";
}

QString AmplitudeLogger::getIp() {
    const QHostAddress& localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress& address : QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) {
            return address.toString();
        }
    }
    return "0.0.0.0";
}

QString AmplitudeLogger::apiKey;
QString AmplitudeLogger::apiUrl{DEFAULT_API_URL};
QString AmplitudeLogger::userId{getHardwareAddress()};
QString AmplitudeLogger::ip{getIp()};
