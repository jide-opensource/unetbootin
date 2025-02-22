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

#include "qpen.h"
#include "qdatastream.h"
#include "qvariant.h"
#include "qbrush.h"

#include <qdebug.h>

QT_BEGIN_NAMESPACE

/*!
    \class QPen
    \ingroup multimedia
    \ingroup shared
    \mainclass

    \brief The QPen class defines how a QPainter should draw lines and outlines
    of shapes.

    A pen has a style(), width(), brush(), capStyle() and joinStyle().

    The pen style defines the line type. The brush is used to fill
    strokes generated with the pen. Use the QBrush class to specify
    fill styles.  The cap style determines the line end caps that can
    be drawn using QPainter, while the join style describes how joins
    between two lines are drawn. The pen width can be specified in
    both integer (width()) and floating point (widthF()) precision. A
    line width of zero indicates a cosmetic pen.  This means that the
    pen width is always drawn one pixel wide, independent of the \l
    {QPainter#Coordinate Transformations}{transformation} set on the
    painter.

    The various settings can easily be modified using the
    corresponding setStyle(), setWidth(), setBrush(), setCapStyle()
    and setJoinStyle() functions (note that the painter's pen must be
    reset when altering the pen's properties).

    For example:

    \snippet doc/src/snippets/code/src.gui.painting.qpen.cpp 0

    which is equivalent to

    \snippet doc/src/snippets/code/src.gui.painting.qpen.cpp 1

    The default pen is a solid black brush with 0 width, square
    cap style (Qt::SquareCap), and  bevel join style (Qt::BevelJoin).

    In addition QPen provides the color() and setColor()
    convenience functions to extract and set the color of the pen's
    brush, respectively. Pens may also be compared and streamed.

    For more information about painting in general, see \l{The Paint
    System} documentation.

    \tableofcontents

    \section1 Pen Style

    Qt provides several built-in styles represented by the
    Qt::PenStyle enum:

    \table
    \row
    \o \inlineimage qpen-solid.png
    \o \inlineimage qpen-dash.png
    \o \inlineimage qpen-dot.png
    \row
    \o Qt::SolidLine
    \o Qt::DashLine
    \o Qt::DotLine
    \row
    \o \inlineimage qpen-dashdot.png
    \o \inlineimage qpen-dashdotdot.png
    \o \inlineimage qpen-custom.png
    \row
    \o Qt::DashDotLine
    \o Qt::DashDotDotLine
    \o Qt::CustomDashLine
    \endtable

    Simply use the setStyle() function to convert the pen style to
    either of the built-in styles, except the Qt::CustomDashLine style
    which we will come back to shortly. Setting the style to Qt::NoPen
    tells the painter to not draw lines or outlines. The default pen
    style is Qt::SolidLine.

    Since Qt 4.1 it is also possible to specify a custom dash pattern
    using the setDashPattern() function which implicitly converts the
    style of the pen to Qt::CustomDashLine. The pattern argument, a
    QVector, must be specified as an even number of \l qreal entries
    where the entries 1, 3, 5... are the dashes and 2, 4, 6... are the
    spaces. For example, the custom pattern shown above is created
    using the following code:

    \snippet doc/src/snippets/code/src.gui.painting.qpen.cpp 2

    Note that the dash pattern is specified in units of the pens
    width, e.g. a dash of length 5 in width 10 is 50 pixels long.

    The currently set dash pattern can be retrieved using the
    dashPattern() function. Use the isSolid() function to determine
    whether the pen has a solid fill, or not.

    \section1 Cap Style

    The cap style defines how the end points of lines are drawn using
    QPainter.  The cap style only apply to wide lines, i.e. when the
    width is 1 or greater. The Qt::PenCapStyle enum provides the
    following styles:

    \table
    \row
    \o \inlineimage qpen-square.png
    \o \inlineimage qpen-flat.png
    \o \inlineimage qpen-roundcap.png
    \row
    \o Qt::SquareCap
    \o Qt::FlatCap
    \o Qt::RoundCap
    \endtable

    The Qt::SquareCap style is a square line end that covers the end
    point and extends beyond it by half the line width. The
    Qt::FlatCap style is a square line end that does not cover the end
    point of the line. And the Qt::RoundCap style is a rounded line
    end covering the end point.

    The default is Qt::SquareCap.

    Whether or not end points are drawn when the pen width is 0 or 1
    depends on the cap style. Using Qt::SquareCap or Qt::RoundCap they
    are drawn, using Qt::FlatCap they are not drawn.

    \section1 Join Style

    The join style defines how joins between two connected lines can
    be drawn using QPainter. The join style only apply to wide lines,
    i.e. when the width is 1 or greater. The Qt::PenJoinStyle enum
    provides the following styles:

    \table
    \row
    \o \inlineimage qpen-bevel.png
    \o \inlineimage qpen-miter.png
    \o \inlineimage qpen-roundjoin.png
    \row
    \o Qt::BevelJoin
    \o Qt::MiterJoin
    \o Qt::RoundJoin
    \endtable

    The Qt::BevelJoin style fills the triangular notch between the two
    lines. The Qt::MiterJoin style extends the lines to meet at an
    angle. And the Qt::RoundJoin style fills a circular arc between
    the two lines.

    The default is Qt::BevelJoin.

    \image qpen-miterlimit.png

    When the Qt::MiterJoin style is applied, it is possible to use the
    setMiterLimit() function to specify how far the miter join can
    extend from the join point. The miterLimit() is used to reduce
    artifacts between line joins where the lines are close to
    parallel.

    The miterLimit() must be specified in units of the pens width,
    e.g. a miter limit of 5 in width 10 is 50 pixels long. The
    default miter limit is 2, i.e. twice the pen width in pixels.

    \table 100%
    \row
    \o \inlineimage qpen-demo.png
    \o \bold {\l {demos/pathstroke}{The Path Stroking Demo}}

    The Path Stroking demo shows Qt's built-in dash patterns and shows
    how custom patterns can be used to extend the range of available
    patterns.
    \endtable

    \sa QPainter, QBrush, {demos/pathstroke}{Path Stroking Demo},
        {Scribble Example}
*/

