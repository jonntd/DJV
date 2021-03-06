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

#include <djvViewLib/DisplayProfileWidget.h>

#include <djvViewLib/ImagePrefs.h>
#include <djvViewLib/Util.h>
#include <djvViewLib/ViewContext.h>

#include <djvUI/FileEdit.h>
#include <djvUI/FloatEditSlider.h>
#include <djvUI/FloatObject.h>
#include <djvUI/IconLibrary.h>
#include <djvUI/InputDialog.h>
#include <djvUI/ToolButton.h>

#include <djvGraphics/Image.h>

#include <djvCore/SignalBlocker.h>

#include <QApplication>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPointer>
#include <QScrollArea>
#include <QVBoxLayout>

namespace djv
{
    namespace ViewLib
    {
        struct DisplayProfileWidget::Private
        {
            Private(
                const QPointer<ImageView> & viewWidget,
                const QPointer<ViewContext> & context) :
                context(context),
                viewWidget(viewWidget)
            {}

            QPointer<ViewContext> context;
            QPointer<ImageView> viewWidget;
            DisplayProfile displayProfile;
            QPointer<UI::FileEdit> lutWidget;
            QPointer<UI::FloatEditSlider> brightnessWidget;
            QPointer<UI::FloatEditSlider> contrastWidget;
            QPointer<UI::FloatEditSlider> saturationWidget;
            QPointer<UI::FloatEditSlider> levelsInWidget[2];
            QPointer<UI::FloatEditSlider> gammaWidget;
            QPointer<UI::FloatEditSlider> levelsOutWidget[2];
            QPointer<UI::FloatEditSlider> softClipWidget;
            QPointer<UI::ToolButton> addButton;
            QPointer<UI::ToolButton> resetButton;
        };

