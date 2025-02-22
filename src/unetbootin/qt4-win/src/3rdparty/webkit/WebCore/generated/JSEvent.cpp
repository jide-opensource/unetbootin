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

#include "JSEvent.h"

#include <wtf/GetPtr.h>

#include "Event.h"
#include "EventTargetNode.h"
#include "JSEventTargetNode.h"
#include "PlatformString.h"
#include "kjs_dom.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSEventTableEntries[] =
{
    { 0, 0, 0, 0, 0 },
    { "srcElement", JSEvent::SrcElementAttrNum, DontDelete|ReadOnly, 0, &JSEventTableEntries[16] },
    { "bubbles", JSEvent::BubblesAttrNum, DontDelete|ReadOnly, 0, &JSEventTableEntries[13] },
    { "currentTarget", JSEvent::CurrentTargetAttrNum, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "type", JSEvent::TypeAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "returnValue", JSEvent::ReturnValueAttrNum, DontDelete, 0, 0 },
    { "constructor", JSEvent::ConstructorAttrNum, DontDelete|DontEnum|ReadOnly, 0, 0 },
    { "target", JSEvent::TargetAttrNum, DontDelete|ReadOnly, 0, &JSEventTableEntries[12] },
    { 0, 0, 0, 0, 0 },
    { "eventPhase", JSEvent::EventPhaseAttrNum, DontDelete|ReadOnly, 0, &JSEventTableEntries[14] },
    { "cancelable", JSEvent::CancelableAttrNum, DontDelete|ReadOnly, 0, &JSEventTableEntries[15] },
    { "timeStamp", JSEvent::TimeStampAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "cancelBubble", JSEvent::CancelBubbleAttrNum, DontDelete, 0, 0 },
    { "clipboardData", JSEvent::ClipboardDataAttrNum, DontDelete|ReadOnly, 0, 0 }
};

static const HashTable JSEventTable = 
{
    2, 17, JSEventTableEntries, 12
};

/* Hash table for constructor */

static const HashEntry JSEventConstructorTableEntries[] =
{
    { "MOUSEOUT", Event::MOUSEOUT, DontDelete|ReadOnly, 0, 0 },
    { "MOUSEOVER", Event::MOUSEOVER, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "CAPTURING_PHASE", Event::CAPTURING_PHASE, DontDelete|ReadOnly, 0, &JSEventConstructorTableEntries[23] },
    { 0, 0, 0, 0, 0 },
    { "MOUSEDOWN", Event::MOUSEDOWN, DontDelete|ReadOnly, 0, &JSEventConstructorTableEntries[22] },
    { "AT_TARGET", Event::AT_TARGET, DontDelete|ReadOnly, 0, &JSEventConstructorTableEntries[19] },
    { 0, 0, 0, 0, 0 },
    { "DRAGDROP", Event::DRAGDROP, DontDelete|ReadOnly, 0, 0 },
    { "KEYUP", Event::KEYUP, DontDelete|ReadOnly, 0, 0 },
    { "SELECT", Event::SELECT, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "BLUR", Event::BLUR, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "MOUSEMOVE", Event::MOUSEMOVE, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "CLICK", Event::CLICK, DontDelete|ReadOnly, 0, &JSEventConstructorTableEntries[20] },
    { "MOUSEUP", Event::MOUSEUP, DontDelete|ReadOnly, 0, &JSEventConstructorTableEntries[21] },
    { "BUBBLING_PHASE", Event::BUBBLING_PHASE, DontDelete|ReadOnly, 0, 0 },
    { "MOUSEDRAG", Event::MOUSEDRAG, DontDelete|ReadOnly, 0, 0 },
    { "DBLCLICK", Event::DBLCLICK, DontDelete|ReadOnly, 0, &JSEventConstructorTableEntries[24] },
    { "KEYDOWN", Event::KEYDOWN, DontDelete|ReadOnly, 0, 0 },
    { "KEYPRESS", Event::KEYPRESS, DontDelete|ReadOnly, 0, 0 },
    { "FOCUS", Event::FOCUS, DontDelete|ReadOnly, 0, 0 },
    { "CHANGE", Event::CHANGE, DontDelete|ReadOnly, 0, 0 }
};

