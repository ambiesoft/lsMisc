
#ifndef STDQT_H
#define STDQT_H

#include <QString>
#include <QApplication>
#include <QMessageBox>
#include <QSet>
#include <QTextCodec>

namespace AmbiesoftQt {

QString rstrip(const QString& str, QChar c);
QString pathCombine(const QString& path1, const QString& path2);
QString pathCombine(const QString& path1, const QString& path2, const QString& path3);
QString normalizeDir(const QString& dir);
QString doublequoteIfNecessary(const QString& s);
QString undoublequoteIfNecessary(QString s);
bool Move3Files(const QString& replaced, const QString& replacement, const QString& backup, QString* pError = nullptr);

void Info(QWidget* parent, QString message);
void Alert(QWidget* parent, QString message);
bool YesNo(QWidget* parent,
           QString message,
           QMessageBox::Icon = QMessageBox::Question);
int YesNoCancel(QWidget* parent,
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
bool GetFreeStorage(const QString dir, qint64& bytesFree, QString& root);

bool isLegalFilePath(QString filename, QString* pError = nullptr);
QString GetIllegalFilenameCharacters();

QStringList SortDevice1by1(const QStringList& dirs);
bool IsSameContents(const QStringList& dirs1, const QStringList& dirs2);
bool showInGraphicalShell(QWidget *parent, const QString &pathIn);
QString RemoveExtensionFromPath(QString path);
bool GetExeVersionCommon(const QString& exe, QString& errString, QString& outString);
QString MakeUniquablePath(const QString& path);
QString ToAsciiLower(const QString& s);
bool fileExists(const QString& path);
bool fileEmpty(const QString& path);
bool isRootDriveExists(const QString& path);
QChar getFirstCharcter(const QString& text, const QChar c = '_');
void CenterWidgets(QWidget *widget, QWidget *host = Q_NULLPTR) ;

#ifdef _WIN32
std::wstring QStringToStdString(const QString& inQ);
#else
std::string QStringToStdString(const QString& inQ);
#endif
} // namespace AmbiesoftQt

#endif // STDQT_H