        DisplayProfileWidget::DisplayProfileWidget(
            const QPointer<ImageView> & viewWidget,
            const QPointer<ViewContext> & context,
            QWidget * parent) :
            QWidget(parent),
            _p(new Private(viewWidget, context))
        {
            // Create the LUT widgets.
            auto lutGroup = new QGroupBox(
                qApp->translate("djv::ViewLib::DisplayProfileWidget", "Lookup Table"));

            _p->lutWidget = new UI::FileEdit(context.data());

            // Create the color widgets.
            auto colorGroup = new QGroupBox(
                qApp->translate("djv::ViewLib::DisplayProfileWidget", "Color"));

            _p->brightnessWidget = new UI::FloatEditSlider(context.data());
            _p->brightnessWidget->setDefaultValue(_p->displayProfile.color.brightness);
            _p->brightnessWidget->editObject()->setClamp(false);
            _p->brightnessWidget->sliderObject()->setRange(0.f, 4.f);

            _p->contrastWidget = new UI::FloatEditSlider(context.data());
            _p->contrastWidget->setDefaultValue(_p->displayProfile.color.contrast);
            _p->contrastWidget->editObject()->setClamp(false);
            _p->contrastWidget->sliderObject()->setRange(0.f, 4.f);

            _p->saturationWidget = new UI::FloatEditSlider(context.data());
            _p->saturationWidget->setDefaultValue(_p->displayProfile.color.saturation);
            _p->saturationWidget->editObject()->setClamp(false);
            _p->saturationWidget->sliderObject()->setRange(0.f, 4.f);

            // Create the levels widgets.
            auto levelsGroup = new QGroupBox(
                qApp->translate("djv::ViewLib::DisplayProfileWidget", "Levels"));

            _p->levelsInWidget[0] = new UI::FloatEditSlider(context.data());
            _p->levelsInWidget[0]->setDefaultValue(_p->displayProfile.levels.inLow);
            _p->levelsInWidget[0]->editObject()->setClamp(false);
            _p->levelsInWidget[0]->sliderObject()->setRange(0.f, 1.f);

            _p->levelsInWidget[1] = new UI::FloatEditSlider(context.data());
            _p->levelsInWidget[1]->setDefaultValue(_p->displayProfile.levels.inHigh);
            _p->levelsInWidget[1]->editObject()->setClamp(false);
            _p->levelsInWidget[1]->sliderObject()->setRange(0.f, 1.f);

            _p->gammaWidget = new UI::FloatEditSlider(context.data());
            _p->gammaWidget->setDefaultValue(_p->displayProfile.levels.gamma);
            _p->gammaWidget->editObject()->setClamp(false);
            _p->gammaWidget->sliderObject()->setRange(0.f, 4.f);

            _p->levelsOutWidget[0] = new UI::FloatEditSlider(context.data());
            _p->levelsOutWidget[0]->setDefaultValue(_p->displayProfile.levels.outLow);
            _p->levelsOutWidget[0]->editObject()->setClamp(false);
            _p->levelsOutWidget[0]->sliderObject()->setRange(0.f, 1.f);

            _p->levelsOutWidget[1] = new UI::FloatEditSlider(context.data());
            _p->levelsOutWidget[1]->setDefaultValue(_p->displayProfile.levels.outHigh);
            _p->levelsOutWidget[1]->editObject()->setClamp(false);
            _p->levelsOutWidget[1]->sliderObject()->setRange(0.f, 1.f);

            _p->softClipWidget = new UI::FloatEditSlider(context.data());
            _p->softClipWidget->setDefaultValue(_p->displayProfile.softClip);
            _p->softClipWidget->editObject()->setClamp(false);
            _p->softClipWidget->sliderObject()->setRange(0.f, 1.f);

            // Create the other widgets.
            _p->addButton = new UI::ToolButton(context.data());
            _p->addButton->setToolTip(
                qApp->translate("djv::ViewLib::DisplayProfileWidget", "Add the current display profile to the favorites list"));

            _p->resetButton = new UI::ToolButton(context.data());
            _p->resetButton->setToolTip(
                qApp->translate("djv::ViewLib::DisplayProfileWidget", "Reset the current display profile"));

            // Layout the widgets.
            auto layout = new QVBoxLayout(this);
            auto scrollArea = new QScrollArea;
            scrollArea->setWidgetResizable(true);
            layout->addWidget(scrollArea);
            auto scrollWidget = new QWidget;
            scrollArea->setWidget(scrollWidget);
            auto scrollLayout = new QVBoxLayout(scrollWidget);

            scrollLayout->addWidget(lutGroup);
            auto formLayout = new QFormLayout(lutGroup);
            formLayout->addRow(_p->lutWidget);

            scrollLayout->addWidget(colorGroup);
            formLayout = new QFormLayout(colorGroup);
            formLayout->addRow(
                qApp->translate("djv::ViewLib::DisplayProfileWidget", "Brightness:"),
                _p->brightnessWidget);
            formLayout->addRow(
                qApp->translate("djv::ViewLib::DisplayProfileWidget", "Contrast:"),
                _p->contrastWidget);
            formLayout->addRow(
                qApp->translate("djv::ViewLib::DisplayProfileWidget", "Saturation:"),
                _p->saturationWidget);

            scrollLayout->addWidget(levelsGroup);
            formLayout = new QFormLayout(levelsGroup);
            auto vLayout = new QVBoxLayout;
            vLayout->addWidget(_p->levelsInWidget[0]);
            vLayout->addWidget(_p->levelsInWidget[1]);
            formLayout->addRow(
                qApp->translate("djv::ViewLib::DisplayProfileWidget", "Input:"),
                vLayout);
            formLayout->addRow(
                qApp->translate("djv::ViewLib::DisplayProfileWidget", "Gamma:"),
                _p->gammaWidget);
            vLayout = new QVBoxLayout;
            vLayout->addWidget(_p->levelsOutWidget[0]);
            vLayout->addWidget(_p->levelsOutWidget[1]);
            formLayout->addRow(
                qApp->translate("djv::ViewLib::DisplayProfileWidget", "Output:"),
                vLayout);
            formLayout->addRow(
                qApp->translate("djv::ViewLib::DisplayProfileWidget", "Soft clip:"),
                _p->softClipWidget);

            auto hLayout = new QHBoxLayout;
            hLayout->addStretch();
            hLayout->addWidget(_p->addButton);
            hLayout->addWidget(_p->resetButton);
            layout->addLayout(hLayout);

            // Initialize.
            styleUpdate();
            widgetUpdate();

            // Setup the LUT callbacks.
            connect(
                _p->lutWidget,
                SIGNAL(fileInfoChanged(const djv::Core::FileInfo &)),
                SLOT(lutCallback(const djv::Core::FileInfo &)));

            // Setup the color callbacks.
            connect(
                _p->brightnessWidget,
                SIGNAL(valueChanged(float)),
                SLOT(brightnessCallback(float)));
            connect(
                _p->contrastWidget,
                SIGNAL(valueChanged(float)),
                SLOT(contrastCallback(float)));
            connect(
                _p->saturationWidget,
                SIGNAL(valueChanged(float)),
                SLOT(saturationCallback(float)));

            // Setup the levels callbacks.
            connect(
                _p->levelsInWidget[0],
                SIGNAL(valueChanged(float)),
                SLOT(levelsIn0Callback(float)));
            connect(
                _p->levelsInWidget[1],
                SIGNAL(valueChanged(float)),
                SLOT(levelsIn1Callback(float)));
            connect(
                _p->gammaWidget,
                SIGNAL(valueChanged(float)),
                SLOT(gammaCallback(float)));
            connect(
                _p->levelsOutWidget[0],
                SIGNAL(valueChanged(float)),
                SLOT(levelsOut0Callback(float)));
            connect(
                _p->levelsOutWidget[1],
                SIGNAL(valueChanged(float)),
                SLOT(levelsOut1Callback(float)));
            connect(
                _p->softClipWidget,
                SIGNAL(valueChanged(float)),
                SLOT(softClipCallback(float)));

            // Setup the other callbacks.
            connect(
                _p->addButton,
                SIGNAL(clicked()),
                SLOT(addCallback()));
            connect(
                _p->resetButton,
                SIGNAL(clicked()),
                SLOT(resetCallback()));
        }

