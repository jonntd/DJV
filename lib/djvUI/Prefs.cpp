//------------------------------------------------------------------------------
// Copyright (c) 2004-2018 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include <djvUI/Prefs.h>

#include <djvCore/FileInfoUtil.h>
#include <djvCore/System.h>

#include <QApplication>
#include <QSettings>

namespace djv
{
    namespace UI
    {
        namespace
        {
            const int version = 3;
            
        } // namespace
        
        struct Prefs::Private
        {
            //! \todo QSettings::SystemScope isn't working?
            //! \todo The organization name is hard-coded here since preferences can be
            //! created before the name is set in the application constructor.
            Private(Prefs::SCOPE scope) :
                scope(scope),
                settings(
                    QSettings::UserScope,
                    //            USER == scope ? QSettings::UserScope : QSettings::SystemScope,
                    Core::System::env("LANG").isEmpty() ?
                    QString("djv.sourceforge.net.v%1").arg(version) :
                    QString("djv.sourceforge.net.v%1.%2").arg(version).arg(Core::System::env("LANG")))
            {}

            Prefs::SCOPE scope;
            QSettings settings;
        };

        namespace
        {
            bool _reset = false;

        } // namespace

        Prefs::Prefs(const QString & group, SCOPE scope) :
            _p(new Private(scope))
        {
            //DJV_DEBUG("Prefs::Prefs");
            //DJV_DEBUG_PRINT("file = " << _p->settings.fileName());
            //DJV_DEBUG_PRINT("keys = " << _p->settings.childKeys());

            _p->settings.beginGroup(group);
        }

        Prefs::~Prefs()
        {
            _p->settings.endGroup();
        }

        Prefs::SCOPE Prefs::scope() const
        {
            return _p->scope;
        }

        void Prefs::remove(const QString & name)
        {
            _p->settings.remove(name);
        }

        bool Prefs::contains(const QString & name) const
        {
            return _reset ? false : _p->settings.contains(name);
        }

        QStringList Prefs::list() const
        {
            QStringList out;
            Q_FOREACH(QString key, _p->settings.allKeys())
            {
                out += key;
            }
            return out;
        }

        QString Prefs::fileName() const
        {
            return _p->settings.fileName();
        }

        bool Prefs::hasReset()
        {
            return _reset;
        }

        void Prefs::setReset(bool reset)
        {
            _reset = reset;
        }

        bool Prefs::_get(const QString & name, QStringList & out) const
        {
            if (_reset)
                return false;
            //DJV_DEBUG("Prefs::_get");
            //DJV_DEBUG_PRINT("name = " << name);

            if (_p->settings.contains(name))
            {
                QStringList list = _p->settings.value(name).toStringList();
                //DJV_DEBUG_PRINT("list = " << list.count());
                Q_FOREACH(QString string, list)
                {
                    //DJV_DEBUG_PRINT("string = " << string);
                    out += string;
                }
                return true;
            }
            //DJV_DEBUG_PRINT("out = " << out);
            return false;
        }

        void Prefs::_set(const QString & name, const QStringList & in)
        {
            _p->settings.setValue(name, in);
        }

    } // namespace UI
} // namespace djv
