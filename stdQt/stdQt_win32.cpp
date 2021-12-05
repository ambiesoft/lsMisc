#include <string>
#include <memory>
#include <functional>

#include <Windows.h>
#include <ShlObj.h>
#include <psapi.h>
#include <Shlwapi.h>


#include <QString>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QProcessEnvironment>

#include "../GetLastErrorString.h"
#include "../../profile/cpp/Profile/include/ambiesoft.profile.h"

#include "stdQt.h"

using namespace std;
using namespace Ambiesoft;

namespace AmbiesoftQt {

bool Move3Files(const QString& replaced, const QString& replacement, const QString& backup, QString* pError)
{
    Q_ASSERT(!replaced.isEmpty());
    Q_ASSERT(!replacement.isEmpty());

    // maybe first save, original file not exist
    if(!QFileInfo(replaced).exists())
    {
        if(!MoveFileW(replacement.toStdWString().c_str(),
                      replaced.toStdWString().c_str()))
        {
            if(pError)
                *pError = QString::fromStdWString(GetLastErrorString(GetLastError()));
            return false;
        }
        return true;
    }
    if(!ReplaceFile(replaced.toStdWString().c_str(),
                    replacement.toStdWString().c_str(),
                    backup.isEmpty() ? nullptr : backup.toStdWString().c_str(),
                    0, // flag
                    nullptr,nullptr // reserved
                    ))
    {
        if(pError)
            *pError = QString::fromStdWString(GetLastErrorString(GetLastError()));
        return false;
    }
    return true;
}

static QString getSpecialFolder(int id,
                                const QString& company,
                                const QString& appname)
{
    wchar_t path[MAX_PATH] = {0};
    if(S_OK != SHGetFolderPath(
                nullptr,
                id,
                nullptr,
                SHGFP_TYPE_CURRENT,
                path))

    {
        Alert(nullptr, QObject::tr("Failed to get folder location."));
        return QString();
    }


    //    PWSTR pOut=NULL;
    //    if(S_OK != SHGetKnownFolderPath(id,0,NULL,&pOut))
    //    {
    //        Alert(nullptr, QObject::tr("Failed to get Roaming folder. Default folder will be used."));
    //        return QString();
    //    }

    QString dir = QString::fromUtf16((const ushort*)path);
    //    CoTaskMemFree(pOut);

    dir = QDir(dir).absolutePath();

    QDir(dir).mkdir(company);
    dir = pathCombine(dir, company);

    QDir(dir).mkdir(appname);
    dir = pathCombine(dir, appname);

    return dir;
}
static QString getSpecialFolder(int id)
{
    const QString company = QApplication::organizationName();
    const QString appname = QApplication::applicationName();
    Q_ASSERT(!company.isEmpty());
    Q_ASSERT(!appname.isEmpty());

    return getSpecialFolder(id,company,appname);
}


QString getInifile(bool& bExit,
                   const QString& company,
                   const QString& appname)
{
    QFileInfo trfi(QCoreApplication::applicationFilePath());
    std::string folini = pathCombine(trfi.absolutePath(), "folder.ini").toStdString();
    int intval = -1;
    Profile::GetInt("Main", "PathType", -1, intval, folini);

    QString dir;
    switch (intval)
    {
    case 0:
        dir = trfi.absolutePath();
        break;

    case 1:
        dir = getSpecialFolder(CSIDL_LOCAL_APPDATA,
                               company,
                               appname);
        break;

    default:
    case 2:
        dir = getSpecialFolder(CSIDL_APPDATA,
                               company,
                               appname);
        break;

    case 3:
    {
        std::string t;
        Profile::GetString("Main", "folder", "", t, folini);
        dir = t.c_str();
        if(dir.isEmpty())
        {
            Alert(nullptr, QObject::tr("Folder settings does not have sufficient data. Lanuch FolderConfig.exe and reconfigure."));
            bExit=true;
            return QString();
        }
        else
        {
            if(!QDir(dir).exists())
            {
                if(!YesNo(nullptr, QObject::tr("\"%1\" does not exist. Do you want to create it?").arg(dir)))
                {
                    bExit=true;
                    return QString();
                }
                QDir(dir).mkpath(".");
                if(!QDir(dir).exists())
                {
                    Alert(nullptr, QObject::tr("Failed to create directory."));
                    bExit=true;
                    return QString();
                }
            }
        }
    }
        break;
    } // switch

    if (dir.isEmpty())
        return QString();

    return pathCombine(dir, appname + ".ini");
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

    result = getSpecialFolder(CSIDL_MYDOCUMENTS);
    if(!result.isEmpty())
        return result;

    return QDir::homePath();
}

QSet<int> GetAllProcessIDs()
{
    QSet<int> result;

    DWORD pids[4096];
    DWORD dwNeeded=0;
    if(!EnumProcesses(pids, _countof(pids), &dwNeeded))
        return result;

    for(auto&& pid : pids)
    {
        result.insert((int)pid);
    }
    return result;
}

static bool myPathIsRoot(LPCWSTR pIN)
{
    if(!pIN || pIN[0]==0)
        return false;
    if(PathIsRoot(pIN))
        return true;
    unique_ptr<WCHAR> p(new WCHAR[wcslen(pIN) + 2]);
    wcscpy(p.get(), pIN);
    PathAddBackslash(p.get());
    return PathIsRoot(p.get());
}
static QString getRoot(const QString& dir)
{
    if(dir.isEmpty())
        return dir;

    unique_ptr<WCHAR, function<void(void*)>> p(_wcsdup(QDir::toNativeSeparators(dir).toStdWString().c_str()),
                                               [](void*p){
        free(p);
    });
    while(true)
    {
        // qDebug() << QString::fromStdWString(p.get()) << __FUNCTION__;
        if(myPathIsRoot((p.get())))
            return QString::fromStdWString(p.get()).replace('\\','/');

        WCHAR* pA = wcsrchr(p.get(), L'\\');
        if(!pA)
            return QString();
        *(pA)=0;
    }
}
bool GetFreeStorage(const QString dir, qint64& bytesFree, QString& root)
{
    ULARGE_INTEGER available;
    ULARGE_INTEGER total;
    ULARGE_INTEGER freebytes;
    if(!GetDiskFreeSpaceEx(dir.toStdWString().c_str(),
                           &available,
                           &total,
                           &freebytes))
    {
        return false;
    }
    bytesFree = static_cast<qint64>(freebytes.QuadPart);
    root = getRoot(dir);
    return true;
}

// https://stackoverflow.com/a/45282192
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

