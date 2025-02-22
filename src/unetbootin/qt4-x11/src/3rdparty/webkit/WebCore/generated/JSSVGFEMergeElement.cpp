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


#if ENABLE(SVG) && ENABLE(SVG_FILTERS)

#include "SVGElement.h"
#include "JSSVGFEMergeElement.h"

#include <wtf/GetPtr.h>

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "CSSValue.h"
#include "JSCSSStyleDeclaration.h"
#include "JSCSSValue.h"
#include "JSSVGAnimatedLength.h"
#include "JSSVGAnimatedString.h"
#include "SVGFEMergeElement.h"

#include <runtime/Error.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSSVGFEMergeElement)

/* Hash table */

static const HashTableValue JSSVGFEMergeElementTableValues[8] =
{
    { "x", DontDelete|ReadOnly, (intptr_t)jsSVGFEMergeElementX, (intptr_t)0 },
    { "y", DontDelete|ReadOnly, (intptr_t)jsSVGFEMergeElementY, (intptr_t)0 },
    { "width", DontDelete|ReadOnly, (intptr_t)jsSVGFEMergeElementWidth, (intptr_t)0 },
    { "height", DontDelete|ReadOnly, (intptr_t)jsSVGFEMergeElementHeight, (intptr_t)0 },
    { "result", DontDelete|ReadOnly, (intptr_t)jsSVGFEMergeElementResult, (intptr_t)0 },
    { "className", DontDelete|ReadOnly, (intptr_t)jsSVGFEMergeElementClassName, (intptr_t)0 },
    { "style", DontDelete|ReadOnly, (intptr_t)jsSVGFEMergeElementStyle, (intptr_t)0 },
    { 0, 0, 0, 0 }
};

static const HashTable JSSVGFEMergeElementTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 255, JSSVGFEMergeElementTableValues, 0 };
#else
    { 17, 15, JSSVGFEMergeElementTableValues, 0 };
#endif

/* Hash table for prototype */

static const HashTableValue JSSVGFEMergeElementPrototypeTableValues[2] =
{
    { "getPresentationAttribute", DontDelete|Function, (intptr_t)jsSVGFEMergeElementPrototypeFunctionGetPresentationAttribute, (intptr_t)1 },
    { 0, 0, 0, 0 }
};

static const HashTable JSSVGFEMergeElementPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSSVGFEMergeElementPrototypeTableValues, 0 };
#else
    { 2, 1, JSSVGFEMergeElementPrototypeTableValues, 0 };
#endif

const ClassInfo JSSVGFEMergeElementPrototype::s_info = { "SVGFEMergeElementPrototype", 0, &JSSVGFEMergeElementPrototypeTable, 0 };

JSObject* JSSVGFEMergeElementPrototype::self(ExecState* exec)
{
    return getDOMPrototype<JSSVGFEMergeElement>(exec);
}

bool JSSVGFEMergeElementPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSObject>(exec, &JSSVGFEMergeElementPrototypeTable, this, propertyName, slot);
}

const ClassInfo JSSVGFEMergeElement::s_info = { "SVGFEMergeElement", &JSSVGElement::s_info, &JSSVGFEMergeElementTable, 0 };

JSSVGFEMergeElement::JSSVGFEMergeElement(PassRefPtr<Structure> structure, PassRefPtr<SVGFEMergeElement> impl)
    : JSSVGElement(structure, impl)
{
}

JSObject* JSSVGFEMergeElement::createPrototype(ExecState* exec)
{
    return new (exec) JSSVGFEMergeElementPrototype(JSSVGFEMergeElementPrototype::createStructure(JSSVGElementPrototype::self(exec)));
}

bool JSSVGFEMergeElement::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGFEMergeElement, Base>(exec, &JSSVGFEMergeElementTable, this, propertyName, slot);
}

JSValuePtr jsSVGFEMergeElementX(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGFEMergeElement* imp = static_cast<SVGFEMergeElement*>(static_cast<JSSVGFEMergeElement*>(asObject(slot.slotBase()))->impl());
    RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
    return toJS(exec, obj.get(), imp);
}

JSValuePtr jsSVGFEMergeElementY(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGFEMergeElement* imp = static_cast<SVGFEMergeElement*>(static_cast<JSSVGFEMergeElement*>(asObject(slot.slotBase()))->impl());
    RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
    return toJS(exec, obj.get(), imp);
}

JSValuePtr jsSVGFEMergeElementWidth(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGFEMergeElement* imp = static_cast<SVGFEMergeElement*>(static_cast<JSSVGFEMergeElement*>(asObject(slot.slotBase()))->impl());
    RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
    return toJS(exec, obj.get(), imp);
}

JSValuePtr jsSVGFEMergeElementHeight(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGFEMergeElement* imp = static_cast<SVGFEMergeElement*>(static_cast<JSSVGFEMergeElement*>(asObject(slot.slotBase()))->impl());
    RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
    return toJS(exec, obj.get(), imp);
}

JSValuePtr jsSVGFEMergeElementResult(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGFEMergeElement* imp = static_cast<SVGFEMergeElement*>(static_cast<JSSVGFEMergeElement*>(asObject(slot.slotBase()))->impl());
    RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
    return toJS(exec, obj.get(), imp);
}

JSValuePtr jsSVGFEMergeElementClassName(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGFEMergeElement* imp = static_cast<SVGFEMergeElement*>(static_cast<JSSVGFEMergeElement*>(asObject(slot.slotBase()))->impl());
    RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
    return toJS(exec, obj.get(), imp);
}

JSValuePtr jsSVGFEMergeElementStyle(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGFEMergeElement* imp = static_cast<SVGFEMergeElement*>(static_cast<JSSVGFEMergeElement*>(asObject(slot.slotBase()))->impl());
    return toJS(exec, WTF::getPtr(imp->style()));
}

JSValuePtr jsSVGFEMergeElementPrototypeFunctionGetPresentationAttribute(ExecState* exec, JSObject*, JSValuePtr thisValue, const ArgList& args)
{
    if (!thisValue->isObject(&JSSVGFEMergeElement::s_info))
        return throwError(exec, TypeError);
    JSSVGFEMergeElement* castedThisObj = static_cast<JSSVGFEMergeElement*>(asObject(thisValue));
    SVGFEMergeElement* imp = static_cast<SVGFEMergeElement*>(castedThisObj->impl());
    const UString& name = args.at(exec, 0)->toString(exec);


    JSC::JSValuePtr result = toJS(exec, WTF::getPtr(imp->getPresentationAttribute(name)));
    return result;
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_FILTERS)
