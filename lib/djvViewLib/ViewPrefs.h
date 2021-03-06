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

#pragma once

#include <djvViewLib/AbstractPrefs.h>
#include <djvViewLib/ImageView.h>

#include <djvGraphics/Color.h>

namespace djv
{
    namespace ViewLib
    {
        //! This class provides the view group preferences.
        class ViewPrefs : public AbstractPrefs
        {
            Q_OBJECT

        public:
            explicit ViewPrefs(const QPointer<ViewContext> &, QObject * parent = nullptr);
            ~ViewPrefs() override;

            //! Get the default mouse wheel zoom factor.
            static Enum::ZOOM_FACTOR zoomFactorDefault();

            //! Get the mouse wheel zoom factor.
            Enum::ZOOM_FACTOR zoomFactor() const;

            //! Get the default background color.
            static Graphics::Color backgroundDefault();

            //! Get the background color.
            const Graphics::Color & background() const;

            //! Get the default grid.
            static Enum::GRID gridDefault();

            //! Get the view grid.
            Enum::GRID grid() const;

            //! Get the default view grid color.
            static Graphics::Color gridColorDefault();

            //! Get the view grid color.
            const Graphics::Color & gridColor() const;

            //! Get the default for whether the HUD is enabled.
            static bool hudEnabledDefault();

            //! Get whether the HUD is enabled.
            bool isHudEnabled() const;

            //! Get the default HUD information.
            static QVector<bool> hudInfoDefault();

            //! Get the HUD information.
            QVector<bool> hudInfo() const;

            //! Get the HUD information.
            bool isHudInfo(Enum::HUD in) const;

            //! Get the default HUD color.
            static Graphics::Color hudColorDefault();

            //! Get the HUD color.
            const Graphics::Color & hudColor() const;

            //! Get the default HUD background.
            static Enum::HUD_BACKGROUND hudBackgroundDefault();

            //! Get the HUD background.
            Enum::HUD_BACKGROUND hudBackground() const;

            //! Get the default HUD background color.
            static Graphics::Color hudBackgroundColorDefault();

            //! Get the HUD background color.
            const Graphics::Color & hudBackgroundColor() const;

        public Q_SLOTS:
            //! Set the mouse wheel zoom factor.
            void setZoomFactor(djv::ViewLib::Enum::ZOOM_FACTOR);

            //! Set the background color.
            void setBackground(const djv::Graphics::Color &);

            //! Set the view grid.
            void setGrid(djv::ViewLib::Enum::GRID);

            //! Set the view grid color.
            void setGridColor(const djv::Graphics::Color &);

            //! Set whether the HUD is enabled.
            void setHudEnabled(bool);

            //! Set the HUD informati0on.
            void setHudInfo(const QVector<bool> &);

            //! Set the HUD information.
            void setHudInfo(djv::ViewLib::Enum::HUD, bool);

            //! Set the HUD color.
            void setHudColor(const djv::Graphics::Color &);

            //! Set the HUD background.
            void setHudBackground(djv::ViewLib::Enum::HUD_BACKGROUND);

            //! Set the HUD background color.
            void setHudBackgroundColor(const djv::Graphics::Color &);

        Q_SIGNALS:
            //! This signal is emitted when the mouse wheel zoom factor is changed.
            void zoomFactorChanged(djv::ViewLib::Enum::ZOOM_FACTOR);

            //! This signal is emitted when the background color is changed.
            void backgroundChanged(const djv::Graphics::Color &);

            //! This signal is emitted when the view grid is changed.
            void gridChanged(djv::ViewLib::Enum::GRID);

            //! This signal is emitted when the view grid color is changed.
            void gridColorChanged(const djv::Graphics::Color &);

            //! This signal is emitted when the HUD is enabled or disabled.
            void hudEnabledChanged(bool);

            //! This signal is emitted when the HUD information is changed.
            void hudInfoChanged(const QVector<bool> &);

            //! This signal is emitted when the HUD color is changed.
            void hudColorChanged(const djv::Graphics::Color &);

            //! This signal is emitted when the HUD background is changed.
            void hudBackgroundChanged(djv::ViewLib::Enum::HUD_BACKGROUND);

            //! This signal is emitted when the HUD background color is changed.
            void hudBackgroundColorChanged(const djv::Graphics::Color &);

        private:
            Enum::ZOOM_FACTOR    _zoomFactor;
            Graphics::Color      _background;
            Enum::GRID           _grid;
            Graphics::Color      _gridColor;
            bool                 _hudEnabled;
            QVector<bool>        _hudInfo;
            Graphics::Color      _hudColor;
            Enum::HUD_BACKGROUND _hudBackground;
            Graphics::Color      _hudBackgroundColor;
        };

    } // namespace ViewLib
} // namespace djv