        DisplayProfileWidget::~DisplayProfileWidget()
        {}

        const DisplayProfile & DisplayProfileWidget::displayProfile() const
        {
            return _p->displayProfile;
        }

        void DisplayProfileWidget::setDisplayProfile(const DisplayProfile & displayProfile)
        {
            if (displayProfile == _p->displayProfile)
                return;
            _p->displayProfile = displayProfile;
            widgetUpdate();
            Q_EMIT displayProfileChanged(_p->displayProfile);
        }

        bool DisplayProfileWidget::event(QEvent * event)
        {
            if (QEvent::StyleChange == event->type())
            {
                styleUpdate();
            }
            return QWidget::event(event);
        }

        void DisplayProfileWidget::lutCallback(const Core::FileInfo & in)
        {
            _p->displayProfile.lutFile = in;
            try
            {
                Util::loadLut(
                    _p->displayProfile.lutFile,
                    _p->displayProfile.lut,
                    _p->context);
            }
            catch (const Core::Error & error)
            {
                _p->displayProfile = DisplayProfile();
                _p->context->printError(error);
            }
            Q_EMIT displayProfileChanged(_p->displayProfile);
        }

        void DisplayProfileWidget::brightnessCallback(float value)
        {
            //DJV_DEBUG("DisplayProfileWidget::brightnessCallback");
            //DJV_DEBUG_PRINT("value = " << value);
            _p->displayProfile.color.brightness = value;
            Q_EMIT displayProfileChanged(_p->displayProfile);
        }

        void DisplayProfileWidget::contrastCallback(float in)
        {
            _p->displayProfile.color.contrast = in;
            Q_EMIT displayProfileChanged(_p->displayProfile);
        }

