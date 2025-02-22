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

#include "JSMouseEvent.h"

#include <wtf/GetPtr.h>

#include "Clipboard.h"
#include "EventTargetNode.h"
#include "ExceptionCode.h"
#include "JSDOMWindow.h"
#include "JSEventTargetNode.h"
#include "JSNode.h"
#include "MouseEvent.h"
#include "Node.h"
#include "kjs_dom.h"
#include "kjs_events.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSMouseEventTableEntries[] =
{
    { "metaKey", JSMouseEvent::MetaKeyAttrNum, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "toElement", JSMouseEvent::ToElementAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "relatedTarget", JSMouseEvent::RelatedTargetAttrNum, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "x", JSMouseEvent::XAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "clientY", JSMouseEvent::ClientYAttrNum, DontDelete|ReadOnly, 0, &JSMouseEventTableEntries[19] },
    { "screenX", JSMouseEvent::ScreenXAttrNum, DontDelete|ReadOnly, 0, &JSMouseEventTableEntries[20] },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "screenY", JSMouseEvent::ScreenYAttrNum, DontDelete|ReadOnly, 0, &JSMouseEventTableEntries[17] },
    { 0, 0, 0, 0, 0 },
    { "dataTransfer", JSMouseEvent::DataTransferAttrNum, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "offsetY", JSMouseEvent::OffsetYAttrNum, DontDelete|ReadOnly, 0, &JSMouseEventTableEntries[24] },
    { "clientX", JSMouseEvent::ClientXAttrNum, DontDelete|ReadOnly, 0, &JSMouseEventTableEntries[18] },
    { "ctrlKey", JSMouseEvent::CtrlKeyAttrNum, DontDelete|ReadOnly, 0, &JSMouseEventTableEntries[21] },
    { "shiftKey", JSMouseEvent::ShiftKeyAttrNum, DontDelete|ReadOnly, 0, &JSMouseEventTableEntries[22] },
    { "altKey", JSMouseEvent::AltKeyAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "button", JSMouseEvent::ButtonAttrNum, DontDelete|ReadOnly, 0, &JSMouseEventTableEntries[23] },
    { "offsetX", JSMouseEvent::OffsetXAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "y", JSMouseEvent::YAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "fromElement", JSMouseEvent::FromElementAttrNum, DontDelete|ReadOnly, 0, 0 }
};

static const HashTable JSMouseEventTable = 
{
    2, 25, JSMouseEventTableEntries, 17
};

/* Hash table for prototype */

static const HashEntry JSMouseEventPrototypeTableEntries[] =
{
    { "initMouseEvent", JSMouseEvent::InitMouseEventFuncNum, DontDelete|Function, 15, 0 }
};

static const HashTable JSMouseEventPrototypeTable = 
{
    2, 1, JSMouseEventPrototypeTableEntries, 1
};

const ClassInfo JSMouseEventPrototype::info = { "MouseEventPrototype", 0, &JSMouseEventPrototypeTable, 0 };

JSObject* JSMouseEventPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSMouseEventPrototype>(exec, "[[JSMouseEvent.prototype]]");
}

bool JSMouseEventPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSMouseEventPrototypeFunction, JSObject>(exec, &JSMouseEventPrototypeTable, this, propertyName, slot);
}

const ClassInfo JSMouseEvent::info = { "MouseEvent", &JSUIEvent::info, &JSMouseEventTable, 0 };

JSMouseEvent::JSMouseEvent(ExecState* exec, MouseEvent* impl)
    : JSUIEvent(exec, impl)
{
    setPrototype(JSMouseEventPrototype::self(exec));
}

bool JSMouseEvent::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSMouseEvent, JSUIEvent>(exec, &JSMouseEventTable, this, propertyName, slot);
}

JSValue* JSMouseEvent::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case ScreenXAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return jsNumber(imp->screenX());
    }
    case ScreenYAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return jsNumber(imp->screenY());
    }
    case ClientXAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return jsNumber(imp->clientX());
    }
    case ClientYAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return jsNumber(imp->clientY());
    }
    case CtrlKeyAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return jsBoolean(imp->ctrlKey());
    }
    case ShiftKeyAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return jsBoolean(imp->shiftKey());
    }
    case AltKeyAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return jsBoolean(imp->altKey());
    }
    case MetaKeyAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return jsBoolean(imp->metaKey());
    }
    case ButtonAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return jsNumber(imp->button());
    }
    case RelatedTargetAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return toJS(exec, WTF::getPtr(imp->relatedTarget()));
    }
    case OffsetXAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return jsNumber(imp->offsetX());
    }
    case OffsetYAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return jsNumber(imp->offsetY());
    }
    case XAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return jsNumber(imp->x());
    }
    case YAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return jsNumber(imp->y());
    }
    case FromElementAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return toJS(exec, WTF::getPtr(imp->fromElement()));
    }
    case ToElementAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return toJS(exec, WTF::getPtr(imp->toElement()));
    }
    case DataTransferAttrNum: {
        MouseEvent* imp = static_cast<MouseEvent*>(impl());

        return toJS(exec, WTF::getPtr(imp->dataTransfer()));
    }
    }
    return 0;
}

JSValue* JSMouseEventPrototypeFunction::callAsFunction(ExecState* exec, JSObject* thisObj, const List& args)
{
    if (!thisObj->inherits(&JSMouseEvent::info))
      return throwError(exec, TypeError);

    MouseEvent* imp = static_cast<MouseEvent*>(static_cast<JSMouseEvent*>(thisObj)->impl());

    switch (id) {
    case JSMouseEvent::InitMouseEventFuncNum: {
        AtomicString type = args[0]->toString(exec);
        bool canBubble = args[1]->toBoolean(exec);
        bool cancelable = args[2]->toBoolean(exec);
        DOMWindow* view = toDOMWindow(args[3]);
        bool detailOk;
        int detail = args[4]->toInt32(exec, detailOk);
        if (!detailOk) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }
        bool screenXOk;
        int screenX = args[5]->toInt32(exec, screenXOk);
        if (!screenXOk) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }
        bool screenYOk;
        int screenY = args[6]->toInt32(exec, screenYOk);
        if (!screenYOk) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }
        bool clientXOk;
        int clientX = args[7]->toInt32(exec, clientXOk);
        if (!clientXOk) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }
        bool clientYOk;
        int clientY = args[8]->toInt32(exec, clientYOk);
        if (!clientYOk) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }
        bool ctrlKey = args[9]->toBoolean(exec);
        bool altKey = args[10]->toBoolean(exec);
        bool shiftKey = args[11]->toBoolean(exec);
        bool metaKey = args[12]->toBoolean(exec);
        unsigned short button = args[13]->toInt32(exec);
        EventTargetNode* relatedTarget = toEventTargetNode(args[14]);

        imp->initMouseEvent(type, canBubble, cancelable, view, detail, screenX, screenY, clientX, clientY, ctrlKey, altKey, shiftKey, metaKey, button, relatedTarget);
        return jsUndefined();
    }
    }
    return 0;
}

}
