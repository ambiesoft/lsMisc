#include <QFile>
#include <QDebug>
#include <QLibraryInfo>
#include <QStyle>

#include "stdQt.h"
#include "runguard.h"


#include "ambapp.h"

using namespace AmbiesoftQt;

namespace AmbiesoftQt {

namespace  {
bool IsAvailableLanguage(const QString& lang)
{
    QString i18nFile = ":/translations/i18n_" + lang + ".qm";
    return QFile(i18nFile).exists();
}
} // namespace


AmbApp::AmbApp(int& argc, char *argv[], const AmbAppArgs& args) :
    QApplication(argc,argv),
    args_(args)
{
}
AmbApp::~AmbApp()
{}

bool AmbApp::InitApplication(QScopedPointer<IniSettings>* settings)
{
    bool bExit = false;
    Q_ASSERT(isLegalFilePath(organizationName()));
    Q_ASSERT(isLegalFilePath(applicationName()));
    QString inifile = getInifile(bExit,
                                 organizationName(),
                                 applicationName());
    if(bExit)
        return false;
    settings->reset(inifile.isEmpty() ?
                        new IniSettings(QApplication::organizationName(), QApplication::applicationName()) :
                        new IniSettings(inifile));
    if(!(*settings)->isAccessible())
    {
        Alert(nullptr,
              QObject::tr("\"%1\" is not accessible.").arg((*settings)->fileName()));
        return false;
    }

    const QString lang = (*settings)->valueString(args_.keyLanguage);


    {
        do {
            QString qti18nFile = "qt_";
            if(lang.isEmpty())
            {
                // default
                QString syslang = GetSystemDefaultLang();
                if(syslang=="English" || !IsAvailableLanguage(syslang))
                    break;

                qti18nFile += QLocale::system().name();
            }
            else if(lang=="English")
            {
                qti18nFile += "en_US";
                break;
            }
            else if(lang=="Japanese")
            {
                qti18nFile += "ja_JP";
            }
            else
            {
                Q_ASSERT(false);
                break;
            }
            qDebug() << "Qt language" << qti18nFile << __FUNCTION__;
            if(qtTranslator_.load(qti18nFile,
                                  QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
            {
                installTranslator(&qtTranslator_);
            }
            else
            {
                Alert(nullptr, QObject::tr("Failed to set Qt language to '%1'.").arg(lang));
            }
        } while(false);


        QString i18nFile = ":/translations/i18n_";

        do {
            if(lang.isEmpty())
            {
                // default
                QString syslang = GetSystemDefaultLang();
                if(syslang=="English" || !IsAvailableLanguage(syslang))
                    break;

                i18nFile += syslang;
            }
            else if(lang=="English")
            {
                i18nFile.clear();
                break;
            }
            else if(lang=="Japanese")
            {
                i18nFile += lang;
            }
            else
            {
                Q_ASSERT(false);
                break;
            }
            if(appTranslator_.load(i18nFile)) //, trdir);
            {
                installTranslator(&appTranslator_);
            }
            else
            {
                Alert(nullptr, QObject::tr("Failed to set language to '%1'.").arg(lang));
            }
        } while(false);
    }

    qDebug () << "CurrentStyle: " << QApplication::style()->objectName() << __FUNCTION__;

    // style:  "windows", "windowsvista", "fusion", or "macintosh".
    QString style = (*settings)->valueString(args_.keyStyle);
    if(!style.isEmpty())
    {
        if(!QApplication::setStyle(style))
        {
            Alert(nullptr, QObject::tr("Failed to set style") + ": " + style);
        }
    }
    return true;
}

bool AmbApp::InitAndCheckDuplicateInstance(const QString &duptoken)
{
    Q_ASSERT(!duptoken.isEmpty());
    Q_ASSERT(!guard_);
    Q_ASSERT(!applicationName().isEmpty());
    guard_ = std::unique_ptr<RunGuard>(new RunGuard(applicationName() + duptoken));
    Q_ASSERT(guard_);
    return guard_->tryToRun();
}

} // namespace AmbiesoftQt
