#include <dirent.h>

#include <stdio.h>

#include <QString>
#include <QDir>
#include <QStandardPaths>
#include <QSet>
#include <QStorageInfo>
#include <QDesktopServices>
#include <QUrl>

#include "stdQt.h"

namespace AmbiesoftQt {

bool Move3Files(const QString& replaced, const QString& replacement, const QString& backup, QString* pError)
{
    Q_ASSERT(!replaced.isEmpty());
    Q_ASSERT(!replacement.isEmpty());

    // maybe first save, original file not exist
    if(!QFile(replaced).exists())
    {
        if(0 != rename(replacement.toStdString().c_str(),
                       replaced.toStdString().c_str()))
        {
            if(pError)
                *pError = QString::fromStdString(strerror(errno));
            return false;
        }
        return true;
    }

    if(!backup.isEmpty())
    {
        // first rename(replaced, backup)
        if(0 != rename(replaced.toStdString().c_str(), backup.toStdString().c_str()))
        {
            if(pError)
                *pError = QString::fromStdString(strerror(errno));
            return false;
        }
    }

    // Second, rename(replacement,replaced)
    if(0 != rename(replacement.toStdString().c_str(),
                   replaced.toStdString().c_str()))
    {
        if(pError)
            *pError = QString::fromStdString(strerror(errno));
        return false;
    }
    return true;
}


QString getInifile(bool& bExit,
                   const QString& company,
                   const QString& appname)
{
    Q_UNUSED(bExit);
    Q_UNUSED(company);
    Q_UNUSED(appname);
    return QString();
}
QString getInifile(bool& bExit)
{
    const QString company = QApplication::organizationName();
    const QString appname = QApplication::applicationName();
    Q_ASSERT(!company.isEmpty());
    Q_ASSERT(!appname.isEmpty());

    return getInifile(bExit,company,appname);
}


QString GetUserDocumentDirectory()
{
    QString result = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if(!result.isEmpty())
        return result;

    return QDir::homePath();
}

namespace  {
bool isNumber(const char* name)
{
    if(Q_UNLIKELY( !name || name[0]==0))
        return false;

    for(;*name;++name)
    {
        if(!( '0' <= *name && *name <= '9'))
            return false;
    }
    return true;
}
} // namespace

QSet<int> GetAllProcessIDs()
{
    QSet<int> result;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("/proc")) != nullptr)
    {
        while ((ent = readdir (dir)) != nullptr)
        {
            if(ent->d_type == DT_DIR && isNumber(ent->d_name))
            {
                result.insert(atoi(ent->d_name));
            }
        }
        closedir (dir);
    }
    return result;
}

bool GetFreeStorage(const QString dir, qint64& bytesFree, QString& root)
{
    if(dir.isEmpty())
        return false;
    QStorageInfo si(dir);
    bytesFree = si.bytesAvailable();
    root = si.rootPath();
    return bytesFree >= 0;
}

bool isLegalFilePath(QString filename, QString* pError)
{
    if(filename.isEmpty())
    {
        if(pError)
        {
            *pError = QObject::tr("Filename could not be empty.");
        }
        return false;
    }

    for( QChar& c : filename)
    {
        if(c=='/')
        {
            if(pError)
            {
                *pError = QObject::tr("Filename could not have '/'.");
                return false;
            }
        }
        if(c==0)
        {
            if(pError)
            {
                *pError = QObject::tr("Filename could not have Nul character.");
                return false;
            }
        }
    }
    return true;
}
QString GetIllegalFilenameCharacters()
{
    return "/";
}

// https://stackoverflow.com/a/3546503
bool showInGraphicalShell(QWidget *parent, const QString &pathIn)
{
    Q_UNUSED(parent);
    return QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(pathIn).absolutePath()));
}

} // namespace
