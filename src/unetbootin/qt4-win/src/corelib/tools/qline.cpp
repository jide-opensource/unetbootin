/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the QtCore module of the Qt Toolkit.
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

#include "qline.h"
#include "qdebug.h"
#include "qdatastream.h"
#include <private/qnumeric_p.h>

#include <math.h>

QT_BEGIN_NAMESPACE

/*!
    \class QLine
    \ingroup multimedia

    \brief The QLine class provides a two-dimensional vector using
    integer precision.

    A QLine describes a finite length line (or a line segment) on a
    two-dimensional surface. The start and end points of the line are
    specified using integer point accuracy for coordinates. Use the
    QLineF constructor to retrieve a floating point copy.

    \table
    \row
        \o \inlineimage qline-point.png
        \o \inlineimage qline-coordinates.png
    \endtable

    The positions of the line's start and end points can be retrieved
    using the p1(), x1(), y1(), p2(), x2(), and y2() functions. The
    dx() and dy() functions return the horizontal and vertical
    components of the line. Use isNull() to determine whether the
    QLine represents a valid line or a null line.

    Finally, the line can be translated a given offset using the
    translate() function.

    \sa QLineF, QPolygon, QRect
*/

/*!
    \fn QLine::QLine()

    Constructs a null line.
*/

/*!
    \fn QLine::QLine(const QPoint &p1, const QPoint &p2)

    Constructs a line object that represents the line between \a p1 and
    \a p2.
*/

/*!
    \fn QLine::QLine(int x1, int y1, int x2, int y2)

    Constructs a line object that represents the line between (\a x1, \a y1) and
    (\a x2, \a y2).
*/

/*!
    \fn bool QLine::isNull() const

    Returns true if the line is not set up with valid start and end point;
    otherwise returns false.
*/

/*!
    \fn QPoint QLine::p1() const

    Returns the line's start point.

    \sa x1(), y1(), p2()
*/

/*!
    \fn QPoint QLine::p2() const

    Returns the line's end point.

    \sa x2(), y2(), p1()
*/

/*!
    \fn int QLine::x1() const

    Returns the x-coordinate of the line's start point.

    \sa p1()
*/

/*!
    \fn int QLine::y1() const

    Returns the y-coordinate of the line's start point.

    \sa p1()
*/

/*!
    \fn int QLine::x2() const

    Returns the x-coordinate of the line's end point.

    \sa p2()
*/

/*!
    \fn int QLine::y2() const

    Returns the y-coordinate of the line's end point.

    \sa p2()
*/

/*!
    \fn int QLine::dx() const

    Returns the horizontal component of the line's vector.

    \sa dy()
*/

/*!
    \fn int QLine::dy() const

    Returns the vertical component of the line's vector.

    \sa dx()
*/

/*!
    \fn bool QLine::operator!=(const QLine &line) const

    Returns true if the given \a line is not the same as \e this line.

    A line is different from another line if any of their start or
    end points differ, or the internal order of the points is different.
*/

/*!
    \fn bool QLine::operator==(const QLine &line) const

    Returns true if the given \a line is the same as \e this line.

    A line is identical to another line if the start and end points
    are identical, and the internal order of the points is the same.
*/

/*!
    \fn void QLine::translate(const QPoint &offset)

    Translates this line by the given \a offset.
*/

/*!
    \fn void QLine::translate(int dx, int dy)
    \overload

    Translates this line the distance specified by \a dx and \a dy.
*/

/*!
    \fn QLine QLine::translated(const QPoint &offset) const

    \since 4.4

    Returns this line translated by the given \a offset.
*/

/*!
    \fn QLine QLine::translated(int dx, int dy) const
    \overload
    \since 4.4

    Returns this line translated the distance specified by \a dx and \a dy.
*/


/*!
    \fn void QLine::setP1(const QPoint &p1)

    Sets the starting point of this line to \a p1.

    \sa setP2(), p1()
*/


/*!
    \fn void QLine::setP2(const QPoint &p2)

    Sets the end point of this line to \a p2.

    \sa setP1(), p2()
*/


/*!
    \fn void QLine::setPoints(const QPoint &p1, const QPoint &p2)

    Sets the start point of this line to \a p1 and the end point of this line to \a p2.

    \sa setP1(), setP2(), p1(), p2()
*/


