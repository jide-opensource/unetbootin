/****************************************************************************
**
** Copyright (C) 2006-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the QtXMLPatterns module of the Qt Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef Patternist_TypeChecker_H
#define Patternist_TypeChecker_H

#include "qstaticcontext_p.h"
#include "qexpression_p.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    /**
     * @short Contains functions that applies Function Conversion Rules and other
     * kinds of compile-time type checking tasks.
     *
     * @ingroup Patternist_types
     * @author Frans Englich <fenglich@trolltech.com>
     */
    class TypeChecker
    {
    public:
        enum Option
        {
            /**
             * @short When set, the function conversion rules are applied.
             *
             * For instance, this is type promotion and conversions from @c
             * xs:untypedAtomic to @c xs:date. This is done for function calls,
             * but not when binding an expression to a variable.
             */
            AutomaticallyConvert = 1,

            /**
             * @short Whether the focus should be checked or not.
             *
             * Sometimes the focus is unknown at the time
             * applyFunctionConversion() is called, and therefore it is
             * of interest to post pone the check to later on.
             */
            CheckFocus = 2,

            /**
             * When applyFunctionConversion() is passed AutomaticallyConvert
             * and promotion is required, such as from @c xs:integer to
             * @c xs:float, there will be no conversion performed, with the
             * assumption that the receiver will call Numeric::toFloat() or
             * similar.
             *
             * However, when GeneratePromotion is set, code will be generated
             * that performs this conversion regardless of what any receiver
             * do.
             *
             * This is useful in the case where one Expression only pipes the
             * result of another. The only known case of that as of this
             * writing is when UserFunctionCallsite evaluates its body.
             */
            GeneratePromotion
        };
        typedef QFlags<Option> Options;

        /**
         * @short Builds a pipeline of artificial AST nodes that ensures @p operand
         * conforms to the type @p reqType by applying the Function
         * Conversion Rules.
         *
         * This new Expression is returned, or, if no conversions were necessary,
         * @p operand as it is.
         *
         * applyFunctionConversion() also performs various checks, such as if
         * @p operand needs the focus and that the focus is defined in the
         * @p context. These checks are largely guided by @p operand's
         * Expression::properties().
         *
         * @see <a href="http://www.w3.org/TR/xpath20/\#id-function-calls">XML Path
         * Language (XPath) 2.0, 3.1.5 Function Calls</a>, more specifically the
         * Function Conversion Rules
         */
        static Expression::Ptr
        applyFunctionConversion(const Expression::Ptr &operand,
                                const SequenceType::Ptr &reqType,
                                const StaticContext::Ptr &context,
                                const ReportContext::ErrorCode code = ReportContext::XPTY0004,
                                const Options = Options(AutomaticallyConvert | CheckFocus));
    private:

        static inline Expression::Ptr typeCheck(Expression *const op,
                                                const StaticContext::Ptr &context,
                                                const SequenceType::Ptr &reqType);
        /**
         * @short Implements the type checking and promotion part of the Function Conversion Rules.
         */
        static Expression::Ptr verifyType(const Expression::Ptr &operand,
                                          const SequenceType::Ptr &reqSeqType,
                                          const StaticContext::Ptr &context,
                                          const ReportContext::ErrorCode code,
                                          const Options options);

        /**
         * Determines whether type promotion is possible from one type to another. False
         * is returned when a promotion is not possible or if a promotion is not needed(as when
         * the types are identical), since that can be considered to not be type promotion.
         *
         * @returns @c true if @p fromType can be promoted to @p toType.
         * @see <a href="http://www.w3.org/TR/xpath20/#promotion">XML Path Language
         * (XPath) 2.0, B.1 Type Promotion</a>
         */
        static bool promotionPossible(const ItemType::Ptr &fromType,
                                      const ItemType::Ptr &toType,
                                      const StaticContext::Ptr &context);

        /**
         * @short Centralizes a message-string to reduce work for translators
         * and increase consistency.
         */
        static inline QString wrongType(const NamePool::Ptr &np,
                                        const ItemType::Ptr &reqType,
                                        const ItemType::Ptr &opType);

        /**
         * No implementation is provided for this constructor. This class
         * is not supposed to be instantiated.
         */
        inline TypeChecker();

        Q_DISABLE_COPY(TypeChecker)
    };
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
