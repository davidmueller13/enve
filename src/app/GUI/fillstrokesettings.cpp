﻿#include "fillstrokesettings.h"
#include "GUI/GradientWidgets/gradientwidget.h"
#include "mainwindow.h"
#include "undoredo.h"
#include "canvas.h"
#include "qrealanimatorvalueslider.h"
#include "GUI/ColorWidgets/colorsettingswidget.h"
#include "GUI/canvaswindow.h"
#include "actionbutton.h"
#include "qdoubleslider.h"
#include "segment1deditor.h"
#include "namedcontainer.h"
#include <QDockWidget>
#include "paintsettingsapplier.h"
#include "Animators/gradient.h"

FillStrokeSettingsWidget::FillStrokeSettingsWidget(Document &document,
                                                   QWidget * const parent) :
    QTabWidget(parent), mDocument(document) {
    connect(&mDocument, &Document::selectedPaintSettingsChanged,
            this, &FillStrokeSettingsWidget::updateCurrentSettings);
    //setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    mGradientWidget = new GradientWidget(this);
    mStrokeSettingsWidget = new QWidget(this);
    mColorsSettingsWidget = new ColorSettingsWidget(this);

    mTargetLayout->setSpacing(0);
    const auto iconsDir = EnveSettings::sIconsDir();
    mFillTargetButton = new QPushButton(QIcon(iconsDir + "/properties_fill.png"),
                                        "Fill", this);
    mFillTargetButton->setObjectName("leftButton");
    mStrokeTargetButton = new QPushButton(QIcon(iconsDir + "/properties_stroke_paint.png"),
                                          "Stroke", this);
    mStrokeTargetButton->setObjectName("rightButton");
    mFillAndStrokeWidget = new QWidget(this);
    mFillAndStrokeWidget->setLayout(mMainLayout);
    mMainLayout->setAlignment(Qt::AlignTop);

    mColorTypeLayout = new QHBoxLayout();
    mColorTypeLayout->setSpacing(0);
    mFillNoneButton = new QPushButton(QIcon(iconsDir + "/fill_none.png"),
                                      "None", this);
    mFillNoneButton->setCheckable(true);
    mFillNoneButton->setObjectName("leftButton");
    connect(mFillNoneButton, &QPushButton::released,
            this, &FillStrokeSettingsWidget::setNoneFill);
    mFillFlatButton = new QPushButton(QIcon(iconsDir + "/fill_flat.png"),
                                      "Flat", this);
    mFillFlatButton->setCheckable(true);
    mFillFlatButton->setObjectName("middleButton");
    connect(mFillFlatButton, &QPushButton::released,
            this, &FillStrokeSettingsWidget::setFlatFill);
    mFillGradientButton = new QPushButton(QIcon(iconsDir + "/fill_gradient.png"),
                                          "Gradient", this);
    mFillGradientButton->setCheckable(true);
    mFillGradientButton->setObjectName("rightButton");
    connect(mFillGradientButton, &QPushButton::released,
            this, &FillStrokeSettingsWidget::setGradientFill);

    mColorTypeLayout->addWidget(mFillNoneButton);
    mColorTypeLayout->addWidget(mFillFlatButton);
    mColorTypeLayout->addWidget(mFillGradientButton);

    mFillTargetButton->setCheckable(true);
    mFillTargetButton->setFocusPolicy(Qt::NoFocus);
    mStrokeTargetButton->setCheckable(true);
    mStrokeTargetButton->setFocusPolicy(Qt::NoFocus);
    mTargetLayout->addWidget(mFillTargetButton);
    mTargetLayout->addWidget(mStrokeTargetButton);

    //mLineWidthSpin = new QDoubleSpinBox(this);
    mLineWidthSpin = new QrealAnimatorValueSlider("line width", 0., 1000., 1., this);
    mLineWidthSpin->setNameVisible(false);
    //mLineWidthSpin->setValueSliderVisibile(false);
    //mLineWidthSpin->setRange(0.0, 1000.0);
    //mLineWidthSpin->setSuffix(" px");
    //mLineWidthSpin->setSingleStep(0.1);
    mLineWidthLayout->addWidget(mLineWidthLabel);
    mLineWidthLayout->addWidget(mLineWidthSpin, Qt::AlignLeft);

    mStrokeSettingsLayout->addLayout(mLineWidthLayout);

    mJoinStyleLayout->setSpacing(0);
    mBevelJoinStyleButton = new QPushButton(QIcon(iconsDir + "/joinBevel.png"), "", this);
    mBevelJoinStyleButton->setObjectName("leftButton");
    mMiterJointStyleButton = new QPushButton(QIcon(iconsDir + "/joinMiter.png"), "", this);
    mMiterJointStyleButton->setObjectName("middleButton");
    mRoundJoinStyleButton = new QPushButton(QIcon(iconsDir + "/joinRound.png"), "", this);
    mRoundJoinStyleButton->setObjectName("rightButton");

    mBevelJoinStyleButton->setCheckable(true);
    mMiterJointStyleButton->setCheckable(true);
    mRoundJoinStyleButton->setCheckable(true);
    connect(mBevelJoinStyleButton, &QPushButton::released,
            this, &FillStrokeSettingsWidget::setBevelJoinStyle);
    connect(mMiterJointStyleButton, &QPushButton::released,
            this, &FillStrokeSettingsWidget::setMiterJoinStyle);
    connect(mRoundJoinStyleButton, &QPushButton::released,
            this, &FillStrokeSettingsWidget::setRoundJoinStyle);

    mJoinStyleLayout->addWidget(new QLabel("Join:", this));
    mJoinStyleLayout->addWidget(mBevelJoinStyleButton);
    mJoinStyleLayout->addWidget(mMiterJointStyleButton);
    mJoinStyleLayout->addWidget(mRoundJoinStyleButton);

    mStrokeJoinCapWidget = new QWidget(this);
    const auto strokeJoinCapLay = new QVBoxLayout(mStrokeJoinCapWidget);
    mStrokeJoinCapWidget->setLayout(strokeJoinCapLay);
    strokeJoinCapLay->addLayout(mJoinStyleLayout);

    mCapStyleLayout->setSpacing(0);
    mFlatCapStyleButton = new QPushButton(QIcon(iconsDir + "/capFlat.png"),
                                          "", this);
    mFlatCapStyleButton->setObjectName("leftButton");
    mSquareCapStyleButton = new QPushButton(QIcon(iconsDir + "/capSquare.png"),
                                            "", this);
    mSquareCapStyleButton->setObjectName("middleButton");
    mRoundCapStyleButton = new QPushButton(QIcon(iconsDir + "/capRound.png"),
                                           "", this);
    mRoundCapStyleButton->setObjectName("rightButton");
    mFlatCapStyleButton->setCheckable(true);
    mSquareCapStyleButton->setCheckable(true);
    mRoundCapStyleButton->setCheckable(true);
    connect(mFlatCapStyleButton, &QPushButton::released,
            this, &FillStrokeSettingsWidget::setFlatCapStyle);
    connect(mSquareCapStyleButton, &QPushButton::released,
            this, &FillStrokeSettingsWidget::setSquareCapStyle);
    connect(mRoundCapStyleButton, &QPushButton::released,
            this, &FillStrokeSettingsWidget::setRoundCapStyle);

    mCapStyleLayout->addWidget(new QLabel("Cap:", this));
    mCapStyleLayout->addWidget(mFlatCapStyleButton);
    mCapStyleLayout->addWidget(mSquareCapStyleButton);
    mCapStyleLayout->addWidget(mRoundCapStyleButton);

    strokeJoinCapLay->addLayout(mCapStyleLayout);

    const auto actions = Actions::sInstance;
    connect(mLineWidthSpin, &QrealAnimatorValueSlider::editingStarted,
            actions, [actions]() {
        actions->strokeWidthAction(QrealAction::sMakeStart());
    });
    connect(mLineWidthSpin, &QrealAnimatorValueSlider::valueChanged,
            actions, [actions](const qreal value) {
        actions->strokeWidthAction(QrealAction::sMakeSet(value));
    });
    connect(mLineWidthSpin, &QrealAnimatorValueSlider::editingFinished,
            actions, [actions]() {
        actions->strokeWidthAction(QrealAction::sMakeFinish());
    });

    mStrokeSettingsLayout->addWidget(mStrokeJoinCapWidget);
    mStrokeSettingsWidget->setLayout(mStrokeSettingsLayout);

    connect(mFillTargetButton, &QPushButton::released,
            this, &FillStrokeSettingsWidget::setFillTarget);
    connect(mStrokeTargetButton, &QPushButton::released,
            this, &FillStrokeSettingsWidget::setStrokeTarget);

    connect(mColorsSettingsWidget, &ColorSettingsWidget::colorSettingSignal,
            this, &FillStrokeSettingsWidget::colorSettingReceived);

    mGradientTypeLayout = new QHBoxLayout();
    mGradientTypeLayout->setSpacing(0);
    mLinearGradientButton = new QPushButton(QIcon(iconsDir + "/fill_gradient.png"),
                                            "Linear", this);
    mLinearGradientButton->setCheckable(true);
    mLinearGradientButton->setObjectName("leftButton");
    connect(mLinearGradientButton, &QPushButton::released,
            this, &FillStrokeSettingsWidget::setLinearGradientFill);

    mRadialGradientButton = new QPushButton(QIcon(iconsDir + "/fill_gradient_radial.png"),
                                            "Radial", this);
    mRadialGradientButton->setCheckable(true);
    mRadialGradientButton->setObjectName("rightButton");
    connect(mRadialGradientButton, &QPushButton::released,
            this, &FillStrokeSettingsWidget::setRadialGradientFill);

    mGradientTypeLayout->addWidget(mLinearGradientButton);
    mGradientTypeLayout->addWidget(mRadialGradientButton);
    mGradientTypeWidget = new QWidget(this);
    mGradientTypeWidget->setContentsMargins(0, 0, 0, 0);
    mGradientTypeWidget->setLayout(mGradientTypeLayout);

    mMainLayout->addLayout(mTargetLayout);
    mMainLayout->addLayout(mColorTypeLayout);
    mMainLayout->addWidget(mGradientTypeWidget);
    mMainLayout->addWidget(mStrokeSettingsWidget);
    mMainLayout->addWidget(mGradientWidget);
    mMainLayout->addWidget(mColorsSettingsWidget);

    setTabPosition(QTabWidget::South);
    mMainLayout->addStretch(1);

    const auto fillAndStrokeArea = new ScrollArea(this);
    fillAndStrokeArea->setWidget(mFillAndStrokeWidget);
    addTab(fillAndStrokeArea, "Fill and Stroke");

    mGradientTypeWidget->hide();

    setFillTarget();
    setCapStyle(SkPaint::kRound_Cap);
    setJoinStyle(SkPaint::kRound_Join);
}

