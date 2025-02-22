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

#include "JSHTMLIFrameElement.h"

#include <wtf/GetPtr.h>

#include "DOMWindow.h"
#include "Document.h"
#include "HTMLIFrameElement.h"
#include "JSDOMBinding.h"
#include "JSDOMWindow.h"
#include "JSDocument.h"
#include "JSSVGDocument.h"
#include "KURL.h"
#include "SVGDocument.h"

#include <runtime/Error.h>
#include <runtime/JSNumberCell.h>
#include <runtime/JSString.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSHTMLIFrameElement)

/* Hash table */

static const HashTableValue JSHTMLIFrameElementTableValues[14] =
{
    { "align", DontDelete, (intptr_t)jsHTMLIFrameElementAlign, (intptr_t)setJSHTMLIFrameElementAlign },
    { "frameBorder", DontDelete, (intptr_t)jsHTMLIFrameElementFrameBorder, (intptr_t)setJSHTMLIFrameElementFrameBorder },
    { "height", DontDelete, (intptr_t)jsHTMLIFrameElementHeight, (intptr_t)setJSHTMLIFrameElementHeight },
    { "longDesc", DontDelete, (intptr_t)jsHTMLIFrameElementLongDesc, (intptr_t)setJSHTMLIFrameElementLongDesc },
    { "marginHeight", DontDelete, (intptr_t)jsHTMLIFrameElementMarginHeight, (intptr_t)setJSHTMLIFrameElementMarginHeight },
    { "marginWidth", DontDelete, (intptr_t)jsHTMLIFrameElementMarginWidth, (intptr_t)setJSHTMLIFrameElementMarginWidth },
    { "name", DontDelete, (intptr_t)jsHTMLIFrameElementName, (intptr_t)setJSHTMLIFrameElementName },
    { "scrolling", DontDelete, (intptr_t)jsHTMLIFrameElementScrolling, (intptr_t)setJSHTMLIFrameElementScrolling },
    { "src", DontDelete, (intptr_t)jsHTMLIFrameElementSrc, (intptr_t)setJSHTMLIFrameElementSrc },
    { "width", DontDelete, (intptr_t)jsHTMLIFrameElementWidth, (intptr_t)setJSHTMLIFrameElementWidth },
    { "contentDocument", DontDelete|ReadOnly, (intptr_t)jsHTMLIFrameElementContentDocument, (intptr_t)0 },
    { "contentWindow", DontDelete|ReadOnly, (intptr_t)jsHTMLIFrameElementContentWindow, (intptr_t)0 },
    { "constructor", DontEnum|ReadOnly, (intptr_t)jsHTMLIFrameElementConstructor, (intptr_t)0 },
    { 0, 0, 0, 0 }
};

static const HashTable JSHTMLIFrameElementTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 16383, JSHTMLIFrameElementTableValues, 0 };
#else
    { 36, 31, JSHTMLIFrameElementTableValues, 0 };
#endif

/* Hash table for constructor */

static const HashTableValue JSHTMLIFrameElementConstructorTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static const HashTable JSHTMLIFrameElementConstructorTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSHTMLIFrameElementConstructorTableValues, 0 };
#else
    { 1, 0, JSHTMLIFrameElementConstructorTableValues, 0 };
#endif

class JSHTMLIFrameElementConstructor : public DOMObject {
public:
    JSHTMLIFrameElementConstructor(ExecState* exec)
        : DOMObject(JSHTMLIFrameElementConstructor::createStructure(exec->lexicalGlobalObject()->objectPrototype()))
    {
        putDirect(exec->propertyNames().prototype, JSHTMLIFrameElementPrototype::self(exec), None);
    }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    virtual const ClassInfo* classInfo() const { return &s_info; }
    static const ClassInfo s_info;

    static PassRefPtr<Structure> createStructure(JSValuePtr proto) 
    { 
        return Structure::create(proto, TypeInfo(ObjectType, ImplementsHasInstance)); 
    }
};

const ClassInfo JSHTMLIFrameElementConstructor::s_info = { "HTMLIFrameElementConstructor", 0, &JSHTMLIFrameElementConstructorTable, 0 };

bool JSHTMLIFrameElementConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLIFrameElementConstructor, DOMObject>(exec, &JSHTMLIFrameElementConstructorTable, this, propertyName, slot);
}

/* Hash table for prototype */

static const HashTableValue JSHTMLIFrameElementPrototypeTableValues[2] =
{
    { "getSVGDocument", DontDelete|Function, (intptr_t)jsHTMLIFrameElementPrototypeFunctionGetSVGDocument, (intptr_t)0 },
    { 0, 0, 0, 0 }
};

static const HashTable JSHTMLIFrameElementPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSHTMLIFrameElementPrototypeTableValues, 0 };
#else
    { 2, 1, JSHTMLIFrameElementPrototypeTableValues, 0 };
#endif

const ClassInfo JSHTMLIFrameElementPrototype::s_info = { "HTMLIFrameElementPrototype", 0, &JSHTMLIFrameElementPrototypeTable, 0 };

JSObject* JSHTMLIFrameElementPrototype::self(ExecState* exec)
{
    return getDOMPrototype<JSHTMLIFrameElement>(exec);
}

bool JSHTMLIFrameElementPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSObject>(exec, &JSHTMLIFrameElementPrototypeTable, this, propertyName, slot);
}

const ClassInfo JSHTMLIFrameElement::s_info = { "HTMLIFrameElement", &JSHTMLElement::s_info, &JSHTMLIFrameElementTable, 0 };

JSHTMLIFrameElement::JSHTMLIFrameElement(PassRefPtr<Structure> structure, PassRefPtr<HTMLIFrameElement> impl)
    : JSHTMLElement(structure, impl)
{
}

