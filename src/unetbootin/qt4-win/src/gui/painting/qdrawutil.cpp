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

#include "qdrawutil.h"
#include "qbitmap.h"
#include "qpixmapcache.h"
#include "qapplication.h"
#include "qpainter.h"
#include "qpalette.h"

QT_BEGIN_NAMESPACE

/*!
    \fn void qDrawShadeLine(QPainter *painter, int x1, int y1, int x2, int y2,
                     const QPalette &palette, bool sunken,
                     int lineWidth, int midLineWidth)
    \relates QPainter

    Draws a horizontal (\a y1 == \a y2) or vertical (\a x1 == \a x2)
    shaded line using the given \a painter.  Note that nothing is
    drawn if \a y1 != \a y2 and \a x1 != \a x2 (i.e. the line is
    neither horizontal nor vertical).

    The provided \a palette specifies the shading colors (\l
    {QPalette::light()}{light}, \l {QPalette::dark()}{dark} and \l
    {QPalette::mid()}{middle} colors).  The given \a lineWidth
    specifies the line width for each of the lines; it is not the
    total line width. The given \a midLineWidth specifies the width of
    a middle line drawn in the QPalette::mid() color.

    The line appears sunken if \a sunken is true, otherwise raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style().  Use the drawing functions in QStyle to
    make widgets that follow the current GUI style.


    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a shaded line:

    \snippet doc/src/snippets/code/src.gui.painting.qdrawutil.cpp 0

    \sa qDrawShadeRect(), qDrawShadePanel(), QStyle
*/

void qDrawShadeLine(QPainter *p, int x1, int y1, int x2, int y2,
                     const QPalette &pal, bool sunken,
                     int lineWidth, int midLineWidth)
{
    if (!(p && lineWidth >= 0 && midLineWidth >= 0))        {
        qWarning("qDrawShadeLine: Invalid parameters");
        return;
    }
    int tlw = lineWidth*2 + midLineWidth;        // total line width
    QPen oldPen = p->pen();                        // save pen
    if (sunken)
        p->setPen(pal.color(QPalette::Dark));
    else
        p->setPen(pal.light().color());
    QPolygon a;
    int i;
    if (y1 == y2) {                                // horizontal line
        int y = y1 - tlw/2;
        if (x1 > x2) {                        // swap x1 and x2
            int t = x1;
            x1 = x2;
            x2 = t;
        }
        x2--;
        for (i=0; i<lineWidth; i++) {                // draw top shadow
            a.setPoints(3, x1+i, y+tlw-1-i,
                         x1+i, y+i,
                         x2-i, y+i);
            p->drawPolyline(a);
        }
        if (midLineWidth > 0) {
            p->setPen(pal.mid().color());
            for (i=0; i<midLineWidth; i++)        // draw lines in the middle
                p->drawLine(x1+lineWidth, y+lineWidth+i,
                             x2-lineWidth, y+lineWidth+i);
        }
        if (sunken)
            p->setPen(pal.light().color());
        else
            p->setPen(pal.dark().color());
        for (i=0; i<lineWidth; i++) {                // draw bottom shadow
            a.setPoints(3, x1+i, y+tlw-i-1,
                         x2-i, y+tlw-i-1,
                         x2-i, y+i+1);
            p->drawPolyline(a);
        }
    }
    else if (x1 == x2) {                        // vertical line
        int x = x1 - tlw/2;
        if (y1 > y2) {                        // swap y1 and y2
            int t = y1;
            y1 = y2;
            y2 = t;
        }
        y2--;
        for (i=0; i<lineWidth; i++) {                // draw left shadow
            a.setPoints(3, x+i, y2,
                         x+i, y1+i,
                         x+tlw-1, y1+i);
            p->drawPolyline(a);
        }
        if (midLineWidth > 0) {
            p->setPen(pal.mid().color());
            for (i=0; i<midLineWidth; i++)        // draw lines in the middle
                p->drawLine(x+lineWidth+i, y1+lineWidth, x+lineWidth+i, y2);
        }
        if (sunken)
            p->setPen(pal.light().color());
        else
            p->setPen(pal.dark().color());
        for (i=0; i<lineWidth; i++) {                // draw right shadow
            a.setPoints(3, x+lineWidth, y2-i,
                         x+tlw-i-1, y2-i,
                         x+tlw-i-1, y1+lineWidth);
            p->drawPolyline(a);
        }
    }
    p->setPen(oldPen);
}