void FillStrokeSettingsWidget::setLinearGradientFill() {
    setGradientType(GradientType::LINEAR);
}

void FillStrokeSettingsWidget::setRadialGradientFill() {
    setGradientType(GradientType::RADIAL);
}

void FillStrokeSettingsWidget::setGradientFill() {
    if(mTarget == PaintSetting::OUTLINE) mStrokeJoinCapWidget->show();
    mFillGradientButton->setChecked(true);
    mFillFlatButton->setChecked(false);
    mFillNoneButton->setChecked(false);
    colorTypeSet(GRADIENTPAINT);
}

void FillStrokeSettingsWidget::setFlatFill() {
    if(mTarget == PaintSetting::OUTLINE) mStrokeJoinCapWidget->show();
    mFillGradientButton->setChecked(false);
    mFillFlatButton->setChecked(true);
    mFillNoneButton->setChecked(false);
    colorTypeSet(FLATPAINT);
}

void FillStrokeSettingsWidget::setNoneFill() {
    if(mTarget == PaintSetting::OUTLINE) mStrokeJoinCapWidget->show();
    mFillGradientButton->setChecked(false);
    mFillFlatButton->setChecked(false);
    mFillNoneButton->setChecked(true);
    colorTypeSet(NOPAINT);
}

void FillStrokeSettingsWidget::updateColorAnimator() {
    if(getCurrentPaintTypeVal() == NOPAINT) {
        setColorAnimatorTarget(nullptr);
    } else if(getCurrentPaintTypeVal() == FLATPAINT) {
        if(mTarget == PaintSetting::FILL) {
            setColorAnimatorTarget(mCurrentFillColorAnimator);
        } else {
            setColorAnimatorTarget(mCurrentStrokeColorAnimator);
        }
    } else if(getCurrentPaintTypeVal() == GRADIENTPAINT) {
        setColorAnimatorTarget(mGradientWidget->getColorAnimator());
    }
}

