/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the QtGui module of the Qt Toolkit.
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

#include "qglobal.h"

#ifndef QT_NO_GRAPHICSVIEW

#include "qgraphicswidget.h"
#include "qgraphicswidget_p.h"
#include "qgraphicslayout.h"
#include "qgraphicslayout_p.h"
#include "qgraphicsscene.h"
#include "qgraphicssceneevent.h"
#include <private/qgraphicsscene_p.h>

#include <QtCore/qmutex.h>
#include <QtGui/qapplication.h>
#include <QtGui/qgraphicsview.h>
#include <QtGui/qgraphicsproxywidget.h>
#include <QtGui/qpalette.h>
#include <QtGui/qstyleoption.h>

#include <qdebug.h>

QT_BEGIN_NAMESPACE

/*!
    \class QGraphicsWidget
    \brief The QGraphicsWidget class is the base class for all widget
    items in a QGraphicsScene.
    \since 4.4
    \ingroup multimedia

    QGraphicsWidget is an extended base item that provides extra functionality
    over QGraphicsItem. It is similar to QWidget in many ways:

    \list
        \o Provides a \l palette, a \l font and a \l style().
        \o Has a defined geometry().
        \o Supports layouts with setLayout() and layout().
    \endlist

    Unlike QGraphicsItem, QGraphicsWidget is not an abstract class; you can
    create instances of a QGraphicsWidget without having to subclass it.
    This approach is useful for widgets that only serve the purpose of
    organizing child widgets into a layout.

    QGraphicsWidget can be used as a base item for your own custom item if
    you require advanced input focus handling, e.g., tab focus and activation, or
    layouts.

    Since QGraphicsWidget resembles QWidget and has similar API, it is
    easier to port a widget from QWidget to QGraphicsWidget, instead of
    QGraphicsItem.

    \note QWidget-based widgets can be directly embedded into a
    QGraphicsScene using QGraphicsProxyWidget.

    Noticeable differences between QGraphicsWidget and QWidget are:

    \table
    \header \o QGraphicsWidget
                \o QWidget
    \row      \o Coordinates and geometry are defined with qreals (doubles or
                    floats, depending on the platform).
                \o QWidget uses integer geometry (QPoint, QRect).
    \row      \o The widget is already visible by default; you do not have to
                    call show() to display the widget.
                \o QWidget is hidden by default until you call show().
    \row      \o A subset of widget attributes are supported.
                \o All widget attributes are supported.
    \row      \o A top-level item's style defaults to QGraphicsScene::style
                \o A top-level widget's style defaults to QApplication::style
    \row      \o Graphics View provides a custom DnD framework, different
                    from QWidget.
                \o Standard DnD framework.
    \row      \o Widget items do not support modality.
                \o Full modality support.
    \endtable

    QGraphicsWidget supports a subset of Qt's widget attributes,
    (Qt::WidgetAttribute), as shown in the table below. Any attributes not
    listed in this table are unsupported, or otherwise unused.

    \table
    \header \o Widget Attribute                         \o Usage
    \row    \o Qt::WA_SetLayoutDirection
                    \o Set by setLayoutDirection(), cleared by
                        unsetLayoutDirection(). You can test this attribute to
                        check if the widget has been explicitly assigned a
                        \l{QGraphicsWidget::layoutDirection()}
                        {layoutDirection}. If the attribute is not set, the
                        \l{QGraphicsWidget::layoutDirection()}
                        {layoutDirection()} is inherited.
    \row    \o Qt::WA_RightToLeft
                    \o Toggled by setLayoutDirection(). Inherited from the
                        parent/scene. If set, the widget's layout will order
                        horizontally arranged widgets from right to left.
    \row    \o Qt::WA_SetStyle
                    \o Set and cleared by setStyle(). If this attribute is
                        set, the widget has been explicitly assigned a style.
                        If it is unset, the widget will use the scene's or the
                        application's style.
    \row    \o Qt::WA_Resized
                    \o Set by setGeometry() and resize().
    \endtable

    Although QGraphicsWidget inherits from both QObject and QGraphicsItem,
    you should use the functions provided by QGraphicsItem, \e not QObject, to
    manage the relationships between parent and child items. These functions
    control the stacking order of items as well as their ownership.

    \note The QObject::parent() should always return 0 for QGraphicsWidgets,
    but this policy is not strictly defined.

    \sa QGraphicsProxyWidget, QGraphicsItem, {Widgets and Layouts}
*/

/*!
    \property QGraphicsWidget::enabled
    \brief whether the item is enabled or not

    This property is declared in QGraphicsItem.

    \sa QGraphicsItem::isEnabled(), QGraphicsItem::setEnabled()
*/

/*!
    \property QGraphicsWidget::visible
    \brief whether the item is visible or not

    This property is declared in QGraphicsItem.

    \sa QGraphicsItem::isVisible(), QGraphicsItem::setVisible(), show(),
    hide()
*/