/*!
    \fn void qDrawShadeRect(QPainter *painter, int x, int y, int width, int height,
                     const QPalette &palette, bool sunken,
                     int lineWidth, int midLineWidth,
                     const QBrush *fill)
    \relates QPainter

    Draws the shaded rectangle beginning at (\a x, \a y) with the
    given \a width and \a height using the provided \a painter.

    The provide \a palette specifies the shading colors (\l
    {QPalette::light()}{light}, \l {QPalette::dark()}{dark} and \l
    {QPalette::mid()}{middle} colors.  The given \a lineWidth
    specifies the line width for each of the lines; it is not the
    total line width.  The \a midLineWidth specifies the width of a
    middle line drawn in the QPalette::mid() color.  The rectangle's
    interior is filled with the \a fill brush unless \a fill is 0.

    The rectangle appears sunken if \a sunken is true, otherwise
    raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style(). Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a shaded rectangle:

    \snippet doc/src/snippets/code/src.gui.painting.qdrawutil.cpp 1

    \sa qDrawShadeLine(), qDrawShadePanel(), qDrawPlainRect(), QStyle
*/

void qDrawShadeRect(QPainter *p, int x, int y, int w, int h,
                     const QPalette &pal, bool sunken,
                     int lineWidth, int midLineWidth,
                     const QBrush *fill)
{
    if (w == 0 || h == 0)
        return;
    if (! (w > 0 && h > 0 && lineWidth >= 0 && midLineWidth >= 0)) {
        qWarning("qDrawShadeRect: Invalid parameters");
        return;
    }
    QPen oldPen = p->pen();
    if (sunken)
        p->setPen(pal.dark().color());
    else
        p->setPen(pal.light().color());
    int x1=x, y1=y, x2=x+w-1, y2=y+h-1;

    if (lineWidth == 1 && midLineWidth == 0) {// standard shade rectangle
        p->drawRect(x1, y1, w-2, h-2);
        if (sunken)
            p->setPen(pal.light().color());
        else
            p->setPen(pal.dark().color());
        QLineF lines[4] = { QLineF(x1+1, y1+1, x2-2, y1+1),
                            QLineF(x1+1, y1+2, x1+1, y2-2),
                            QLineF(x1, y2, x2, y2),
                            QLineF(x2,y1, x2,y2-1) };
        p->drawLines(lines, 4);              // draw bottom/right lines
    } else {                                        // more complicated
        int m = lineWidth+midLineWidth;
        int i, j=0, k=m;
        for (i=0; i<lineWidth; i++) {                // draw top shadow
            QLineF lines[4] = { QLineF(x1+i, y2-i, x1+i, y1+i),
                                QLineF(x1+i, y1+i, x2-i, y1+i),
                                QLineF(x1+k, y2-k, x2-k, y2-k),
                                QLineF(x2-k, y2-k, x2-k, y1+k) };
            p->drawLines(lines, 4);
            k++;
        }
        p->setPen(pal.mid().color());
        j = lineWidth*2;
        for (i=0; i<midLineWidth; i++) {        // draw lines in the middle
            p->drawRect(x1+lineWidth+i, y1+lineWidth+i, w-j-1, h-j-1);
            j += 2;
        }
        if (sunken)
            p->setPen(pal.light().color());
        else
            p->setPen(pal.dark().color());
        k = m;
        for (i=0; i<lineWidth; i++) {                // draw bottom shadow
            QLineF lines[4] = { QLineF(x1+1+i, y2-i, x2-i, y2-i),
                                QLineF(x2-i, y2-i, x2-i, y1+i+1),
                                QLineF(x1+k, y2-k, x1+k, y1+k),
                                QLineF(x1+k, y1+k, x2-k, y1+k) };
            p->drawLines(lines, 4);
            k++;
        }
    }
    if (fill) {
        QBrush oldBrush = p->brush();
        int tlw = lineWidth + midLineWidth;
        p->setPen(Qt::NoPen);
        p->setBrush(*fill);
        p->drawRect(x+tlw, y+tlw, w-2*tlw, h-2*tlw);
        p->setBrush(oldBrush);
    }
    p->setPen(oldPen);                        // restore pen
}


/*!
    \fn void qDrawShadePanel(QPainter *painter, int x, int y, int width, int height,
                      const QPalette &palette, bool sunken,
                      int lineWidth, const QBrush *fill)
    \relates QPainter

    Draws the shaded panel beginning at (\a x, \a y) with the given \a
    width and \a height using the provided \a painter and the given \a
    lineWidth.

    The given \a palette specifies the shading colors (\l
    {QPalette::light()}{light}, \l {QPalette::dark()}{dark} and \l
    {QPalette::mid()}{middle} colors).  The panel's interior is filled
    with the \a fill brush unless \a fill is 0.

    The panel appears sunken if \a sunken is true, otherwise raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style(). Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a shaded panel:

    \snippet doc/src/snippets/code/src.gui.painting.qdrawutil.cpp 2

    \sa qDrawWinPanel(), qDrawShadeLine(), qDrawShadeRect(), QStyle
*/

