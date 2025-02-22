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
#include "JSSVGFontFaceNameElement.h"

#include <wtf/GetPtr.h>

#include "SVGFontFaceNameElement.h"


using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSSVGFontFaceNameElement)

/* Hash table for prototype */

static const HashTableValue JSSVGFontFaceNameElementPrototypeTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static const HashTable JSSVGFontFaceNameElementPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSSVGFontFaceNameElementPrototypeTableValues, 0 };
#else
    { 1, 0, JSSVGFontFaceNameElementPrototypeTableValues, 0 };
#endif

const ClassInfo JSSVGFontFaceNameElementPrototype::s_info = { "SVGFontFaceNameElementPrototype", 0, &JSSVGFontFaceNameElementPrototypeTable, 0 };

JSObject* JSSVGFontFaceNameElementPrototype::self(ExecState* exec)
{
    return getDOMPrototype<JSSVGFontFaceNameElement>(exec);
}

const ClassInfo JSSVGFontFaceNameElement::s_info = { "SVGFontFaceNameElement", &JSSVGElement::s_info, 0, 0 };

JSSVGFontFaceNameElement::JSSVGFontFaceNameElement(PassRefPtr<Structure> structure, PassRefPtr<SVGFontFaceNameElement> impl)
    : JSSVGElement(structure, impl)
{
}

JSObject* JSSVGFontFaceNameElement::createPrototype(ExecState* exec)
{
    return new (exec) JSSVGFontFaceNameElementPrototype(JSSVGFontFaceNameElementPrototype::createStructure(JSSVGElementPrototype::self(exec)));
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_FONTS)
