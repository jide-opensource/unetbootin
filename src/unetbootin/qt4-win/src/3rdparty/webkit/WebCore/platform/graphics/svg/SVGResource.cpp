/*
 * Copyright (C) 2006 Nikolas Zimmermann <zimmermann@kde.org>
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"

#if ENABLE(SVG)
#include "SVGResource.h"

#include "RenderPath.h"
#include "SVGElement.h"
#include "SVGStyledElement.h"

namespace WebCore {

SVGResource::SVGResource()
{
}

SVGResource::~SVGResource()
{
}

void SVGResource::invalidate()
{
}

void SVGResource::addClient(SVGStyledElement* item)
{
    if (m_clients.contains(item))
        return;

    m_clients.add(item);
}

void SVGResource::repaintClients() const
{
    SVGResource::repaintClients(m_clients);
}

void SVGResource::repaintClients(HashSet<SVGStyledElement*> clients)
{
    HashSet<SVGStyledElement*>::const_iterator it = clients.begin();
    const HashSet<SVGStyledElement*>::const_iterator end = clients.end();

    for (; it != end; ++it) {
        SVGStyledElement* cur = *it;
        cur->setChanged();

        if (cur->renderer())
            cur->renderer()->repaint();

        cur->notifyResourceParentIfExistant();
    }
}

TextStream& SVGResource::externalRepresentation(TextStream& ts) const
{
    return ts;
}

SVGResource* getResourceById(Document* document, const AtomicString& id)
{
    if (id.isEmpty())
        return 0;

    Element* element = document->getElementById(id);
    SVGElement* svgElement = svg_dynamic_cast(element);

    if (svgElement && svgElement->isStyled())
        return static_cast<SVGStyledElement*>(svgElement)->canvasResource();

    return 0;
}

TextStream& operator<<(TextStream& ts, const SVGResource& r)
{
    return r.externalRepresentation(ts);
}

} // namespace WebCore

#endif
