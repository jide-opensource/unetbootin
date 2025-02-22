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


#if ENABLE(SVG)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "JSSVGTransformList.h"

#include <wtf/GetPtr.h>

#include "ExceptionCode.h"
#include "JSSVGMatrix.h"
#include "JSSVGTransform.h"
#include "SVGTransform.h"
#include "SVGTransformList.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSSVGTransformListTableEntries[] =
{
    { "numberOfItems", JSSVGTransformList::NumberOfItemsAttrNum, DontDelete|ReadOnly, 0, 0 }
};

static const HashTable JSSVGTransformListTable = 
{
    2, 1, JSSVGTransformListTableEntries, 1
};

/* Hash table for prototype */

static const HashEntry JSSVGTransformListPrototypeTableEntries[] =
{
    { "removeItem", JSSVGTransformList::RemoveItemFuncNum, DontDelete|Function, 1, 0 },
    { "insertItemBefore", JSSVGTransformList::InsertItemBeforeFuncNum, DontDelete|Function, 2, &JSSVGTransformListPrototypeTableEntries[10] },
    { "initialize", JSSVGTransformList::InitializeFuncNum, DontDelete|Function, 1, &JSSVGTransformListPrototypeTableEntries[12] },
    { 0, 0, 0, 0, 0 },
    { "clear", JSSVGTransformList::ClearFuncNum, DontDelete|Function, 0, &JSSVGTransformListPrototypeTableEntries[9] },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "appendItem", JSSVGTransformList::AppendItemFuncNum, DontDelete|Function, 1, &JSSVGTransformListPrototypeTableEntries[11] },
    { "getItem", JSSVGTransformList::GetItemFuncNum, DontDelete|Function, 1, 0 },
    { "replaceItem", JSSVGTransformList::ReplaceItemFuncNum, DontDelete|Function, 2, 0 },
    { "createSVGTransformFromMatrix", JSSVGTransformList::CreateSVGTransformFromMatrixFuncNum, DontDelete|Function, 1, 0 },
    { "consolidate", JSSVGTransformList::ConsolidateFuncNum, DontDelete|Function, 0, 0 }
};

static const HashTable JSSVGTransformListPrototypeTable = 
{
    2, 13, JSSVGTransformListPrototypeTableEntries, 9
};

const ClassInfo JSSVGTransformListPrototype::info = { "SVGTransformListPrototype", 0, &JSSVGTransformListPrototypeTable, 0 };

JSObject* JSSVGTransformListPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSSVGTransformListPrototype>(exec, "[[JSSVGTransformList.prototype]]");
}

bool JSSVGTransformListPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSSVGTransformListPrototypeFunction, JSObject>(exec, &JSSVGTransformListPrototypeTable, this, propertyName, slot);
}

const ClassInfo JSSVGTransformList::info = { "SVGTransformList", 0, &JSSVGTransformListTable, 0 };

JSSVGTransformList::JSSVGTransformList(ExecState* exec, SVGTransformList* impl)
    : m_impl(impl)
{
    setPrototype(JSSVGTransformListPrototype::self(exec));
}

JSSVGTransformList::~JSSVGTransformList()
{
    ScriptInterpreter::forgetDOMObject(m_impl.get());
}

bool JSSVGTransformList::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGTransformList, KJS::DOMObject>(exec, &JSSVGTransformListTable, this, propertyName, slot);
}

JSValue* JSSVGTransformList::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case NumberOfItemsAttrNum: {
        SVGTransformList* imp = static_cast<SVGTransformList*>(impl());

        return jsNumber(imp->numberOfItems());
    }
    }
    return 0;
}

