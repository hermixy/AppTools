#ifndef JSON_H
#define JSON_H

#include <QJsonObject>
#include <QMap>
#include <QStringList>

#pragma execution_character_set("utf-8")

class Json
{
public:
    Json(QString jsonPath_);
    ~Json();
    //解析json普通节点
    void parseJsonObject(QString rootName,QStringList objectName);
    //解析json数组
    void parseJsonArray(QString rootName);

    QMap<QString,QString> getJsonObject(QString rootName,QStringList objectName);

    QStringList getJsonArray(QString rootName);

private:
    bool jsonLoad;
    QString jsonPath;
    QJsonObject rootObj;
    QMap<QString,QString> jsonObject;
    QStringList jsonArray;
};

#endif // JSON_H