/*!
    Constructs a QGraphicsWidget instance. The optional \a parent argument is
    passed to QGraphicsItem's constructor. The optional \a wFlags argument
    specifies the widget's window flags (e.g., whether the widget should be a
    window, a tool, a popup, etc).
*/
QGraphicsWidget::QGraphicsWidget(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QGraphicsItem(*new QGraphicsWidgetPrivate, 0, 0), QGraphicsLayoutItem(0, false)
{
    Q_D(QGraphicsWidget);
    d->init(parent, wFlags);
}

/*!
    \internal

    Constructs a new QGraphicsWidget, using \a dd as parent.
*/
QGraphicsWidget::QGraphicsWidget(QGraphicsWidgetPrivate &dd, QGraphicsItem *parent, QGraphicsScene *scene, Qt::WindowFlags wFlags)
    : QGraphicsItem(dd, 0, scene), QGraphicsLayoutItem(0, false)
{
    Q_D(QGraphicsWidget);
    d->init(parent, wFlags);
}

/*!
    Destroys the QGraphicsWidget instance.
*/
QGraphicsWidget::~QGraphicsWidget()
{
    Q_D(QGraphicsWidget);
    d->focusPrev->d_func()->focusNext = d->focusNext;
    d->focusNext->d_func()->focusPrev = d->focusPrev;

    // Play it really safe
    d->focusNext = this;
    d->focusPrev = this;

    delete d->layout;

    clearFocus();
}

/*!
    \property QGraphicsWidget::size
    \brief the size of the widget

    Calling setSize() resizes the widget to \a size bounded by minimumSize()
    and maximumSize(). This property only affects the widget's width and
    height (e.g., its right and bottom edges); the widget's position and
    top-left corner remains unaffected.

    Resizing a widget triggers the widget to immediately receive a
    \l{QEvent::GraphicsSceneResize}{GraphicsSceneResize} event with the
    widget's old and new size.  If the widget has a layout assigned when this
    event arrives, the layout will be activated and it will automatically
    update any child widgets's geometry.

    This property does not affect any layout of the parent widget. If the
    widget itself is managed by a parent layout, e.g., it has a parent widget
    with a layout assigned, that layout will not activate.

    \sa setGeometry(), QGraphicsSceneResizeEvent, QGraphicsLayout
*/
QSizeF QGraphicsWidget::size() const
{
    return QGraphicsLayoutItem::geometry().size();
}

void QGraphicsWidget::resize(const QSizeF &size)
{
    setGeometry(QRectF(pos(), size));
}

/*!
    \fn void QGraphicsWidget::resize(qreal w, qreal h)

    This convenience function is equivalent to calling resize(QSizeF(w, h)).

    \sa setGeometry(), setTransform()
*/

/*!
    Sets the item's geometry to \a rect. The item's position and size are
    modified as a result of calling this function. The item is first moved,
    then resized.

    A side effect of calling this function is that the widget will receive
    a move event and a resize event. Also, if the widget has a layout
    assigned, the layout will activate.
    
    \sa geometry(), resize()
*/
void QGraphicsWidget::setGeometry(const QRectF &rect)
{
    const QGraphicsLayoutItemPrivate *d = QGraphicsLayoutItem::d_ptr;
    QRectF newGeom = rect;
    newGeom.setSize(rect.size().expandedTo(effectiveSizeHint(Qt::MinimumSize))
                               .boundedTo(effectiveSizeHint(Qt::MaximumSize)));
    if (newGeom == d->geom)
        return;

    // setPos triggers ItemPositionChange, which can adjust position
    QPointF oldPos = d->geom.topLeft();
    setPos(newGeom.topLeft());
    newGeom.moveTopLeft(pos());
    
    if (newGeom == d->geom)
        return;

    // Update the layout item geometry
    if (oldPos == pos()) {
        prepareGeometryChange();
    } else {
        // Send move event.
        QGraphicsSceneMoveEvent event;
        event.setOldPos(oldPos);
        event.setNewPos(pos());
        QApplication::sendEvent(this, &event);
    }
    QSizeF oldSize = size();
    QGraphicsLayoutItem::setGeometry(newGeom);

    // Send resize event
    if (newGeom.size() != oldSize) {
        setAttribute(Qt::WA_Resized);
        QGraphicsSceneResizeEvent re;
        re.setOldSize(oldSize);
        re.setNewSize(newGeom.size());
        QApplication::sendEvent(this, &re);

        // ### When only resizing we can avoid the full update.
        update();
    }
}

/*!
    \fn QRectF QGraphicsWidget::rect() const

    Returns the item's local rect as a QRectF. This function is equivalent
    to QRectF(QPointF(), size()).

    \sa setGeometry(), resize()
*/

/*!
    \fn void QGraphicsWidget::setGeometry(qreal x, qreal y, qreal w, qreal h)

    This convenience function is equivalent to calling setGeometry(QRectF(
    \a x, \a y, \a w, \a h)).

    \sa geometry(), resize()
*/

/*!
    Sets the widget's contents margins to \a left, \a top, \a right and \a
    bottom.

    Contents margins are used by the assigned layout to define the placement
    of subwidgets and layouts. Margins are particularily useful for widgets
    that constrain subwidgets to only a section of its own geometry. For
    example, a group box with a layout will place subwidgets inside its frame,
    but below the title.

    Changing a widget's contents margins will always trigger an update(), and
    any assigned layout will be activated automatically. The widget will then
    receive a \l{QEvent::ContentsRectChange}{ContentsRectChange} event.

    \sa getContentsMargins(), setGeometry()
*/
void QGraphicsWidget::setContentsMargins(qreal left, qreal top, qreal right, qreal bottom)
{
    Q_D(QGraphicsWidget);

    if (left == d->leftMargin
        && top == d->topMargin
        && right == d->rightMargin
        && bottom == d->bottomMargin) {
        return;
    }

    d->leftMargin = left;
    d->topMargin = top;
    d->rightMargin = right;
    d->bottomMargin = bottom;

    if (QGraphicsLayout *l = d->layout)
        l->invalidate();
    else
        updateGeometry();

    QEvent e(QEvent::ContentsRectChange);
    QApplication::sendEvent(this, &e);
}

/*!
    Gets the widget's contents margins. The margins are stored in \a left, \a
    top, \a right and \a bottom, as pointers to qreals. Each argument can
    be \e {omitted} by passing 0.

    \sa setContentsMargins()
*/
void QGraphicsWidget::getContentsMargins(qreal *left, qreal *top, qreal *right, qreal *bottom) const
{
    Q_D(const QGraphicsWidget);
    if (left)
        *left = d->leftMargin;
    if (top)
        *top = d->topMargin;
    if (right)
        *right = d->rightMargin;
    if (bottom)
        *bottom = d->bottomMargin;
}

/*!
    Sets the widget's window frame margins to \a left, \a top, \a right and
    \a bottom. The default frame margins are provided by the style, and they
    depend on the current window flags.

    If you would like to draw your own window decoration, you can set your
    own frame margins to override the default margins.

    \sa unsetWindowFrameMargins(), getWindowFrameMargins(), windowFrameRect()
*/
void QGraphicsWidget::setWindowFrameMargins(qreal left, qreal top, qreal right, qreal bottom)
{
    Q_D(QGraphicsWidget);
    bool unchanged = left == d->leftWindowFrameMargin && top == d->topWindowFrameMargin
                     && right == d->rightWindowFrameMargin && bottom == d->bottomWindowFrameMargin;
    if (d->setWindowFrameMargins && unchanged)
        return;
    if (!unchanged)
        prepareGeometryChange();
    d->leftWindowFrameMargin = left;
    d->topWindowFrameMargin = top;
    d->rightWindowFrameMargin = right;
    d->bottomWindowFrameMargin = bottom;
    d->setWindowFrameMargins = true;
}

/*!
    Gets the widget's window frame margins. The margins are stored in \a left,
    \a top, \a right and \a bottom as pointers to qreals. Each argument can
    be \e {omitted} by passing 0.

    \sa setWindowFrameMargins(), windowFrameRect()
*/
void QGraphicsWidget::getWindowFrameMargins(qreal *left, qreal *top, qreal *right, qreal *bottom) const
{
    Q_D(const QGraphicsWidget);
    if (left)
        *left = d->leftWindowFrameMargin;
    if (top)
        *top = d->topWindowFrameMargin;
    if (right)
        *right = d->rightWindowFrameMargin;
    if (bottom)
        *bottom = d->bottomWindowFrameMargin;
}

/*!
    Resets the window frame margins to the default value, provided by the style.

    \sa setWindowFrameMargins(), getWindowFrameMargins(), windowFrameRect()
*/
void QGraphicsWidget::unsetWindowFrameMargins()
{
    Q_D(QGraphicsWidget);
    if ((d->windowFlags & Qt::Window) && (d->windowFlags & Qt::WindowType_Mask) != Qt::Popup && 
         (d->windowFlags & Qt::WindowType_Mask) != Qt::ToolTip && !(d->windowFlags & Qt::FramelessWindowHint)) {
        QStyleOptionTitleBar bar;
        d->initStyleOptionTitleBar(&bar);
        QStyle *style = this->style();
        qreal margin = style->pixelMetric(QStyle::PM_MdiSubWindowFrameWidth);
        qreal titleBarHeight  = d->titleBarHeight(bar);
        setWindowFrameMargins(margin, titleBarHeight, margin, margin);
    } else {
        setWindowFrameMargins(0, 0, 0, 0);
    }
    d->setWindowFrameMargins = false;
}

/*!
    Returns the widget's geometry in parent coordinates including any window
    frame.

    \sa windowFrameRect(), getWindowFrameMargins(), setWindowFrameMargins()
*/
QRectF QGraphicsWidget::windowFrameGeometry() const
{
    Q_D(const QGraphicsWidget);
    return geometry().adjusted(-d->leftWindowFrameMargin, -d->topWindowFrameMargin,
                               d->rightWindowFrameMargin, d->bottomWindowFrameMargin);
}

/*!
    Returns the widget's local rect including any window frame.

    \sa windowFrameGeometry(), getWindowFrameMargins(), setWindowFrameMargins()
*/
QRectF QGraphicsWidget::windowFrameRect() const
{
    Q_D(const QGraphicsWidget);
    return rect().adjusted(-d->leftWindowFrameMargin, -d->topWindowFrameMargin,
                           d->rightWindowFrameMargin, d->bottomWindowFrameMargin);
}

/*!
    Populates a style option object for this widget based on its current
    state, and stores the output in \a option. The default implementation
    populates \a option with the following properties.

    \table  \o Style Option Property                \o Value
    \row    \o state & QStyle::State_Enabled
                    \o Corresponds to QGraphicsItem::isEnabled().
    \row    \o state & QStyle::State_HasFocus
                    \o Corresponds to QGraphicsItem::hasFocus().
    \row    \o state & QStyle::State_MouseOver
                    \o Corresponds to QGraphicsItem::isUnderMouse().
    \row    \o direction
                    \o Corresponds to QGraphicsWidget::layoutDirection().
    \row    \o rect
                    \o Corresponds to QGraphicsWidget::rect().toRect().
    \row    \o palette
                    \o Corresponds to QGraphicsWidget::palette().
    \row    \o fontMetrics
                    \o Corresponds to QFontMetrics(QGraphicsWidget::font()).
    \endtable

    Subclasses of QGraphicsWidget should call the base implementation, and
    then test the type of \a option using qstyleoption_cast<>() or test
    QStyleOption::Type before storing widget-specific options.

    For example:

    \snippet doc/src/snippets/code/src.gui.graphicsview.qgraphicswidget.cpp 0

    \sa QStyleOption::initFrom()
*/
void QGraphicsWidget::initStyleOption(QStyleOption *option) const
{
    Q_ASSERT(option);

    option->state = QStyle::State_None;
    if (isEnabled())
        option->state |= QStyle::State_Enabled;
    if (hasFocus())
        option->state |= QStyle::State_HasFocus;
    // if (window->testAttribute(Qt::WA_KeyboardFocusChange)) // ### Window
    //     option->state |= QStyle::State_KeyboardFocusChange;
    if (isUnderMouse())
        option->state |= QStyle::State_MouseOver;
    if (QGraphicsWidget *w = window()) {
        if (w->isActiveWindow())
            option->state |= QStyle::State_Active;
    }
    if (isWindow())
        option->state |= QStyle::State_Window;
    /*
      ###
#ifdef Q_WS_MAC
    extern bool qt_mac_can_clickThrough(const QGraphicsWidget *w); //qwidget_mac.cpp
    if (!(option->state & QStyle::State_Active) && !qt_mac_can_clickThrough(widget))
        option->state &= ~QStyle::State_Enabled;

    switch (QMacStyle::widgetSizePolicy(widget)) {
    case QMacStyle::SizeSmall:
        option->state |= QStyle::State_Small;
        break;
    case QMacStyle::SizeMini:
        option->state |= QStyle::State_Mini;
        break;
    default:
        ;
    }
#endif
#ifdef QT_KEYPAD_NAVIGATION
    if (widget->hasEditFocus())
        state |= QStyle::State_HasEditFocus;
#endif
    */
    option->direction = layoutDirection();
    option->rect = rect().toRect(); // ### truncation!
    option->palette = palette();
    if (!isEnabled()) {
        option->palette.setCurrentColorGroup(QPalette::Disabled);
    } else if (isActiveWindow()) {
        option->palette.setCurrentColorGroup(QPalette::Active);
    } else {
        option->palette.setCurrentColorGroup(QPalette::Inactive);
    }
    option->fontMetrics = QFontMetrics(font());
}

/*!
    \reimp
*/
QSizeF QGraphicsWidget::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const QGraphicsWidget);
    QSizeF sh;
    if (d->layout) {
        sh = d->layout->effectiveSizeHint(which, constraint);
        sh += QSizeF(d->leftMargin + d->rightMargin, d->topMargin + d->bottomMargin);
    } else {
        switch (which) {
            case Qt::MinimumSize:
                sh = QSizeF(0, 0);
                break;
            case Qt::PreferredSize:
                sh = QSizeF(50, 50);    //rather arbitrary
                break;
            case Qt::MaximumSize:
                sh = QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
                break;
            default:
                qWarning("QGraphicsWidget::sizeHint(): Don't know how to handle the value of 'which'");
                break;
        }
    }
    return sh;
}