void FillStrokeSettingsWidget::updateAfterTargetChanged() {
    if(getCurrentPaintTypeVal() == GRADIENTPAINT) {
        mGradientWidget->setCurrentGradient(getCurrentGradientVal(), false);
        const auto gradType = getCurrentGradientTypeVal();
        mLinearGradientButton->setChecked(gradType == GradientType::LINEAR);
        mRadialGradientButton->setChecked(gradType == GradientType::RADIAL);
    }
    setCurrentPaintType(getCurrentPaintTypeVal());
    if(getCurrentPaintTypeVal() == NOPAINT) {
        mFillGradientButton->setChecked(false);
        mFillFlatButton->setChecked(false);
        mFillNoneButton->setChecked(true);
    } else if(getCurrentPaintTypeVal() == FLATPAINT) {
        mFillGradientButton->setChecked(false);
        mFillFlatButton->setChecked(true);
        mFillNoneButton->setChecked(false);
    } else if(getCurrentPaintTypeVal() == GRADIENTPAINT) {
        mFillGradientButton->setChecked(true);
        mFillFlatButton->setChecked(false);
        mFillNoneButton->setChecked(false);
    }
}

void FillStrokeSettingsWidget::setCurrentPaintType(
        const PaintType &paintType) {
    if(paintType == NOPAINT) setNoPaintType();
    else if(paintType == FLATPAINT) setFlatPaintType();
    else setGradientPaintType();
}

