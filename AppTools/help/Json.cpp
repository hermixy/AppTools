#include "Json.h"

#include <QFile>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonArray>

Json::Json(QString jsonPath_)
    :jsonLoad(false)
    ,jsonPath(jsonPath_)
{
    QString jsonError;
    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        jsonError=QString("Cannot open the file: %1").arg(jsonPath);
        qDebug()<<jsonError;
        qDebug()<<"could't open file json";
        return;
    }
    QByteArray json = file.readAll();
    file.close();

    // 解析 Json
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json, &error);
    if (QJsonParseError::NoError != error.error)
    {
        jsonError = QString("%1\nOffset: %2").arg(error.errorString()).arg(error.offset);
        qDebug() << jsonError;
        return;
    }
    rootObj = jsonDoc.object();
    jsonLoad=true;
}

Json::~Json()
{

}

void Json::parseJsonObject(QString rootName,QStringList objectName)
{
    jsonObject.clear();
    if(!jsonLoad)
        return;
    if(!rootObj.contains(rootName))
    {
        QString jsonError;
        jsonError = QString("can not find %1").arg(rootName);
        qDebug() << jsonError;
        return;
    }
    QJsonObject subObj = rootObj.value(rootName).toObject();
    QString value;
    for(int i=0;i<rootObj.size();i++)
    {
        if(subObj.contains(objectName.at(i)))
        {
            jsonObject.insert(objectName.at(i),subObj.value(objectName.at(i)).toString());
        }
    }
}

void Json::parseJsonArray(QString rootName)
{
    jsonArray.clear();
    if(!jsonLoad)
        return;
    if(!rootObj.contains(rootName))
    {
        QString jsonError;
        jsonError = QString("can not find %1").arg(rootName);
        qDebug() << jsonError;
        return;
    }
    QJsonArray subArray = rootObj.value(rootName).toArray();
    for(int i = 0; i< subArray.size(); i++)
    {
        jsonArray << subArray.at(i).toString();
    }
}

QMap<QString, QString> Json::getJsonObject(QString rootName, QStringList objectName)
{
    parseJsonObject(rootName,objectName);
    return jsonObject;
}

QStringList Json::getJsonArray(QString rootName)
{
    parseJsonArray(rootName);
    return jsonArray;
}
