/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the plugins of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file.  Alternatively you may (at
** your option) use any later version of the GNU General Public
** License if such license has been publicly approved by Trolltech ASA
** (or its successors, if any) and the KDE Free Qt Foundation. In
** addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.2, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/. If
** you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech, as the sole
** copyright holder for Qt Designer, grants users of the Qt/Eclipse
** Integration plug-in the right for the Qt/Eclipse Integration to
** link to functionality provided by Qt Designer and its related
** libraries.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
** granted herein.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "simplewidgets.h"

#include <qabstractbutton.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qprogressbar.h>
#include <qradiobutton.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qlcdnumber.h>
#include <qlineedit.h>
#include <qstyle.h>
#include <qstyleoption.h>

#ifdef Q_OS_MAC
#include <qfocusframe.h>
#endif

QT_BEGIN_NAMESPACE

#ifndef QT_NO_ACCESSIBILITY

using namespace QAccessible2;
extern QList<QWidget*> childWidgets(const QWidget *widget, bool includeTopLevel = false);

QString Q_GUI_EXPORT qt_accStripAmp(const QString &text);
QString Q_GUI_EXPORT qt_accHotKey(const QString &text);

/*!
  \class QAccessibleButton qaccessible.h
  \brief The QAccessibleButton class implements the QAccessibleInterface for button type widgets.
  \internal

  \ingroup accessibility
*/

/*!
  Creates a QAccessibleButton object for \a w.
  \a role is propagated to the QAccessibleWidgetEx constructor.
*/
QAccessibleButton::QAccessibleButton(QWidget *w, Role role)
: QAccessibleWidgetEx(w, role)
{
    Q_ASSERT(button());
    if (button()->isCheckable())
        addControllingSignal(QLatin1String("toggled(bool)"));
    else
        addControllingSignal(QLatin1String("clicked()"));
}

/*! Returns the button. */
QAbstractButton *QAccessibleButton::button() const
{
    return qobject_cast<QAbstractButton*>(object());
}

/*! \reimp */
QString QAccessibleButton::actionText(int action, Text text, int child) const
{
    if (child)
        return QString();

    if (text == Name) switch (action) {
    case Press:
    case DefaultAction: // press, checking or open
        switch (role(0)) {
        case ButtonMenu:
            return QPushButton::tr("Open");
        case CheckBox:
            {
                if (state(child) & Checked)
                    return QCheckBox::tr("Uncheck");
                QCheckBox *cb = qobject_cast<QCheckBox*>(object());
                if (!cb || !cb->isTristate() || cb->checkState() == Qt::PartiallyChecked)
                    return QCheckBox::tr("Check");
                return QCheckBox::tr("Toggle");
            }
            break;
        case RadioButton:
            return QRadioButton::tr("Check");
        default:
            break;
        }
        break;
    }
    return QAccessibleWidgetEx::actionText(action, text, child);
}

/*! \reimp */
bool QAccessibleButton::doAction(int action, int child, const QVariantList &params)
{
    if (child || !widget()->isEnabled() || !widget()->isVisible())
        return false;

    switch (action) {
    case DefaultAction:
    case Press:
        {
#ifndef QT_NO_MENU
            QPushButton *pb = qobject_cast<QPushButton*>(object());
            if (pb && pb->menu())
                pb->showMenu();
            else
#endif
                button()->animateClick();
        }
        return true;
    }
    return QAccessibleWidgetEx::doAction(action, child, params);
}

/*! \reimp */
QString QAccessibleButton::text(Text t, int child) const
{
    QString str;
    if (!widget()->isVisible())
        return str;

    switch (t) {
    case Accelerator:
        {
#ifndef QT_NO_SHORTCUT
            QPushButton *pb = qobject_cast<QPushButton*>(object());
            if (pb && pb->isDefault())
                str = (QString)QKeySequence(Qt::Key_Enter);
#endif
            if (str.isEmpty())
                str = qt_accHotKey(button()->text());
        }
        break;
    case Name:
        str = widget()->accessibleName();
        if (str.isEmpty())
            str = button()->text();
        break;
    default:
        break;
    }
    if (str.isEmpty())
        str = QAccessibleWidgetEx::text(t, child);;
    return qt_accStripAmp(str);
}