void FillStrokeSettingsWidget::updateCurrentSettings() {
    const auto scene = mDocument.fActiveScene;
    if(scene) {
        PaintSettingsAnimator* fillSetings;
        OutlineSettingsAnimator* strokeSettings;
        scene->getDisplayedFillStrokeSettingsFromLastSelected(
                    fillSetings, strokeSettings);
        setCurrentSettings(fillSetings, strokeSettings);
    } else {
        setCurrentSettings(nullptr, nullptr);
    }
}

void FillStrokeSettingsWidget::setCurrentSettings(
        PaintSettingsAnimator *fillPaintSettings,
        OutlineSettingsAnimator *strokePaintSettings) {
    setFillValuesFromFillSettings(fillPaintSettings);
    setStrokeValuesFromStrokeSettings(strokePaintSettings);
    //mLineWidthSpin->setValue(strokePaintSettings->getCurrentStrokeWidth());

    if(mTarget == PaintSetting::FILL) setFillTarget();
    else setStrokeTarget();
}

GradientWidget *FillStrokeSettingsWidget::getGradientWidget() {
    return mGradientWidget;
}

void FillStrokeSettingsWidget::clearAll() {
    mGradientWidget->clearAll();
}

void FillStrokeSettingsWidget::colorTypeSet(const PaintType &type) {
    if(type == NOPAINT) {
        setNoPaintType();
    } else if(type == FLATPAINT) {
        setFlatPaintType();
    } else if(type == GRADIENTPAINT) {
        if(mTarget == PaintSetting::FILL ? !mCurrentFillGradient :
                !mCurrentStrokeGradient) {
            mGradientWidget->setCurrentGradient(nullptr);
        }
        setGradientPaintType();
    } else {
        RuntimeThrow("Invalid fill type.");
    }

    PaintType currentPaintType;
    Gradient *currentGradient;
    GradientType currentGradientType;
    if(mTarget == PaintSetting::FILL) {
        currentPaintType = mCurrentFillPaintType;
        currentGradient = mCurrentFillGradient;
        currentGradientType = mCurrentFillGradientType;
    } else {
        currentPaintType = mCurrentStrokePaintType;
        currentGradient = mCurrentStrokeGradient;
        currentGradientType = mCurrentStrokeGradientType;
    }
    PaintSettingsApplier paintSetting;
    if(currentPaintType == FLATPAINT) {
//        const auto colorSetting = mColorsSettingsWidget->getColorSetting(ColorSettingType::apply,
//                                                                         ColorParameter::all);
//        paintSetting << std::make_shared<ColorPaintSetting>(
//                            mTarget, colorSetting);
    } else if(currentPaintType == GRADIENTPAINT) {
        paintSetting << std::make_shared<GradientPaintSetting>(mTarget, currentGradient);
        paintSetting << std::make_shared<GradientTypePaintSetting>(mTarget, currentGradientType);
    }
    paintSetting << std::make_shared<PaintTypePaintSetting>(mTarget, currentPaintType);
    const auto scene = mDocument.fActiveScene;
    if(scene) scene->applyPaintSettingToSelected(paintSetting);
}

