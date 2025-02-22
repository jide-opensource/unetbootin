/*
    Copyright (C) 2004, 2005 Nikolas Zimmermann <wildfox@kde.org>
    Copyright (C) 2004, 2005 Rob Buis <buis@kde.org>
    Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
                  
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
 
#ifndef SVGZoomEvent_h
#define SVGZoomEvent_h
#if ENABLE(SVG)

#include "FloatRect.h"
#include "UIEvent.h"

namespace WebCore {

    class SVGZoomEvent : public UIEvent {
    public:
        SVGZoomEvent();
        virtual ~SVGZoomEvent();

        // 'SVGZoomEvent' functions
        FloatRect zoomRectScreen() const;

        float previousScale() const;
        void setPreviousScale(float);

        FloatPoint previousTranslate() const;

        float newScale() const;
        void setNewScale(float);

        FloatPoint newTranslate() const;

        virtual bool isSVGZoomEvent() const;

    private:
        float m_newScale;
        float m_previousScale;
        
        FloatRect m_zoomRectScreen;
        
        FloatPoint m_newTranslate;
        FloatPoint m_previousTranslate;
    };

} // namespace WebCore

#endif // ENABLE(SVG)
#endif // SVGZoomEvent_h

// vim:ts=4:noet