/*!
    Returns this widget's layout, or 0 if no layout is currently managing this
    widget.

    \sa setLayout()
*/
QGraphicsLayout *QGraphicsWidget::layout() const
{
    Q_D(const QGraphicsWidget);
    return d->layout;
}

/*!
    \fn void QGraphicsWidget::setLayout(QGraphicsLayout *layout)

    Sets the layout for this widget to \a layout. Any existing layout manager
    is deleted before the new layout is assigned. If \a layout is 0, the
    widget is left without a layout. Existing subwidgets' geometries will
    remain unaffected.

    All widgets that are currently managed by \a layout or all of its
    sublayouts, are automatically reparented to this item. The layout is then
    invalidated, and the child widget geometries are adjusted according to
    this item's geometry() and contentsMargins(). Children who are not
    explicitly managed by \a layout remain unaffected by the layout after
    it has been assigned to this widget.

    QGraphicsWidget takes ownership of \a layout.

    \sa layout(), QGraphicsLinearLayout::addItem(), QGraphicsLayout::invalidate()
*/
void QGraphicsWidget::setLayout(QGraphicsLayout *l)
{
    Q_D(QGraphicsWidget);
    if (d->layout == l)
        return;
    d->setLayout_helper(l);
    if (!l)
        return;

    // Prevent assigning a layout that is already assigned to another widget.
    QGraphicsLayoutItem *oldParent = l->parentLayoutItem();
    if (oldParent && oldParent != this) {
        qWarning("QGraphicsWidget::setLayout: Attempting to set a layout on %s"
                 " \"%s\", when the layout already has a parent",
                 metaObject()->className(), qPrintable(objectName()));
        return;
    }

    // Install and activate the layout.
    l->setParentLayoutItem(this);
    l->d_func()->reparentChildWidgets(this);
    l->invalidate();
}

/*!
    Adjusts the size of the widget to its effective minimum size hint.

    This function is called implicitly when the item is shown for the first
    time.

    \sa effectiveSizeHint(), Qt::MinimumSize
*/
void QGraphicsWidget::adjustSize()
{
    QSizeF sz = effectiveSizeHint(Qt::MinimumSize);
    // What if sz is not valid?!
    if (sz.isValid())
        resize(sz);
}

/*!
    \property QGraphicsWidget::layoutDirection
    \brief the layout direction for this widget.

    This property modifies this widget's and all of its descendants'
    Qt::WA_RightToLeft attribute. It also sets this widget's
    Qt::WA_SetLayoutDirection attribute.

    The widget's layout direction determines the order in which the layout
    manager horizontally arranges subwidgets of this widget. The default
    value depends on the language and locale of the application, and is
    typically in the same direction as words are read and written. With
    Qt::LeftToRight, the layout starts placing subwidgets from the left
    side of this widget towards the right. Qt::RightToLeft does the opposite -
    the layout will place widgets starting from the right edge moving towards
    the left.

    Subwidgets inherit their layout direction from the parent. Top-level
    widget items inherit their layout direction from
    QGraphicsScene::layoutDirection. If you change a widget's layout direction
    by calling setLayoutDirection(), the widget will send itself a
    \l{QEvent::LayoutDirectionChange}{LayoutDirectionChange} event, and then
    propagate the new layout direction to all its descendants.

    \sa QWidget::layoutDirection, QApplication::layoutDirection
*/
Qt::LayoutDirection QGraphicsWidget::layoutDirection() const
{
    return testAttribute(Qt::WA_RightToLeft) ? Qt::RightToLeft : Qt::LeftToRight;
}
void QGraphicsWidget::setLayoutDirection(Qt::LayoutDirection direction)
{
    Q_D(QGraphicsWidget);
    setAttribute(Qt::WA_SetLayoutDirection, true);
    d->setLayoutDirection_helper(direction);
}
void QGraphicsWidget::unsetLayoutDirection()
{
    Q_D(QGraphicsWidget);
    setAttribute(Qt::WA_SetLayoutDirection, false);
    d->resolveLayoutDirection();
}