        void DisplayProfileWidget::saturationCallback(float in)
        {
            _p->displayProfile.color.saturation = in;
            Q_EMIT displayProfileChanged(_p->displayProfile);
        }

        void DisplayProfileWidget::levelsIn0Callback(float in)
        {
            _p->displayProfile.levels.inLow = in;
            Q_EMIT displayProfileChanged(_p->displayProfile);
        }

        void DisplayProfileWidget::levelsIn1Callback(float in)
        {
            _p->displayProfile.levels.inHigh = in;
            Q_EMIT displayProfileChanged(_p->displayProfile);
        }

        void DisplayProfileWidget::gammaCallback(float in)
        {
            _p->displayProfile.levels.gamma = in;
            Q_EMIT displayProfileChanged(_p->displayProfile);
        }

        void DisplayProfileWidget::levelsOut0Callback(float in)
        {
            _p->displayProfile.levels.outLow = in;
            Q_EMIT displayProfileChanged(_p->displayProfile);
        }

        void DisplayProfileWidget::levelsOut1Callback(float in)
        {
            _p->displayProfile.levels.outHigh = in;
            Q_EMIT displayProfileChanged(_p->displayProfile);
        }

        void DisplayProfileWidget::softClipCallback(float in)
        {
            _p->displayProfile.softClip = in;
            Q_EMIT displayProfileChanged(_p->displayProfile);
        }

        void DisplayProfileWidget::addCallback()
        {
            UI::InputDialog dialog(
                qApp->translate("djv::ViewLib::DisplayProfileWidget", "Name this display profile:"));
            if (QDialog::Accepted == dialog.exec())
            {
                QVector<DisplayProfile> displayProfiles =
                    _p->context->imagePrefs()->displayProfiles();
                DisplayProfile displayProfile(_p->displayProfile);
                displayProfile.name = dialog.text();
                displayProfiles += displayProfile;
                _p->context->imagePrefs()->setDisplayProfiles(displayProfiles);
            }
        }

        void DisplayProfileWidget::resetCallback()
        {
            _p->displayProfile = DisplayProfile();
            widgetUpdate();
            Q_EMIT displayProfileChanged(_p->displayProfile);
        }

        void DisplayProfileWidget::styleUpdate()
        {
            _p->addButton->setIcon(_p->context->iconLibrary()->icon("djv/UI/AddIcon"));
            _p->resetButton->setIcon(_p->context->iconLibrary()->icon("djv/UI/RemoveIcon"));
        }
        
        void DisplayProfileWidget::widgetUpdate()
        {
            Core::SignalBlocker signalBlocker(QObjectList() <<
                _p->lutWidget <<
                _p->brightnessWidget <<
                _p->contrastWidget <<
                _p->saturationWidget <<
                _p->levelsInWidget[0] <<
                _p->levelsInWidget[1] <<
                _p->gammaWidget <<
                _p->levelsOutWidget[0] <<
                _p->levelsOutWidget[1] <<
                _p->softClipWidget);
            _p->lutWidget->setFileInfo(_p->displayProfile.lutFile);
            _p->brightnessWidget->setValue(_p->displayProfile.color.brightness);
            _p->contrastWidget->setValue(_p->displayProfile.color.contrast);
            _p->saturationWidget->setValue(_p->displayProfile.color.saturation);
            _p->levelsInWidget[0]->setValue(_p->displayProfile.levels.inLow);
            _p->levelsInWidget[1]->setValue(_p->displayProfile.levels.inHigh);
            _p->gammaWidget->setValue(_p->displayProfile.levels.gamma);
            _p->levelsOutWidget[0]->setValue(_p->displayProfile.levels.outLow);
            _p->levelsOutWidget[1]->setValue(_p->displayProfile.levels.outHigh);
            _p->softClipWidget->setValue(_p->displayProfile.softClip);
        }

    } // namespace ViewLib
} // namespace djv
