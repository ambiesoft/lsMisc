#include <stdio.h>

#include <QString>
#include <QDir>
#include <QStandardPaths>

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
} // namespace