/*
    \internal
    \class QGraphicsWidgetStyles

    We use this thread-safe class to maintain a hash of styles for widgets
    styles. Note that QApplication::style() itself isn't thread-safe, QStyle
    isn't thread-safe, and we don't have a thread-safe factory for creating
    the default style, nor cloning a style.
*/
class QGraphicsWidgetStyles
{
public:
    QStyle *styleForWidget(const QGraphicsWidget *widget) const
    {
        QMutexLocker locker(&mutex);
        return styles.value(widget, 0);
    }
    
    void setStyleForWidget(QGraphicsWidget *widget, QStyle *style)
    {
        QMutexLocker locker(&mutex);
        if (style)
            styles[widget] = style;
        else
            styles.remove(widget);
    }

private:
    QMap<const QGraphicsWidget *, QStyle *> styles;
    mutable QMutex mutex;
};
Q_GLOBAL_STATIC(QGraphicsWidgetStyles, widgetStyles)

/*!
    Returns a pointer to the widget's style. If this widget does not have any
    explicitly assigned style, the scene's style is returned instead. In turn,
    if the scene does not have any assigned style, this function returns
    QApplication::style().

    \sa setStyle()
*/
QStyle *QGraphicsWidget::style() const
{
    if (QStyle *style = widgetStyles()->styleForWidget(this))
        return style;
    // ### This is not thread-safe. QApplication::style() is not thread-safe.
    return scene() ? scene()->style() : QApplication::style();
}

/*!
    Sets the widget's style to \a style. QGraphicsWidget does \e not take
    ownership of \a style.

    If no style is assigned, or \a style is 0, the widget will use
    QGraphicsScene::style() (if this has been set). Otherwise the widget will
    use QApplication::style().

    This function sets the Qt::WA_SetStyle attribute if \a style is not 0;
    otherwise it clears the attribute.

    \sa style()
*/
void QGraphicsWidget::setStyle(QStyle *style)
{
    setAttribute(Qt::WA_SetStyle, style != 0);
    widgetStyles()->setStyleForWidget(this, style);

    // Deliver StyleChange to the widget itself (doesn't propagate).
    QEvent event(QEvent::StyleChange);
    QApplication::sendEvent(this, &event);
}

/*!
    \property QGraphicsWidget::font
    \brief the widgets' font

    This property provides the widget's font.

    QFont consists of font properties that have been explicitly defined and
    properties implicitly inherited from the widget's parent. Hence, font()
    can return a different font compared to the one set with setFont().
    This scheme allows you to define single entries in a font without
    affecting the font's inherited entries.

    When a widget's font changes, it resolves its entries against its
    parent widget. If the widget does not have a parent widget, it resolves
    its entries against the scene. The widget then sends itself a
    \l{QEvent::FontChange}{FontChange} event and notifies all its
    descendants so that they can resolve their fonts as well.

    \sa QApplication::font(), QGraphicsScene::font, QFont::resolve()
*/
QFont QGraphicsWidget::font() const
{
    Q_D(const QGraphicsWidget);
    return d->font;
}
void QGraphicsWidget::setFont(const QFont &font)
{
    Q_D(QGraphicsWidget);
    d->font = font;
    d->resolveFont();
}

/*!
    \property QGraphicsWidget::palette
    \brief the widgets' palette

    This property provides the widget's palette. The palette provides colors
    and brushes for color groups (e.g., QPalette::Button) and states (e.g.,
    QPalette::Inactive), loosely defining the general look of the widget and
    its children.

    QPalette consists of color groups that have been explicitly defined, and
    groups that are implicitly inherited from the widget's parent. Because of
    this, palette() can return a different palette than what has been set with
    setPalette(). This scheme allows you to define single entries in a palette
    without affecting the palette's inherited entries.

    When a widget's palette changes, it resolves its entries against its
    parent widget, or if it doesn't have a parent widget, it resolves against
    the scene. It then sends itself a \l{QEvent::PaletteChange}{PaletteChange}
    event, and notifies all its descendents so they can resolve their palettes
    as well.

    \sa QApplication::palette(), QGraphicsScene::palette, QPalette::resolve()
*/
QPalette QGraphicsWidget::palette() const
{
    Q_D(const QGraphicsWidget);
    return d->palette;
}
void QGraphicsWidget::setPalette(const QPalette &palette)
{
    Q_D(QGraphicsWidget);
    d->palette = palette;
    d->resolvePalette();
}

/*!
    If this widget is currently managed by a layout, this function notifies
    the layout that the widget's size hints have changed and the layout
    may need to resize and reposition the widget accordingly.

    Call this function if the widget's sizeHint() has changed.

    \sa QGraphicsLayout::invalidate()
*/
void QGraphicsWidget::updateGeometry()
{
    QGraphicsLayoutItem::updateGeometry();
    QGraphicsWidget *parent = parentWidget();
    if (parent && parent->layout()) {
        parent->layout()->invalidate();
    } else {
        if (parent && parent->isVisible())
            QApplication::postEvent(parent, new QEvent(QEvent::LayoutRequest));
        bool wasResized = testAttribute(Qt::WA_Resized);
        resize(size()); // this will restrict the size
        setAttribute(Qt::WA_Resized, wasResized);
    }
}

/*!
    \reimp

    QGraphicsWidget uses the base implementation of this function to catch and
    deliver events related to state changes in the item. Because of this, it is
    very important that subclasses call the base implementation.

    For example, QGraphicsWidget uses ItemVisibleChange to deliver \l Show and
    \l Hide events, ItemPositionHasChanged to deliver \l Move events, and
    ItemParentChange both to deliver \l ParentChange events, and for managing
    the focus chain.

    \sa propertyChange()
*/
QVariant QGraphicsWidget::itemChange(GraphicsItemChange change, const QVariant &value)
{
    Q_D(QGraphicsWidget);
    switch (change) {
    case ItemEnabledHasChanged: {
        // Send EnabledChange after the enabled state has changed.
        QEvent event(QEvent::EnabledChange);
        QApplication::sendEvent(this, &event);
        break;
    }
    case ItemVisibleChange:
        if (value.toBool()) {
            // Send Show event before the item has been shown.
            QShowEvent event;
            QApplication::sendEvent(this, &event);
            bool resized = testAttribute(Qt::WA_Resized);
            if (!resized) {
                adjustSize();
                setAttribute(Qt::WA_Resized, false);
            }
        }
        break;
    case ItemVisibleHasChanged:
        if (!value.toBool()) {
            // Send Hide event after the item has been hidden.
            QHideEvent event;
            QApplication::sendEvent(this, &event);
        }
        break;
    case ItemPositionHasChanged:
        setGeometry(QRectF(pos(), size()));
        break;
    case ItemParentChange: {
        QGraphicsItem *parent = qVariantValue<QGraphicsItem *>(value);
        d->fixFocusChainBeforeReparenting((parent && parent->isWidget()) ? static_cast<QGraphicsWidget *>(parent) : 0);

        // Deliver ParentAboutToChange.
        QEvent event(QEvent::ParentAboutToChange);
        QApplication::sendEvent(this, &event);
        break;
    }
    case ItemParentHasChanged: {
        // reset window type on parent change in order to automagically remove decorations etc.
        Qt::WindowFlags wflags = d->windowFlags & ~Qt::WindowType_Mask;
        d->adjustWindowFlags(&wflags);
        setWindowFlags(wflags);
        // Deliver ParentChange.
        QEvent event(QEvent::ParentChange);
        QApplication::sendEvent(this, &event);
        break;
    }
    case ItemCursorChange: {
        // Deliver CursorChange.
        QEvent event(QEvent::CursorChange);
        QApplication::sendEvent(this, &event);
        break;
    }
    case ItemToolTipChange: {
        // Deliver ToolTipChange.
        QEvent event(QEvent::ToolTipChange);
        QApplication::sendEvent(this, &event);
        break;
    }
    default:
        break;
    }
    return QGraphicsItem::itemChange(change, value);
}

