/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtXMLPatterns module of the Qt Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef Patternist_Numeric_H
#define Patternist_Numeric_H

#include "qitem_p.h"
#include "qprimitives_p.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

/**
 * @file
 * @short Contains class Numeric. This file was originally called qnumeric_p.h,
 * but various build systems cannot handle that that name happens to be
 * identical to another one, the one in QtCore.
 */

namespace QPatternist
{
    /**
     * @short Base class for all numeric values.
     *
     * @section creation Creating Instances
     *
     * @todo
     * - Depending on what type of val
     * - Numeric::createFromString
     * - Various classes has ::Zero(), ::PosINF(), ::NaN(), NegINF()
     * - Never use constructor, use createFromNative, or createFromString.
     *
     * @see <a href="http://www.w3.org/TR/xquery-operators/#numeric-functions">XQuery 1.0
     * and XPath 2.0 Functions and Operators, 6 Functions and Operators on Numerics</a>
     * @see <a href="http://www.w3.org/TR/xquery-operators/#func-overloading">XQuery 1.0
     * and XPath 2.0 Functions and Operators, 1.2 Function Overloading</a>
     * @author Frans Englich <fenglich@trolltech.com>
     * @ingroup Patternist_xdm
     * @todo discuss data hierarchy the non existatnt number data type
     */
    class Numeric : public AtomicValue
    {
    public:

        typedef QExplicitlySharedDataPointer<Numeric> Ptr;

        /**
         * Creates a Numeric sub-class that is appropriate for @p number.
         *
         * @note usages of e/E is not handled; Double::fromLexical should
         * be used in that case. There is no function similar to fromLexical that also
         * takes double values into account(because that distinction is done in the scanner).
         *
         * Currently used in the parser to create appropriate expressions.
         */
        static AtomicValue::Ptr fromLexical(const QString &number);

        /**
         * @returns the particular number's value as a native representation of
         * the type xs:double. This can be considered that the value is cast to
         * xs:double.
         */
        virtual xsDouble toDouble() const = 0;

        /**
         * @returns the particular number's value as a native representation of
         * the type xs:integer. This can be considered that the value is cast to
         * xs:integer.
         */
        virtual xsInteger toInteger() const = 0;

        /**
         * @returns the number as an unsigned integer. If the value is not
         * unsigned, the code asserts and behavior is undefined.
         */
        virtual qulonglong toUnsignedInteger() const = 0;

        /**
         * @returns the particular number's value as a native representation of
         * the type xs:float. This can be considered that the value is cast to
         * xs:float.
         */
        virtual xsFloat toFloat() const = 0;

        /**
         * @returns the particular number's value as a native representation of
         * the type xs:decimal. This can be considered that the value is cast to
         * xs:decimal.
         */
        virtual xsFloat toDecimal() const = 0;

        /**
         * Performs the algorithm specified for the function fn:round on this Numeric,
         * and whose result is returned.
         *
         * @see <a href="http://www.w3.org/TR/xpath-functions/#func-round">XQuery 1.0
         * and XPath 2.0 Functions and Operators, 6.4.4 fn:round</a>
         */
        virtual Numeric::Ptr round() const = 0;

        /**
         * Performs rounding as defined for the fn:round-half-to-even  on this Numeric,
         * and whose result is returned.
         *
         * @see <a href="http://www.w3.org/TR/xpath-functions/#func-round-half-to-even">XQuery 1.0
         * and XPath 2.0 Functions and Operators, 6.4.5 fn:round-half-to-even</a>
         */
        virtual Numeric::Ptr roundHalfToEven(const xsInteger scale) const = 0;

        /**
         * Performs the algorithm specified for the function fn:floor on this Numeric,
         * and whose result is returned.
         *
         * @see <a href="http://www.w3.org/TR/xpath-functions/#func-floor">XQuery 1.0
         * and XPath 2.0 Functions and Operators, 6.4.3 fn:floor</a>
         */
        virtual Numeric::Ptr floor() const = 0;

        /**
         * Performs the algorithm specified for the function fn:ceiling on this Numeric,
         * and whose result is returned.
         *
         * @see <a href="http://www.w3.org/TR/xpath-functions/#func-ceiling">XQuery 1.0
         * and XPath 2.0 Functions and Operators, 6.4.2 fn:ceiling</a>
         */
        virtual Numeric::Ptr ceiling() const = 0;

        /**
         * Performs the algorithm specified for the function fn:abs on this Numeric,
         * and whose result is returned.
         *
         * @see <a href="http://www.w3.org/TR/xpath-functions/#func-ceiling">XQuery 1.0
         * and XPath 2.0 Functions and Operators, 6.4.1 fn:abs</a>
         */
        virtual Numeric::Ptr abs() const = 0;

        /**
         * Determines whether this Numeric is not-a-number, @c NaN. For numeric types
         * that cannot represent @c NaN, this function should return @c false.
         *
         * @returns @c true if this Numeric is @c NaN
         */
        virtual bool isNaN() const = 0;

        /**
         * Determines whether this Numeric is an infinite number. Signedness
         * is irrelevant, -INF as well as INF is considered infinity.
         *
         * For numeric types that cannot represent infinity, such as xs:integer
         * , this function should return @c false.
         *
         * @returns @c true if this Numeric is an infinite number
         */
        virtual bool isInf() const = 0;

        /**
         * Unary minus.
         */
        virtual Item toNegated() const = 0;

        /**
         * @short Returns @c true if this value is signed. If @c false is
         * returned, the value is unsigned.
         *
         * For float and decimal values, @c xs:double, @c xs:float and @c
         * xs:decimal, the code asserts and behavior is undefined.
         */
        virtual bool isSigned() const = 0;

    protected:
        /**
         * @short Implements @c fn:round() for types implemented with floating
         * point.
         *
         * MS Windows and at least IRIX does not have C99's nearbyint() function(see the man
         * page), so we reinvent it.
         */
        static xsDouble roundFloat(const xsDouble val);
    };
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