void qDrawShadePanel(QPainter *p, int x, int y, int w, int h,
                      const QPalette &pal, bool sunken,
                      int lineWidth, const QBrush *fill)
{
    if (w == 0 || h == 0)
        return;
    if (!(w > 0 && h > 0 && lineWidth >= 0)) {
        qWarning("qDrawShadePanel: Invalid parameters");
    }
    QColor shade = pal.dark().color();
    QColor light = pal.light().color();
    if (fill) {
        if (fill->color() == shade)
            shade = pal.shadow().color();
        if (fill->color() == light)
            light = pal.midlight().color();
    }
    QPen oldPen = p->pen();                        // save pen
    QVector<QLineF> lines;
    lines.reserve(2*lineWidth);

    if (sunken)
        p->setPen(shade);
    else
        p->setPen(light);
    int x1, y1, x2, y2;
    int i;
    x1 = x;
    y1 = y2 = y;
    x2 = x+w-2;
    for (i=0; i<lineWidth; i++) {                // top shadow
        lines << QLineF(x1, y1++, x2--, y2++);
    }
    x2 = x1;
    y1 = y+h-2;
    for (i=0; i<lineWidth; i++) {                // left shado
        lines << QLineF(x1++, y1, x2++, y2--);
    }
    p->drawLines(lines);
    lines.clear();
    if (sunken)
        p->setPen(light);
    else
        p->setPen(shade);
    x1 = x;
    y1 = y2 = y+h-1;
    x2 = x+w-1;
    for (i=0; i<lineWidth; i++) {                // bottom shadow
        lines << QLineF(x1++, y1--, x2, y2--);
    }
    x1 = x2;
    y1 = y;
    y2 = y+h-lineWidth-1;
    for (i=0; i<lineWidth; i++) {                // right shadow
        lines << QLineF(x1--, y1++, x2--, y2);
    }
    p->drawLines(lines);
    if (fill)                                // fill with fill color
        p->fillRect(x+lineWidth, y+lineWidth, w-lineWidth*2, h-lineWidth*2, *fill);
    p->setPen(oldPen);                        // restore pen
}


/*!
  \internal
  This function draws a rectangle with two pixel line width.
  It is called from qDrawWinButton() and qDrawWinPanel().

  c1..c4 and fill are used:

    1 1 1 1 1 2
    1 3 3 3 4 2
    1 3 F F 4 2
    1 3 F F 4 2
    1 4 4 4 4 2
    2 2 2 2 2 2
*/

static void qDrawWinShades(QPainter *p,
                           int x, int y, int w, int h,
                           const QColor &c1, const QColor &c2,
                           const QColor &c3, const QColor &c4,
                           const QBrush *fill)
{
    if (w < 2 || h < 2)                        // can't do anything with that
        return;
    QPen oldPen = p->pen();
    QPoint a[3] = { QPoint(x, y+h-2), QPoint(x, y), QPoint(x+w-2, y) };
    p->setPen(c1);
    p->drawPolyline(a, 3);
    QPoint b[3] = { QPoint(x, y+h-1), QPoint(x+w-1, y+h-1), QPoint(x+w-1, y) };
    p->setPen(c2);
    p->drawPolyline(b, 3);
    if (w > 4 && h > 4) {
        QPoint c[3] = { QPoint(x+1, y+h-3), QPoint(x+1, y+1), QPoint(x+w-3, y+1) };
        p->setPen(c3);
        p->drawPolyline(c, 3);
        QPoint d[3] = { QPoint(x+1, y+h-2), QPoint(x+w-2, y+h-2), QPoint(x+w-2, y+1) };
        p->setPen(c4);
        p->drawPolyline(d, 3);
        if (fill)
            p->fillRect(QRect(x+2, y+2, w-4, h-4), *fill);
    }
    p->setPen(oldPen);
}