/*! \reimp */
QAccessible::State QAccessibleButton::state(int child) const
{
    State state = QAccessibleWidgetEx::state(child);

    QAbstractButton *b = button();
    QCheckBox *cb = qobject_cast<QCheckBox *>(b);
    if (b->isChecked())
        state |= Checked;
    else if (cb && cb->checkState() == Qt::PartiallyChecked)
        state |= Mixed;
    if (b->isDown())
        state |= Pressed;
    QPushButton *pb = qobject_cast<QPushButton*>(b);
    if (pb) {
        if (pb->isDefault())
            state |= DefaultButton;
#ifndef QT_NO_MENU
        if (pb->menu())
            state |= HasPopup;
#endif
    }

    return state;
}

#ifndef QT_NO_TOOLBUTTON
/*!
  \class QAccessibleToolButton qaccessible.h
  \brief The QAccessibleToolButton class implements the QAccessibleInterface for tool buttons.
  \internal

  \ingroup accessibility
*/

/*!
    \enum QAccessibleToolButton::ToolButtonElements

    This enum identifies the components of the tool button.

    \value ToolButtonSelf The tool button as a whole.
    \value ButtonExecute The button.
    \value ButtonDropMenu The drop down menu.
*/

/*!
  Creates a QAccessibleToolButton object for \a w.
  \a role is propagated to the QAccessibleWidgetEx constructor.
*/
QAccessibleToolButton::QAccessibleToolButton(QWidget *w, Role role)
: QAccessibleButton(w, role)
{
    Q_ASSERT(toolButton());
}

/*! Returns the button. */
QToolButton *QAccessibleToolButton::toolButton() const
{
    return qobject_cast<QToolButton*>(object());
}

/*!
    Returns true if this tool button is a split button.
*/
bool QAccessibleToolButton::isSplitButton() const
{
#ifndef QT_NO_MENU
    return toolButton()->menu() && toolButton()->popupMode() == QToolButton::MenuButtonPopup;
#else
    return false;
#endif
}

/*! \reimp */
QAccessible::Role QAccessibleToolButton::role(int child) const
{
    if (isSplitButton()) switch(child) {
    case ButtonExecute:
        return PushButton;
    case ButtonDropMenu:
        return ButtonMenu;
    }
    return QAccessibleButton::role(child);
}

/*! \reimp */
QAccessible::State QAccessibleToolButton::state(int child) const
{
    QAccessible::State st = QAccessibleButton::state(child);
    if (toolButton()->autoRaise())
        st |= HotTracked;
#ifndef QT_NO_MENU
    if (toolButton()->menu() && child != ButtonExecute)
        st |= HasPopup;
#endif
    return st;
}

/*! \reimp */
int QAccessibleToolButton::childCount() const
{
    if (!toolButton()->isVisible())
        return 0;
    return isSplitButton() ? ButtonDropMenu : 0;
}

/*!
    \internal

    Returns the rectangle occupied by this button, depending on \a
    child.
*/
QRect QAccessibleToolButton::rect(int child) const
{
    if (!toolButton()->isVisible())
        return QRect();
    if (!child)
        return QAccessibleButton::rect(child);

    QStyleOptionToolButton opt;
    opt.init(widget());
    QRect subrect = widget()->style()->subControlRect(QStyle::CC_ToolButton, &opt,
                                                      QStyle::SC_ToolButtonMenu, toolButton());

    if (child == ButtonExecute)
        subrect = QRect(0, 0, subrect.x(), widget()->height());

    QPoint ntl = widget()->mapToGlobal(subrect.topLeft());
    subrect.moveTopLeft(ntl);
    return subrect;
}

/*!
    \internal

    Returns the button's text label, depending on the text \a t, and
    the \a child.
*/
QString QAccessibleToolButton::text(Text t, int child) const
{
    QString str;
    if (!toolButton()->isVisible())
        return str;

    switch (t) {
    case Name:
        str = toolButton()->text();
        if (str.isEmpty())
            str = toolButton()->text();
        break;
    default:
        break;
    }
    if (str.isEmpty())
        str = QAccessibleButton::text(t, child);;
    return qt_accStripAmp(str);
}