void FillStrokeSettingsWidget::colorSettingReceived(
        const ColorSetting &colorSetting) {
    PaintSettingsApplier paintSetting;
    paintSetting << std::make_shared<ColorPaintSetting>(mTarget, colorSetting);
    const auto scene = mDocument.fActiveScene;
    if(scene) scene->applyPaintSettingToSelected(paintSetting);
    mDocument.setBrushColor(colorSetting.getColor());
}

void FillStrokeSettingsWidget::connectGradient() {
    connect(mGradientWidget,
            &GradientWidget::selectedColorChanged,
            mColorsSettingsWidget,
            &ColorSettingsWidget::setTarget);
    connect(mGradientWidget,
            &GradientWidget::currentGradientChanged,
            this, &FillStrokeSettingsWidget::setGradient);
}

void FillStrokeSettingsWidget::disconnectGradient() {
    disconnect(mGradientWidget,
               &GradientWidget::selectedColorChanged,
               mColorsSettingsWidget,
               &ColorSettingsWidget::setTarget);
    disconnect(mGradientWidget, &GradientWidget::currentGradientChanged,
               this, &FillStrokeSettingsWidget::setGradient);
}

void FillStrokeSettingsWidget::setColorAnimatorTarget(
        ColorAnimator *animator) {
    mColorsSettingsWidget->setTarget(animator);
}

void FillStrokeSettingsWidget::setJoinStyle(const SkPaint::Join joinStyle) {
    mCurrentJoinStyle = joinStyle;
    mBevelJoinStyleButton->setChecked(joinStyle == SkPaint::kBevel_Join);
    mMiterJointStyleButton->setChecked(joinStyle == SkPaint::kMiter_Join);
    mRoundJoinStyleButton->setChecked(joinStyle == SkPaint::kRound_Join);
}

void FillStrokeSettingsWidget::setCapStyle(const SkPaint::Cap capStyle) {
    mCurrentCapStyle = capStyle;
    mFlatCapStyleButton->setChecked(capStyle == SkPaint::kButt_Cap);
    mSquareCapStyleButton->setChecked(capStyle == SkPaint::kSquare_Cap);
    mRoundCapStyleButton->setChecked(capStyle == SkPaint::kRound_Cap);
}

PaintType FillStrokeSettingsWidget::getCurrentPaintTypeVal() {
    if(mTarget == PaintSetting::FILL) return mCurrentFillPaintType;
    else return mCurrentStrokePaintType;
}

