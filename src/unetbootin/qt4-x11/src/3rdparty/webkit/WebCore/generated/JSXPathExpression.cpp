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


#if ENABLE(XPATH)

#include "JSXPathExpression.h"

#include <wtf/GetPtr.h>

#include "JSNode.h"
#include "JSXPathResult.h"
#include "XPathExpression.h"
#include "XPathResult.h"

#include <runtime/Error.h>
#include <runtime/JSNumberCell.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSXPathExpression)

/* Hash table */

static const HashTableValue JSXPathExpressionTableValues[2] =
{
    { "constructor", DontEnum|ReadOnly, (intptr_t)jsXPathExpressionConstructor, (intptr_t)0 },
    { 0, 0, 0, 0 }
};

static const HashTable JSXPathExpressionTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSXPathExpressionTableValues, 0 };
#else
    { 2, 1, JSXPathExpressionTableValues, 0 };
#endif

/* Hash table for constructor */

static const HashTableValue JSXPathExpressionConstructorTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static const HashTable JSXPathExpressionConstructorTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSXPathExpressionConstructorTableValues, 0 };
#else
    { 1, 0, JSXPathExpressionConstructorTableValues, 0 };
#endif

class JSXPathExpressionConstructor : public DOMObject {
public:
    JSXPathExpressionConstructor(ExecState* exec)
        : DOMObject(JSXPathExpressionConstructor::createStructure(exec->lexicalGlobalObject()->objectPrototype()))
    {
        putDirect(exec->propertyNames().prototype, JSXPathExpressionPrototype::self(exec), None);
    }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    virtual const ClassInfo* classInfo() const { return &s_info; }
    static const ClassInfo s_info;

    static PassRefPtr<Structure> createStructure(JSValuePtr proto) 
    { 
        return Structure::create(proto, TypeInfo(ObjectType, ImplementsHasInstance)); 
    }
};

const ClassInfo JSXPathExpressionConstructor::s_info = { "XPathExpressionConstructor", 0, &JSXPathExpressionConstructorTable, 0 };

bool JSXPathExpressionConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSXPathExpressionConstructor, DOMObject>(exec, &JSXPathExpressionConstructorTable, this, propertyName, slot);
}

/* Hash table for prototype */

static const HashTableValue JSXPathExpressionPrototypeTableValues[2] =
{
    { "evaluate", DontDelete|Function, (intptr_t)jsXPathExpressionPrototypeFunctionEvaluate, (intptr_t)3 },
    { 0, 0, 0, 0 }
};

static const HashTable JSXPathExpressionPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSXPathExpressionPrototypeTableValues, 0 };
#else
    { 2, 1, JSXPathExpressionPrototypeTableValues, 0 };
#endif

const ClassInfo JSXPathExpressionPrototype::s_info = { "XPathExpressionPrototype", 0, &JSXPathExpressionPrototypeTable, 0 };

JSObject* JSXPathExpressionPrototype::self(ExecState* exec)
{
    return getDOMPrototype<JSXPathExpression>(exec);
}

bool JSXPathExpressionPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSObject>(exec, &JSXPathExpressionPrototypeTable, this, propertyName, slot);
}

const ClassInfo JSXPathExpression::s_info = { "XPathExpression", 0, &JSXPathExpressionTable, 0 };

JSXPathExpression::JSXPathExpression(PassRefPtr<Structure> structure, PassRefPtr<XPathExpression> impl)
    : DOMObject(structure)
    , m_impl(impl)
{
}

JSXPathExpression::~JSXPathExpression()
{
    forgetDOMObject(*Heap::heap(this)->globalData(), m_impl.get());

}

JSObject* JSXPathExpression::createPrototype(ExecState* exec)
{
    return new (exec) JSXPathExpressionPrototype(JSXPathExpressionPrototype::createStructure(exec->lexicalGlobalObject()->objectPrototype()));
}

bool JSXPathExpression::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSXPathExpression, Base>(exec, &JSXPathExpressionTable, this, propertyName, slot);
}

JSValuePtr jsXPathExpressionConstructor(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    return static_cast<JSXPathExpression*>(asObject(slot.slotBase()))->getConstructor(exec);
}
JSValuePtr JSXPathExpression::getConstructor(ExecState* exec)
{
    return getDOMConstructor<JSXPathExpressionConstructor>(exec);
}

JSValuePtr jsXPathExpressionPrototypeFunctionEvaluate(ExecState* exec, JSObject*, JSValuePtr thisValue, const ArgList& args)
{
    if (!thisValue->isObject(&JSXPathExpression::s_info))
        return throwError(exec, TypeError);
    JSXPathExpression* castedThisObj = static_cast<JSXPathExpression*>(asObject(thisValue));
    XPathExpression* imp = static_cast<XPathExpression*>(castedThisObj->impl());
    ExceptionCode ec = 0;
    Node* contextNode = toNode(args.at(exec, 0));
    unsigned short type = args.at(exec, 1)->toInt32(exec);
    XPathResult* inResult = toXPathResult(args.at(exec, 2));


    JSC::JSValuePtr result = toJS(exec, WTF::getPtr(imp->evaluate(contextNode, type, inResult, ec)));
    setDOMException(exec, ec);
    return result;
}

JSC::JSValuePtr toJS(JSC::ExecState* exec, XPathExpression* object)
{
    return getDOMObjectWrapper<JSXPathExpression>(exec, object);
}
XPathExpression* toXPathExpression(JSC::JSValuePtr value)
{
    return value->isObject(&JSXPathExpression::s_info) ? static_cast<JSXPathExpression*>(asObject(value))->impl() : 0;
}

}

#endif // ENABLE(XPATH)
