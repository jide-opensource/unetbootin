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

#ifndef Patternist_AbstractFloat_H
#define Patternist_AbstractFloat_H

#include <math.h>

#include <qnumeric.h>

#include "qcommonvalues_p.h"
#include "qdecimal_p.h"
#include "qschemanumeric_p.h"
#include "qvalidationerror_p.h"
#include "qbuiltintypes_p.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    /**
     * @short Base template class for Float and Double classes.
     *
     * @author Vincent Ricard <magic@magicninja.org>
     * @ingroup Patternist_xdm
     */
    template <const bool isDouble>
    class AbstractFloat : public Numeric
    {
    public:
        static Numeric::Ptr fromValue(const xsDouble num);
        static AtomicValue::Ptr fromLexical(const QString &strNumeric);

        /**
         * @todo more extensive docs.
         *
         * Performs floating point comparison.
         *
         * @returns @c true if @p a and @p are equal, otherwise @c false.
         */
        static bool isEqual(const xsDouble a, const xsDouble b);

        /**
         * Determines the Effective %Boolean Value of this number.
         *
         * @returns @c false if the number is 0 or @c NaN, otherwise @c true.
         */
        bool evaluateEBV(const QExplicitlySharedDataPointer<DynamicContext> &) const;

        /**
         * Returns this AbstractFloat represented as an @c xs:string.
         *
         * @note In the XPath/XQuery languages, converting @c xs:double and @c xs:float
         * to @c xs:string is not specified in XML Schema 1.0 Part 2: Datatypes Second Edition,
         * but in XQuery 1.0 and XPath 2.0 Functions and Operators. This will change with W3C XML
         * Schema 1.1
         *
         * @see <a href="http://www.w3.org/TR/xpath-functions/#casting-to-string">XQuery 1.0
         * and XPath 2.0 Functions and Operators, 17.1.2 Casting to xs:string and xdt:untypedAtomic</a>
         */
        virtual QString stringValue() const;

        virtual xsDouble toDouble() const;
        virtual xsInteger toInteger() const;
        virtual xsFloat toFloat() const;
        virtual xsDecimal toDecimal() const;

        virtual Numeric::Ptr round() const;
        virtual Numeric::Ptr roundHalfToEven(const xsInteger scale) const;
        virtual Numeric::Ptr floor() const;
        virtual Numeric::Ptr ceiling() const;
        virtual Numeric::Ptr abs() const;

        virtual bool isNaN() const;
        virtual bool isInf() const;

        virtual ItemType::Ptr type() const;
        virtual Item toNegated() const;
        virtual qulonglong toUnsignedInteger() const;

        virtual bool isSigned() const;
    protected:
        AbstractFloat(const xsDouble num);

    private:
        /**
         * From the Open Group's man page: "The signbit() macro shall return a
         * non-zero value if and only if the sign of its argument value is
         * negative."
         *
         * MS Windows doesn't have std::signbit() so here's
         * a reinvention of that function.
         */
        static inline int internalSignbit(const xsDouble v);
        inline bool isZero() const;

        const xsDouble m_value;
    };

    template <const bool isDouble>
    Numeric::Ptr createFloat(const xsDouble num);

#include "qabstractfloat.cpp"

    /**
     * @short An instantiation of AbsbstractFloat suitable for @c xs:double.
     *
     * @ingroup Patternist_xdm
     */
    typedef AbstractFloat<true> Double;

    /**
     * @short An instantiation of AbstractFloat suitable for @c xs:float.
     *
     * @ingroup Patternist_xdm
     */
    typedef AbstractFloat<false> Float;
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