void FillStrokeSettingsWidget::setCurrentPaintTypeVal(const PaintType& paintType) {
    if(mTarget == PaintSetting::FILL) mCurrentFillPaintType = paintType;
    else mCurrentStrokePaintType = paintType;
}

QColor FillStrokeSettingsWidget::getColorVal() {
    if(mTarget == PaintSetting::FILL) return mCurrentFillColor;
    else return mCurrentStrokeColor;
}

void FillStrokeSettingsWidget::setCurrentColorVal(const QColor& color) {
    if(mTarget == PaintSetting::FILL) mCurrentFillColor = color;
    else mCurrentStrokeColor = color;
}

Gradient *FillStrokeSettingsWidget::getCurrentGradientVal() {
    if(mTarget == PaintSetting::FILL) return mCurrentFillGradient;
    else return mCurrentStrokeGradient;
}

void FillStrokeSettingsWidget::setCurrentGradientVal(Gradient *gradient) {
    if(mTarget == PaintSetting::FILL) mCurrentFillGradient = gradient;
    else mCurrentStrokeGradient = gradient;
}

void FillStrokeSettingsWidget::setCurrentGradientTypeVal(
                                const GradientType &type) {
    if(mTarget == PaintSetting::FILL) mCurrentFillGradientType = type;
    else mCurrentStrokeGradientType = type;
}

void FillStrokeSettingsWidget::setFillValuesFromFillSettings(
        PaintSettingsAnimator *settings) {
    if(settings) {
        mCurrentFillGradientType = settings->getGradientType();
        mCurrentFillColor = settings->getColor();
        mCurrentFillColorAnimator = settings->getColorAnimator();
        mCurrentFillGradient = settings->getGradient();
        mCurrentFillPaintType = settings->getPaintType();
    } else {
        mCurrentFillColorAnimator = nullptr;
    }
}

void FillStrokeSettingsWidget::setStrokeValuesFromStrokeSettings(
        OutlineSettingsAnimator *settings) {
    if(settings) {
        mCurrentStrokeGradientType = settings->getGradientType();
        mCurrentStrokeColor = settings->getColor();
        mCurrentStrokeColorAnimator = settings->getColorAnimator();
        mCurrentStrokeGradient = settings->getGradient();
        mCurrentStrokePaintType = settings->getPaintType();
        mCurrentStrokeWidth = settings->getCurrentStrokeWidth();
        mLineWidthSpin->setTarget(settings->getLineWidthAnimator());
        mCurrentCapStyle = settings->getCapStyle();
        mCurrentJoinStyle = settings->getJoinStyle();
    } else {
        mCurrentStrokeColorAnimator = nullptr;
        mLineWidthSpin->clearTarget();
    }
}

void FillStrokeSettingsWidget::emitCapStyleChanged() {
    const auto scene = mDocument.fActiveScene;
    if(scene) scene->setSelectedCapStyle(mCurrentCapStyle);
}

void FillStrokeSettingsWidget::emitJoinStyleChanged() {
    const auto scene = mDocument.fActiveScene;
    if(scene) scene->setSelectedJoinStyle(mCurrentJoinStyle);
}

void FillStrokeSettingsWidget::applyGradient() {
    Gradient *currentGradient;
    GradientType currentGradientType;
    if(mTarget == PaintSetting::FILL) {
        currentGradient = mCurrentFillGradient;
        currentGradientType = mCurrentFillGradientType;
    } else {
        currentGradient = mCurrentStrokeGradient;
        currentGradientType = mCurrentStrokeGradientType;
    }
    PaintSettingsApplier applier;
    applier << std::make_shared<GradientPaintSetting>(mTarget, currentGradient) <<
               std::make_shared<GradientTypePaintSetting>(mTarget, currentGradientType) <<
               std::make_shared<PaintTypePaintSetting>(mTarget, PaintType::GRADIENTPAINT);

    const auto scene = mDocument.fActiveScene;
    if(scene) scene->applyPaintSettingToSelected(applier);
}

