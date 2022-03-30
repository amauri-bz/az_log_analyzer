#ifndef PROJECTMGR_H
#define PROJECTMGR_H

#include <QFile>
#include <QList>
#include <QMap>
#include <QObject>
#include "singleton.h"

class ProjectMgr : public QObject
{
    Q_OBJECT
public:
    explicit ProjectMgr(QObject *parent = 0);

    bool ReadProjFile();
    bool ReadProjFile(QString proj_dir, QString proj_name);
    bool CreateProjFile(QString proj_dir, QString proj_name) const;

    QString proj_name_;
    QString proj_dir_;
    bool proj_sync_;
    int proj_reload_;
    QVector<QString> proj_servers_;
    QVector<QString> proj_files_;
    QMap<QString, QString> proj_matchs_;

private:
    bool ReadFile(QString proj_dir, QString proj_name);
};

//Global variable
typedef Singleton<ProjectMgr> projectMgr;

#endif // PROJECTMGR_H