/*!
    \internal

    This virtual function is used to notify changes to any property (both
    dynamic properties, and registered with Q_PROPERTY) in the
    widget. Depending on the property itself, the notification can be
    delivered before or after the value has changed.

    \a propertyName is the name of the property (e.g., "size" or "font"), and
    \a value is the (proposed) new value of the property. The function returns
    the new value, which may be different from \a value if the notification
    supports adjusting the property value. The base implementation simply
    returns \a value for any \a propertyName.

    QGraphicsWidget delivers notifications for the following properties:

    \table     \o propertyName        \o Property
    \row       \o layoutDirection     \o QGraphicsWidget::layoutDirection
    \row       \o size                \o QGraphicsWidget::size
    \row       \o font                \o QGraphicsWidget::font
    \row       \o palette             \o QGraphicsWidget::palette
    \endtable

    \sa itemChange()
*/
QVariant QGraphicsWidget::propertyChange(const QString &propertyName, const QVariant &value)
{
    Q_UNUSED(propertyName);
    return value;
}

/*!
    QGraphicsWidget's implementation of sceneEvent() simply passes \a event to
    QGraphicsWidget::event(). You can handle all events for your widget in
    event() or in any of the convenience functions; you should not have to
    reimplement this function in a subclass of QGraphicsWidget.

    \sa QGraphicsItem::sceneEvent()
*/
bool QGraphicsWidget::sceneEvent(QEvent *event)
{
    return QCoreApplication::sendEvent(this, event) || QGraphicsItem::sceneEvent(event);
}

/*!
    This event handler, for \a event, receives events for the window frame if
    this widget is a window. Its base implementation provides support for
    default window frame interaction such as moving, resizing, etc.

    You can reimplement this handler in a subclass of QGraphicsWidget to
    provide your own custom window frame interaction support.

    Returns true if \a event has been recognized and processed; otherwise,
    returns false.
    
    \sa event()
*/
bool QGraphicsWidget::windowFrameEvent(QEvent *event)
{
    Q_D(QGraphicsWidget);
    switch (event->type()) {
    case QEvent::GraphicsSceneMousePress:
        d->windowFrameMousePressEvent(static_cast<QGraphicsSceneMouseEvent *>(event));
        break;
    case QEvent::GraphicsSceneMouseMove:
        if (d->grabbedSection != Qt::NoSection) {
            d->windowFrameMouseMoveEvent(static_cast<QGraphicsSceneMouseEvent *>(event));
            event->accept();
        }
        break;
    case QEvent::GraphicsSceneMouseRelease:
        d->windowFrameMouseReleaseEvent(static_cast<QGraphicsSceneMouseEvent *>(event));
        break;
    case QEvent::GraphicsSceneHoverMove:
        d->windowFrameHoverMoveEvent(static_cast<QGraphicsSceneHoverEvent *>(event));
        break;
    case QEvent::GraphicsSceneHoverLeave:
        d->windowFrameHoverLeaveEvent(static_cast<QGraphicsSceneHoverEvent *>(event));
        break;
    default:
        break;
    }
    return event->isAccepted();
}

/*!
    Returns the window frame section at position \a pos, or
    Qt::NoWindowFrameSection if there is no window frame section at this
    position.

    This function is used in QGraphicsWidget's base implementation for window
    frame interaction.

    You can reimplement this function if you want to customize how a window
    can be interactively moved or resized.  For instance, if you only want to
    allow a window to be resized by the bottom right corner, you can
    reimplement this function to return Qt::NoSection for all sections except
    Qt::BottomRightSection.

    \sa windowFrameEvent(), paintWindowFrame(), windowFrameGeometry()
*/
Qt::WindowFrameSection QGraphicsWidget::windowFrameSectionAt(const QPointF &pos) const
{
    Q_D(const QGraphicsWidget);

    const QRectF r = windowFrameRect();
    if (!r.contains(pos))
        return Qt::NoSection;
    
    const qreal left = r.left();
    const qreal top = r.top();
    const qreal right = r.right();
    const qreal bottom = r.bottom();
    const qreal x = pos.x();
    const qreal y = pos.y();

    const qreal cornerMargin = 20;
    //### Not sure of this one, it should be the same value for all edges.
    const qreal windowFrameWidth = d->leftWindowFrameMargin;

    Qt::WindowFrameSection s = Qt::NoSection;
    if (x <= left + cornerMargin) {
        if (y <= top + windowFrameWidth || (x <= left + windowFrameWidth && y <= top + cornerMargin)) {
            s = Qt::TopLeftSection;
        } else if (y >= bottom - windowFrameWidth || (x <= left + windowFrameWidth && y >= bottom - windowFrameWidth)) {
            s = Qt::BottomLeftSection;
        } else if (x <= left + windowFrameWidth) {
            s = Qt::LeftSection;
        }
    } else if (x >= right - cornerMargin) {
        if (y <= top + windowFrameWidth || (x >= right - windowFrameWidth && y <= top + cornerMargin)) {
            s = Qt::TopRightSection;
        } else if (y >= bottom - windowFrameWidth || (x >= right - windowFrameWidth && y >= bottom - windowFrameWidth)) {
            s = Qt::BottomRightSection;
        } else if (x >= right - windowFrameWidth) {
            s = Qt::RightSection;
        }
    } else if (y <= top + windowFrameWidth) {
        s = Qt::TopSection;
    } else if (y >= bottom - windowFrameWidth) {
        s = Qt::BottomSection;
    }
    if (s == Qt::NoSection) {
        QRectF r1 = r;
        r1.setHeight(d->topWindowFrameMargin);
        if (r1.contains(pos))
            s = Qt::TitleBarArea;
    }
    return s;
}

/*!
    \reimp

    QGraphicsWidget handles the following events:

    \table   \o Event                 \o Usage
    \row     \o Polish
                    \o Delivered to the widget some time after it has been
                        shown.
    \row     \o GraphicsSceneMove
                    \o Delivered to the widget after its local position has
                        changed.
    \row     \o GraphicsSceneResize
                    \o Delivered to the widget after its size has changed.
    \row     \o Show
                    \o Delivered to the widget before it has been shown.
    \row     \o Hide
                    \o Delivered to the widget after it has been hidden.
    \row     \o PaletteChange
                    \o Delivered to the widget after its palette has changed.
    \row     \o FontChange
                    \o Delivered to the widget after its font has changed.
    \row     \o EnabledChange
                    \o Delivered to the widget after its enabled state has
                        changed.
    \row     \o StyleChange
                    \o Delivered to the widget after its style has changed.
    \row     \o LayoutDirectionChange
                    \o Delivered to the widget after its layout direction has
                        changed.
    \row     \o ContentsRectChange
                    \o Delivered to the widget after its contents margins/
                        contents rect has changed.
    \endtable
*/
bool QGraphicsWidget::event(QEvent *event)
{
    Q_D(QGraphicsWidget);
    // Forward the event to the layout first.
    if (d->layout)
        d->layout->widgetEvent(event);

    // Handle the event itself.
    switch (event->type()) {
    case QEvent::GraphicsSceneMove:
        moveEvent(static_cast<QGraphicsSceneMoveEvent *>(event));
        break;
    case QEvent::GraphicsSceneResize:
        resizeEvent(static_cast<QGraphicsSceneResizeEvent *>(event));
        break;
    case QEvent::Show:
        showEvent(static_cast<QShowEvent *>(event));
        break;
    case QEvent::Hide:
        hideEvent(static_cast<QHideEvent *>(event));
        break;
    case QEvent::Polish:
        polishEvent();
        break;
    case QEvent::WindowActivate:
    case QEvent::WindowDeactivate:
        update();
        foreach (QGraphicsItem *child, childItems()) {
            if (child->isWidget())
                QApplication::sendEvent(static_cast<QGraphicsWidget *>(child), event);
        }
        break;
        // Taken from QWidget::event
    case QEvent::ActivationChange:
    case QEvent::EnabledChange:
    case QEvent::FontChange:
    case QEvent::StyleChange:
    case QEvent::PaletteChange:
    case QEvent::ParentChange:
    case QEvent::ContentsRectChange:
    case QEvent::LayoutDirectionChange:
        changeEvent(event);
        break;
    case QEvent::Close:
        closeEvent((QCloseEvent *)event);
        break;
    case QEvent::GrabMouse:
        grabMouseEvent(event);
        break;
    case QEvent::UngrabMouse:
        ungrabMouseEvent(event);
        break;
    case QEvent::GrabKeyboard:
        grabKeyboardEvent(event);
        break;
    case QEvent::UngrabKeyboard:
        ungrabKeyboardEvent(event);
        break;
    case QEvent::GraphicsSceneMousePress:
        if (d->hasDecoration() && windowFrameEvent(event))
            return true;
    case QEvent::GraphicsSceneMouseMove:
    case QEvent::GraphicsSceneMouseRelease:
    case QEvent::GraphicsSceneMouseDoubleClick:
        if (d->hasDecoration() && d->grabbedSection != Qt::NoSection)
            return windowFrameEvent(event);
        break;
    case QEvent::GraphicsSceneHoverEnter:
    case QEvent::GraphicsSceneHoverMove:
    case QEvent::GraphicsSceneHoverLeave:
        if (d->hasDecoration()) {
            windowFrameEvent(event);
            // Filter out hover events if they were sent to us only because of the
            // decoration (special case in QGraphicsScenePrivate::dispatchHoverEvent).
            if (!acceptsHoverEvents())
                return true;
        }
        break;
    default:
        break;
    }
    return QObject::event(event);
}