class QPenPrivate {
public:
    QPenPrivate(const QBrush &brush, qreal width, Qt::PenStyle, Qt::PenCapStyle,
                Qt::PenJoinStyle _joinStyle);

    QAtomicInt ref;
    qreal width;
    QBrush brush;
    Qt::PenStyle style;
    Qt::PenCapStyle capStyle;
    Qt::PenJoinStyle joinStyle;
    mutable QVector<qreal> dashPattern;
    qreal dashOffset;
    qreal miterLimit;
    uint cosmetic : 1;
};


/*!
  \internal
*/
inline QPenPrivate::QPenPrivate(const QBrush &_brush, qreal _width, Qt::PenStyle penStyle,
                                Qt::PenCapStyle _capStyle, Qt::PenJoinStyle _joinStyle)
    : ref(1), width(_width), brush(_brush), style(penStyle), capStyle(_capStyle),
      joinStyle(_joinStyle), dashOffset(0), miterLimit(2),
      cosmetic(false)
{

}

static const Qt::PenCapStyle qpen_default_cap = Qt::SquareCap;
static const Qt::PenJoinStyle qpen_default_join = Qt::BevelJoin;

#ifndef QT_NO_THREAD
// Special deleter that only deletes if the ref-count goes to zero
template <>
class QGlobalStaticDeleter<QPenPrivate>
{
public:
    QGlobalStatic<QPenPrivate> &globalStatic;
    QGlobalStaticDeleter(QGlobalStatic<QPenPrivate> &_globalStatic)
        : globalStatic(_globalStatic)
    { }

    inline ~QGlobalStaticDeleter()
    {
        if (!globalStatic.pointer->ref.deref())
            delete globalStatic.pointer;
        globalStatic.pointer = 0;
        globalStatic.destroyed = true;
    }
};
#endif

Q_GLOBAL_STATIC_WITH_ARGS(QPenPrivate, defaultPenInstance,
                          (Qt::black, 0, Qt::SolidLine, qpen_default_cap, qpen_default_join))
Q_GLOBAL_STATIC_WITH_ARGS(QPenPrivate, nullPenInstance,
                          (Qt::black, 0, Qt::NoPen, qpen_default_cap, qpen_default_join))

/*!
    Constructs a default black solid line pen with 0 width.
*/

QPen::QPen()
{
    d = defaultPenInstance();
    d->ref.ref();
}

/*!
    Constructs a black pen with 0 width and the given \a style.

    \sa setStyle()
*/