/*!
    \fn void QLine::setLine(int x1, int y1, int x2, int y2)

    Sets this line to the start in \a x1, \a y1 and end in \a x2, \a y2.

    \sa setP1(), setP2(), p1(), p2()
*/



#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug d, const QLine &p)
{
    d << "QLine(" << p.p1() << "," << p.p2() << ")";
    return d;
}
#endif

#ifndef QT_NO_DATASTREAM
/*!
    \relates QLine

    Writes the given \a line to the given \a stream and returns a
    reference to the stream.

    \sa {Format of the QDataStream Operators}
*/

QDataStream &operator<<(QDataStream &stream, const QLine &line)
{
    stream << line.p1() << line.p2();
    return stream;
}

/*!
    \relates QLine

    Reads a line from the given \a stream into the given \a line and
    returns a reference to the stream.

    \sa {Format of the QDataStream Operators}
*/

QDataStream &operator>>(QDataStream &stream, QLine &line)
{
    QPoint p1, p2;
    stream >> p1;
    stream >> p2;
    line = QLine(p1, p2);

    return stream;
}

#endif // QT_NO_DATASTREAM


#ifndef M_2PI
#define M_2PI 6.28318530717958647692528676655900576
#endif

/*!
    \class QLineF
    \ingroup multimedia

    \brief The QLineF class provides a two-dimensional vector using
    floating point precision.

    A QLineF describes a finite length line (or line segment) on a
    two-dimensional surface. QLineF defines the start and end points
    of the line using floating point accuracy for coordinates.  Use
    the toLine() function to retrieve an integer based copy of this
    line.

    \table
    \row
        \o \inlineimage qline-point.png
        \o \inlineimage qline-coordinates.png
    \endtable

    The positions of the line's start and end points can be retrieved
    using the p1(), x1(), y1(), p2(), x2(), and y2() functions. The
    dx() and dy() functions return the horizontal and vertical
    components of the line, respectively.

    The line's length can be retrieved using the length() function,
    and altered using the setLength() function.  Similarly, angle()
    and setAngle() are respectively used for retrieving and altering
    the angle of the line. Use the isNull()
    function to determine whether the QLineF represents a valid line
    or a null line.

    The intersect() function determines the IntersectType for this
    line and a given line, while the angle() function returns the
    angle between the lines. In addition, the unitVector() function
    returns a line that has the same starting point as this line, but
    with a length of only 1, while the normalVector() function returns
    a line that is perpendicular to this line with the same starting
    point and length.

    Finally, the line can be translated a given offset using the
    translate() function, and can be traversed using the pointAt()
    function.

    \sa QLine, QPolygonF, QRectF
*/

/*!
    \enum QLineF::IntersectType

    Describes the intersection between two lines.

    \table
    \row
    \o \inlineimage qlinef-unbounded.png
    \o \inlineimage qlinef-bounded.png
    \row
    \o QLineF::UnboundedIntersection
    \o QLineF::BoundedIntersection
    \endtable

    \value NoIntersection Indicates that the lines do not intersect;
    i.e. they are parallel.

    \value UnboundedIntersection The two lines intersect,
    but not within the range defined by their lengths. This will be
    the case if the lines are not parallel.

    \value BoundedIntersection The two lines intersect with each other
    within the start and end points of each line.

    \sa intersect()
*/

/*!
    \fn QLineF::QLineF()

    Constructs a null line.
*/

/*!
    \fn QLineF::QLineF(const QPointF &p1, const QPointF &p2)

    Constructs a line object that represents the line between \a p1 and
    \a p2.
*/

/*!
    \fn QLineF::QLineF(qreal x1, qreal y1, qreal x2, qreal y2)

    Constructs a line object that represents the line between (\a x1, \a y1) and
    (\a x2, \a y2).
*/

/*!
    \fn QLineF::QLineF(const QLine &line)

    Construct a QLineF object from the given integer-based \a line.

    \sa toLine()
*/

/*!
    Returns true if the line is not set up with valid start and end point;
    otherwise returns false.
*/

bool QLineF::isNull() const
{
    return (qFuzzyCompare(pt1.x(), pt2.x()) && qFuzzyCompare(pt1.y(), pt2.y())) ? true : false;
}