static const HashTable JSEventConstructorTable = 
{
    2, 25, JSEventConstructorTableEntries, 19
};

class JSEventConstructor : public DOMObject {
public:
    JSEventConstructor(ExecState* exec)
    {
        setPrototype(exec->lexicalInterpreter()->builtinObjectPrototype());
        putDirect(exec->propertyNames().prototype, JSEventPrototype::self(exec), None);
    }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    JSValue* getValueProperty(ExecState*, int token) const;
    virtual const ClassInfo* classInfo() const { return &info; }
    static const ClassInfo info;

    virtual bool implementsHasInstance() const { return true; }
};

const ClassInfo JSEventConstructor::info = { "EventConstructor", 0, &JSEventConstructorTable, 0 };

bool JSEventConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSEventConstructor, DOMObject>(exec, &JSEventConstructorTable, this, propertyName, slot);
}

JSValue* JSEventConstructor::getValueProperty(ExecState*, int token) const
{
    // The token is the numeric value of its associated constant
    return jsNumber(token);
}

/* Hash table for prototype */

static const HashEntry JSEventPrototypeTableEntries[] =
{
    { "BUBBLING_PHASE", Event::BUBBLING_PHASE, DontDelete|ReadOnly, 0, &JSEventPrototypeTableEntries[27] },
    { 0, 0, 0, 0, 0 },
    { "MOUSEMOVE", Event::MOUSEMOVE, DontDelete|ReadOnly, 0, &JSEventPrototypeTableEntries[24] },
    { "FOCUS", Event::FOCUS, DontDelete|ReadOnly, 0, 0 },
    { "MOUSEOUT", Event::MOUSEOUT, DontDelete|ReadOnly, 0, 0 },
    { "CHANGE", Event::CHANGE, DontDelete|ReadOnly, 0, 0 },
    { "BLUR", Event::BLUR, DontDelete|ReadOnly, 0, 0 },
    { "MOUSEOVER", Event::MOUSEOVER, DontDelete|ReadOnly, 0, 0 },
    { "KEYDOWN", Event::KEYDOWN, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "DRAGDROP", Event::DRAGDROP, DontDelete|ReadOnly, 0, 0 },
    { "stopPropagation", JSEvent::StopPropagationFuncNum, DontDelete|Function, 0, 0 },
    { "AT_TARGET", Event::AT_TARGET, DontDelete|ReadOnly, 0, &JSEventPrototypeTableEntries[23] },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "MOUSEUP", Event::MOUSEUP, DontDelete|ReadOnly, 0, 0 },
    { "CAPTURING_PHASE", Event::CAPTURING_PHASE, DontDelete|ReadOnly, 0, &JSEventPrototypeTableEntries[22] },
    { "SELECT", Event::SELECT, DontDelete|ReadOnly, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "CLICK", Event::CLICK, DontDelete|ReadOnly, 0, 0 },
    { "KEYUP", Event::KEYUP, DontDelete|ReadOnly, 0, 0 },
    { "MOUSEDOWN", Event::MOUSEDOWN, DontDelete|ReadOnly, 0, 0 },
    { "MOUSEDRAG", Event::MOUSEDRAG, DontDelete|ReadOnly, 0, &JSEventPrototypeTableEntries[25] },
    { "DBLCLICK", Event::DBLCLICK, DontDelete|ReadOnly, 0, &JSEventPrototypeTableEntries[26] },
    { "KEYPRESS", Event::KEYPRESS, DontDelete|ReadOnly, 0, 0 },
    { "preventDefault", JSEvent::PreventDefaultFuncNum, DontDelete|Function, 0, 0 },
    { "initEvent", JSEvent::InitEventFuncNum, DontDelete|Function, 3, 0 }
};

static const HashTable JSEventPrototypeTable = 
{
    2, 28, JSEventPrototypeTableEntries, 22
};

const ClassInfo JSEventPrototype::info = { "EventPrototype", 0, &JSEventPrototypeTable, 0 };

JSObject* JSEventPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSEventPrototype>(exec, "[[JSEvent.prototype]]");
}

bool JSEventPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticPropertySlot<JSEventPrototypeFunction, JSEventPrototype, JSObject>(exec, &JSEventPrototypeTable, this, propertyName, slot);
}