/*!
    \fn void qDrawWinButton(QPainter *painter, int x, int y, int width, int height,
                     const QPalette &palette, bool sunken,
                     const QBrush *fill)
    \relates QPainter

    Draws the Windows-style button specified by the given point (\a x,
    \a y}, \a width and \a height using the provided \a painter with a
    line width of 2 pixels. The button's interior is filled with the
    \a{fill} brush unless \a fill is 0.

    The given \a palette specifies the shading colors (\l
    {QPalette::light()}{light}, \l {QPalette::dark()}{dark} and \l
    {QPalette::mid()}{middle} colors).

    The button appears sunken if \a sunken is true, otherwise raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style()-> Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    \sa qDrawWinPanel(), QStyle
*/

void qDrawWinButton(QPainter *p, int x, int y, int w, int h,
                     const QPalette &pal, bool sunken,
                     const QBrush *fill)
{
    if (sunken)
        qDrawWinShades(p, x, y, w, h,
                       pal.shadow().color(), pal.light().color(), pal.dark().color(),
                       pal.button().color(), fill);
    else
        qDrawWinShades(p, x, y, w, h,
                       pal.light().color(), pal.shadow().color(), pal.button().color(),
                       pal.dark().color(), fill);
}

/*!
    \fn void qDrawWinPanel(QPainter *painter, int x, int y, int width, int height,
                    const QPalette &palette, bool        sunken,
                    const QBrush *fill)
    \relates QPainter

    Draws the Windows-style panel specified by the given point(\a x,
    \a y), \a width and \a height using the provided \a painter with a
    line width of 2 pixels. The button's interior is filled with the
    \a fill brush unless \a fill is 0.

    The given \a palette specifies the shading colors.  The panel
    appears sunken if \a sunken is true, otherwise raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style(). Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a shaded panel:

    \snippet doc/src/snippets/code/src.gui.painting.qdrawutil.cpp 3

    \sa qDrawShadePanel(), qDrawWinButton(), QStyle
*/

void qDrawWinPanel(QPainter *p, int x, int y, int w, int h,
                    const QPalette &pal, bool        sunken,
                    const QBrush *fill)
{
    if (sunken)
        qDrawWinShades(p, x, y, w, h,
                        pal.dark().color(), pal.light().color(), pal.shadow().color(),
                       pal.midlight().color(), fill);
    else
        qDrawWinShades(p, x, y, w, h,
                       pal.light().color(), pal.shadow().color(), pal.midlight().color(),
                       pal.dark().color(), fill);
}

/*!
    \fn void qDrawPlainRect(QPainter *painter, int x, int y, int width, int height, const QColor &lineColor,
                     int lineWidth, const QBrush *fill)
    \relates QPainter

    Draws the plain rectangle beginning at (\a x, \a y) with the given
    \a width and \a height, using the specified \a painter, \a lineColor
    and \a lineWidth. The rectangle's interior is filled with the \a
    fill brush unless \a fill is 0.

    \warning This function does not look at QWidget::style() or
    QApplication::style(). Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a plain rectangle:

    \snippet doc/src/snippets/code/src.gui.painting.qdrawutil.cpp 4

    \sa qDrawShadeRect(), QStyle
*/

void qDrawPlainRect(QPainter *p, int x, int y, int w, int h, const QColor &c,
                     int lineWidth, const QBrush *fill)
{
    if (w == 0 || h == 0)
        return;
    if (!(w > 0 && h > 0 && lineWidth >= 0)) {
        qWarning("qDrawPlainRect: Invalid parameters");
    }
    QPen   oldPen   = p->pen();
    QBrush oldBrush = p->brush();
    p->setPen(c);
    p->setBrush(Qt::NoBrush);
    for (int i=0; i<lineWidth; i++)
        p->drawRect(x+i, y+i, w-i*2 - 1, h-i*2 - 1);
    if (fill) {                                // fill with fill color
        p->setPen(Qt::NoPen);
        p->setBrush(*fill);
        p->drawRect(x+lineWidth, y+lineWidth, w-lineWidth*2, h-lineWidth*2);
    }
    p->setPen(oldPen);
    p->setBrush(oldBrush);
}

/*****************************************************************************
  Overloaded functions.
 *****************************************************************************/

/*!
    \fn void qDrawShadeLine(QPainter *painter, const QPoint &p1, const QPoint &p2,
             const QPalette &palette, bool sunken, int lineWidth, int midLineWidth)
    \relates QPainter
    \overload

    Draws a horizontal or vertical shaded line between \a p1 and \a p2
    using the given \a painter.  Note that nothing is drawn if the line
    between the points would be neither horizontal nor vertical.

    The provided \a palette specifies the shading colors (\l
    {QPalette::light()}{light}, \l {QPalette::dark()}{dark} and \l
    {QPalette::mid()}{middle} colors).  The given \a lineWidth
    specifies the line width for each of the lines; it is not the
    total line width. The given \a midLineWidth specifies the width of
    a middle line drawn in the QPalette::mid() color.

    The line appears sunken if \a sunken is true, otherwise raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style().  Use the drawing functions in QStyle to
    make widgets that follow the current GUI style.


    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a shaded line:

    \snippet doc/src/snippets/code/src.gui.painting.qdrawutil.cpp 5

    \sa qDrawShadeRect(), qDrawShadePanel(), QStyle
*/