/*!
    \fn QPointF QLineF::p1() const

    Returns the line's start point.

    \sa x1(), y1(), p2()
*/

/*!
    \fn QPointF QLineF::p2() const

    Returns the line's end point.

    \sa x2(), y2(), p1()
*/

/*!
    \fn QLine QLineF::toLine() const

    Returns an integer based copy of this line.

    Note that the returned line's start and end points are rounded to
    the nearest integer.

    \sa QLineF()
*/
/*!
    \fn qreal QLineF::x1() const

    Returns the x-coordinate of the line's start point.

    \sa p1()
*/

/*!
    \fn qreal QLineF::y1() const

    Returns the y-coordinate of the line's start point.

    \sa p1()
*/

/*!
    \fn qreal QLineF::x2() const

    Returns the x-coordinate of the line's end point.

    \sa p2()
*/

/*!
    \fn qreal QLineF::y2() const

    Returns the y-coordinate of the line's end point.

    \sa p2()
*/

/*!
    \fn qreal QLineF::dx() const

    Returns the horizontal component of the line's vector.

    \sa dy(), pointAt()
*/

/*!
    \fn qreal QLineF::dy() const

    Returns the vertical component of the line's vector.

    \sa dx(), pointAt()
*/

/*!
    \fn QLineF::setLength(qreal length)

    Sets the length of the line to the given \a length. If the line is a
    null line, the length will remain zero regardless of the length
    specified.

    \sa length(), isNull()
*/

/*!
    \fn QLineF QLineF::normalVector() const

    Returns a line that is perpendicular to this line with the same starting
    point and length.

    \image qlinef-normalvector.png

    \sa unitVector()
*/

/*!
    \fn bool QLineF::operator!=(const QLineF &line) const

    Returns true if the given \a line is not the same as \e this line.

    A line is different from another line if their start or end points
    differ, or the internal order of the points is different.
*/

/*!
    \fn bool QLineF::operator==(const QLineF &line) const

    Returns true if the given \a line is the same as this line.

    A line is identical to another line if the start and end points
    are identical, and the internal order of the points is the same.
*/

/*!
  \fn qreal QLineF::pointAt(qreal t) const

  Returns the point at the parameterized position specified by \a
  t. The function returns the line's start point if t = 0, and its end
  point if t = 1.

  \sa dx(), dy()
*/

/*!
    Returns the length of the line.

    \sa setLength()
*/
qreal QLineF::length() const
{
    qreal x = pt2.x() - pt1.x();
    qreal y = pt2.y() - pt1.y();
    return sqrt(x*x + y*y);
}

/*!
    \since 4.4

    Returns the angle of the line in degrees.

    Positive values for the angles mean counter-clockwise while negative values
    mean the clockwise direction. Zero degrees is at the 3 o'clock position.

    \sa setAngle()
*/
qreal QLineF::angle() const
{
    const qreal dx = pt2.x() - pt1.x();
    const qreal dy = pt2.y() - pt1.y();

    const qreal theta = atan2(-dy, dx) * 360.0 / M_2PI;

    const qreal theta_normalized = theta < 0 ? theta + 360 : theta;

    if (qFuzzyCompare(theta_normalized, qreal(360)))
        return qreal(0);
    else
        return theta_normalized;
}

/*!
    \since 4.4

    Sets the angle of the line to the given \a angle (in degrees).
    This will change the position of the second point of the line such that
    the line has the given angle.

    Positive values for the angles mean counter-clockwise while negative values
    mean the clockwise direction. Zero degrees is at the 3 o'clock position.

    \sa angle()
*/
void QLineF::setAngle(qreal angle)
{
    const qreal angleR = angle * M_2PI / 360.0;
    const qreal l = length();

    const qreal dx = cos(angleR) * l;
    const qreal dy = -sin(angleR) * l;

    pt2.rx() = pt1.x() + dx;
    pt2.ry() = pt1.y() + dy;
}

/*!
    \since 4.4

    Returns a QLineF with the given \a length and \a angle.

    The first point of the line will be on the origin.

    Positive values for the angles mean counter-clockwise while negative values
    mean the clockwise direction. Zero degrees is at the 3 o'clock position.
*/
QLineF QLineF::fromPolar(qreal length, qreal angle)
{
    const qreal angleR = angle * M_2PI / 360.0;
    return QLineF(0, 0, cos(angleR) * length, -sin(angleR) * length);
}

