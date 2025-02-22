/*
 * Copyright (C) 2007 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "LayoutState.h"

#include "RenderArena.h"
#include "RenderLayer.h"
#include "RenderView.h"

namespace WebCore {

LayoutState::LayoutState(LayoutState* prev, RenderBox* renderer, const IntSize& offset)
{
    ASSERT(prev);

    m_next = prev;

    bool fixed = renderer->isPositioned() && renderer->style()->position() == FixedPosition;
    if (fixed) {
        int fixedX = 0;
        int fixedY = 0;
        renderer->view()->absolutePosition(fixedX, fixedY, true);
        m_offset = IntSize(fixedX, fixedY) + offset;
    } else
        m_offset = prev->m_offset + offset;

    if (renderer->isRelPositioned()) {
        int relX = 0;
        int relY = 0;
        renderer->layer()->relativePositionOffset(relX, relY);
        m_offset += IntSize(relX, relY);
    } else if (renderer->isPositioned() && !fixed) {
        if (RenderObject* container = renderer->container())
            m_offset += renderer->offsetForPositionedInContainer(container);
    }

    m_clipped = !fixed && prev->m_clipped;
    if (m_clipped)
        m_clipRect = prev->m_clipRect;
    if (renderer->hasOverflowClip()) {
        int x = m_offset.width();
        int y = m_offset.height();
        RenderLayer* layer = renderer->layer();
        IntRect clipRect(x, y, layer->width(), layer->height());
        if (m_clipped)
            m_clipRect.intersect(clipRect);
        else {
            m_clipRect = clipRect;
            m_clipped = true;
        }
        layer->subtractScrollOffset(x, y);
        m_offset = IntSize(x, y);
    }
    // FIXME: <http://bugs.webkit.org/show_bug.cgi?id=13443> Apply control clip if present.
}

LayoutState::LayoutState(RenderObject* root)
{
    RenderObject* container = root->container();
    m_clipped = true;
    m_clipRect = container->absoluteClippedOverflowRect();
    int x = 0;
    int y = 0;
    container->absolutePosition(x, y);
    m_offset = IntSize(x, y);
    m_next = 0;
}

#ifndef NDEBUG
static bool inLayoutStateDestroy;
#endif

void LayoutState::destroy(RenderArena* renderArena)
{
#ifndef NDEBUG
    inLayoutStateDestroy = true;
#endif
    delete this;
#ifndef NDEBUG
    inLayoutStateDestroy = false;
#endif
    renderArena->free(*(size_t*)this, this);
}

void* LayoutState::operator new(size_t sz, RenderArena* renderArena) throw()
{
    return renderArena->allocate(sz);
}

void LayoutState::operator delete(void* ptr, size_t sz)
{
    ASSERT(inLayoutStateDestroy);
    *(size_t*)ptr = sz;
}

} // namespace WebCore
