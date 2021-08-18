#include "cjsonmanager.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

void CJsonManager::saveToFile(QString filename, const SExample *example)
{
    auto jDoc = new QJsonDocument();
    auto jObjInsideDoc = jDoc->object();

    QJsonObject mainSettings;
    mainSettings.insert("locationType", settings.locationType);
    mainSettings.insert("department", settings.department);
    QJsonArray jWords;
    for (auto word : example->words) {
        jWords.append(word);
    }
    mainSettings.insert("words", jWords);
    jObjInsideDoc["mainSettings"] = mainSettings;

    jDoc->setObject(jObjInsideDoc);
    QFile jFile(filename);
    if (!jFile.open(QIODevice::WriteOnly))
        return;
    jFile.write(jDoc->toJson());
    jFile.close();
}

SExample CJsonManager::loadFromFile(QString filename)
{
    //
    return SExample();
}
