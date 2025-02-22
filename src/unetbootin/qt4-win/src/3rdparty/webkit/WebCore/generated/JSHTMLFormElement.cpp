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

#include "JSHTMLFormElement.h"

#include <wtf/GetPtr.h>

#include "AtomicString.h"
#include "HTMLCollection.h"
#include "HTMLFormElement.h"
#include "JSHTMLCollection.h"
#include "PlatformString.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSHTMLFormElementTableEntries[] =
{
    { "action", JSHTMLFormElement::ActionAttrNum, DontDelete, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "target", JSHTMLFormElement::TargetAttrNum, DontDelete, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "method", JSHTMLFormElement::MethodAttrNum, DontDelete, 0, 0 },
    { "length", JSHTMLFormElement::LengthAttrNum, DontDelete|ReadOnly, 0, &JSHTMLFormElementTableEntries[10] },
    { 0, 0, 0, 0, 0 },
    { "constructor", JSHTMLFormElement::ConstructorAttrNum, DontDelete|DontEnum|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "elements", JSHTMLFormElement::ElementsAttrNum, DontDelete|ReadOnly, 0, &JSHTMLFormElementTableEntries[12] },
    { "name", JSHTMLFormElement::NameAttrNum, DontDelete, 0, &JSHTMLFormElementTableEntries[11] },
    { "acceptCharset", JSHTMLFormElement::AcceptCharsetAttrNum, DontDelete, 0, 0 },
    { "encoding", JSHTMLFormElement::EncodingAttrNum, DontDelete, 0, &JSHTMLFormElementTableEntries[13] },
    { "enctype", JSHTMLFormElement::EnctypeAttrNum, DontDelete, 0, 0 }
};

static const HashTable JSHTMLFormElementTable = 
{
    2, 14, JSHTMLFormElementTableEntries, 10
};

/* Hash table for constructor */

static const HashEntry JSHTMLFormElementConstructorTableEntries[] =
{
    { 0, 0, 0, 0, 0 }
};

static const HashTable JSHTMLFormElementConstructorTable = 
{
    2, 1, JSHTMLFormElementConstructorTableEntries, 1
};

class JSHTMLFormElementConstructor : public DOMObject {
public:
    JSHTMLFormElementConstructor(ExecState* exec)
    {
        setPrototype(exec->lexicalInterpreter()->builtinObjectPrototype());
        putDirect(exec->propertyNames().prototype, JSHTMLFormElementPrototype::self(exec), None);
    }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    JSValue* getValueProperty(ExecState*, int token) const;
    virtual const ClassInfo* classInfo() const { return &info; }
    static const ClassInfo info;

    virtual bool implementsHasInstance() const { return true; }
};

const ClassInfo JSHTMLFormElementConstructor::info = { "HTMLFormElementConstructor", 0, &JSHTMLFormElementConstructorTable, 0 };

bool JSHTMLFormElementConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLFormElementConstructor, DOMObject>(exec, &JSHTMLFormElementConstructorTable, this, propertyName, slot);
}

JSValue* JSHTMLFormElementConstructor::getValueProperty(ExecState*, int token) const
{
    // The token is the numeric value of its associated constant
    return jsNumber(token);
}

/* Hash table for prototype */

static const HashEntry JSHTMLFormElementPrototypeTableEntries[] =
{
    { "submit", JSHTMLFormElement::SubmitFuncNum, DontDelete|Function, 0, &JSHTMLFormElementPrototypeTableEntries[2] },
    { 0, 0, 0, 0, 0 },
    { "reset", JSHTMLFormElement::ResetFuncNum, DontDelete|Function, 0, 0 }
};

static const HashTable JSHTMLFormElementPrototypeTable = 
{
    2, 3, JSHTMLFormElementPrototypeTableEntries, 2
};

const ClassInfo JSHTMLFormElementPrototype::info = { "HTMLFormElementPrototype", 0, &JSHTMLFormElementPrototypeTable, 0 };

JSObject* JSHTMLFormElementPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSHTMLFormElementPrototype>(exec, "[[JSHTMLFormElement.prototype]]");
}

bool JSHTMLFormElementPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSHTMLFormElementPrototypeFunction, JSObject>(exec, &JSHTMLFormElementPrototypeTable, this, propertyName, slot);
}

const ClassInfo JSHTMLFormElement::info = { "HTMLFormElement", &JSHTMLElement::info, &JSHTMLFormElementTable, 0 };

JSHTMLFormElement::JSHTMLFormElement(ExecState* exec, HTMLFormElement* impl)
    : JSHTMLElement(exec, impl)
{
    setPrototype(JSHTMLFormElementPrototype::self(exec));
}

bool JSHTMLFormElement::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    if (canGetItemsForName(exec, static_cast<HTMLFormElement*>(impl()), propertyName)) {
        slot.setCustom(this, nameGetter);
        return true;
    }
    const HashEntry* entry = Lookup::findEntry(&JSHTMLFormElementTable, propertyName);
    if (entry) {
        slot.setStaticEntry(this, entry, staticValueGetter<JSHTMLFormElement>);
        return true;
    }
    bool ok;
    unsigned index = propertyName.toUInt32(&ok, false);
    if (ok && index < static_cast<HTMLFormElement*>(impl())->length()) {
        slot.setCustomIndex(this, index, indexGetter);
        return true;
    }
    return JSHTMLElement::getOwnPropertySlot(exec, propertyName, slot);
}