/*!
   This event handler can be reimplemented to handle state changes.

   The state being changed in this event can be retrieved through \a event.

   Change events include: QEvent::ActivationChange, QEvent::EnabledChange,
   QEvent::FontChange, QEvent::StyleChange, QEvent::PaletteChange,
   QEvent::ParentChange, QEvent::LayoutDirectionChange, and
   QEvent::ContentsRectChange.
*/
void QGraphicsWidget::changeEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::FontChange:
        update();
        updateGeometry();
        break;
    case QEvent::PaletteChange:
        update();
        break;
    default:
        break;
    }
}

/*!
    This event handler, for \a event, can be reimplemented in a subclass to
    receive widget close events.  The default implementation accepts the
    event.

    \sa close(), QCloseEvent
*/
void QGraphicsWidget::closeEvent(QCloseEvent *event)
{
    event->accept();
}

/*!
    \reimp
*/
void QGraphicsWidget::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    if (focusPolicy() != Qt::NoFocus)
        update();
}

/*!
    Finds a new widget to give the keyboard focus to, as appropriate for Tab
    and Shift+Tab, and returns true if it can find a new widget; returns false
    otherwise. If \a next is true, this function searches forward; if \a next
    is false, it searches backward.

    Sometimes, you will want to reimplement this function to provide special
    focus handling for your widget and its subwidgets. For example, a web
    browser might reimplement it to move its current active link forward or
    backward, and call the base implementation only when it reaches the last
    or first link on the page.

    Child widgets call focusNextPrevChild() on their parent widgets, but only
    the window that contains the child widgets decides where to redirect
    focus. By reimplementing this function for an object, you gain control of
    focus traversal for all child widgets.

    \sa focusPolicy()
*/
bool QGraphicsWidget::focusNextPrevChild(bool next)
{
    Q_D(QGraphicsWidget);
    // Let the parent's focusNextPrevChild implementation decide what to do.
    QGraphicsWidget *parent = 0;
    if (!isWindow() && (parent = parentWidget()))
        return parent->focusNextPrevChild(next);
    if (!d->scene)
        return false;
    if (d->scene->focusNextPrevChild(next))
        return true;
    if (isWindow()) {
        setFocus(next ? Qt::TabFocusReason : Qt::BacktabFocusReason);
        if (hasFocus())
            return true;
    }
    return d->scene->focusNextPrevChild(next);
}

/*!
    \reimp
*/
void QGraphicsWidget::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    if (focusPolicy() != Qt::NoFocus)
        update();
}

/*!
    This event handler, for \l{QEvent::Hide}{Hide} events, is delivered after
    the widget has been hidden, for example, setVisible(false) has been called
    for the widget or one of its ancestors when the widget was previously
    shown.

    You can reimplement this event handler to detect when your widget is
    hidden. Calling QEvent::accept() or QEvent::ignore() on \a event has no
    effect.

    \sa showEvent(), QWidget::hideEvent(), ItemVisibleChange
*/
void QGraphicsWidget::hideEvent(QHideEvent *event)
{
    ///### focusNextPrevChild(true), don't lose focus when the focus widget
    // is hidden.
    Q_UNUSED(event);
}

/*!
    This event handler, for \l{QEvent::GraphicsSceneMove}{GraphicsSceneMove}
    events, is delivered after the widget has moved (e.g., its local position
    has changed).

    This event is only delivered when the item is moved locally. Calling
    setTransform() or moving any of the item's ancestors does not affect the
    item's local position.

    You can reimplement this event handler to detect when your widget has
    moved. Calling QEvent::accept() or QEvent::ignore() on \a event has no
    effect.

    \sa ItemPositionChange, ItemPositionHasChanged
*/
void QGraphicsWidget::moveEvent(QGraphicsSceneMoveEvent *event)
{
    // ### Last position is always == current position
    Q_UNUSED(event);
}

/*!
    This event is delivered to the item by the scene at some point after it
    has been constructed, but before it is shown or otherwise accessed through
    the scene. You can use this event handler to do last-minute initializations
    of the widget which require the item to be fully constructed.

    The base implementation does nothing.
*/
void QGraphicsWidget::polishEvent()
{
}

/*!
    This event handler, for
    \l{QEvent::GraphicsSceneResize}{GraphicsSceneResize} events, is
    delivered after the widget has been resized (i.e., its local size has
    changed). \a event contains both the old and the new size.

    This event is only delivered when the widget is resized locally; calling
    setTransform() on the widget or any of its ancestors or view, does not
    affect the widget's local size.

    You can reimplement this event handler to detect when your widget has been
    resized. Calling QEvent::accept() or QEvent::ignore() on \a event has no
    effect.

    \sa geometry(), setGeometry()
*/
void QGraphicsWidget::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event);
}

/*!
    This event handler, for \l{QEvent::Show}{Show} events, is delivered before
    the widget has been shown, for example, setVisible(true) has been called
    for the widget or one of its ancestors when the widget was previously
    hidden.

    You can reimplement this event handler to detect when your widget is
    shown. Calling QEvent::accept() or QEvent::ignore() on \a event has no
    effect.

    \sa hideEvent(), QWidget::showEvent(), ItemVisibleChange
*/
void QGraphicsWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
}

/*!
    \reimp
*/
void QGraphicsWidget::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
}

/*!
    \reimp
*/
void QGraphicsWidget::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
}

/*!
    This event handler, for \a event, can be reimplemented in a subclass to
    receive notifications for Qt::GrabMouse events.

    \sa grabMouse(), grabKeyboard()
*/
void QGraphicsWidget::grabMouseEvent(QEvent *event)
{
    Q_UNUSED(event);
}

/*!
    This event handler, for \a event, can be reimplemented in a subclass to
    receive notifications for Qt::UngrabMouse events.

    \sa ungrabMouse(), ungrabKeyboard()
*/
void QGraphicsWidget::ungrabMouseEvent(QEvent *event)
{
    Q_UNUSED(event);
}

