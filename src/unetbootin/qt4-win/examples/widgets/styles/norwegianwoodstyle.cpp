/****************************************************************************
**
** Copyright (C) 2005-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
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

#include <QtGui>

#include "norwegianwoodstyle.h"

//! [0]
void NorwegianWoodStyle::polish(QPalette &palette)
{
    QColor brown(212, 140, 95);
    QColor beige(236, 182, 120);
    QColor slightlyOpaqueBlack(0, 0, 0, 63);

    QPixmap backgroundImage(":/images/woodbackground.png");
    QPixmap buttonImage(":/images/woodbutton.png");
    QPixmap midImage = buttonImage;

    QPainter painter;
    painter.begin(&midImage);
    painter.setPen(Qt::NoPen);
    painter.fillRect(midImage.rect(), slightlyOpaqueBlack);
    painter.end();
//! [0]

//! [1]
    palette = QPalette(brown);

    palette.setBrush(QPalette::BrightText, Qt::white);
    palette.setBrush(QPalette::Base, beige);
    palette.setBrush(QPalette::Highlight, Qt::darkGreen);
    setTexture(palette, QPalette::Button, buttonImage);
    setTexture(palette, QPalette::Mid, midImage);
    setTexture(palette, QPalette::Window, backgroundImage);

    QBrush brush = palette.background();
    brush.setColor(brush.color().dark());

    palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
    palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Mid, brush);
}
//! [1]

//! [3]
void NorwegianWoodStyle::polish(QWidget *widget)
//! [3] //! [4]
{
    if (qobject_cast<QPushButton *>(widget)
            || qobject_cast<QComboBox *>(widget))
        widget->setAttribute(Qt::WA_Hover, true);
}
//! [4]

//! [5]
void NorwegianWoodStyle::unpolish(QWidget *widget)
//! [5] //! [6]
{
    if (qobject_cast<QPushButton *>(widget)
            || qobject_cast<QComboBox *>(widget))
        widget->setAttribute(Qt::WA_Hover, false);
}
//! [6]

//! [7]
int NorwegianWoodStyle::pixelMetric(PixelMetric metric,
//! [7] //! [8]
                                    const QStyleOption *option,
                                    const QWidget *widget) const
{
    switch (metric) {
    case PM_ComboBoxFrameWidth:
        return 8;
    case PM_ScrollBarExtent:
        return QMotifStyle::pixelMetric(metric, option, widget) + 4;
    default:
        return QMotifStyle::pixelMetric(metric, option, widget);
    }
}
//! [8]

//! [9]
int NorwegianWoodStyle::styleHint(StyleHint hint, const QStyleOption *option,
//! [9] //! [10]
                                  const QWidget *widget,
                                  QStyleHintReturn *returnData) const
{
    switch (hint) {
    case SH_DitherDisabledText:
        return int(false);
    case SH_EtchDisabledText:
        return int(true);
    default:
        return QMotifStyle::styleHint(hint, option, widget, returnData);
    }
}
//! [10]

//! [11]
void NorwegianWoodStyle::drawPrimitive(PrimitiveElement element,
//! [11] //! [12]
                                       const QStyleOption *option,
                                       QPainter *painter,
                                       const QWidget *widget) const
{
    switch (element) {
    case PE_PanelButtonCommand:
        {
            int delta = (option->state & State_MouseOver) ? 64 : 0;
            QColor slightlyOpaqueBlack(0, 0, 0, 63);
            QColor semiTransparentWhite(255, 255, 255, 127 + delta);
            QColor semiTransparentBlack(0, 0, 0, 127 - delta);

            int x, y, width, height;
            option->rect.getRect(&x, &y, &width, &height);
//! [12]

//! [13]
            QPainterPath roundRect = roundRectPath(option->rect);
//! [13] //! [14]
            int radius = qMin(width, height) / 2;
//! [14]

//! [15]
            QBrush brush;
//! [15] //! [16]
            bool darker;

            const QStyleOptionButton *buttonOption =
                    qstyleoption_cast<const QStyleOptionButton *>(option);
            if (buttonOption
                    && (buttonOption->features & QStyleOptionButton::Flat)) {
                brush = option->palette.background();
                darker = (option->state & (State_Sunken | State_On));
            } else {
                if (option->state & (State_Sunken | State_On)) {
                    brush = option->palette.mid();
                    darker = !(option->state & State_Sunken);
                } else {
                    brush = option->palette.button();
                    darker = false;
//! [16] //! [17]
                }
//! [17] //! [18]
            }
//! [18]

//! [19]
            painter->save();
//! [19] //! [20]
            painter->setRenderHint(QPainter::Antialiasing, true);
//! [20] //! [21]
            painter->fillPath(roundRect, brush);
//! [21] //! [22]
            if (darker)
//! [22] //! [23]
                painter->fillPath(roundRect, slightlyOpaqueBlack);
//! [23]

//! [24]
            int penWidth;
//! [24] //! [25]
            if (radius < 10)
                penWidth = 3;
            else if (radius < 20)
                penWidth = 5;
            else
                penWidth = 7;

            QPen topPen(semiTransparentWhite, penWidth);
            QPen bottomPen(semiTransparentBlack, penWidth);

            if (option->state & (State_Sunken | State_On))
                qSwap(topPen, bottomPen);
//! [25]

//! [26]
            int x1 = x;
            int x2 = x + radius;
            int x3 = x + width - radius;
            int x4 = x + width;

            if (option->direction == Qt::RightToLeft) {
                qSwap(x1, x4);
                qSwap(x2, x3);
            }

            QPolygon topHalf;
            topHalf << QPoint(x1, y)
                    << QPoint(x4, y)
                    << QPoint(x3, y + radius)
                    << QPoint(x2, y + height - radius)
                    << QPoint(x1, y + height);

            painter->setClipPath(roundRect);
            painter->setClipRegion(topHalf, Qt::IntersectClip);
            painter->setPen(topPen);
            painter->drawPath(roundRect);
//! [26] //! [32]

            QPolygon bottomHalf = topHalf;
            bottomHalf[0] = QPoint(x4, y + height);

            painter->setClipPath(roundRect);
            painter->setClipRegion(bottomHalf, Qt::IntersectClip);
            painter->setPen(bottomPen);
            painter->drawPath(roundRect);

            painter->setPen(option->palette.foreground().color());
            painter->setClipping(false);
            painter->drawPath(roundRect);

            painter->restore();
        }
        break;
//! [32] //! [33]
    default:
//! [33] //! [34]
        QMotifStyle::drawPrimitive(element, option, painter, widget);
    }
}
//! [34]

//! [35]
void NorwegianWoodStyle::drawControl(ControlElement element,
//! [35] //! [36]
                                     const QStyleOption *option,
                                     QPainter *painter,
                                     const QWidget *widget) const
{
    switch (element) {
    case CE_PushButtonLabel:
        {
            QStyleOptionButton myButtonOption;
            const QStyleOptionButton *buttonOption =
                    qstyleoption_cast<const QStyleOptionButton *>(option);
            if (buttonOption) {
                myButtonOption = *buttonOption;
                if (myButtonOption.palette.currentColorGroup()
                        != QPalette::Disabled) {
                    if (myButtonOption.state & (State_Sunken | State_On)) {
                        myButtonOption.palette.setBrush(QPalette::ButtonText,
                                myButtonOption.palette.brightText());
                    }
                }
            }
            QMotifStyle::drawControl(element, &myButtonOption, painter, widget);
        }
        break;
    default:
        QMotifStyle::drawControl(element, option, painter, widget);
    }
}
//! [36]

//! [37]
void NorwegianWoodStyle::setTexture(QPalette &palette, QPalette::ColorRole role,
//! [37] //! [38]
                                    const QPixmap &pixmap)
{
    for (int i = 0; i < QPalette::NColorGroups; ++i) {
        QColor color = palette.brush(QPalette::ColorGroup(i), role).color();
        palette.setBrush(QPalette::ColorGroup(i), role, QBrush(color, pixmap));
    }
}
//! [38]

//! [39]
QPainterPath NorwegianWoodStyle::roundRectPath(const QRect &rect)
//! [39] //! [40]
{
    int radius = qMin(rect.width(), rect.height()) / 2;
    int diam = 2 * radius;

    int x1, y1, x2, y2;
    rect.getCoords(&x1, &y1, &x2, &y2);

    QPainterPath path;
    path.moveTo(x2, y1 + radius);
    path.arcTo(QRect(x2 - diam, y1, diam, diam), 0.0, +90.0);
    path.lineTo(x1 + radius, y1);
    path.arcTo(QRect(x1, y1, diam, diam), 90.0, +90.0);
    path.lineTo(x1, y2 - radius);
    path.arcTo(QRect(x1, y2 - diam, diam, diam), 180.0, +90.0);
    path.lineTo(x1 + radius, y2);
    path.arcTo(QRect(x2 - diam, y2 - diam, diam, diam), 270.0, +90.0);
    path.closeSubpath();
    return path;
}
//! [40]
