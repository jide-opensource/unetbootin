/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QABSTRACTSPINBOX_P_H
#define QABSTRACTSPINBOX_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "QtGui/qabstractspinbox.h"

#ifndef QT_NO_SPINBOX

#include "QtGui/qlineedit.h"
#include "QtGui/qstyleoption.h"
#include "QtGui/qvalidator.h"
#include "QtCore/qdatetime.h"
#include "QtCore/qvariant.h"
#include "private/qwidget_p.h"
#include "private/qdatetime_p.h"

QT_BEGIN_NAMESPACE

QVariant operator+(const QVariant &arg1, const QVariant &arg2);
QVariant operator-(const QVariant &arg1, const QVariant &arg2);
QVariant operator*(const QVariant &arg1, double multiplier);
double operator/(const QVariant &arg1, const QVariant &arg2);

enum EmitPolicy {
    EmitIfChanged,
    AlwaysEmit,
    NeverEmit
};

enum Button {
    None = 0x000,
    Keyboard = 0x001,
    Mouse = 0x002,
    Wheel = 0x004,
    ButtonMask = 0x008,
    Up = 0x010,
    Down = 0x020,
    DirectionMask = 0x040
};
class QSpinBoxValidator;
class QAbstractSpinBoxPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QAbstractSpinBox)
public:
    QAbstractSpinBoxPrivate();
    ~QAbstractSpinBoxPrivate();

    void init();
    void reset();
    void updateState(bool up);
    QString stripped(const QString &text, int *pos = 0) const;
    bool specialValue() const;
    virtual QVariant getZeroVariant() const;
    virtual void setRange(const QVariant &min, const QVariant &max);
    void setValue(const QVariant &val, EmitPolicy ep, bool updateEdit = true);
    virtual QVariant bound(const QVariant &val, const QVariant &old = QVariant(), int steps = 0) const;
    virtual void updateEdit();

    virtual void emitSignals(EmitPolicy ep, const QVariant &old);
    virtual void interpret(EmitPolicy ep);
    virtual QString textFromValue(const QVariant &n) const;
    virtual QVariant valueFromText(const QString &input) const;

    void _q_editorTextChanged(const QString &);
    virtual void _q_editorCursorPositionChanged(int oldpos, int newpos);

    virtual QStyle::SubControl newHoverControl(const QPoint &pos);
    bool updateHoverControl(const QPoint &pos);

    virtual void clearCache() const;
    virtual void updateEditFieldGeometry();

    static int variantCompare(const QVariant &arg1, const QVariant &arg2);
    static QVariant variantBound(const QVariant &min, const QVariant &value, const QVariant &max);

    QLineEdit *edit;
    QString prefix, suffix, specialValueText;
    QVariant value, minimum, maximum, singleStep;
    QVariant::Type type;
    int spinClickTimerId, spinClickTimerInterval, spinClickThresholdTimerId, spinClickThresholdTimerInterval;
    uint buttonState;
    mutable QString cachedText;
    mutable QVariant cachedValue;
    mutable QValidator::State cachedState;
    mutable QSize cachedSizeHint, cachedMinimumSizeHint;
    uint pendingEmit : 1;
    uint spindownEnabled : 1;
    uint spinupEnabled : 1;
    uint readOnly : 1;
    uint wrapping : 1;
    uint ignoreCursorPositionChanged : 1;
    uint frame : 1;
    uint accelerate : 1;
    uint keyboardTracking : 1;
    uint cleared : 1;
    uint ignoreUpdateEdit : 1;
    QAbstractSpinBox::CorrectionMode correctionMode;
    int acceleration;
    QStyle::SubControl hoverControl;
    QRect hoverRect;
    QAbstractSpinBox::ButtonSymbols buttonSymbols;
    QSpinBoxValidator *validator;
};

class QSpinBoxValidator : public QValidator
{
public:
    QSpinBoxValidator(QAbstractSpinBox *qptr, QAbstractSpinBoxPrivate *dptr);
    QValidator::State validate(QString &input, int &) const;
    void fixup(QString &) const;
private:
    QAbstractSpinBox *qptr;
    QAbstractSpinBoxPrivate *dptr;
};

QT_END_NAMESPACE

#endif // QT_NO_SPINBOX

#endif // QABSTRACTSPINBOX_P_H