QPen::QPen(Qt::PenStyle style)
{
    if (style == Qt::NoPen) {
        d = nullPenInstance();
        d->ref.ref();
    } else {
        d = new QPenPrivate(Qt::black, 0, style, qpen_default_cap, qpen_default_join);
    }
}


/*!
    Constructs a solid line pen with 0 width and the given \a color.

    \sa setBrush(), setColor()
*/

QPen::QPen(const QColor &color)
{
    d = new QPenPrivate(color, 0, Qt::SolidLine, qpen_default_cap, qpen_default_join);
}


/*!
    \fn QPen::QPen(const QBrush &brush, qreal width, Qt::PenStyle style, Qt::PenCapStyle cap, Qt::PenJoinStyle join)

    Constructs a pen with the specified \a brush, \a width, pen \a style,
    \a cap style and \a join style.

    \sa setBrush(), setWidth(), setStyle(),  setCapStyle(), setJoinStyle()
*/

QPen::QPen(const QBrush &brush, qreal width, Qt::PenStyle s, Qt::PenCapStyle c, Qt::PenJoinStyle j)
{
    d = new QPenPrivate(brush, width, s, c, j);
}

/*!
    \fn QPen::QPen(const QPen &pen)

    Constructs a pen that is a copy of the given \a pen.
*/

QPen::QPen(const QPen &p)
{
    d = p.d;
    d->ref.ref();
}


/*!
    Destroys the pen.
*/

QPen::~QPen()
{
    if (!d->ref.deref())
        delete d;
}

/*!
    \fn void QPen::detach()
    Detaches from shared pen data to make sure that this pen is the
    only one referring the data.

    If multiple pens share common data, this pen dereferences the data
    and gets a copy of the data. Nothing is done if there is just a
    single reference.
*/

void QPen::detach()
{
    if (d->ref == 1)
        return;

    QPenPrivate *x = new QPenPrivate(d->brush, d->width, d->style, d->capStyle,
                                     d->joinStyle);
    x->miterLimit = d->miterLimit;
    x->dashPattern = d->dashPattern;
    x->dashOffset = d->dashOffset;
    x->cosmetic = d->cosmetic;
    if (!d->ref.deref())
        delete d;
    d = x;
}


/*!
    \fn QPen &QPen::operator=(const QPen &pen)

    Assigns the given \a pen to this pen and returns a reference to
    this pen.
*/

QPen &QPen::operator=(const QPen &p)
{
    qAtomicAssign(d, p.d);
    return *this;
}

/*!
   Returns the pen as a QVariant.
*/
QPen::operator QVariant() const
{
    return QVariant(QVariant::Pen, this);
}

/*!
    \fn Qt::PenStyle QPen::style() const

    Returns the pen style.

    \sa setStyle(), {QPen#Pen Style}{Pen Style}
*/

Qt::PenStyle QPen::style() const
{
    return d->style;
}

/*!
    \fn void QPen::setStyle(Qt::PenStyle style)

    Sets the pen style to the given \a style.

    See the \l Qt::PenStyle documentation for a list of the available
    styles. Since Qt 4.1 it is also possible to specify a custom dash
    pattern using the setDashPattern() function which implicitly
    converts the style of the pen to Qt::CustomDashLine.

    \sa style(), {QPen#Pen Style}{Pen Style}
*/

void QPen::setStyle(Qt::PenStyle s)
{
    if (d->style == s)
        return;
    detach();
    d->style = s;
    d->dashPattern.clear();
}

/*!
    Returns the dash pattern of this pen.

    \sa style(), isSolid()
 */
QVector<qreal> QPen::dashPattern() const
{
    if (d->style == Qt::SolidLine || d->style == Qt::NoPen) {
        return QVector<qreal>();
    } else if (d->dashPattern.isEmpty()) {
        const qreal space = 2;
        const qreal dot = 1;
        const qreal dash = 4;

        switch (d->style) {
        case Qt::DashLine:
            d->dashPattern << dash << space;
            break;
        case Qt::DotLine:
            d->dashPattern << dot << space;
            break;
        case Qt::DashDotLine:
            d->dashPattern << dash << space << dot << space;
            break;
        case Qt::DashDotDotLine:
            d->dashPattern << dash << space << dot << space << dot << space;
            break;
        default:
            break;
        }
    }
    return d->dashPattern;
}

