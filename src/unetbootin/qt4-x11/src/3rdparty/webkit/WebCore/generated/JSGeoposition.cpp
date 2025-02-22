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

#include "JSGeoposition.h"

#include <wtf/GetPtr.h>

#include "Geoposition.h"
#include "KURL.h"

#include <runtime/Error.h>
#include <runtime/JSNumberCell.h>
#include <runtime/JSString.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSGeoposition)

/* Hash table */

static const HashTableValue JSGeopositionTableValues[9] =
{
    { "latitude", DontDelete|ReadOnly, (intptr_t)jsGeopositionLatitude, (intptr_t)0 },
    { "longitude", DontDelete|ReadOnly, (intptr_t)jsGeopositionLongitude, (intptr_t)0 },
    { "altitude", DontDelete|ReadOnly, (intptr_t)jsGeopositionAltitude, (intptr_t)0 },
    { "accuracy", DontDelete|ReadOnly, (intptr_t)jsGeopositionAccuracy, (intptr_t)0 },
    { "altitudeAccuracy", DontDelete|ReadOnly, (intptr_t)jsGeopositionAltitudeAccuracy, (intptr_t)0 },
    { "heading", DontDelete|ReadOnly, (intptr_t)jsGeopositionHeading, (intptr_t)0 },
    { "speed", DontDelete|ReadOnly, (intptr_t)jsGeopositionSpeed, (intptr_t)0 },
    { "timestamp", DontDelete|ReadOnly, (intptr_t)jsGeopositionTimestamp, (intptr_t)0 },
    { 0, 0, 0, 0 }
};

static const HashTable JSGeopositionTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 255, JSGeopositionTableValues, 0 };
#else
    { 17, 15, JSGeopositionTableValues, 0 };
#endif

/* Hash table for prototype */

static const HashTableValue JSGeopositionPrototypeTableValues[2] =
{
    { "toString", DontDelete|DontEnum|Function, (intptr_t)jsGeopositionPrototypeFunctionToString, (intptr_t)0 },
    { 0, 0, 0, 0 }
};

static const HashTable JSGeopositionPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSGeopositionPrototypeTableValues, 0 };
#else
    { 2, 1, JSGeopositionPrototypeTableValues, 0 };
#endif

const ClassInfo JSGeopositionPrototype::s_info = { "GeopositionPrototype", 0, &JSGeopositionPrototypeTable, 0 };

JSObject* JSGeopositionPrototype::self(ExecState* exec)
{
    return getDOMPrototype<JSGeoposition>(exec);
}

bool JSGeopositionPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSObject>(exec, &JSGeopositionPrototypeTable, this, propertyName, slot);
}

const ClassInfo JSGeoposition::s_info = { "Geoposition", 0, &JSGeopositionTable, 0 };

JSGeoposition::JSGeoposition(PassRefPtr<Structure> structure, PassRefPtr<Geoposition> impl)
    : DOMObject(structure)
    , m_impl(impl)
{
}

JSGeoposition::~JSGeoposition()
{
    forgetDOMObject(*Heap::heap(this)->globalData(), m_impl.get());

}

JSObject* JSGeoposition::createPrototype(ExecState* exec)
{
    return new (exec) JSGeopositionPrototype(JSGeopositionPrototype::createStructure(exec->lexicalGlobalObject()->objectPrototype()));
}

bool JSGeoposition::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSGeoposition, Base>(exec, &JSGeopositionTable, this, propertyName, slot);
}

JSValuePtr jsGeopositionLatitude(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    Geoposition* imp = static_cast<Geoposition*>(static_cast<JSGeoposition*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->latitude());
}

JSValuePtr jsGeopositionLongitude(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    Geoposition* imp = static_cast<Geoposition*>(static_cast<JSGeoposition*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->longitude());
}

JSValuePtr jsGeopositionAltitude(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    Geoposition* imp = static_cast<Geoposition*>(static_cast<JSGeoposition*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->altitude());
}

JSValuePtr jsGeopositionAccuracy(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    Geoposition* imp = static_cast<Geoposition*>(static_cast<JSGeoposition*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->accuracy());
}

JSValuePtr jsGeopositionAltitudeAccuracy(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    Geoposition* imp = static_cast<Geoposition*>(static_cast<JSGeoposition*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->altitudeAccuracy());
}

JSValuePtr jsGeopositionHeading(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    Geoposition* imp = static_cast<Geoposition*>(static_cast<JSGeoposition*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->heading());
}

JSValuePtr jsGeopositionSpeed(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    Geoposition* imp = static_cast<Geoposition*>(static_cast<JSGeoposition*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->speed());
}

JSValuePtr jsGeopositionTimestamp(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    Geoposition* imp = static_cast<Geoposition*>(static_cast<JSGeoposition*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->timestamp());
}

JSValuePtr jsGeopositionPrototypeFunctionToString(ExecState* exec, JSObject*, JSValuePtr thisValue, const ArgList& args)
{
    if (!thisValue->isObject(&JSGeoposition::s_info))
        return throwError(exec, TypeError);
    JSGeoposition* castedThisObj = static_cast<JSGeoposition*>(asObject(thisValue));
    Geoposition* imp = static_cast<Geoposition*>(castedThisObj->impl());


    JSC::JSValuePtr result = jsString(exec, imp->toString());
    return result;
}

JSC::JSValuePtr toJS(JSC::ExecState* exec, Geoposition* object)
{
    return getDOMObjectWrapper<JSGeoposition>(exec, object);
}
Geoposition* toGeoposition(JSC::JSValuePtr value)
{
    return value->isObject(&JSGeoposition::s_info) ? static_cast<JSGeoposition*>(asObject(value))->impl() : 0;
}

}