JSValue* JSSVGTransformListPrototypeFunction::callAsFunction(ExecState* exec, JSObject* thisObj, const List& args)
{
    if (!thisObj->inherits(&JSSVGTransformList::info))
      return throwError(exec, TypeError);

    SVGTransformList* imp = static_cast<SVGTransformList*>(static_cast<JSSVGTransformList*>(thisObj)->impl());

    switch (id) {
    case JSSVGTransformList::ClearFuncNum: {
        ExceptionCode ec = 0;

        imp->clear(ec);
        setDOMException(exec, ec);
        return jsUndefined();
    }
    case JSSVGTransformList::InitializeFuncNum: {
        ExceptionCode ec = 0;
        SVGTransform item = toSVGTransform(args[0]);


        KJS::JSValue* result = toJS(exec, new JSSVGPODTypeWrapper<SVGTransform>(imp->initialize(item, ec)));
        setDOMException(exec, ec);
        return result;
    }
    case JSSVGTransformList::GetItemFuncNum: {
        ExceptionCode ec = 0;
        bool indexOk;
        unsigned index = args[0]->toInt32(exec, indexOk);
        if (!indexOk) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }


        KJS::JSValue* result = toJS(exec, new JSSVGPODTypeWrapper<SVGTransform>(imp->getItem(index, ec)));
        setDOMException(exec, ec);
        return result;
    }
    case JSSVGTransformList::InsertItemBeforeFuncNum: {
        ExceptionCode ec = 0;
        SVGTransform item = toSVGTransform(args[0]);
        bool indexOk;
        unsigned index = args[1]->toInt32(exec, indexOk);
        if (!indexOk) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }


        KJS::JSValue* result = toJS(exec, new JSSVGPODTypeWrapper<SVGTransform>(imp->insertItemBefore(item, index, ec)));
        setDOMException(exec, ec);
        return result;
    }
    case JSSVGTransformList::ReplaceItemFuncNum: {
        ExceptionCode ec = 0;
        SVGTransform item = toSVGTransform(args[0]);
        bool indexOk;
        unsigned index = args[1]->toInt32(exec, indexOk);
        if (!indexOk) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }


        KJS::JSValue* result = toJS(exec, new JSSVGPODTypeWrapper<SVGTransform>(imp->replaceItem(item, index, ec)));
        setDOMException(exec, ec);
        return result;
    }
    case JSSVGTransformList::RemoveItemFuncNum: {
        ExceptionCode ec = 0;
        bool indexOk;
        unsigned index = args[0]->toInt32(exec, indexOk);
        if (!indexOk) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }


        KJS::JSValue* result = toJS(exec, new JSSVGPODTypeWrapper<SVGTransform>(imp->removeItem(index, ec)));
        setDOMException(exec, ec);
        return result;
    }
    case JSSVGTransformList::AppendItemFuncNum: {
        ExceptionCode ec = 0;
        SVGTransform item = toSVGTransform(args[0]);


        KJS::JSValue* result = toJS(exec, new JSSVGPODTypeWrapper<SVGTransform>(imp->appendItem(item, ec)));
        setDOMException(exec, ec);
        return result;
    }
    case JSSVGTransformList::CreateSVGTransformFromMatrixFuncNum: {
        AffineTransform matrix = toSVGMatrix(args[0]);


        KJS::JSValue* result = toJS(exec, new JSSVGPODTypeWrapper<SVGTransform>(imp->createSVGTransformFromMatrix(matrix)));
        return result;
    }
    case JSSVGTransformList::ConsolidateFuncNum: {


        KJS::JSValue* result = toJS(exec, new JSSVGPODTypeWrapper<SVGTransform>(imp->consolidate()));
        return result;
    }
    }
    return 0;
}
KJS::JSValue* toJS(KJS::ExecState* exec, SVGTransformList* obj)
{
    return KJS::cacheDOMObject<SVGTransformList, JSSVGTransformList>(exec, obj);
}
SVGTransformList* toSVGTransformList(KJS::JSValue* val)
{
    return val->isObject(&JSSVGTransformList::info) ? static_cast<JSSVGTransformList*>(val)->impl() : 0;
}

}

#endif // ENABLE(SVG)
