/*
    Copyright (C) 2004, 2005, 2006 Nikolas Zimmermann <zimmermann@kde.org>
                  2004, 2005, 2006, 2007 Rob Buis <buis@kde.org>

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

#ifndef SVGSVGElement_h
#define SVGSVGElement_h

#if ENABLE(SVG)

#include "SVGExternalResourcesRequired.h"
#include "SVGFitToViewBox.h"
#include "SVGLangSpace.h"
#include "SVGStyledLocatableElement.h"
#include "SVGTests.h"
#include "SVGZoomAndPan.h"

namespace WebCore
{
    class SVGAngle;
    class SVGLength;
    class SVGTransform;
    class SVGLength;
    class TimeScheduler;
    class SVGSVGElement : public SVGStyledLocatableElement,
                          public SVGTests,
                          public SVGLangSpace,
                          public SVGExternalResourcesRequired,
                          public SVGFitToViewBox,
                          public SVGZoomAndPan
    {
    public:
        SVGSVGElement(const QualifiedName&, Document*);
        virtual ~SVGSVGElement();

        virtual bool isSVG() const { return true; }
        
        virtual bool isValid() const { return SVGTests::isValid(); }

        // 'SVGSVGElement' functions
        const AtomicString& contentScriptType() const;
        void setContentScriptType(const AtomicString& type);

        const AtomicString& contentStyleType() const;
        void setContentStyleType(const AtomicString& type);

        FloatRect viewport() const;

        float pixelUnitToMillimeterX() const;
        float pixelUnitToMillimeterY() const;
        float screenPixelToMillimeterX() const;
        float screenPixelToMillimeterY() const;

        bool useCurrentView() const;
        void setUseCurrentView(bool currentView);

        // SVGViewSpec* currentView() const;

        float currentScale() const;
        void setCurrentScale(float scale);

        FloatPoint currentTranslate() const;
        void setCurrentTranslate(const FloatPoint&);
        
        TimeScheduler* timeScheduler() { return m_timeScheduler; }
        
        void pauseAnimations();
        void unpauseAnimations();
        bool animationsPaused() const;

        float getCurrentTime() const;
        void setCurrentTime(float seconds);

        unsigned long suspendRedraw(unsigned long max_wait_milliseconds);
        void unsuspendRedraw(unsigned long suspend_handle_id, ExceptionCode&);
        void unsuspendRedrawAll();
        void forceRedraw();

        NodeList* getIntersectionList(const FloatRect&, SVGElement* referenceElement);
        NodeList* getEnclosureList(const FloatRect&, SVGElement* referenceElement);
        bool checkIntersection(SVGElement*, const FloatRect&);
        bool checkEnclosure(SVGElement*, const FloatRect&);
        void deselectAll();

        static double createSVGNumber();
        static SVGLength createSVGLength();
        static SVGAngle* createSVGAngle();
        static FloatPoint createSVGPoint();
        static AffineTransform createSVGMatrix();
        static FloatRect createSVGRect();
        static SVGTransform createSVGTransform();
        static SVGTransform createSVGTransformFromMatrix(const AffineTransform&);

        virtual void parseMappedAttribute(MappedAttribute*);

        // 'virtual SVGLocatable' functions
        virtual AffineTransform getCTM() const;
        virtual AffineTransform getScreenCTM() const;

        virtual bool rendererIsNeeded(RenderStyle* style) { return StyledElement::rendererIsNeeded(style); }
        virtual RenderObject* createRenderer(RenderArena*, RenderStyle*);

        virtual void insertedIntoDocument();
        virtual void removedFromDocument();

        virtual void attributeChanged(Attribute*, bool preserveDecls = false);

    protected:
        virtual const SVGElement* contextElement() const { return this; }

        friend class RenderSVGContainer;
        virtual bool hasRelativeValues() const;

    private:
        void addSVGWindowEventListener(const AtomicString& eventType, const Attribute* attr);   

        ANIMATED_PROPERTY_FORWARD_DECLARATIONS(SVGExternalResourcesRequired, bool, ExternalResourcesRequired, externalResourcesRequired)
        ANIMATED_PROPERTY_FORWARD_DECLARATIONS(SVGFitToViewBox, FloatRect, ViewBox, viewBox)
        ANIMATED_PROPERTY_FORWARD_DECLARATIONS(SVGFitToViewBox, SVGPreserveAspectRatio*, PreserveAspectRatio, preserveAspectRatio)

        ANIMATED_PROPERTY_DECLARATIONS(SVGSVGElement, SVGLength, SVGLength, X, x)
        ANIMATED_PROPERTY_DECLARATIONS(SVGSVGElement, SVGLength, SVGLength, Y, y)
        ANIMATED_PROPERTY_DECLARATIONS(SVGSVGElement, SVGLength, SVGLength, Width, width)
        ANIMATED_PROPERTY_DECLARATIONS(SVGSVGElement, SVGLength, SVGLength, Height, height)

        bool m_useCurrentView;
        TimeScheduler* m_timeScheduler;
        FloatPoint m_translation;
    };

} // namespace WebCore

#endif // ENABLE(SVG)
#endif

// vim:ts=4:noet