/*!
    \internal

    Returns the number of actions which is 0, 1, or 2, in part
    depending on \a child.
*/
int QAccessibleToolButton::actionCount(int child) const
{
    // each subelement has one action
    if (child)
        return isSplitButton() ? 1 : 0;
    int ac = widget()->focusPolicy() != Qt::NoFocus ? 1 : 0;
    // button itself has two actions if a menu button
#ifndef QT_NO_MENU
    return ac + (toolButton()->menu() ? 2 : 1);
#else
    return ac + 1;
#endif
}

/*!
    \internal

    If \a text is \c Name, then depending on the \a child or the \a
    action, an action text is returned. This is a translated string
    which in English is one of "Press", "Open", or "Set Focus". If \a
    text is not \c Name, an empty string is returned.
*/
QString QAccessibleToolButton::actionText(int action, Text text, int child) const
{
    if (text == Name) switch(child) {
    case ButtonExecute:
        return QToolButton::tr("Press");
    case ButtonDropMenu:
        return QToolButton::tr("Open");
    default:
        switch(action) {
        case 0:
            return QToolButton::tr("Press");
        case 1:
#ifndef QT_NO_MENU
            if (toolButton()->menu())
                return QToolButton::tr("Open");
#endif
            //fall through
        case 2:
            return QLatin1String("Set Focus");
        }
    }
    return QString();
}

/*!
    \internal
*/
bool QAccessibleToolButton::doAction(int action, int child, const QVariantList &params)
{
    if (!widget()->isEnabled() || !widget()->isVisible())
        return false;
    if (action == 1 || child == ButtonDropMenu) {
        if(!child)
            toolButton()->setDown(true);
#ifndef QT_NO_MENU
        toolButton()->showMenu();
#endif
        return true;
    }
    return QAccessibleButton::doAction(action, 0, params);
}

#endif // QT_NO_TOOLBUTTON

/*!
  \class QAccessibleDisplay qaccessiblewidget.h
  \brief The QAccessibleDisplay class implements the QAccessibleInterface for widgets that display information.
  \internal

  \ingroup accessibility
*/

/*!
  Constructs a QAccessibleDisplay object for \a w.
  \a role is propagated to the QAccessibleWidgetEx constructor.
*/
QAccessibleDisplay::QAccessibleDisplay(QWidget *w, Role role)
: QAccessibleWidgetEx(w, role)
{
}

/*! \reimp */
QAccessible::Role QAccessibleDisplay::role(int child) const
{
    QLabel *l = qobject_cast<QLabel*>(object());
    if (l) {
        if (l->pixmap())
            return Graphic;
#ifndef QT_NO_PICTURE
        if (l->picture())
            return Graphic;
#endif
#ifndef QT_NO_MOVIE
        if (l->movie())
            return Animation;
#endif
#ifndef QT_NO_PROGRESSBAR
    } else if (qobject_cast<QProgressBar*>(object())) {
        return ProgressBar;
#endif
    }
    return QAccessibleWidgetEx::role(child);
}

/*! \reimp */
QString QAccessibleDisplay::text(Text t, int child) const
{
    QString str;
    if (!widget()->isVisible())
        return str;
    switch (t) {
    case Name:
        str = widget()->accessibleName();
        if (str.isEmpty()) {
            if (qobject_cast<QLabel*>(object())) {
                str = qobject_cast<QLabel*>(object())->text();
#ifndef QT_NO_GROUPBOX
            } else if (qobject_cast<QGroupBox*>(object())) {
                str = qobject_cast<QGroupBox*>(object())->title();
#endif
#ifndef QT_NO_LCDNUMBER
            } else if (qobject_cast<QLCDNumber*>(object())) {
                QLCDNumber *l = qobject_cast<QLCDNumber*>(object());
                if (l->numDigits())
                    str = QString::number(l->value());
                else
                    str = QString::number(l->intValue());
#endif
            }
        }
        break;
    case Value:
#ifndef QT_NO_PROGRESSBAR
        if (qobject_cast<QProgressBar*>(object()))
            str = QString::number(qobject_cast<QProgressBar*>(object())->value());
#endif
        break;
    default:
        break;
    }
    if (str.isEmpty())
        str = QAccessibleWidgetEx::text(t, child);;
    return qt_accStripAmp(str);
}

