//SceneExplorer
//Exploring video files by viewer thumbnails
//
//Copyright (C) 2018  Ambiesoft
//
//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

namespace AmbiesoftQt {
class IniSettings
{
private:
    QSettings s_;

public:
    IniSettings(const QString& organization,
             const QString& appname) :
        s_(QSettings::Format::IniFormat,
           QSettings::Scope::UserScope,
           organization,
           appname) {}
    IniSettings(const QString& inifile) : s_(inifile, QSettings::IniFormat) {}
    ~IniSettings();

    QString valueString(const QString& key, const QString& defaultvalue=QString());
    int valueInt(const QString& key, int defaultvalue=0);
    bool valueBool(const QString& key, bool defaultvalue=false);
    QStringList valueStringList(const QString& key);
    QVariant value(const QString& key, const QVariant &defaultValue = QVariant());

    QString fileName() const {
        return s_.fileName();
    }
    void setValue(const QString& key, const QVariant& v);

    void beginGroup(const QString& group) {
        s_.beginGroup(group);
    }
    void endGroup() {
        s_.endGroup();
    }

    bool isAccessible() const {
        return s_.isWritable();
    }
    void sync() {
        s_.sync();
    }

};
}  // namespace AmbiesoftQt {
#endif // SETTINGS_H