void FillStrokeSettingsWidget::setGradient(Gradient *gradient) {
    setCurrentGradientVal(gradient);
    applyGradient();
}

void FillStrokeSettingsWidget::setGradientType(const GradientType &type) {
    setCurrentGradientTypeVal(type);
    mLinearGradientButton->setChecked(type == GradientType::LINEAR);
    mRadialGradientButton->setChecked(type == GradientType::RADIAL);
    applyGradient();
}

void FillStrokeSettingsWidget::setBevelJoinStyle() {
    setJoinStyle(SkPaint::kBevel_Join);
    emitJoinStyleChanged();
}

void FillStrokeSettingsWidget::setMiterJoinStyle() {
    setJoinStyle(SkPaint::kMiter_Join);
    emitJoinStyleChanged();
}

void FillStrokeSettingsWidget::setRoundJoinStyle() {
    setJoinStyle(SkPaint::kRound_Join);
    emitJoinStyleChanged();
}

void FillStrokeSettingsWidget::setFlatCapStyle() {
    setCapStyle(SkPaint::kButt_Cap);
    emitCapStyleChanged();
}

void FillStrokeSettingsWidget::setSquareCapStyle() {
    setCapStyle(SkPaint::kSquare_Cap);
    emitCapStyleChanged();
}

void FillStrokeSettingsWidget::setRoundCapStyle() {
    setCapStyle(SkPaint::kRound_Cap);
    emitCapStyleChanged();
}

void FillStrokeSettingsWidget::setFillTarget() {
    mTarget = PaintSetting::FILL;
    mFillTargetButton->setChecked(true);
    mStrokeTargetButton->setChecked(false);
    mStrokeSettingsWidget->hide();
    updateAfterTargetChanged();
    updateColorAnimator();
}

void FillStrokeSettingsWidget::setStrokeTarget() {
    mTarget = PaintSetting::OUTLINE;
    mStrokeTargetButton->setChecked(true);
    mFillTargetButton->setChecked(false);
    mStrokeSettingsWidget->show();
    updateAfterTargetChanged();
    updateColorAnimator();
}

void FillStrokeSettingsWidget::setNoPaintType() {
    setCurrentPaintTypeVal(NOPAINT);
    mColorsSettingsWidget->hide();
    mGradientWidget->hide();
    mGradientTypeWidget->hide();
    tabBar()->hide();
    setCurrentIndex(0);
    updateColorAnimator();
}

void FillStrokeSettingsWidget::setFlatPaintType() {
    disconnectGradient();
    mColorsSettingsWidget->show();
    mGradientWidget->hide();
    mGradientTypeWidget->hide();
    tabBar()->hide();
    setCurrentIndex(0);
    setCurrentPaintTypeVal(FLATPAINT);
    updateColorAnimator();
    if(mTarget == PaintSetting::OUTLINE) mStrokeJoinCapWidget->show();
}

void FillStrokeSettingsWidget::setGradientPaintType() {
    connectGradient();
    if(mTarget == PaintSetting::FILL) {
        mCurrentFillPaintType = GRADIENTPAINT;
        if(!mCurrentFillGradient) {
            mCurrentFillGradient = mGradientWidget->getCurrentGradient();
        }
    } else {
        mCurrentStrokePaintType = GRADIENTPAINT;
        if(!mCurrentStrokeGradient) {
            mCurrentStrokeGradient = mGradientWidget->getCurrentGradient();
        }
    }
    if(mColorsSettingsWidget->isHidden()) mColorsSettingsWidget->show();
    if(mGradientWidget->isHidden()) mGradientWidget->show();
    if(mGradientTypeWidget->isHidden()) mGradientTypeWidget->show();
    tabBar()->hide();
    setCurrentIndex(0);
    updateColorAnimator();

    mGradientWidget->update();
    if(mTarget == PaintSetting::OUTLINE) mStrokeJoinCapWidget->show();
}