/*!
    Returns the unit vector for this line, i.e a line starting at the
    same point as \e this line with a length of 1.0.

    \sa normalVector()
*/
QLineF QLineF::unitVector() const
{
    qreal x = pt2.x() - pt1.x();
    qreal y = pt2.y() - pt1.y();

    qreal len = sqrt(x*x + y*y);
    QLineF f(p1(), QPointF(pt1.x() + x/len, pt1.y() + y/len));

#ifndef QT_NO_DEBUG
    if (qAbs(f.length() - 1) >= 0.001)
        qWarning("QLine::unitVector: New line does not have unit length");
#endif

    return f;
}

#define SAME_SIGNS(a, b) ((a) * (b) >= 0)

// Line intersection algorithm, copied from Graphics Gems II
static bool qt_linef_intersect(qreal x1, qreal y1, qreal x2, qreal y2,
                               qreal x3, qreal y3, qreal x4, qreal y4)
{
    qreal a1, a2, b1, b2, c1, c2; /* Coefficients of line eqns. */
    qreal r1, r2, r3, r4;         /* 'Sign' values */

    a1 = y2 - y1;
    b1 = x1 - x2;
    c1 = x2 * y1 - x1 * y2;

    r3 = a1 * x3 + b1 * y3 + c1;
    r4 = a1 * x4 + b1 * y4 + c1;

    if ( r3 != 0 && r4 != 0 && SAME_SIGNS( r3, r4 ))
        return false;

    a2 = y4 - y3;
    b2 = x3 - x4;
    c2 = x4 * y3 - x3 * y4;

    r1 = a2 * x1 + b2 * y1 + c2;
    r2 = a2 * x2 + b2 * y2 + c2;

    if ( r1 != 0 && r2 != 0 && SAME_SIGNS( r1, r2 ))
        return false;

    return true;
}

/*!
    \fn QLineF::IntersectType QLineF::intersect(const QLineF &line, QPointF *intersectionPoint) const

    Returns a value indicating whether or not \e this line intersects
    with the given \a line.

    The actual intersection point is extracted to \a intersectionPoint
    (if the pointer is valid). If the lines are parallel, the
    intersection point is undefined.
*/

QLineF::IntersectType QLineF::intersect(const QLineF &l, QPointF *intersectionPoint) const
{
    if (isNull() || l.isNull()
        || !qt_is_finite(pt1.x()) || !qt_is_finite(pt1.y()) || !qt_is_finite(pt2.x()) || !qt_is_finite(pt2.y())
        || !qt_is_finite(l.pt1.x()) || !qt_is_finite(l.pt1.y()) || !qt_is_finite(l.pt2.x()) || !qt_is_finite(l.pt2.y()))
        return NoIntersection;

    QPointF isect;
    IntersectType type = qt_linef_intersect(pt1.x(), pt1.y(), pt2.x(), pt2.y(),
                                            l.x1(), l.y1(), l.x2(), l.y2())
                         ? BoundedIntersection : UnboundedIntersection;

    bool dx_zero = qFuzzyCompare(dx() + 1, 1);
    bool ldx_zero = qFuzzyCompare(l.dx() + 1, 1);

    // For special case where one of the lines are vertical
    if (dx_zero && ldx_zero) {
        type = NoIntersection;
    } else if (dx_zero) {
        qreal la = l.dy() / l.dx();
        isect = QPointF(pt1.x(), la * pt1.x() + l.y1() - la*l.x1());
    } else if (ldx_zero) {
        qreal ta = dy() / dx();
        isect = QPointF(l.x1(), ta * l.x1() + y1() - ta*x1());
    } else {
        qreal ta = dy()/dx();
        qreal la = l.dy()/l.dx();
        if (ta == la) // no intersection
            return NoIntersection;

        qreal x = ( - l.y1() + la * l.x1() + pt1.y() - ta * pt1.x() ) / (la - ta);
        isect = QPointF(x, ta*(x - pt1.x()) + pt1.y());
    }
    if (intersectionPoint)
        *intersectionPoint = isect;
    return type;
}

/*!
    \fn void QLineF::translate(const QPointF &offset)

    Translates this line by the given \a offset.
*/

