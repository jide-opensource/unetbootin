/*
    Copyright (C) 2004, 2005 Nikolas Zimmermann <wildfox@kde.org>
                  2004, 2005, 2006 Rob Buis <buis@kde.org>

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
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef SVGTextPositioningElement_h
#define SVGTextPositioningElement_h
#if ENABLE(SVG)

#include "SVGTextContentElement.h"

namespace WebCore
{
    class SVGLengthList;
    class SVGNumberList;

    class SVGTextPositioningElement : public SVGTextContentElement
    {
    public:
        SVGTextPositioningElement(const QualifiedName&, Document*);
        virtual ~SVGTextPositioningElement();

        // 'SVGTextPositioningElement' functions
        virtual void parseMappedAttribute(MappedAttribute*);

    private:
        ANIMATED_PROPERTY_DECLARATIONS(SVGTextPositioningElement, SVGLengthList*, RefPtr<SVGLengthList>, X, x)
        ANIMATED_PROPERTY_DECLARATIONS(SVGTextPositioningElement, SVGLengthList*, RefPtr<SVGLengthList>, Y, y)
        ANIMATED_PROPERTY_DECLARATIONS(SVGTextPositioningElement, SVGLengthList*, RefPtr<SVGLengthList>, Dx, dx)
        ANIMATED_PROPERTY_DECLARATIONS(SVGTextPositioningElement, SVGLengthList*, RefPtr<SVGLengthList>, Dy, dy)
        ANIMATED_PROPERTY_DECLARATIONS(SVGTextPositioningElement, SVGNumberList*, RefPtr<SVGNumberList>, Rotate, rotate)
    };

} // namespace WebCore

#endif // ENABLE(SVG)
#endif

// vim:ts=4:noet