JSObject* JSHTMLIFrameElement::createPrototype(ExecState* exec)
{
    return new (exec) JSHTMLIFrameElementPrototype(JSHTMLIFrameElementPrototype::createStructure(JSHTMLElementPrototype::self(exec)));
}

bool JSHTMLIFrameElement::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLIFrameElement, Base>(exec, &JSHTMLIFrameElementTable, this, propertyName, slot);
}

JSValuePtr jsHTMLIFrameElementAlign(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(asObject(slot.slotBase()))->impl());
    return jsString(exec, imp->align());
}

JSValuePtr jsHTMLIFrameElementFrameBorder(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(asObject(slot.slotBase()))->impl());
    return jsString(exec, imp->frameBorder());
}

JSValuePtr jsHTMLIFrameElementHeight(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(asObject(slot.slotBase()))->impl());
    return jsString(exec, imp->height());
}

JSValuePtr jsHTMLIFrameElementLongDesc(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(asObject(slot.slotBase()))->impl());
    return jsString(exec, imp->longDesc());
}

JSValuePtr jsHTMLIFrameElementMarginHeight(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(asObject(slot.slotBase()))->impl());
    return jsString(exec, imp->marginHeight());
}

JSValuePtr jsHTMLIFrameElementMarginWidth(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(asObject(slot.slotBase()))->impl());
    return jsString(exec, imp->marginWidth());
}

JSValuePtr jsHTMLIFrameElementName(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(asObject(slot.slotBase()))->impl());
    return jsString(exec, imp->name());
}

JSValuePtr jsHTMLIFrameElementScrolling(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(asObject(slot.slotBase()))->impl());
    return jsString(exec, imp->scrolling());
}

JSValuePtr jsHTMLIFrameElementSrc(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(asObject(slot.slotBase()))->impl());
    return jsString(exec, imp->src());
}

JSValuePtr jsHTMLIFrameElementWidth(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(asObject(slot.slotBase()))->impl());
    return jsString(exec, imp->width());
}

JSValuePtr jsHTMLIFrameElementContentDocument(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(asObject(slot.slotBase()))->impl());
    return checkNodeSecurity(exec, imp->contentDocument()) ? toJS(exec, WTF::getPtr(imp->contentDocument())) : jsUndefined();
}

JSValuePtr jsHTMLIFrameElementContentWindow(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(asObject(slot.slotBase()))->impl());
    return toJS(exec, WTF::getPtr(imp->contentWindow()));
}

JSValuePtr jsHTMLIFrameElementConstructor(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    return static_cast<JSHTMLIFrameElement*>(asObject(slot.slotBase()))->getConstructor(exec);
}
void JSHTMLIFrameElement::put(ExecState* exec, const Identifier& propertyName, JSValuePtr value, PutPropertySlot& slot)
{
    lookupPut<JSHTMLIFrameElement, Base>(exec, propertyName, value, &JSHTMLIFrameElementTable, this, slot);
}

void setJSHTMLIFrameElementAlign(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(thisObject)->impl());
    imp->setAlign(valueToStringWithNullCheck(exec, value));
}

void setJSHTMLIFrameElementFrameBorder(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(thisObject)->impl());
    imp->setFrameBorder(valueToStringWithNullCheck(exec, value));
}

void setJSHTMLIFrameElementHeight(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(thisObject)->impl());
    imp->setHeight(valueToStringWithNullCheck(exec, value));
}

void setJSHTMLIFrameElementLongDesc(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(thisObject)->impl());
    imp->setLongDesc(valueToStringWithNullCheck(exec, value));
}

void setJSHTMLIFrameElementMarginHeight(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(thisObject)->impl());
    imp->setMarginHeight(valueToStringWithNullCheck(exec, value));
}

void setJSHTMLIFrameElementMarginWidth(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(thisObject)->impl());
    imp->setMarginWidth(valueToStringWithNullCheck(exec, value));
}

void setJSHTMLIFrameElementName(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(thisObject)->impl());
    imp->setName(valueToStringWithNullCheck(exec, value));
}

void setJSHTMLIFrameElementScrolling(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(thisObject)->impl());
    imp->setScrolling(valueToStringWithNullCheck(exec, value));
}

void setJSHTMLIFrameElementSrc(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    static_cast<JSHTMLIFrameElement*>(thisObject)->setSrc(exec, value);
}

void setJSHTMLIFrameElementWidth(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(static_cast<JSHTMLIFrameElement*>(thisObject)->impl());
    imp->setWidth(valueToStringWithNullCheck(exec, value));
}

JSValuePtr JSHTMLIFrameElement::getConstructor(ExecState* exec)
{
    return getDOMConstructor<JSHTMLIFrameElementConstructor>(exec);
}

JSValuePtr jsHTMLIFrameElementPrototypeFunctionGetSVGDocument(ExecState* exec, JSObject*, JSValuePtr thisValue, const ArgList& args)
{
    if (!thisValue->isObject(&JSHTMLIFrameElement::s_info))
        return throwError(exec, TypeError);
    JSHTMLIFrameElement* castedThisObj = static_cast<JSHTMLIFrameElement*>(asObject(thisValue));
    HTMLIFrameElement* imp = static_cast<HTMLIFrameElement*>(castedThisObj->impl());
    ExceptionCode ec = 0;
    if (!checkNodeSecurity(exec, imp->getSVGDocument(ec)))
        return jsUndefined();


    JSC::JSValuePtr result = toJS(exec, WTF::getPtr(imp->getSVGDocument(ec)));
    setDOMException(exec, ec);
    return result;
}


}
