/*
    Copyright (C) 2004, 2005, 2006 Nikolas Zimmermann <wildfox@kde.org>
                  2004, 2005 Rob Buis <buis@kde.org>
                  2005 Eric Seidel <eric.seidel@kdemail.net>

    This file is part of the KDE project

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    aint with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"

#if ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
#include "SVGFEOffset.h"
#include "TextStream.h"

namespace WebCore {

float SVGFEOffset::dx() const
{
    return m_dx;
}

void SVGFEOffset::setDx(float dx)
{
    m_dx = dx;
}

float SVGFEOffset::dy() const
{
    return m_dy;
}

void SVGFEOffset::setDy(float dy)
{
    m_dy = dy;
}

TextStream& SVGFEOffset::externalRepresentation(TextStream& ts) const
{
    ts << "[type=OFFSET] "; SVGFilterEffect::externalRepresentation(ts)
        << " [dx=" << dx() << " dy=" << dy() << "]";
    return ts;
}

} // namespace WebCore

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