/*!
    Sets the dash pattern for this pen to the given \a pattern. This
    implicitly converts the style of the pen to Qt::CustomDashLine.

    The pattern must be specified as an even number of entries where
    the entries 1, 3, 5... are the dashes and 2, 4, 6... are the
    spaces. For example:

    \table 100%
    \row
    \o \inlineimage qpen-custom.png
    \o
    \snippet doc/src/snippets/code/src.gui.painting.qpen.cpp 3
    \endtable

    The dash pattern is specified in units of the pens width; e.g. a
    dash of length 5 in width 10 is 50 pixels long. Note that a pen
    with zero width is equivalent to a cosmetic pen with a width of 1
    pixel.

    Each dash is also subject to cap styles so a dash of 1 with square
    cap set will extend 0.5 pixels out in each direction resulting in
    a total width of 2.

    Note that the default cap style is Qt::SquareCap, meaning that a
    square line end covers the end point and extends beyond it by half
    the line width.

    \sa setStyle(), dashPattern(), setCapStyle(), setCosmetic()
 */
void QPen::setDashPattern(const QVector<qreal> &pattern)
{
    if (pattern.isEmpty())
        return;
    detach();
    d->dashPattern = pattern;
    d->style = Qt::CustomDashLine;

    if ((d->dashPattern.size() % 2) == 1) {
        qWarning("QPen::setDashPattern: Pattern not of even length");
        d->dashPattern << 1;
    }
}


/*!
    Returns the dash offset for the pen.

    \sa setDashOffset()
*/
qreal QPen::dashOffset() const
{
    return d->dashOffset;
}
/*!
    Sets the dash offset for this pen to the given \a offset. This
    implicitly converts the style of the pen to Qt::CustomDashLine.
*/
void QPen::setDashOffset(qreal offset)
{
    if (qFuzzyCompare(offset, d->dashOffset))
        return;
    detach();
    d->dashOffset = offset;
    d->style = Qt::CustomDashLine;
}

/*!
    Returns the miter limit of the pen. The miter limit is only
    relevant when the join style is set to Qt::MiterJoin.

    \sa setMiterLimit(),  {QPen#Join Style}{Join Style}
*/
qreal QPen::miterLimit() const
{
    return d->miterLimit;
}

/*!
    Sets the miter limit of this pen to the given \a limit.

    \image qpen-miterlimit.png

    The miter limit describes how far a miter join can extend from the
    join point. This is used to reduce artifacts between line joins
    where the lines are close to parallel.

    This value does only have effect when the pen style is set to
    Qt::MiterJoin. The value is specified in units of the pen's width,
    e.g. a miter limit of 5 in width 10 is 50 pixels long. The default
    miter limit is 2, i.e. twice the pen width in pixels.

    \sa miterLimit(), setJoinStyle(), {QPen#Join Style}{Join Style}
*/
void QPen::setMiterLimit(qreal limit)
{
    detach();
    d->miterLimit = limit;
}


/*!
    \fn qreal QPen::width() const

    Returns the pen width with integer precision.

    \sa setWidth(), widthF()
*/

int QPen::width() const
{
    return qRound(d->width);
}

/*!
    \fn qreal QPen::widthF() const

    Returns the pen width with floating point precision.

    \sa setWidthF() width()
*/
qreal QPen::widthF() const
{
    return d->width;
}

/*!
    \fn QPen::setWidth(int width)

    Sets the pen width to the given \a width in pixels with integer
    precision.

    A line width of zero indicates a cosmetic pen. This means that the
    pen width is always drawn one pixel wide, independent of the \l
    {QPainter#Coordinate Transformations}{transformation} set on the
    painter.

    Setting a pen width with a negative value is not supported.

    \sa setWidthF(), width()
*/
void QPen::setWidth(int width)
{
    if (width < 0)
        qWarning("QPen::setWidth: Setting a pen width with a negative value is not defined");
    if ((qreal)width == d->width)
        return;
    detach();
    d->width = width;
}

/*!
    Sets the pen width to the given \a width in pixels with floating point
    precision.

    A line width of zero indicates a cosmetic pen. This means that the
    pen width is always drawn one pixel wide, independent of the \l
    {QPainter#Coordinate Transformations}{transformation} on the
    painter.

    Setting a pen width with a negative value is not supported.

    \sa setWidth() widthF()
*/

