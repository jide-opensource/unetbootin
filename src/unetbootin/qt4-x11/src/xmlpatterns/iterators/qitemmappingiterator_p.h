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

#ifndef Patternist_ItemMappingIterator_H
#define Patternist_ItemMappingIterator_H

#include "qabstractxmlforwarditerator_p.h"
#include "qdynamiccontext_p.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    /**
     * @short Proxies another QAbstractXmlForwardIterator, and for each item, returns the
     * Item returned from a mapping function.
     *
     * ItemMappingIterator is practical when the items in an QAbstractXmlForwardIterator needs to
     * be translated to another sequence, while still doing it in a pipe-lined
     * fashion.
     *
     * This is achieved by that ItemMappingIterator's constructor takes
     * an instance of a class, that must have the following member:
     *
     * @code
     * TResult::Ptr mapToItem(const TSource &item,
     *                        const Context &context) const
     * @endcode
     *
     * For each item in the QAbstractXmlForwardIterator ItemMappingIterator proxies, this function is
     * called and its return value becomes the return value of the ItemMappingIterator. If the
     * mapping function returns null, ItemMappingIterator maps the next item in the source sequence
     * such that a contiguous sequence of items is returned.
     *
     * Declaring the mapToItem() function as inline, can be a good way to improve performance.
     *
     * @see SequenceMappingIterator
     * @author Frans Englich <fenglich@trolltech.com>
     * @ingroup Patternist_iterators
     */
    template<typename TResult, typename TSource, typename TMapper, typename Context = DynamicContext::Ptr>
    class ItemMappingIterator : public QAbstractXmlForwardIterator<TResult>
    {
    public:
        /**
         * Constructs an ItemMappingIterator.
         *
         * @param mapper the object that has the mapToItem() sequence.
         * @param iterator the QAbstractXmlForwardIterator whose items should be mapped.
         * @param context the context that will be passed to the map function.
         * May be null.
         */
        ItemMappingIterator(const TMapper &mapper,
                            const typename QAbstractXmlForwardIterator<TSource>::Ptr &iterator,
                            const Context &context) : m_mapper(mapper)
                                                    , m_it(iterator)
                                                    , m_context(context)
                                                    , m_position(0)
        {
            Q_ASSERT(mapper);
            Q_ASSERT(iterator);
        }

        /**
         * @returns the next item in the sequence, or
         * @c null if the end have been reached.
         */
        virtual TResult next()
        {
            const TSource sourceItem(m_it->next());

            if(qIsForwardIteratorEnd(sourceItem))
            {
                m_current = TResult();
                m_position = -1;
                return TResult();
            }
            else
            {
                m_current = m_mapper->mapToItem(sourceItem, m_context);
                if(qIsForwardIteratorEnd(m_current))
                    return next(); /* The mapper returned null, so continue with the next in the source. */
                else
                {
                    ++m_position;
                    return m_current;
                }
            }
        }

        virtual TResult current() const
        {
            return m_current;
        }

        virtual xsInteger position() const
        {
            return m_position;
        }

        virtual typename QAbstractXmlForwardIterator<TResult>::Ptr copy() const
        {
            return typename QAbstractXmlForwardIterator<TResult>::Ptr
                (new ItemMappingIterator<TResult, TSource, TMapper, Context>(m_mapper, m_it->copy(), m_context));
        }

    private:
        const TMapper                                               m_mapper;
        const typename QAbstractXmlForwardIterator<TSource>::Ptr    m_it;
        const Context                                               m_context;
        TResult                                                     m_current;
        xsInteger                                                   m_position;
    };

    /**
     * @short An object generator for ItemMappingIterator.
     *
     * makeItemMappingIterator() is a convenience function for avoiding specifying
     * the full template instantiation for ItemMappingIterator. Conceptually, it
     * is identical to Qt's qMakePair().
     *
     * @relates ItemMappingIterator
     */
    template<typename TResult, typename TSource, typename TMapper, typename Context>
    static inline
    typename QAbstractXmlForwardIterator<TResult>::Ptr
    makeItemMappingIterator(const TMapper &mapper,
                            const QExplicitlySharedDataPointer<QAbstractXmlForwardIterator<TSource> > &source,
                            const Context &context)
    {
        return typename QAbstractXmlForwardIterator<TResult>::Ptr
            (new ItemMappingIterator<TResult, TSource, TMapper, Context>(mapper, source, context));
    }
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