/*!
    This event handler, for \a event, can be reimplemented in a subclass to
    receive notifications for Qt::GrabKeyboard events.

    \sa grabKeyboard(), grabMouse()
*/
void QGraphicsWidget::grabKeyboardEvent(QEvent *event)
{
    Q_UNUSED(event);
}

/*!
    This event handler, for \a event, can be reimplemented in a subclass to
    receive notifications for Qt::UngrabKeyboard events.

    \sa ungrabKeyboard(), ungrabMouse()
*/
void QGraphicsWidget::ungrabKeyboardEvent(QEvent *event)
{
    Q_UNUSED(event);
}

/*!
    Returns the widgets window type.
*/
Qt::WindowType QGraphicsWidget::windowType() const
{
    return Qt::WindowType(int(windowFlags()) & Qt::WindowType_Mask);
}

/*!
    \property QGraphicsWidget::windowFlags
    \brief This propery holds the widget's window flags.

    Window flags are a combination a window type (e.g. Qt::Dialog) and several
    flags giving hints on the behaviour of the window. The behaviour is
    platform dependent.
*/
Qt::WindowFlags QGraphicsWidget::windowFlags() const
{
    Q_D(const QGraphicsWidget);
    return d->windowFlags;
}
void QGraphicsWidget::setWindowFlags(Qt::WindowFlags wFlags)
{
    Q_D(QGraphicsWidget);
    if (d->windowFlags == wFlags)
        return;
    bool wasPopup = (d->windowFlags & Qt::WindowType_Mask) == Qt::Popup;

    d->windowFlags = wFlags;
    if (!d->setWindowFrameMargins)
        unsetWindowFrameMargins();

    bool isPopup = (d->windowFlags & Qt::WindowType_Mask) == Qt::Popup;
    if (d->scene && isVisible() && wasPopup != isPopup) {
        // Popup state changed; update implicit mouse grab.
        if (!isPopup)
            d->scene->d_func()->removePopup(this);
        else
            d->scene->d_func()->addPopup(this);
    }
}

/*!
    Returns true if this widget's window is the active window.

    The active window is the window that either contains a child widget that
    currently has input focus, or that itself has input focus.
*/
bool QGraphicsWidget::isActiveWindow() const
{
    Q_D(const QGraphicsWidget);
    if (!d->scene)
        return false;
    const QGraphicsWidget *w = isWindow() ? this : window();
    return w && d->scene->activeWindow() == w;
}

/*!
    \property QGraphicsWidget::windowTitle
    \brief This property holds the window title (caption).

    This property is only used for windows. If no title has been set, the
    title is an empty string.
*/
void QGraphicsWidget::setWindowTitle(const QString &title)
{
    Q_D(QGraphicsWidget);
    d->windowTitle = title;
}
QString QGraphicsWidget::windowTitle() const
{
    Q_D(const QGraphicsWidget);
    return d->windowTitle;
}

/*!
    \property QGraphicsWidget::focusPolicy
    \brief the way the widget accepts keyboard focus

    The focus policy is Qt::TabFocus if the widget accepts keyboard focus by
    tabbing, Qt::ClickFocus if the widget accepts focus by clicking,
    Qt::StrongFocus if it accepts both, and Qt::NoFocus (the default) if it
    does not accept focus at all.

    You must enable keyboard focus for a widget if it processes keyboard
    events. This is normally done from the widget's constructor. For instance,
    the QLineEdit constructor calls setFocusPolicy(Qt::StrongFocus).

    If you enable a focus policy (i.e., not Qt::NoFocus), QGraphicsWidget will
    automatically enable the ItemIsFocusable flag.  Setting Qt::NoFocus on a
    widget will clear the ItemIsFocusable flag. If the widget currently has
    keyboard focus, the widget will automatically lose focus.

    \sa focusInEvent(), focusOutEvent(), keyPressEvent(), keyReleaseEvent(), enabled
*/
Qt::FocusPolicy QGraphicsWidget::focusPolicy() const
{
    Q_D(const QGraphicsWidget);
    return d->focusPolicy;
}
void QGraphicsWidget::setFocusPolicy(Qt::FocusPolicy policy)
{
    Q_D(QGraphicsWidget);
    if (d->focusPolicy == policy)
        return;
    d->focusPolicy = policy;
    if (hasFocus() && policy == Qt::NoFocus)
        clearFocus();
    setFlag(ItemIsFocusable, policy != Qt::NoFocus);
}

/*!
    If this widget, a child or descendent of this widget currently has input
    focus, this function will return a pointer to that widget. If
    no descendent has input focus, 0 is returned.

    \sa QWidget::focusWidget()
*/
QGraphicsWidget *QGraphicsWidget::focusWidget() const
{
    Q_D(const QGraphicsWidget);
    return d->focusChild;
}

/*!
    Moves the \a second widget around the ring of focus widgets so that
    keyboard focus moves from the \a first widget to the \a second widget when
    the Tab key is pressed.

    Note that since the tab order of the \a second widget is changed, you
    should order a chain like this:

    \snippet doc/src/snippets/code/src.gui.graphicsview.qgraphicswidget.cpp 1

    \e not like this:

    \snippet doc/src/snippets/code/src.gui.graphicsview.qgraphicswidget.cpp 2

    If \a first is 0, this indicates that \a second should be the first widget
    to receive input focus should the scene gain Tab focus (i.e., the user
    hits Tab so that focus passes into the scene). If \a second is 0, this
    indicates that \a first should be the first widget to gain focus if the
    scene gained BackTab focus.

    By default, tab order is defined implicitly using widget creation order.

    \sa focusPolicy, {Keyboard Focus}
*/
void QGraphicsWidget::setTabOrder(QGraphicsWidget *first, QGraphicsWidget *second)
{
    if (!first && !second) {
        qWarning("QGraphicsWidget::setTabOrder(0, 0) is undefined");
        return;
    }
    if ((first && second) && first->scene() != second->scene()) {
        qWarning("QGraphicsWidget::setTabOrder: scenes %p and %p are different",
                 first->scene(), second->scene());
        return;
    }
    QGraphicsScene *scene = first ? first->scene() : second->scene();
    if (!scene && (!first || !second)) {
        qWarning("QGraphicsWidget::setTabOrder: assigning tab order from/to the"
                 " scene requires the item to be in a scene.");
        return;
    }

    // If either first or second are 0, the scene's tabFocusFirst is updated
    // to point to the first item in the scene's focus chain. Then first or
    // second are set to point to tabFocusFirst.
    QGraphicsScenePrivate *sceneD = scene->d_func();
    if (!first) {
        sceneD->tabFocusFirst = second;
        return;
    }
    if (!second) {
        sceneD->tabFocusFirst = first->d_func()->focusNext;
        return;
    }

    // Both first and second are != 0.
    QGraphicsWidget *firstFocusNext = first->d_func()->focusNext;
    if (firstFocusNext == second) {
        // Nothing to do.
        return;
    }

    // Update the focus chain.
    QGraphicsWidget *secondFocusPrev = second->d_func()->focusPrev;
    QGraphicsWidget *secondFocusNext = second->d_func()->focusNext;
    firstFocusNext->d_func()->focusPrev = second;
    first->d_func()->focusNext = second;
    second->d_func()->focusNext = firstFocusNext;
    second->d_func()->focusPrev = first;
    secondFocusPrev->d_func()->focusNext = secondFocusNext;
    secondFocusNext->d_func()->focusPrev = secondFocusPrev;

    Q_ASSERT(first->d_func()->focusNext->d_func()->focusPrev == first);
    Q_ASSERT(first->d_func()->focusPrev->d_func()->focusNext == first);

    Q_ASSERT(second->d_func()->focusNext->d_func()->focusPrev == second);
    Q_ASSERT(second->d_func()->focusPrev->d_func()->focusNext == second);

}

