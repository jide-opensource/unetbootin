/**
 * This file is part of the DOM implementation for KDE.
 *
 * Copyright (C) 1997 Martin Jones (mjones@kde.org)
 *           (C) 1997 Torben Weis (weis@kde.org)
 *           (C) 1998 Waldo Bastian (bastian@kde.org)
 *           (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006 Apple Computer, Inc.
 * Copyright (C) 2007 Trolltech ASA
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "RenderTableCol.h"

#include "CachedImage.h"
#include "HTMLNames.h"
#include "HTMLTableColElement.h"
#include "TextStream.h"

namespace WebCore {

using namespace HTMLNames;

RenderTableCol::RenderTableCol(Node* node)
    : RenderContainer(node), m_span(1)
{
    // init RenderObject attributes
    setInline(true); // our object is not Inline
    updateFromElement();
}

void RenderTableCol::updateFromElement()
{
    int oldSpan = m_span;
    Node* node = element();
    if (node && (node->hasTagName(colTag) || node->hasTagName(colgroupTag))) {
        HTMLTableColElement* tc = static_cast<HTMLTableColElement*>(node);
        m_span = tc->span();
    } else
        m_span = !(style() && style()->display() == TABLE_COLUMN_GROUP);
    if (m_span != oldSpan && style() && parent())
        setNeedsLayoutAndPrefWidthsRecalc();
}

bool RenderTableCol::isChildAllowed(RenderObject* child, RenderStyle* style) const
{
    return !child->isText() && style && (style->display() == TABLE_COLUMN);
}

bool RenderTableCol::canHaveChildren() const
{
    // Cols cannot have children. This is actually necessary to fix a bug
    // with libraries.uc.edu, which makes a <p> be a table-column.
    return style()->display() == TABLE_COLUMN_GROUP;
}

IntRect RenderTableCol::absoluteClippedOverflowRect()
{
    // For now, just repaint the whole table.
    // FIXME: Find a better way to do this, e.g., need to repaint all the cells that we
    // might have propagated a background color or borders into.
    RenderObject* table = parent();
    if (table && !table->isTable())
        table = table->parent();
    if (table && table->isTable())
        return table->absoluteClippedOverflowRect();

    return IntRect();
}

void RenderTableCol::imageChanged(CachedImage* image)
{
    if (!image || !image->canRender() || !parent())
        return;

    // FIXME: Repaint only the rect the image paints in.
    repaint();
}

#ifndef NDEBUG
void RenderTableCol::dump(TextStream* stream, DeprecatedString ind) const
{
    *stream << " span=" << m_span;
    RenderContainer::dump(stream, ind);
}
#endif

}