    // Windows filenames are not case sensitive.
    filename = filename.toUpper();

    QString illegal=GetIllegalFilenameCharacters();

    foreach (const QChar& c, filename)
    {
        // Check for control characters
        if (c.toLatin1() > 0 && c.toLatin1() < 32)
        {
            if(pError)
            {
                *pError = QObject::tr("Filename could not have control characters.");
            }
            return false;
        }
        // Check for illegal characters
        if (illegal.contains(c))
        {
            if(pError)
            {
                *pError = QObject::tr("Filename could not have these characters.");
                *pError += "\n\n";
                *pError += GetIllegalFilenameCharacters();
            }
            return false;
        }
    }

    // Check for device names in filenames
    static QStringList devices;

    if (!devices.count())
        devices << "CON" << "PRN" << "AUX" << "NUL" << "COM0" << "COM1" << "COM2"
                << "COM3" << "COM4" << "COM5" << "COM6" << "COM7" << "COM8" << "COM9" << "LPT0"
                << "LPT1" << "LPT2" << "LPT3" << "LPT4" << "LPT5" << "LPT6" << "LPT7" << "LPT8"
                << "LPT9";

    const QFileInfo fi(filename);
    const QString basename = fi.baseName();

    foreach (const QString& s, devices)
    {
        if (basename == s)
        {
            if(pError)
            {
                *pError = QObject::tr("Filename must not be device names.");
            }
            return false;
        }
    }
    // Check for trailing periods or spaces
    if (filename.right(1)==".")
    {
        if(pError)
        {
            *pError = QObject::tr("Filename should not end with period.");
        }
        return false;
    }
    if( filename.right(1)==" ")
    {
        if(pError)
        {
            *pError = QObject::tr("Filename must not end with space character.");
        }
        return false;
    }

    if (filename.right(1)=="\\")
    {
        if(pError)
        {
            *pError = QObject::tr("Filename must not end with backslash character.");
        }
        return false;
    }
    if (filename.right(1)=="/")
    {
        if(pError)
        {
            *pError = QObject::tr("Filename must not end with slash character.");
        }
        return false;
    }
    // Check for pathnames that are too long (disregarding raw pathnames)
    if (filename.length()>260)
    {
        if(pError)
        {
            *pError = QObject::tr("Filename is too long.");
        }
        return false;
    }

    //    // Exclude raw device names
    //    if (filename.left(4)=="\\\\.\\")
    //        return false;


    return true;
}
QString GetIllegalFilenameCharacters()
{
    return "<>:\"|?*";
}

// https://stackoverflow.com/a/3546503
bool showInGraphicalShell(QWidget *parent, const QString &pathIn)
{
    Q_UNUSED(parent);

    QString explorer;
    QString path = QProcessEnvironment::systemEnvironment().value("PATH");
    QStringList paths = path.split(';');
    for (int i = 0; i < paths.length(); ++i)
    {
        QString s = pathCombine(paths[i], "explorer.exe");

        if (QFileInfo(s).exists())
        {
            explorer = s;
            break;
        }
    }
    if (explorer.isEmpty()) {
        return false;
    }
    QString param;
    if (!QFileInfo(pathIn).isDir())
    {
        QStringList args;
        args << "/select,";
        args << QDir::toNativeSeparators(pathIn);
        args << ",/n";
        QProcess::startDetached(explorer, args);
    }
    else
    {
        QStringList args;
        args << QDir::toNativeSeparators(pathIn);
        QProcess::startDetached(explorer, args);
    }

    return true;
}
QString MakeUniquablePath(const QString& path)
{
    return ToAsciiLower(QFileInfo(path).absoluteFilePath());
}

QString ToAsciiLower(const QString& s)
{
    std::wstring w = s.toStdWString();

    transform(
      w.begin(), w.end(),
      w.begin(),
      towlower);

    return QString::fromStdWString(w);
}
bool isRootDriveExists(const QString& path)
{
    if(path.isEmpty())
        return false;
    if(PathIsRelative(path.toStdWString().c_str()))
        return true;
    return QDir(getRoot(path)).exists();
}
}
