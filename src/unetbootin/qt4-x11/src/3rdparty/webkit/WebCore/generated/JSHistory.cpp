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

#include "JSHistory.h"

#include <wtf/GetPtr.h>

#include "History.h"
#include "JSHistoryCustom.h"

#include <runtime/Error.h>
#include <runtime/JSNumberCell.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSHistory)

/* Hash table */

static const HashTableValue JSHistoryTableValues[2] =
{
    { "length", DontDelete|ReadOnly, (intptr_t)jsHistoryLength, (intptr_t)0 },
    { 0, 0, 0, 0 }
};

static const HashTable JSHistoryTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSHistoryTableValues, 0 };
#else
    { 2, 1, JSHistoryTableValues, 0 };
#endif

/* Hash table for prototype */

static const HashTableValue JSHistoryPrototypeTableValues[4] =
{
    { "back", DontDelete|Function, (intptr_t)jsHistoryPrototypeFunctionBack, (intptr_t)0 },
    { "forward", DontDelete|Function, (intptr_t)jsHistoryPrototypeFunctionForward, (intptr_t)0 },
    { "go", DontDelete|Function, (intptr_t)jsHistoryPrototypeFunctionGo, (intptr_t)1 },
    { 0, 0, 0, 0 }
};

static const HashTable JSHistoryPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 31, JSHistoryPrototypeTableValues, 0 };
#else
    { 9, 7, JSHistoryPrototypeTableValues, 0 };
#endif

const ClassInfo JSHistoryPrototype::s_info = { "HistoryPrototype", 0, &JSHistoryPrototypeTable, 0 };

JSObject* JSHistoryPrototype::self(ExecState* exec)
{
    return getDOMPrototype<JSHistory>(exec);
}

bool JSHistoryPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSObject>(exec, &JSHistoryPrototypeTable, this, propertyName, slot);
}

const ClassInfo JSHistory::s_info = { "History", 0, &JSHistoryTable, 0 };

JSHistory::JSHistory(PassRefPtr<Structure> structure, PassRefPtr<History> impl)
    : DOMObject(structure)
    , m_impl(impl)
{
}

JSHistory::~JSHistory()
{
    forgetDOMObject(*Heap::heap(this)->globalData(), m_impl.get());

}

JSObject* JSHistory::createPrototype(ExecState* exec)
{
    return new (exec) JSHistoryPrototype(JSHistoryPrototype::createStructure(exec->lexicalGlobalObject()->objectPrototype()));
}

bool JSHistory::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    if (customGetOwnPropertySlot(exec, propertyName, slot))
        return true;
    return getStaticValueSlot<JSHistory, Base>(exec, &JSHistoryTable, this, propertyName, slot);
}

JSValuePtr jsHistoryLength(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    History* imp = static_cast<History*>(static_cast<JSHistory*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->length());
}

void JSHistory::put(ExecState* exec, const Identifier& propertyName, JSValuePtr value, PutPropertySlot& slot)
{
    if (customPut(exec, propertyName, value, slot))
        return;
    Base::put(exec, propertyName, value, slot);
}

void JSHistory::getPropertyNames(ExecState* exec, PropertyNameArray& propertyNames)
{
    if (customGetPropertyNames(exec, propertyNames))
        return;
     Base::getPropertyNames(exec, propertyNames);
}

JSValuePtr jsHistoryPrototypeFunctionBack(ExecState* exec, JSObject*, JSValuePtr thisValue, const ArgList& args)
{
    if (!thisValue->isObject(&JSHistory::s_info))
        return throwError(exec, TypeError);
    JSHistory* castedThisObj = static_cast<JSHistory*>(asObject(thisValue));
    History* imp = static_cast<History*>(castedThisObj->impl());

    imp->back();
    return jsUndefined();
}

JSValuePtr jsHistoryPrototypeFunctionForward(ExecState* exec, JSObject*, JSValuePtr thisValue, const ArgList& args)
{
    if (!thisValue->isObject(&JSHistory::s_info))
        return throwError(exec, TypeError);
    JSHistory* castedThisObj = static_cast<JSHistory*>(asObject(thisValue));
    History* imp = static_cast<History*>(castedThisObj->impl());

    imp->forward();
    return jsUndefined();
}

JSValuePtr jsHistoryPrototypeFunctionGo(ExecState* exec, JSObject*, JSValuePtr thisValue, const ArgList& args)
{
    if (!thisValue->isObject(&JSHistory::s_info))
        return throwError(exec, TypeError);
    JSHistory* castedThisObj = static_cast<JSHistory*>(asObject(thisValue));
    History* imp = static_cast<History*>(castedThisObj->impl());
    int distance = args.at(exec, 0)->toInt32(exec);

    imp->go(distance);
    return jsUndefined();
}

JSC::JSValuePtr toJS(JSC::ExecState* exec, History* object)
{
    return getDOMObjectWrapper<JSHistory>(exec, object);
}
History* toHistory(JSC::JSValuePtr value)
{
    return value->isObject(&JSHistory::s_info) ? static_cast<JSHistory*>(asObject(value))->impl() : 0;
}

}