void qDrawShadeLine(QPainter *p, const QPoint &p1, const QPoint &p2,
                     const QPalette &pal, bool sunken,
                     int lineWidth, int midLineWidth)
{
    qDrawShadeLine(p, p1.x(), p1.y(), p2.x(), p2.y(), pal, sunken,
                    lineWidth, midLineWidth);
}

/*!
    \fn void qDrawShadeRect(QPainter *painter, const QRect &rect, const QPalette &palette,
             bool sunken, int lineWidth, int midLineWidth, const QBrush *fill)
    \relates QPainter
    \overload

    Draws the shaded rectangle specified by \a rect using the given \a painter.

    The provide \a palette specifies the shading colors (\l
    {QPalette::light()}{light}, \l {QPalette::dark()}{dark} and \l
    {QPalette::mid()}{middle} colors.  The given \a lineWidth
    specifies the line width for each of the lines; it is not the
    total line width.  The \a midLineWidth specifies the width of a
    middle line drawn in the QPalette::mid() color.  The rectangle's
    interior is filled with the \a fill brush unless \a fill is 0.

    The rectangle appears sunken if \a sunken is true, otherwise
    raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style(). Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a shaded rectangle:

    \snippet doc/src/snippets/code/src.gui.painting.qdrawutil.cpp 6

    \sa qDrawShadeLine(), qDrawShadePanel(), qDrawPlainRect(), QStyle
*/

void qDrawShadeRect(QPainter *p, const QRect &r,
                     const QPalette &pal, bool sunken,
                     int lineWidth, int midLineWidth,
                     const QBrush *fill)
{
    qDrawShadeRect(p, r.x(), r.y(), r.width(), r.height(), pal, sunken,
                    lineWidth, midLineWidth, fill);
}

/*!
    \fn void qDrawShadePanel(QPainter *painter, const QRect &rect, const QPalette &palette,
             bool sunken, int lineWidth, const QBrush *fill)
    \relates QPainter
    \overload

    Draws the shaded panel at the rectangle specified by \a rect using the
    given \a painter and the given \a lineWidth.

    The given \a palette specifies the shading colors (\l
    {QPalette::light()}{light}, \l {QPalette::dark()}{dark} and \l
    {QPalette::mid()}{middle} colors).  The panel's interior is filled
    with the \a fill brush unless \a fill is 0.

    The panel appears sunken if \a sunken is true, otherwise raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style(). Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a shaded panel:

    \snippet doc/src/snippets/code/src.gui.painting.qdrawutil.cpp 7

    \sa qDrawWinPanel(), qDrawShadeLine(), qDrawShadeRect(), QStyle
*/

void qDrawShadePanel(QPainter *p, const QRect &r,
                      const QPalette &pal, bool sunken,
                      int lineWidth, const QBrush *fill)
{
    qDrawShadePanel(p, r.x(), r.y(), r.width(), r.height(), pal, sunken,
                     lineWidth, fill);
}

/*!
    \fn void qDrawWinButton(QPainter *painter, const QRect &rect, const QPalette &palette,
             bool sunken, const QBrush *fill)
    \relates QPainter
    \overload

    Draws the Windows-style button at the rectangle specified by \a rect using
    the given \a painter with a line width of 2 pixels. The button's interior
    is filled with the \a{fill} brush unless \a fill is 0.

    The given \a palette specifies the shading colors (\l
    {QPalette::light()}{light}, \l {QPalette::dark()}{dark} and \l
    {QPalette::mid()}{middle} colors).

    The button appears sunken if \a sunken is true, otherwise raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style()-> Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    \sa qDrawWinPanel(), QStyle
*/

void qDrawWinButton(QPainter *p, const QRect &r,
                     const QPalette &pal, bool sunken, const QBrush *fill)
{
    qDrawWinButton(p, r.x(), r.y(), r.width(), r.height(), pal, sunken, fill);
}

