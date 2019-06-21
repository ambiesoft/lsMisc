
#ifndef STDQT_H
#define STDQT_H

#include <QString>
#include <QApplication>
#include <QMessageBox>
#include <QSet>

namespace AmbiesoftQt {

QString rstrip(const QString& str, QChar c);
QString pathCombine(const QString& path1, const QString& path2);
QString pathCombine(const QString& path1, const QString& path2, const QString& path3);
QString normalizeDir(const QString& dir);
QString doublequoteIfNecessary(const QString& s);
QString undoublequoteIfNecessary(QString s);
bool Move3Files(const QString& replaced, const QString& replacement, const QString& backup, QString* pError = nullptr);

class CWaitCursor
{
public:
    CWaitCursor()
    {
#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    }
    ~CWaitCursor()
    {
#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
    }
};


void Info(QWidget* parent, QString message);
void Alert(QWidget* parent, QString message);
bool YesNo(QWidget* parent,
           QString message,
           QMessageBox::Icon = QMessageBox::Question);
bool HasModalWindow();
QTextCodec* GetUtf8Codec();
QString getInifile(bool& bExit,
                   const QString& company,
                   const QString& appname);
QString getInifile(bool& bExit);
QString GetUserDocumentDirectory();
QString GetSystemDefaultLang();
QSet<int> GetAllProcessIDs();
}

#endif // STDQT_H