void QPen::setWidthF(qreal width)
{
    if (width < 0.f)
        qWarning("QPen::setWidthF: Setting a pen width with a negative value is not defined");
    if (qAbs(d->width - width) < 0.00000001f)
        return;
    detach();
    d->width = width;
}


/*!
    Returns the pen's cap style.

    \sa setCapStyle(), {QPen#Cap Style}{Cap Style}
*/
Qt::PenCapStyle QPen::capStyle() const
{
    return d->capStyle;
}

/*!
    \fn void QPen::setCapStyle(Qt::PenCapStyle style)

    Sets the pen's cap style to the given \a style. The default value
    is Qt::SquareCap.

    \sa capStyle(), {QPen#Cap Style}{Cap Style}
*/

void QPen::setCapStyle(Qt::PenCapStyle c)
{
    if (d->capStyle == c)
        return;
    detach();
    d->capStyle = c;
}

/*!
    Returns the pen's join style.

    \sa setJoinStyle(),  {QPen#Join Style}{Join Style}
*/
Qt::PenJoinStyle QPen::joinStyle() const
{
    return d->joinStyle;
}

/*!
    \fn void QPen::setJoinStyle(Qt::PenJoinStyle style)

    Sets the pen's join style to the given \a style. The default value
    is Qt::BevelJoin.

    \sa joinStyle(), {QPen#Join Style}{Join Style}
*/

void QPen::setJoinStyle(Qt::PenJoinStyle j)
{
    if (d->joinStyle == j)
        return;
    detach();
    d->joinStyle = j;
}

/*!
    \fn const QColor &QPen::color() const

    Returns the color of this pen's brush.

    \sa brush(), setColor()
*/
QColor QPen::color() const
{
    return d->brush.color();
}

/*!
    \fn void QPen::setColor(const QColor &color)

    Sets the color of this pen's brush to the given \a color.

    \sa setBrush(), color()
*/

void QPen::setColor(const QColor &c)
{
    detach();
    d->brush = QBrush(c);
}


/*!
    Returns the brush used to fill strokes generated with this pen.
*/
QBrush QPen::brush() const
{
    return d->brush;
}


/*!
    Sets the brush used to fill strokes generated with this pen to the given
    \a brush.

    \sa brush(), setColor()
*/
void QPen::setBrush(const QBrush &brush)
{
    detach();
    d->brush = brush;
}


/*!
    Returns true if the pen has a solid fill, otherwise false.

    \sa style(), dashPattern()
*/
bool QPen::isSolid() const
{
    return d->brush.style() == Qt::SolidPattern;
}


/*!
    Returns true if the pen is cosmetic; otherwise returns false.

    Cosmetic pens are used to draw strokes that have a constant width
    regardless of any transformations applied to the QPainter they are
    used with. Drawing a shape with a cosmetic pen ensures that its
    outline will have the same thickness at different scale factors.

    A zero width pen is cosmetic by default; pens with a non-zero width
    are non-cosmetic.

    \sa setCosmetic(), widthF()
*/

bool QPen::isCosmetic() const
{
    return (d->cosmetic == true) || d->width == 0;
}


/*!
    Sets this pen to cosmetic or non-cosmetic, depending on the value of
    \a cosmetic.

    \sa isCosmetic()
*/

void QPen::setCosmetic(bool cosmetic)
{
    detach();
    d->cosmetic = cosmetic;
}



/*!
    \fn bool QPen::operator!=(const QPen &pen) const

    Returns true if the pen is different from the given \a pen;
    otherwise false. Two pens are different if they have different
    styles, widths or colors.

    \sa operator==()
*/

/*!
    \fn bool QPen::operator==(const QPen &pen) const

    Returns true if the pen is equal to the given \a pen; otherwise
    false. Two pens are equal if they have equal styles, widths and
    colors.

    \sa operator!=()
*/

bool QPen::operator==(const QPen &p) const
{
    return (p.d == d) || (p.d->style == d->style
                          && p.d->capStyle == d->capStyle
                          && p.d->joinStyle == d->joinStyle
                          && p.d->width == d->width
                          && p.d->miterLimit == d->miterLimit
                          && (d->style != Qt::CustomDashLine
                              || (qFuzzyCompare(p.dashOffset(), dashOffset()) &&
                                  p.dashPattern() == dashPattern()))
                          && p.d->brush == d->brush
                          && p.d->cosmetic == d->cosmetic);
}


