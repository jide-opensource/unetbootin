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

#ifndef Patternist_ApplyTemplate_H
#define Patternist_ApplyTemplate_H

#include "qtemplatemode_p.h"

QT_BEGIN_HEADER
QT_BEGIN_NAMESPACE

namespace QPatternist
{
    /**
     * @short When combined with other components, implements
     * @c xsl:apply-templates.
     *
     * Note that ApplyTemplate isn't named ApplyTemplates. The reason for this
     * is that ApplyTemplate doesn't do the iteration part. An @c
     * <xsl:apply-templates/> instruction is rewritten into:
     *
     * @code
     * child::node/() map apply-template()
     * @endcode
     *
     * Hence, this expression requires a focus, although it can consist of
     * atomic values.
     *
     * @since 4.5
     * @author Frans Englich <fenglich@trolltech.com>
     * @ingroup Patternist_expressions
     */
    class ApplyTemplate : public TemplateInvoker
    {
    public:
        typedef QExplicitlySharedDataPointer<ApplyTemplate> Ptr;

        /**
         * @short @p mode may be @c null. If it is, ApplyTemplate interprets
         * that as that it should use the #current mode.
         *
         * @see StaticContext::currentTemplateMode()
         */
        ApplyTemplate(const TemplateMode::Ptr &mode,
                      const WithParam::Hash &withParams,
                      const TemplateMode::Ptr &defaultMode);

        virtual Item::Iterator::Ptr evaluateSequence(const DynamicContext::Ptr &context) const;

        virtual SequenceType::Ptr staticType() const;
        virtual ExpressionVisitorResult::Ptr accept(const ExpressionVisitor::Ptr &visitor) const;
        virtual Properties properties() const;

        /**
         * The only reason this function exists, is in order to convert
         * QXmlNodeModelIndex to QPatternist::Item. So, this is a huge
         * performance setback. It applies for one of the builtin templates.
         */
        inline Item mapToItem(const QXmlNodeModelIndex &node,
                              const DynamicContext::Ptr &context) const;
        inline Item::Iterator::Ptr mapToSequence(const Item &item,
                                                 const DynamicContext::Ptr &context) const;

        inline TemplateMode::Ptr mode() const;

        virtual bool configureRecursion(const CallTargetDescription::Ptr &sign);
        virtual Expression::Ptr body() const;
        virtual CallTargetDescription::Ptr callTargetDescription() const;

        Expression::Ptr compress(const StaticContext::Ptr &context);

    private:
        typedef QExplicitlySharedDataPointer<const ApplyTemplate> ConstPtr;

        Template::Ptr findTemplate(const DynamicContext::Ptr &context,
                                   const TemplateMode::Ptr &templateMode) const;
        /**
         * @note You typically want to use effectiveMode().
         */
        const TemplateMode::Ptr m_mode;

        TemplateMode::Ptr m_defaultMode;

        inline TemplateMode::Ptr effectiveMode(const DynamicContext::Ptr &context) const;
    };

    TemplateMode::Ptr ApplyTemplate::mode() const
    {
        return m_mode;
    }
}

QT_END_NAMESPACE
QT_END_HEADER

#endif