/*! \reimp */
QAccessible::Relation QAccessibleDisplay::relationTo(int child, const QAccessibleInterface *other,
                                                     int otherChild) const
{
    Relation relation = QAccessibleWidgetEx::relationTo(child, other, otherChild);
    if (child || otherChild)
        return relation;

    QObject *o = other->object();
    QLabel *label = qobject_cast<QLabel*>(object());
    if (label) {
#ifndef QT_NO_SHORTCUT
        if (o == label->buddy())
            relation |= Label;
#endif
#ifndef QT_NO_GROUPBOX
    } else {
	QGroupBox *groupbox = qobject_cast<QGroupBox*>(object());
	if (groupbox && !groupbox->title().isEmpty())
	    if (groupbox->children().contains(o))
		relation |= Label;
#endif
    }
    return relation;
}

/*! \reimp */
int QAccessibleDisplay::navigate(RelationFlag rel, int entry, QAccessibleInterface **target) const
{
    *target = 0;
    if (rel == Labelled) {
        QObject *targetObject = 0;
        QLabel *label = qobject_cast<QLabel*>(object());
        if (label) {
#ifndef QT_NO_SHORTCUT
            if (entry == 1)
                targetObject = label->buddy();
#endif
#ifndef QT_NO_GROUPBOX
        } else {
	    QGroupBox *groupbox = qobject_cast<QGroupBox*>(object());
	    if (groupbox && !groupbox->title().isEmpty())
		rel = Child;
#endif
        }
        *target = QAccessible::queryAccessibleInterface(targetObject);
        if (*target)
            return 0;
    }
    return QAccessibleWidgetEx::navigate(rel, entry, target);
}

#ifndef QT_NO_LINEEDIT
/*!
  \class QAccessibleLineEdit qaccessiblewidget.h
  \brief The QAccessibleLineEdit class implements the QAccessibleInterface for widgets with editable text
  \internal

  \ingroup accessibility
*/

/*!
  Constructs a QAccessibleLineEdit object for \a w.
  \a name is propagated to the QAccessibleWidgetEx constructor.
*/
QAccessibleLineEdit::QAccessibleLineEdit(QWidget *w, const QString &name)
: QAccessibleWidgetEx(w, EditableText, name), QAccessibleSimpleEditableTextInterface(this)
{
    addControllingSignal(QLatin1String("textChanged(const QString&)"));
    addControllingSignal(QLatin1String("returnPressed()"));
}

/*! Returns the line edit. */
QLineEdit *QAccessibleLineEdit::lineEdit() const
{
    return qobject_cast<QLineEdit*>(object());
}

/*! \reimp */
QString QAccessibleLineEdit::text(Text t, int child) const
{
    QString str;
    if (!lineEdit()->isVisible())
        return str;
    switch (t) {
    case Value:
        str = lineEdit()->text();
        break;
    default:
        break;
    }
    if (str.isEmpty())
        str = QAccessibleWidgetEx::text(t, child);;
    return qt_accStripAmp(str);
}

/*! \reimp */
void QAccessibleLineEdit::setText(Text t, int control, const QString &text)
{
    if (!lineEdit()->isVisible())
        return;
    if (t != Value || control) {
        QAccessibleWidgetEx::setText(t, control, text);
        return;
    }
    lineEdit()->setText(text);
}

/*! \reimp */
QAccessible::State QAccessibleLineEdit::state(int child) const
{
    State state = QAccessibleWidgetEx::state(child);

    QLineEdit *l = lineEdit();
    if (l->isReadOnly())
        state |= ReadOnly;
    if (l->echoMode() == QLineEdit::Password)
        state |= Protected;
    state |= Selectable;
    if (l->hasSelectedText())
        state |= Selected;

    if (l->contextMenuPolicy() != Qt::NoContextMenu
        && l->contextMenuPolicy() != Qt::PreventContextMenu)
        state |= HasPopup;

    return state;
}