/*!
    \fn void qDrawWinPanel(QPainter *painter, const QRect &rect, const QPalette &palette,
             bool sunken, const QBrush *fill)
    \overload

    Draws the Windows-style panel at the rectangle specified by \a rect using
    the given \a painter with a line width of 2 pixels. The button's interior
    is filled with the \a fill brush unless \a fill is 0.

    The given \a palette specifies the shading colors.  The panel
    appears sunken if \a sunken is true, otherwise raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style(). Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a shaded panel:

    \snippet doc/src/snippets/code/src.gui.painting.qdrawutil.cpp 8

    \sa qDrawShadePanel(), qDrawWinButton(), QStyle
*/

void qDrawWinPanel(QPainter *p, const QRect &r,
                    const QPalette &pal, bool sunken, const QBrush *fill)
{
    qDrawWinPanel(p, r.x(), r.y(), r.width(), r.height(), pal, sunken, fill);
}

/*!
    \fn void qDrawPlainRect(QPainter *painter, const QRect &rect, const QColor &lineColor, int lineWidth, const QBrush *fill)
    \relates QPainter
    \overload

    Draws the plain rectangle specified by \a rect using the given \a painter,
    \a lineColor and \a lineWidth. The rectangle's interior is filled with the
    \a fill brush unless \a fill is 0.

    \warning This function does not look at QWidget::style() or
    QApplication::style(). Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a plain rectangle:

    \snippet doc/src/snippets/code/src.gui.painting.qdrawutil.cpp 9

    \sa qDrawShadeRect(), QStyle
*/

void qDrawPlainRect(QPainter *p, const QRect &r, const QColor &c,
                     int lineWidth, const QBrush *fill)
{
    qDrawPlainRect(p, r.x(), r.y(), r.width(), r.height(), c,
                    lineWidth, fill);
}

#ifdef QT3_SUPPORT
static void qDrawWinArrow(QPainter *p, Qt::ArrowType type, bool down,
                           int x, int y, int w, int h,
                           const QPalette &pal, bool enabled)
{
    QPolygon a;                                // arrow polygon
    switch (type) {
    case Qt::UpArrow:
        a.setPoints(7, -3,1, 3,1, -2,0, 2,0, -1,-1, 1,-1, 0,-2);
        break;
    case Qt::DownArrow:
        a.setPoints(7, -3,-1, 3,-1, -2,0, 2,0, -1,1, 1,1, 0,2);
        break;
    case Qt::LeftArrow:
        a.setPoints(7, 1,-3, 1,3, 0,-2, 0,2, -1,-1, -1,1, -2,0);
        break;
    case Qt::RightArrow:
        a.setPoints(7, -1,-3, -1,3, 0,-2, 0,2, 1,-1, 1,1, 2,0);
        break;
    default:
        break;
    }
    if (a.isEmpty())
        return;

    if (down) {
        x++;
        y++;
    }

    QPen savePen = p->pen();                        // save current pen
    if (down)
        p->setBrushOrigin(p->brushOrigin() + QPoint(1,1));
    p->fillRect(x, y, w, h, pal.brush(QPalette::Button));
    if (down)
        p->setBrushOrigin(p->brushOrigin() - QPoint(1,1));
    if (enabled) {
        a.translate(x+w/2, y+h/2);
        p->setPen(pal.foreground().color());
        p->drawLine(a.at(0), a.at(1));
        p->drawLine(a.at(2), a.at(2));
        p->drawPoint(a[6]);
    } else {
        a.translate(x+w/2+1, y+h/2+1);
        p->setPen(pal.light().color());
        p->drawLine(a.at(0), a.at(1));
        p->drawLine(a.at(2), a.at(2));
        p->drawPoint(a[6]);
        a.translate(-1, -1);
        p->setPen(pal.mid().color());
        p->drawLine(a.at(0), a.at(1));
        p->drawLine(a.at(2), a.at(2));
        p->drawPoint(a[6]);
    }
    p->setPen(savePen);                        // restore pen
}
#endif // QT3_SUPPORT

#if defined(Q_CC_MSVC)
#pragma warning(disable: 4244)
#endif

