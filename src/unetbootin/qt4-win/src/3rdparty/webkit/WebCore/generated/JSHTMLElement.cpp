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

#include "JSHTMLElement.h"

#include <wtf/GetPtr.h>

#include "HTMLCollection.h"
#include "HTMLElement.h"
#include "JSHTMLCollection.h"
#include "PlatformString.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSHTMLElementTableEntries[] =
{
    { 0, 0, 0, 0, 0 },
    { "innerText", JSHTMLElement::InnerTextAttrNum, DontDelete, 0, 0 },
    { "constructor", JSHTMLElement::ConstructorAttrNum, DontDelete|DontEnum|ReadOnly, 0, 0 },
    { "outerHTML", JSHTMLElement::OuterHTMLAttrNum, DontDelete, 0, &JSHTMLElementTableEntries[15] },
    { "id", JSHTMLElement::IdAttrNum, DontDelete, 0, 0 },
    { "isContentEditable", JSHTMLElement::IsContentEditableAttrNum, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "dir", JSHTMLElement::DirAttrNum, DontDelete, 0, 0 },
    { "title", JSHTMLElement::TitleAttrNum, DontDelete, 0, &JSHTMLElementTableEntries[14] },
    { 0, 0, 0, 0, 0 },
    { "className", JSHTMLElement::ClassNameAttrNum, DontDelete, 0, &JSHTMLElementTableEntries[13] },
    { "contentEditable", JSHTMLElement::ContentEditableAttrNum, DontDelete, 0, 0 },
    { "lang", JSHTMLElement::LangAttrNum, DontDelete, 0, 0 },
    { "innerHTML", JSHTMLElement::InnerHTMLAttrNum, DontDelete, 0, 0 },
    { "outerText", JSHTMLElement::OuterTextAttrNum, DontDelete, 0, 0 },
    { "children", JSHTMLElement::ChildrenAttrNum, DontDelete|ReadOnly, 0, 0 }
};

static const HashTable JSHTMLElementTable = 
{
    2, 16, JSHTMLElementTableEntries, 13
};

/* Hash table for constructor */

static const HashEntry JSHTMLElementConstructorTableEntries[] =
{
    { 0, 0, 0, 0, 0 }
};

static const HashTable JSHTMLElementConstructorTable = 
{
    2, 1, JSHTMLElementConstructorTableEntries, 1
};

class JSHTMLElementConstructor : public DOMObject {
public:
    JSHTMLElementConstructor(ExecState* exec)
    {
        setPrototype(exec->lexicalInterpreter()->builtinObjectPrototype());
        putDirect(exec->propertyNames().prototype, JSHTMLElementPrototype::self(exec), None);
    }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    JSValue* getValueProperty(ExecState*, int token) const;
    virtual const ClassInfo* classInfo() const { return &info; }
    static const ClassInfo info;

    virtual bool implementsHasInstance() const { return true; }
};

const ClassInfo JSHTMLElementConstructor::info = { "HTMLElementConstructor", 0, &JSHTMLElementConstructorTable, 0 };

bool JSHTMLElementConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLElementConstructor, DOMObject>(exec, &JSHTMLElementConstructorTable, this, propertyName, slot);
}

JSValue* JSHTMLElementConstructor::getValueProperty(ExecState*, int token) const
{
    // The token is the numeric value of its associated constant
    return jsNumber(token);
}

/* Hash table for prototype */

static const HashEntry JSHTMLElementPrototypeTableEntries[] =
{
    { 0, 0, 0, 0, 0 }
};

static const HashTable JSHTMLElementPrototypeTable = 
{
    2, 1, JSHTMLElementPrototypeTableEntries, 1
};

const ClassInfo JSHTMLElementPrototype::info = { "HTMLElementPrototype", 0, &JSHTMLElementPrototypeTable, 0 };

JSObject* JSHTMLElementPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSHTMLElementPrototype>(exec, "[[JSHTMLElement.prototype]]");
}

const ClassInfo JSHTMLElement::info = { "HTMLElement", &JSElement::info, &JSHTMLElementTable, 0 };

JSHTMLElement::JSHTMLElement(ExecState* exec, HTMLElement* impl)
    : JSElement(exec, impl)
{
    setPrototype(JSHTMLElementPrototype::self(exec));
}

bool JSHTMLElement::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLElement, JSElement>(exec, &JSHTMLElementTable, this, propertyName, slot);
}

JSValue* JSHTMLElement::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case IdAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        return jsString(imp->id());
    }
    case TitleAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        return jsString(imp->title());
    }
    case LangAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        return jsString(imp->lang());
    }
    case DirAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        return jsString(imp->dir());
    }
    case ClassNameAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        return jsString(imp->className());
    }
    case InnerHTMLAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        return jsString(imp->innerHTML());
    }
    case InnerTextAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        return jsString(imp->innerText());
    }
    case OuterHTMLAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        return jsString(imp->outerHTML());
    }
    case OuterTextAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        return jsString(imp->outerText());
    }
    case ChildrenAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        return toJS(exec, WTF::getPtr(imp->children()));
    }
    case ContentEditableAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        return jsString(imp->contentEditable());
    }
    case IsContentEditableAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        return jsBoolean(imp->isContentEditable());
    }
    case ConstructorAttrNum:
        return getConstructor(exec);
    }
    return 0;
}

void JSHTMLElement::put(ExecState* exec, const Identifier& propertyName, JSValue* value, int attr)
{
    lookupPut<JSHTMLElement, JSElement>(exec, propertyName, value, attr, &JSHTMLElementTable, this);
}

void JSHTMLElement::putValueProperty(ExecState* exec, int token, JSValue* value, int /*attr*/)
{
    switch (token) {
    case IdAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        imp->setId(valueToStringWithNullCheck(exec, value));
        break;
    }
    case TitleAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        imp->setTitle(valueToStringWithNullCheck(exec, value));
        break;
    }
    case LangAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        imp->setLang(valueToStringWithNullCheck(exec, value));
        break;
    }
    case DirAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        imp->setDir(valueToStringWithNullCheck(exec, value));
        break;
    }
    case ClassNameAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        imp->setClassName(valueToStringWithNullCheck(exec, value));
        break;
    }
    case InnerHTMLAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        ExceptionCode ec = 0;
        imp->setInnerHTML(valueToStringWithNullCheck(exec, value), ec);
        setDOMException(exec, ec);
        break;
    }
    case InnerTextAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        ExceptionCode ec = 0;
        imp->setInnerText(valueToStringWithNullCheck(exec, value), ec);
        setDOMException(exec, ec);
        break;
    }
    case OuterHTMLAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        ExceptionCode ec = 0;
        imp->setOuterHTML(valueToStringWithNullCheck(exec, value), ec);
        setDOMException(exec, ec);
        break;
    }
    case OuterTextAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        ExceptionCode ec = 0;
        imp->setOuterText(valueToStringWithNullCheck(exec, value), ec);
        setDOMException(exec, ec);
        break;
    }
    case ContentEditableAttrNum: {
        HTMLElement* imp = static_cast<HTMLElement*>(impl());

        imp->setContentEditable(valueToStringWithNullCheck(exec, value));
        break;
    }
    }
}

JSValue* JSHTMLElement::getConstructor(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSHTMLElementConstructor>(exec, "[[HTMLElement.constructor]]");
}
HTMLElement* toHTMLElement(KJS::JSValue* val)
{
    return val->isObject(&JSHTMLElement::info) ? static_cast<JSHTMLElement*>(val)->impl() : 0;
}

HTMLElement* JSHTMLElement::impl() const
{
    return static_cast<HTMLElement*>(JSElement::impl());
}

}