/*!
    \fn bool QPen::isDetached()

    \internal
*/

bool QPen::isDetached()
{
    return d->ref == 1;
}


/*****************************************************************************
  QPen stream functions
 *****************************************************************************/
#ifndef QT_NO_DATASTREAM
/*!
    \fn QDataStream &operator<<(QDataStream &stream, const QPen &pen)
    \relates QPen

    Writes the given \a pen to the given \a stream and returns a reference to
    the \a stream.

    \sa {Format of the QDataStream Operators}
*/

QDataStream &operator<<(QDataStream &s, const QPen &p)
{
    if (s.version() < 3) {
        s << (quint8)p.style();
    } else if (s.version() < QDataStream::Qt_4_3) {
        s << (quint8)(p.style() | p.capStyle() | p.joinStyle());
    } else {
        s << (quint16)(p.style() | p.capStyle() | p.joinStyle());
        s << (bool)(p.d->cosmetic);
    }

    if (s.version() < 7) {
        s << (quint8)p.width();
        s << p.color();
    } else {
        s << double(p.widthF());
        s << p.brush();
        s << double(p.miterLimit());
        if (sizeof(qreal) == sizeof(double)) {
            s << p.dashPattern();
        } else {
            // ensure that we write doubles here instead of streaming the pattern
            // directly; otherwise, platforms that redefine qreal might generate
            // data that cannot be read on other platforms.
            QVector<qreal> pattern = p.dashPattern();
            s << quint32(pattern.size());
            for (int i = 0; i < pattern.size(); ++i)
                s << double(pattern.at(i));
        }
        if (s.version() >= 9)
            s << double(p.dashOffset());
    }
    return s;
}

/*!
    \fn QDataStream &operator>>(QDataStream &stream, QPen &pen)
    \relates QPen

    Reads a pen from the given \a stream into the given \a pen and
    returns a reference to the \a stream.

    \sa {Format of the QDataStream Operators}
*/

QDataStream &operator>>(QDataStream &s, QPen &p)
{
    quint16 style;
    quint8 width8 = 0;
    double width = 0;
    QColor color;
    QBrush brush;
    double miterLimit = 2;
    QVector<qreal> dashPattern;
    double dashOffset = 0;
    bool cosmetic = false;
    if (s.version() < QDataStream::Qt_4_3) {
        quint8 style8;
        s >> style8;
        style = style8;
    } else {
        s >> style;
        s >> cosmetic;
    }
    if (s.version() < 7) {
        s >> width8;
        s >> color;
        brush = color;
        width = width8;
    } else {
        s >> width;
        s >> brush;
        s >> miterLimit;
        if (sizeof(qreal) == sizeof(double)) {
            s >> dashPattern;
        } else {
            quint32 numDashes;
            s >> numDashes;
            double dash;
            for (quint32 i = 0; i < numDashes; ++i) {
                s >> dash;
                dashPattern << dash;
            }
        }
        if (s.version() >= 9)
            s >> dashOffset;
    }

    p.detach();
    p.d->width = width;
    p.d->brush = brush;
    p.d->style = Qt::PenStyle(style & Qt::MPenStyle);
    p.d->capStyle = Qt::PenCapStyle(style & Qt::MPenCapStyle);
    p.d->joinStyle = Qt::PenJoinStyle(style & Qt::MPenJoinStyle);
    p.d->dashPattern = dashPattern;
    p.d->miterLimit = miterLimit;
    p.d->dashOffset = dashOffset;
    p.d->cosmetic = cosmetic;

    return s;
}
#endif //QT_NO_DATASTREAM

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QPen &p)
{
#ifndef Q_BROKEN_DEBUG_STREAM
    dbg.nospace() << "QPen(" << p.width() << ',' << p.brush()
                  << ',' << int(p.style()) << ',' << int(p.capStyle())
                  << ',' << int(p.joinStyle()) << ',' << p.dashPattern()
                  << "," << p.dashOffset()
                  << ',' << p.miterLimit() << ')';
    return dbg.space();
#else
    qWarning("This compiler doesn't support streaming QPen to QDebug");
    return dbg;
    Q_UNUSED(p);
#endif
}
#endif

/*!
    \fn DataPtr &QPen::data_ptr()
    \internal
*/

/*!
    \typedef QPen::DataPtr

    \internal
*/

QT_END_NAMESPACE