#ifdef QT3_SUPPORT
#ifndef QT_NO_STYLE_MOTIF
// motif arrows look the same whether they are used or not
// is this correct?
static void qDrawMotifArrow(QPainter *p, Qt::ArrowType type, bool down,
                             int x, int y, int w, int h,
                             const QPalette &pal, bool)
{
    QPolygon bFill;                                // fill polygon
    QPolygon bTop;                                // top shadow.
    QPolygon bBot;                                // bottom shadow.
    QPolygon bLeft;                                // left shadow.
    QTransform matrix;                            // xform matrix
    bool vertical = type == Qt::UpArrow || type == Qt::DownArrow;
    bool horizontal = !vertical;
    int         dim = w < h ? w : h;
    int         colspec = 0x0000;                        // color specification array

    if (dim < 2)                                // too small arrow
        return;

    if (dim > 3) {
        if (dim > 6)
            bFill.resize(dim & 1 ? 3 : 4);
        bTop.resize((dim/2)*2);
        bBot.resize(dim & 1 ? dim + 1 : dim);
        bLeft.resize(dim > 4 ? 4 : 2);
        bLeft.putPoints(0, 2, 0,0, 0,dim-1);
        if (dim > 4)
            bLeft.putPoints(2, 2, 1,2, 1,dim-3);
        bTop.putPoints(0, 4, 1,0, 1,1, 2,1, 3,1);
        bBot.putPoints(0, 4, 1,dim-1, 1,dim-2, 2,dim-2, 3,dim-2);

        for(int i=0; i<dim/2-2 ; i++) {
            bTop.putPoints(i*2+4, 2, 2+i*2,2+i, 5+i*2, 2+i);
            bBot.putPoints(i*2+4, 2, 2+i*2,dim-3-i, 5+i*2,dim-3-i);
        }
        if (dim & 1)                                // odd number size: extra line
            bBot.putPoints(dim-1, 2, dim-3,dim/2, dim-1,dim/2);
        if (dim > 6) {                        // dim>6: must fill interior
            bFill.putPoints(0, 2, 1,dim-3, 1,2);
            if (dim & 1)                        // if size is an odd number
                bFill.setPoint(2, dim - 3, dim / 2);
            else
                bFill.putPoints(2, 2, dim-4,dim/2-1, dim-4,dim/2);
        }
    }
    else {
        if (dim == 3) {                        // 3x3 arrow pattern
            bLeft.setPoints(4, 0,0, 0,2, 1,1, 1,1);
            bTop .setPoints(2, 1,0, 1,0);
            bBot .setPoints(2, 1,2, 2,1);
        }
        else {                                        // 2x2 arrow pattern
            bLeft.setPoints(2, 0,0, 0,1);
            bTop .setPoints(2, 1,0, 1,0);
            bBot .setPoints(2, 1,1, 1,1);
        }
    }

    if (type == Qt::UpArrow || type == Qt::LeftArrow) {
        matrix.translate(x, y);
        if (vertical) {
            matrix.translate(0, h - 1);
            matrix.rotate(-90);
        } else {
            matrix.translate(w - 1, h - 1);
            matrix.rotate(180);
        }
        if (down)
            colspec = horizontal ? 0x2334 : 0x2343;
        else
            colspec = horizontal ? 0x1443 : 0x1434;
    }
    else if (type == Qt::DownArrow || type == Qt::RightArrow) {
        matrix.translate(x, y);
        if (vertical) {
            matrix.translate(w-1, 0);
            matrix.rotate(90);
        }
        if (down)
            colspec = horizontal ? 0x2443 : 0x2434;
        else
            colspec = horizontal ? 0x1334 : 0x1343;
    }

    const QColor *cols[5];
    cols[0] = 0;
    cols[1] = &pal.button().color();
    cols[2] = &pal.mid().color();
    cols[3] = &pal.light().color();
    cols[4] = &pal.dark().color();
#define CMID        *cols[(colspec>>12) & 0xf]
#define CLEFT        *cols[(colspec>>8) & 0xf]
#define CTOP        *cols[(colspec>>4) & 0xf]
#define CBOT        *cols[colspec & 0xf]

    QPen     savePen   = p->pen();                // save current pen
    QBrush   saveBrush = p->brush();                // save current brush
    QTransform wxm = p->transform();
    QPen     pen(Qt::NoPen);
    const QBrush &brush = pal.brush(QPalette::Button);

    p->setPen(pen);
    p->setBrush(brush);
    p->setTransform(matrix, true);                // set transformation matrix
    p->drawPolygon(bFill);                        // fill arrow
    p->setBrush(Qt::NoBrush);                        // don't fill

    p->setPen(CLEFT);
    p->drawLines(bLeft);
    p->setPen(CTOP);
    p->drawLines(bTop);
    p->setPen(CBOT);
    p->drawLines(bBot);

    p->setTransform(wxm);
    p->setBrush(saveBrush);                        // restore brush
    p->setPen(savePen);                        // restore pen

#undef CMID
#undef CLEFT
#undef CTOP
#undef CBOT
}
#endif // QT_NO_STYLE_MOTIF

