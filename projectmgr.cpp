#include "projectmgr.h"

#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*!
    \brief Object constructor
*/
ProjectMgr::ProjectMgr(QObject *parent) :
    QObject(parent)
{
}

/*!
    \brief Parse the project file
*/
bool ProjectMgr::ReadFile(QString proj_dir, QString proj_name) {
    QString filePath(proj_dir + "/" + proj_name + ".json");
    QFile loadFile(filePath);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("ReadProjFile Couldn't open save file.");
        return false;
    }

    projectMgr::Instance()->proj_matchs_.clear();

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    projectMgr::Instance()->proj_name_ = proj_name;
    projectMgr::Instance()->proj_dir_ = proj_dir;

    if (loadDoc.object().contains("sync") && loadDoc.object()["sync"].isString()) {
         projectMgr::Instance()->proj_sync_ = loadDoc["sync"].toBool();
    }
    if (loadDoc.object().contains("reload") && loadDoc.object()["reload"].isDouble()) {
         projectMgr::Instance()->proj_reload_ = loadDoc.object()["reload"].toInt();
    }

    if (loadDoc.object().contains("servers") && loadDoc.object()["servers"].isArray()) {
        projectMgr::Instance()->proj_servers_.clear();
        QJsonArray sArray = loadDoc.object()["servers"].toArray();

        for (int Index = 0; Index < sArray.size(); ++Index) {
            QJsonObject levelObject = sArray[Index].toObject();
            if (sArray[Index].isString()) {
                 projectMgr::Instance()->proj_servers_.append(sArray[Index].toString());
            }
        }
    }

    if (loadDoc.object().contains("files") && loadDoc.object()["files"].isArray()) {
        projectMgr::Instance()->proj_files_.clear();
        QJsonArray sArray = loadDoc.object()["files"].toArray();

        for (int Index = 0; Index < sArray.size(); ++Index) {
            QJsonObject levelObject = sArray[Index].toObject();
            if (sArray[Index].isString()) {
                 projectMgr::Instance()->proj_files_.append(sArray[Index].toString());
            }
        }
    }

    if (loadDoc.object().contains("match") && loadDoc.object()["match"].isObject()) {
        auto json = loadDoc.object()["match"].toObject();
        foreach(const QString& key, json.keys()) {
            QJsonValue value = json.value(key);
            projectMgr::Instance()->proj_matchs_[key] = value.toString();
        }
    }

    return true;
}

/*!
    \brief Create a default project file
*/
bool ProjectMgr::CreateProjFile(QString proj_dir, QString proj_name) const
{
    QFile saveFile(proj_dir + "/" +proj_name + ".json");

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject projFile;

    projFile["project"] = proj_name;
    projFile["sync"] = "false";
    projFile["reload"] = 0;

    QJsonArray serversArray;
    projFile["servers"] = serversArray;

    QJsonArray filesArray;
    projFile["files"] = filesArray;

    QJsonObject regexObject;
    projFile["match"] = regexObject;

    saveFile.write( QJsonDocument(projFile).toJson());

    projectMgr::Instance()->ReadFile(proj_dir, proj_name);

    return true;
}


/*!
    \brief Parse an existente project file
*/
bool ProjectMgr::ReadProjFile()
{
    return projectMgr::Instance()->ReadFile(projectMgr::Instance()->proj_dir_, projectMgr::Instance()->proj_name_);
}

/*!
    \brief Parse a new project file
*/
bool ProjectMgr::ReadProjFile(QString proj_dir, QString proj_name)
{
    return projectMgr::Instance()->ReadFile(proj_dir, proj_name);
}