/*!
    \fn void QLineF::translate(qreal dx, qreal dy)
    \overload

    Translates this line the distance specified by \a dx and \a dy.
*/

/*!
    \fn QLineF QLineF::translated(const QPointF &offset) const

    \since 4.4

    Returns this line translated by the given \a offset.
*/

/*!
    \fn QLineF QLineF::translated(qreal dx, qreal dy) const
    \overload
    \since 4.4

    Returns this line translated the distance specified by \a dx and \a dy.
*/

/*!
    \fn void QLineF::setP1(const QPointF &p1)

    Sets the starting point of this line to \a p1.

    \sa setP2(), p1()
*/


/*!
    \fn void QLineF::setP2(const QPointF &p2)

    Sets the end point of this line to \a p2.

    \sa setP1(), p2()
*/


/*!
    \fn void QLineF::setPoints(const QPointF &p1, const QPointF &p2)

    Sets the start point of this line to \a p1 and the end point of this line to \a p2.

    \sa setP1(), setP2(), p1(), p2()
*/


/*!
    \fn void QLineF::setLine(qreal x1, qreal y1, qreal x2, qreal y2)

    Sets this line to the start in \a x1, \a y1 and end in \a x2, \a y2.

    \sa setP1(), setP2(), p1(), p2()
*/

/*!
  \fn qreal QLineF::angleTo(const QLineF &line) const

  \since 4.4

  Returns the angle (in degrees) from this line to the given \a
  line, taking the direction of the lines into account. If the lines
  do not intersect within their range, it is the intersection point of
  the extended lines that serves as origin (see
  QLineF::UnboundedIntersection).

  The returned value represents the number of degrees you need to add
  to this line to make it have the same angle as the given \a line,
  going counter-clockwise.

  \sa intersect()
*/
qreal QLineF::angleTo(const QLineF &l) const
{
    if (isNull() || l.isNull())
        return 0;

    const qreal a1 = angle();
    const qreal a2 = l.angle();

    const qreal delta = a2 - a1;
    const qreal delta_normalized = delta < 0 ? delta + 360 : delta;

    if (qFuzzyCompare(delta, qreal(360)))
        return 0;
    else
        return delta_normalized;
}

/*!
  \fn qreal QLineF::angle(const QLineF &line) const

  \obsolete

  Returns the angle (in degrees) between this line and the given \a
  line, taking the direction of the lines into account. If the lines
  do not intersect within their range, it is the intersection point of
  the extended lines that serves as origin (see
  QLineF::UnboundedIntersection).

  \table
  \row
  \o \inlineimage qlinef-angle-identicaldirection.png
  \o \inlineimage qlinef-angle-oppositedirection.png
  \endtable

  When the lines are parallel, this function returns 0 if they have
  the same direction; otherwise it returns 180.

  \sa intersect()
*/
qreal QLineF::angle(const QLineF &l) const
{
    if (isNull() || l.isNull())
        return 0;
    qreal cos_line = (dx()*l.dx() + dy()*l.dy()) / (length()*l.length());
    qreal rad = 0;
    // only accept cos_line in the range [-1,1], if it is outside, use 0 (we return 0 rather than PI for those cases)
    if (cos_line >= -1.0 && cos_line <= 1.0) rad = acos( cos_line );
    return rad * 360 / M_2PI;
}


#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug d, const QLineF &p)
{
    d << "QLineF(" << p.p1() << "," << p.p2() << ")";
    return d;
}
#endif

#ifndef QT_NO_DATASTREAM
/*!
    \relates QLineF

    Writes the given \a line to the given \a stream and returns a
    reference to the stream.

    \sa {Format of the QDataStream Operators}
*/

QDataStream &operator<<(QDataStream &stream, const QLineF &line)
{
    stream << line.p1() << line.p2();
    return stream;
}

/*!
    \relates QLineF

    Reads a line from the given \a stream into the given \a line and
    returns a reference to the stream.

    \sa {Format of the QDataStream Operators}
*/

QDataStream &operator>>(QDataStream &stream, QLineF &line)
{
    QPointF start, end;
    stream >> start;
    stream >> end;
    line = QLineF(start, end);

    return stream;
}

#endif // QT_NO_DATASTREAM

QT_END_NAMESPACE