JSValue* JSEventPrototype::getValueProperty(ExecState*, int token) const
{
    // The token is the numeric value of its associated constant
    return jsNumber(token);
}

const ClassInfo JSEvent::info = { "Event", 0, &JSEventTable, 0 };

JSEvent::JSEvent(ExecState* exec, Event* impl)
    : m_impl(impl)
{
    setPrototype(JSEventPrototype::self(exec));
}

JSEvent::~JSEvent()
{
    ScriptInterpreter::forgetDOMObject(m_impl.get());
}

bool JSEvent::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSEvent, KJS::DOMObject>(exec, &JSEventTable, this, propertyName, slot);
}

JSValue* JSEvent::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case TypeAttrNum: {
        Event* imp = static_cast<Event*>(impl());

        return jsString(imp->type());
    }
    case TargetAttrNum: {
        Event* imp = static_cast<Event*>(impl());

        return toJS(exec, WTF::getPtr(imp->target()));
    }
    case CurrentTargetAttrNum: {
        Event* imp = static_cast<Event*>(impl());

        return toJS(exec, WTF::getPtr(imp->currentTarget()));
    }
    case EventPhaseAttrNum: {
        Event* imp = static_cast<Event*>(impl());

        return jsNumber(imp->eventPhase());
    }
    case BubblesAttrNum: {
        Event* imp = static_cast<Event*>(impl());

        return jsBoolean(imp->bubbles());
    }
    case CancelableAttrNum: {
        Event* imp = static_cast<Event*>(impl());

        return jsBoolean(imp->cancelable());
    }
    case TimeStampAttrNum: {
        Event* imp = static_cast<Event*>(impl());

        return jsNumber(imp->timeStamp());
    }
    case SrcElementAttrNum: {
        Event* imp = static_cast<Event*>(impl());

        return toJS(exec, WTF::getPtr(imp->srcElement()));
    }
    case ReturnValueAttrNum: {
        Event* imp = static_cast<Event*>(impl());

        return jsBoolean(imp->returnValue());
    }
    case CancelBubbleAttrNum: {
        Event* imp = static_cast<Event*>(impl());

        return jsBoolean(imp->cancelBubble());
    }
    case ClipboardDataAttrNum: {
        return clipboardData(exec);
    }
    case ConstructorAttrNum:
        return getConstructor(exec);
    }
    return 0;
}

void JSEvent::put(ExecState* exec, const Identifier& propertyName, JSValue* value, int attr)
{
    lookupPut<JSEvent, KJS::DOMObject>(exec, propertyName, value, attr, &JSEventTable, this);
}

void JSEvent::putValueProperty(ExecState* exec, int token, JSValue* value, int /*attr*/)
{
    switch (token) {
    case ReturnValueAttrNum: {
        Event* imp = static_cast<Event*>(impl());

        imp->setReturnValue(value->toBoolean(exec));
        break;
    }
    case CancelBubbleAttrNum: {
        Event* imp = static_cast<Event*>(impl());

        imp->setCancelBubble(value->toBoolean(exec));
        break;
    }
    }
}

JSValue* JSEvent::getConstructor(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSEventConstructor>(exec, "[[Event.constructor]]");
}
JSValue* JSEventPrototypeFunction::callAsFunction(ExecState* exec, JSObject* thisObj, const List& args)
{
    if (!thisObj->inherits(&JSEvent::info))
      return throwError(exec, TypeError);

    Event* imp = static_cast<Event*>(static_cast<JSEvent*>(thisObj)->impl());

    switch (id) {
    case JSEvent::StopPropagationFuncNum: {

        imp->stopPropagation();
        return jsUndefined();
    }
    case JSEvent::PreventDefaultFuncNum: {

        imp->preventDefault();
        return jsUndefined();
    }
    case JSEvent::InitEventFuncNum: {
        String eventTypeArg = args[0]->toString(exec);
        bool canBubbleArg = args[1]->toBoolean(exec);
        bool cancelableArg = args[2]->toBoolean(exec);

        imp->initEvent(eventTypeArg, canBubbleArg, cancelableArg);
        return jsUndefined();
    }
    }
    return 0;
}
Event* toEvent(KJS::JSValue* val)
{
    return val->isObject(&JSEvent::info) ? static_cast<JSEvent*>(val)->impl() : 0;
}

}
