#include <QApplication>
#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QTextCodec>
#include <QStorageInfo>

#include "stdQt.h"

namespace AmbiesoftQt {


QString rstrip(const QString& str, QChar c)
{
    int n = str.size() - 1;
    for (; n >= 0; --n)
    {
        if (str.at(n) != c)
        {
            return str.left(n + 1);
        }
    }
    return "";
}

QString pathCombine(const QString& path1, const QString& path2)
{
    if(QDir::isAbsolutePath(path2))
        return path2;

    return rstrip(path1, '/') + '/' + rstrip(path2, '/');
}
QString pathCombine(const QString& path1, const QString& path2, const QString& path3)
{
    return pathCombine(pathCombine(path1,path2),path3);
}

QString normalizeDir(const QString& dir)
{
    return QDir(dir).absolutePath() + '/';
}

QString doublequoteIfNecessary(const QString& s)
{
    if (s.isEmpty())
        return "\"\"";

    if (s[0] == '\\')
        return s;

    if(s[0] == '"')
        return s;

    if (!s.contains(" ") && !s.contains(","))
        return s;

    return "\"" + s + "\"";
}
QString undoublequoteIfNecessary(QString s)
{
    if(s.isEmpty())
        return s;
    if(s[0] != '"')
        return s;
    if(!s.endsWith('"'))
        return s;

    s = s.right(s.length()-1);
    s = s.left(s.length()-1);

    return s;
}

void Info(QWidget* parent, QString message)
{
    QMessageBox msgBox(parent && parent->isVisible() ? parent:nullptr);

    msgBox.setWindowTitle(qAppName());
    // msgBox.setInformativeText(message);
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}
void Alert(QWidget* parent, QString message)
{
    QMessageBox msgBox(parent && parent->isVisible() ? parent:nullptr);

    // msgBox.setInformativeText(message);
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.exec();
}


bool YesNo(QWidget* parent,
           QString message,
           QMessageBox::Icon icon)
{
    QMessageBox msgBox(parent && parent->isVisible() ? parent:nullptr);

    msgBox.setWindowTitle(qAppName());
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(icon);
    return msgBox.exec() == QMessageBox::Yes;
}
int YesNoCancel(QWidget* parent,
           QString message,
           QMessageBox::Icon icon)
{
    QMessageBox msgBox(parent && parent->isVisible() ? parent:nullptr);

    msgBox.setWindowTitle(qAppName());
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.addButton(QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(icon);
    return msgBox.exec();
}

bool HasModalWindow()
{
//    QWidget* focus = QApplication::focusWidget();
//    qDebug() << "focus widget" << focus;
//    qDebug() << "activeModalWidget" << QApplication::activeModalWidget();

    return // (focus == nullptr) && // || !focus->isAncestorOf(this))
        QApplication::activeModalWidget() != nullptr;
}
QTextCodec* GetUtf8Codec()
{
    return QTextCodec::codecForName("UTF-8");
}

QString GetSystemDefaultLang()
{
    return QLocale::languageToString(QLocale::system().language());
}

QStringList SortDevice1by1(const QStringList& dirs)
{
    QMap<QString,QStringList> mapDeviceToDirs;
    for(auto&& dir : dirs)
    {
        QStorageInfo si(dir);
        mapDeviceToDirs[si.device()] << dir;
    }

    QList<QStringList> eachDeviceDirs;
    for(auto&& key : mapDeviceToDirs.keys())
    {
        eachDeviceDirs.append(mapDeviceToDirs.value(key));
    }

    QStringList dirsResult;
    bool hasValues = false;
    do {
        int i=0;
        for(; i < eachDeviceDirs.length(); ++i)
        {
            if(!eachDeviceDirs[i].isEmpty())
            {
                dirsResult << eachDeviceDirs[i][0];
                eachDeviceDirs[i].pop_front();
            }

            // check all empty
            hasValues = false;
            for(auto&& eachDevice :eachDeviceDirs)
            {
                if(!eachDevice.isEmpty())
                {
                    hasValues=true;
                    break;
                }
            }
            if(hasValues)
                continue;
            break;
        }
    }while(hasValues);

    return dirsResult;
}
bool IsSameContents(const QStringList& dirs1, const QStringList& dirs2)
{
    if(dirs1.length() != dirs2.length())
        return false;

    QStringList t1 = dirs1;
    QStringList t2 = dirs2;
    t1.sort();
    t2.sort();
    return t1 == t2;
}
QString RemoveExtensionFromPath(QString path)
{
    path = QDir::toNativeSeparators(path);
    QFileInfo fi(path);
    QString ret = fi. fileName();

    do {
        int dotIndex = ret.lastIndexOf('.');
        if(dotIndex < 0)
            break;

        ret = ret.left(dotIndex);

        while(!ret.isEmpty() && ( ret.endsWith(" ") || ret.endsWith(".")))
        {
            ret.chop(1);
        }
    } while(false);

    if(!isLegalFilePath(ret))
        return QString();

    return ret;
}

bool GetExeVersionCommon(const QString& exe, QString& errString, QString& outString)
{
    QProcess process;
    process.setProgram(exe);
    process.setArguments( QStringList() <<
                          "-version"
                          );

    process.start(QProcess::ReadOnly);
    if(!process.waitForStarted(-1))
    {
        errString = process.errorString();
        return false;
    }
    if(!process.waitForFinished(-1))
    {
        errString = process.errorString();
        return false;
    }

    if(0 != process.exitCode())
    {
        errString = process.errorString();
        return false;
    }

    outString = process.readAllStandardOutput().data();
    return true;
}


// https://stackoverflow.com/a/26991243
bool fileExists(const QString& path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

bool fileEmpty(const QString& path)
{
    if(!fileExists(path))
        return false;
    {
        QFileInfo fi(path);
        if(fi.size()==0)
            return true;
        if(fi.size() != 3)
            return false;
    }

    // file size == 3
    QFile file(path);
    QByteArray all = file.readAll();
    if(all.size() != 3)
        return false;

    unsigned char c1 = all.data()[0];
    unsigned char c2 = all.data()[1];
    unsigned char c3 = all.data()[2];

    if(c1==0xEF && c2==0xBB && c3==0xBF)
        return true;

    return false;
}

bool isSamePath(const QString& path1, const QString& path2)
{
    return QDir(path1)==QDir(path2);
}

} // namespace AmbiesoftQt
