/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

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

#include "config.h"


#if ENABLE(SVG) && ENABLE(SVG_FONTS)

#include "SVGElement.h"
#include "JSSVGGlyphElement.h"

#include <wtf/GetPtr.h>

#include "SVGGlyphElement.h"


using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSSVGGlyphElement)

/* Hash table for prototype */

static const HashTableValue JSSVGGlyphElementPrototypeTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static const HashTable JSSVGGlyphElementPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSSVGGlyphElementPrototypeTableValues, 0 };
#else
    { 1, 0, JSSVGGlyphElementPrototypeTableValues, 0 };
#endif

const ClassInfo JSSVGGlyphElementPrototype::s_info = { "SVGGlyphElementPrototype", 0, &JSSVGGlyphElementPrototypeTable, 0 };

JSObject* JSSVGGlyphElementPrototype::self(ExecState* exec)
{
    return getDOMPrototype<JSSVGGlyphElement>(exec);
}

const ClassInfo JSSVGGlyphElement::s_info = { "SVGGlyphElement", &JSSVGElement::s_info, 0, 0 };

JSSVGGlyphElement::JSSVGGlyphElement(PassRefPtr<Structure> structure, PassRefPtr<SVGGlyphElement> impl)
    : JSSVGElement(structure, impl)
{
}

JSObject* JSSVGGlyphElement::createPrototype(ExecState* exec)
{
    return new (exec) JSSVGGlyphElementPrototype(JSSVGGlyphElementPrototype::createStructure(JSSVGElementPrototype::self(exec)));
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_FONTS)
