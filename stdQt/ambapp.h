#ifndef AMBAPP_H
#define AMBAPP_H

#include <memory>
#include <utility>

#include <QScopedPointer>
#include <QTranslator>
#include <QApplication>

#include "inisettings.h"
#include "runguard.h"

namespace AmbiesoftQt {
class AmbApp : public QApplication
{
    Q_OBJECT
public:
    class AmbAppArgs
    {
    public:
        QString keyLanguage;
        QString keyStyle;
        bool singleInstance;
    };
    explicit AmbApp(int& argc, char *argv[], const AmbAppArgs& args);
    virtual ~AmbApp();

    bool InitApplication(QScopedPointer<AmbiesoftQt::IniSettings>* settings);

protected:
    const AmbAppArgs args_;
    QTranslator qtTranslator_;
    QTranslator appTranslator_;
    std::unique_ptr<RunGuard> guard_;
};


} // namespace AmbiesoftQt
#endif // AMBAPP_H
