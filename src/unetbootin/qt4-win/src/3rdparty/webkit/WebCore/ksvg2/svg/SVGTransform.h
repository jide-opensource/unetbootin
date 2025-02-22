/*
    Copyright (C) 2004, 2005 Nikolas Zimmermann <wildfox@kde.org>
                  2004, 2005 Rob Buis <buis@kde.org>

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

#ifndef SVGTransform_h
#define SVGTransform_h
#if ENABLE(SVG)

#include "AffineTransform.h"
#include "FloatPoint.h"
#include "Shared.h"
#include <wtf/RefPtr.h>

namespace WebCore {
    
    class FloatSize;

    class SVGTransform {
    public:
        enum SVGTransformType {
            SVG_TRANSFORM_UNKNOWN           = 0,
            SVG_TRANSFORM_MATRIX            = 1,
            SVG_TRANSFORM_TRANSLATE         = 2,
            SVG_TRANSFORM_SCALE             = 3,
            SVG_TRANSFORM_ROTATE            = 4,
            SVG_TRANSFORM_SKEWX             = 5,
            SVG_TRANSFORM_SKEWY             = 6
        };
 
        SVGTransform();
        SVGTransform(SVGTransformType type);
        explicit SVGTransform(const AffineTransform&);
        virtual ~SVGTransform();
               
        SVGTransformType type() const;

        AffineTransform matrix() const;
    
        float angle() const;
        FloatPoint rotationCenter() const;

        void setMatrix(const AffineTransform&);
        void setTranslate(float tx, float ty);
        void setScale(float sx, float sy);
        void setRotate(float angle, float cx, float cy);
        void setSkewX(float angle);
        void setSkewY(float angle);
        
        // Internal use only (animation system)
        FloatPoint translate() const;
        FloatSize scale() const;
        
        bool isValid();

    private:
        SVGTransformType m_type;
        float m_angle;
        FloatPoint m_center;
        AffineTransform m_matrix;
    };

    inline bool operator==(const SVGTransform& a, const SVGTransform& b)
    {
        return a.type() == b.type() && a.angle() == b.angle() && a.matrix() == b.matrix();
    }
    
    inline bool operator!=(const SVGTransform& a, const SVGTransform& b)
    {
        return !(a == b);
    }
    
} // namespace WebCore

#endif // ENABLE(SVG)
#endif

// vim:ts=4:noet