/*!
    If \a on is true, this function enables \a attribute; otherwise
    \a attribute is disabled.

    See the class documentation for QGraphicsWidget for a complete list of
    which attributes are supported, and what they are for.

    \sa testAttribute(), QWidget::setAttribute()
*/
void QGraphicsWidget::setAttribute(Qt::WidgetAttribute attribute, bool on)
{
    Q_D(QGraphicsWidget);
    // ### most flags require some immediate action
    // ### we might want to qWarn use of unsupported attributes
    // ### we might want to not use Qt::WidgetAttribute, but roll our own instead
    d->setAttribute(attribute, on);
}

/*!
    Returns true if \a attribute is enabled for this widget; otherwise,
    returns false.

    \sa setAttribute()
*/
bool QGraphicsWidget::testAttribute(Qt::WidgetAttribute attribute) const
{
    Q_D(const QGraphicsWidget);
    return d->testAttribute(attribute);
}

/*!
    \reimp
*/
int QGraphicsWidget::type() const
{
    return Type;
}

/*!
    \reimp
*/
void QGraphicsWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

/*!
    This virtual function is called by QGraphicsScene to draw the window frame
    for windows using \a painter, \a option, and \a widget, in local
    coordinates. The base implementation uses the current style to render the
    frame and title bar.

    You can reimplement this function in a subclass of QGraphicsWidget to
    provide custom rendering of the widget's window frame.

    \sa QGraphicsItem::paint()
*/
void QGraphicsWidget::paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                       QWidget *widget)
{
    const bool fillBackground = !testAttribute(Qt::WA_OpaquePaintEvent)
                                && !testAttribute(Qt::WA_NoSystemBackground);
    QGraphicsProxyWidget *proxy = qobject_cast<QGraphicsProxyWidget *>(this);
    const bool embeddedWidgetFillsOwnBackground = proxy && proxy->widget();

    if (rect().contains(option->exposedRect)) {
        if (fillBackground && !embeddedWidgetFillsOwnBackground)
            painter->fillRect(option->exposedRect, palette().window());
        return;
    }

    Q_D(QGraphicsWidget);

    QRect windowFrameRect = QRect(QPoint(), windowFrameGeometry().size().toSize());
    QStyleOptionTitleBar bar;
    bar.QStyleOption::operator=(*option);
    d->initStyleOptionTitleBar(&bar);   // this clear flags in bar.state
    if (d->buttonMouseOver)
        bar.state |= QStyle::State_MouseOver;
    else
        bar.state &= ~QStyle::State_MouseOver;
    if (d->buttonSunken)
        bar.state |= QStyle::State_Sunken;
    else
        bar.state &= ~QStyle::State_Sunken;

    bar.rect = windowFrameRect;

    // translate painter to make the style happy
    const QPointF styleOrigin = this->windowFrameRect().topLeft();
    painter->translate(styleOrigin);

#ifdef Q_WS_MAC
    const QSize pixmapSize = windowFrameRect.size();
    if (pixmapSize.width() <= 0 || pixmapSize.height() <= 0)
        return;
    QPainter *realPainter = painter;
    QPixmap pm(pixmapSize);
    painter = new QPainter(&pm);
#endif

    // Fill background
    QStyleHintReturnMask mask;
    bool setMask = style()->styleHint(QStyle::SH_WindowFrame_Mask, &bar, widget, &mask) && !mask.region.isEmpty();
    if (setMask) {
        painter->save();
        painter->setClipRegion(mask.region, Qt::IntersectClip);
    }
    if (fillBackground) {
        if (embeddedWidgetFillsOwnBackground) {
            // Don't fill the background twice.
            QPainterPath windowFrameBackground;
            windowFrameBackground.addRect(windowFrameRect);
            // Adjust with 0.5 to avoid border artifacts between
            // widget background and frame background.
            windowFrameBackground.addRect(rect().translated(-styleOrigin).adjusted(0.5, 0.5, -0.5, -0.5));
            painter->fillPath(windowFrameBackground, palette().window());
        } else {
            painter->fillRect(windowFrameRect, palette().window());
        }
    }
    if (setMask)
        painter->restore();
    painter->setRenderHint(QPainter::NonCosmeticDefaultPen);

    // Draw title
    int height = (int)d->titleBarHeight(bar);
    bar.rect.setHeight(height);
    painter->save();
    style()->drawComplexControl(QStyle::CC_TitleBar, &bar, painter, widget);
    painter->restore();

    // Draw window frame
    QStyleOptionFrame frameOptions;
    frameOptions.QStyleOption::operator=(*option);
    initStyleOption(&frameOptions);
    painter->setClipRect(windowFrameRect.adjusted(0, +height, 0, 0), Qt::IntersectClip);
    if (hasFocus()) {
        frameOptions.state |= QStyle::State_HasFocus;
    } else {
        frameOptions.state &= ~QStyle::State_HasFocus;
    }
    bool isActive = isActiveWindow();
    if (isActive) {
        frameOptions.state |= QStyle::State_Active;
    } else {
        frameOptions.state &= ~QStyle::State_Active;
    }

    frameOptions.palette.setCurrentColorGroup(isActive ? QPalette::Active : QPalette::Normal);
    frameOptions.rect = windowFrameRect;
    frameOptions.lineWidth = style()->pixelMetric(QStyle::PM_MdiSubWindowFrameWidth, 0, widget);
    frameOptions.midLineWidth = 1;
    style()->drawPrimitive(QStyle::PE_FrameWindow, &frameOptions, painter, widget);

#ifdef Q_WS_MAC
    realPainter->drawPixmap(QPoint(), pm);
    delete painter;
#endif
}

/*!
    \reimp
*/
QRectF QGraphicsWidget::boundingRect() const
{
    return windowFrameRect();
}

/*!
    \reimp
*/
QPainterPath QGraphicsWidget::shape() const
{
    QPainterPath path;
    path.addRect(rect());
    return path;
}

/*!
    Call this function to close the widget.

    Returns true if the widget was closed; otherwise returns false.
    This slot will first send a QCloseEvent to the widget, which may or may
    not accept the event. If the event was ignored, nothing happens. If the
    event was accepted, it will hide() the widget.

    If the widget has the Qt::WA_DeleteOnClose attribute set it will be
    deleted.
*/
bool QGraphicsWidget::close()
{
    QCloseEvent closeEvent;
    QApplication::sendEvent(this, &closeEvent);
    if (!closeEvent.isAccepted()) {
        return false;
    }
    // hide
    if (isVisible()) {
        hide();
    }
    if (testAttribute(Qt::WA_DeleteOnClose)) {
        deleteLater();
    }
    return true;
}

#ifdef Q_NO_USING_KEYWORD
/*!
    \fn const QObjectList &QGraphicsWidget::children() const

    This function returns the same value as QObject::children(). It's
    provided to differentiate between the obsolete member
    QGraphicsItem::children() and QObject::children(). QGraphicsItem now
    provides childItems() instead.
*/
#endif

#if 0
void QGraphicsWidget::dumpFocusChain(QGraphicsWidget *widget)
{
    qDebug() << "=========== Dumping focus chain ==============";
    int i = 0;
    QGraphicsWidget *fw = widget;
    QGraphicsWidget *next = fw;
    if (fw) {
        do {
            qDebug() << i++ << next->className() << next->data(0) << "focusItem:" << (static_cast<QGraphicsItem*>(next) == fw ? "true" : "false") << "next:" << next->d_func()->focusNext->data(0) << "prev:" << next->d_func()->focusPrev->data(0);
            next = next->d_func()->focusNext; 
        } while (next != fw);
    }
}
#endif

QT_END_NAMESPACE
        
#endif //QT_NO_GRAPHICSVIEW