QRect qItemRect(QPainter *p, Qt::GUIStyle gs,
                int x, int y, int w, int h,
                int flags,
                bool enabled,
                const QPixmap *pixmap,
                const QString& text, int len)
{
    QRect result;

    if (pixmap) {
        if ((flags & Qt::AlignVCenter) == Qt::AlignVCenter)
            y += h/2 - pixmap->height()/2;
        else if ((flags & Qt::AlignBottom) == Qt::AlignBottom)
            y += h - pixmap->height();
        if ((flags & Qt::AlignRight) == Qt::AlignRight)
            x += w - pixmap->width();
        else if ((flags & Qt::AlignHCenter) == Qt::AlignHCenter)
            x += w/2 - pixmap->width()/2;
        else if ((flags & Qt::AlignLeft) != Qt::AlignLeft && QApplication::isRightToLeft())
            x += w - pixmap->width();
        result = QRect(x, y, pixmap->width(), pixmap->height());
    } else if (!text.isNull() && p) {
        result = p->boundingRect(QRect(x, y, w, h), flags, text.left(len));
        if (gs == Qt::WindowsStyle && !enabled) {
            result.setWidth(result.width()+1);
            result.setHeight(result.height()+1);
        }
    } else {
        result = QRect(x, y, w, h);
    }

    return result;
}

void qDrawArrow(QPainter *p, Qt::ArrowType type, Qt::GUIStyle style, bool down,
                 int x, int y, int w, int h,
                 const QPalette &pal, bool enabled)
{
    switch (style) {
        case Qt::WindowsStyle:
            qDrawWinArrow(p, type, down, x, y, w, h, pal, enabled);
            break;
#ifndef QT_NO_STYLE_MOTIF
        case Qt::MotifStyle:
            qDrawMotifArrow(p, type, down, x, y, w, h, pal, enabled);
            break;
#endif
        default:
            qWarning("qDrawArrow: Requested unsupported GUI style");
    }
}

void qDrawItem(QPainter *p, Qt::GUIStyle gs,
                int x, int y, int w, int h,
                int flags,
                const QPalette &pal, bool enabled,
                const QPixmap *pixmap,
                const QString& text, int len , const QColor* penColor)
{
    p->setPen(penColor?*penColor:pal.foreground().color());
    if (pixmap) {
        QPixmap  pm(*pixmap);
        bool clip = (flags & Qt::TextDontClip) == 0;
        if (clip) {
            if (pm.width() < w && pm.height() < h)
                clip = false;
            else
                p->setClipRect(x, y, w, h);
        }
        if ((flags & Qt::AlignVCenter) == Qt::AlignVCenter)
            y += h/2 - pm.height()/2;
        else if ((flags & Qt::AlignBottom) == Qt::AlignBottom)
            y += h - pm.height();
        if ((flags & Qt::AlignRight) == Qt::AlignRight)
            x += w - pm.width();
        else if ((flags & Qt::AlignHCenter) == Qt::AlignHCenter)
            x += w/2 - pm.width()/2;
        else if (((flags & Qt::AlignLeft) != Qt::AlignLeft) && QApplication::isRightToLeft()) // Qt::AlignAuto && rightToLeft
            x += w - pm.width();

        if (!enabled) {
            if (pm.hasAlphaChannel()) {                        // pixmap with a mask
                pm = pm.mask();
            } else if (pm.depth() == 1) {        // monochrome pixmap, no mask
                ;
#ifndef QT_NO_IMAGE_HEURISTIC_MASK
            } else {                                // color pixmap, no mask
                QString k;
                k.sprintf("$qt-drawitem-%llx", pm.cacheKey());
                if (!QPixmapCache::find(k, pm)) {
                    pm = pm.createHeuristicMask();
                    pm.setMask((QBitmap&)pm);
                    QPixmapCache::insert(k, pm);
                }
#endif
            }
            if (gs == Qt::WindowsStyle) {
                p->setPen(pal.light().color());
                p->drawPixmap(x+1, y+1, pm);
                p->setPen(pal.text().color());
            }
        }
        p->drawPixmap(x, y, pm);
        if (clip)
            p->setClipping(false);
    } else if (!text.isNull()) {
        if (gs == Qt::WindowsStyle && !enabled) {
            p->setPen(pal.light().color());
            p->drawText(x+1, y+1, w, h, flags, text.left(len));
            p->setPen(pal.text().color());
        }
        p->drawText(x, y, w, h, flags, text.left(len));
    }
}

#endif

QT_END_NAMESPACE