QVariant QAccessibleLineEdit::invokeMethodEx(QAccessible::Method method, int child,
                                                     const QVariantList &params)
{
    if (child)
        return QVariant();

    switch (method) {
    case ListSupportedMethods: {
        QSet<QAccessible::Method> set;
        set << ListSupportedMethods << SetCursorPosition << GetCursorPosition;
        return qVariantFromValue(set | qvariant_cast<QSet<QAccessible::Method> >(
                QAccessibleWidgetEx::invokeMethodEx(method, child, params)));
    }
    case SetCursorPosition:
        setCursorPosition(params.value(0).toInt());
        return true;
    case GetCursorPosition:
        return cursorPosition();
    default:
        return QAccessibleWidgetEx::invokeMethodEx(method, child, params);
    }
}

void QAccessibleLineEdit::addSelection(int startOffset, int endOffset)
{
    setSelection(0, startOffset, endOffset);
}

QString QAccessibleLineEdit::attributes(int offset, int *startOffset, int *endOffset)
{
    // QLineEdit doesn't have text attributes
    *startOffset = *endOffset = offset;
    return QString();
}

int QAccessibleLineEdit::cursorPosition()
{
    return lineEdit()->cursorPosition();
}

QRect QAccessibleLineEdit::characterRect(int /*offset*/, CoordinateType /*coordType*/)
{
    // QLineEdit doesn't hand out character rects
    return QRect();
}

int QAccessibleLineEdit::selectionCount()
{
    return lineEdit()->hasSelectedText() ? 1 : 0;
}

int QAccessibleLineEdit::offsetAtPoint(const QPoint &point, CoordinateType coordType)
{
    QPoint p = point;
    if (coordType == RelativeToScreen)
        p = lineEdit()->mapFromGlobal(p);

    return lineEdit()->cursorPositionAt(p);
}

void QAccessibleLineEdit::selection(int selectionIndex, int *startOffset, int *endOffset)
{
    *startOffset = *endOffset = 0;
    if (selectionIndex != 0)
        return;

    *startOffset = lineEdit()->selectionStart();
    *endOffset = *startOffset + lineEdit()->selectedText().count();
}

QString QAccessibleLineEdit::text(int startOffset, int endOffset)
{
    if (startOffset > endOffset)
        return QString();
    return lineEdit()->text().mid(startOffset, endOffset - startOffset);
}

QString QAccessibleLineEdit::textBeforeOffset (int /*offset*/, BoundaryType /*boundaryType*/,
        int * /*startOffset*/, int * /*endOffset*/)
{
    // TODO
    return QString();
}

QString QAccessibleLineEdit::textAfterOffset(int /*offset*/, BoundaryType /*boundaryType*/,
        int * /*startOffset*/, int * /*endOffset*/)
{
    // TODO
    return QString();
}

QString QAccessibleLineEdit::textAtOffset(int /*offset*/, BoundaryType /*boundaryType*/,
        int * /*startOffset*/, int * /*endOffset*/)
{
    // TODO
    return QString();
}

void QAccessibleLineEdit::removeSelection(int selectionIndex)
{
    if (selectionIndex != 0)
        return;

    lineEdit()->deselect();
}

void QAccessibleLineEdit::setCursorPosition(int position)
{
    lineEdit()->setCursorPosition(position);
}

void QAccessibleLineEdit::setSelection(int selectionIndex, int startOffset, int endOffset)
{
    if (selectionIndex != 0)
        return;

    lineEdit()->setSelection(startOffset, endOffset - startOffset);
}

int QAccessibleLineEdit::characterCount()
{
    return lineEdit()->text().count();
}

void QAccessibleLineEdit::scrollToSubstring(int startIndex, int endIndex)
{
    lineEdit()->setCursorPosition(endIndex);
    lineEdit()->setCursorPosition(startIndex);
}

#endif // QT_NO_LINEEDIT

#endif // QT_NO_ACCESSIBILITY

QT_END_NAMESPACE