JSValue* JSHTMLFormElement::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case ElementsAttrNum: {
        HTMLFormElement* imp = static_cast<HTMLFormElement*>(impl());

        return toJS(exec, WTF::getPtr(imp->elements()));
    }
    case LengthAttrNum: {
        HTMLFormElement* imp = static_cast<HTMLFormElement*>(impl());

        return jsNumber(imp->length());
    }
    case NameAttrNum: {
        HTMLFormElement* imp = static_cast<HTMLFormElement*>(impl());

        return jsString(imp->name());
    }
    case AcceptCharsetAttrNum: {
        HTMLFormElement* imp = static_cast<HTMLFormElement*>(impl());

        return jsString(imp->acceptCharset());
    }
    case ActionAttrNum: {
        HTMLFormElement* imp = static_cast<HTMLFormElement*>(impl());

        return jsString(imp->action());
    }
    case EncodingAttrNum: {
        HTMLFormElement* imp = static_cast<HTMLFormElement*>(impl());

        return jsString(imp->encoding());
    }
    case EnctypeAttrNum: {
        HTMLFormElement* imp = static_cast<HTMLFormElement*>(impl());

        return jsString(imp->enctype());
    }
    case MethodAttrNum: {
        HTMLFormElement* imp = static_cast<HTMLFormElement*>(impl());

        return jsString(imp->method());
    }
    case TargetAttrNum: {
        HTMLFormElement* imp = static_cast<HTMLFormElement*>(impl());

        return jsString(imp->target());
    }
    case ConstructorAttrNum:
        return getConstructor(exec);
    }
    return 0;
}

void JSHTMLFormElement::put(ExecState* exec, const Identifier& propertyName, JSValue* value, int attr)
{
    lookupPut<JSHTMLFormElement, JSHTMLElement>(exec, propertyName, value, attr, &JSHTMLFormElementTable, this);
}

void JSHTMLFormElement::putValueProperty(ExecState* exec, int token, JSValue* value, int /*attr*/)
{
    switch (token) {
    case NameAttrNum: {
        HTMLFormElement* imp = static_cast<HTMLFormElement*>(impl());

        imp->setName(valueToStringWithNullCheck(exec, value));
        break;
    }
    case AcceptCharsetAttrNum: {
        HTMLFormElement* imp = static_cast<HTMLFormElement*>(impl());

        imp->setAcceptCharset(valueToStringWithNullCheck(exec, value));
        break;
    }
    case ActionAttrNum: {
        HTMLFormElement* imp = static_cast<HTMLFormElement*>(impl());

        imp->setAction(valueToStringWithNullCheck(exec, value));
        break;
    }
    case EncodingAttrNum: {
        HTMLFormElement* imp = static_cast<HTMLFormElement*>(impl());

        imp->setEncoding(valueToStringWithNullCheck(exec, value));
        break;
    }
    case EnctypeAttrNum: {
        HTMLFormElement* imp = static_cast<HTMLFormElement*>(impl());

        imp->setEnctype(valueToStringWithNullCheck(exec, value));
        break;
    }
    case MethodAttrNum: {
        HTMLFormElement* imp = static_cast<HTMLFormElement*>(impl());

        imp->setMethod(valueToStringWithNullCheck(exec, value));
        break;
    }
    case TargetAttrNum: {
        HTMLFormElement* imp = static_cast<HTMLFormElement*>(impl());

        imp->setTarget(valueToStringWithNullCheck(exec, value));
        break;
    }
    }
}

JSValue* JSHTMLFormElement::getConstructor(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSHTMLFormElementConstructor>(exec, "[[HTMLFormElement.constructor]]");
}
JSValue* JSHTMLFormElementPrototypeFunction::callAsFunction(ExecState* exec, JSObject* thisObj, const List& args)
{
    if (!thisObj->inherits(&JSHTMLFormElement::info))
      return throwError(exec, TypeError);

    HTMLFormElement* imp = static_cast<HTMLFormElement*>(static_cast<JSHTMLFormElement*>(thisObj)->impl());

    switch (id) {
    case JSHTMLFormElement::SubmitFuncNum: {

        imp->submit();
        return jsUndefined();
    }
    case JSHTMLFormElement::ResetFuncNum: {

        imp->reset();
        return jsUndefined();
    }
    }
    return 0;
}

JSValue* JSHTMLFormElement::indexGetter(ExecState* exec, JSObject* originalObject, const Identifier& propertyName, const PropertySlot& slot)
{
    JSHTMLFormElement* thisObj = static_cast<JSHTMLFormElement*>(slot.slotBase());
    return toJS(exec, static_cast<HTMLFormElement*>(thisObj->impl())->item(slot.index()));
}

}
