/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007 Apple Inc. All rights reserved.
 *           (C) 2006 Alexey Proskuryakov (ap@nypop.com)
 * Copyright (C) 2007 Samuel Weinig (sam@webkit.org)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#include "config.h"
#include "HTMLButtonElement.h"

#include "EventNames.h"
#include "FormDataList.h"
#include "HTMLFormElement.h"
#include "HTMLNames.h"
#include "KeyboardEvent.h"
#include "RenderButton.h"

namespace WebCore {

using namespace EventNames;
using namespace HTMLNames;

HTMLButtonElement::HTMLButtonElement(Document* doc, HTMLFormElement* form)
    : HTMLGenericFormElement(buttonTag, doc, form)
    , m_type(SUBMIT)
    , m_activeSubmit(false)
{
}

HTMLButtonElement::~HTMLButtonElement()
{
}

RenderObject* HTMLButtonElement::createRenderer(RenderArena* arena, RenderStyle* style)
{
    return new (arena) RenderButton(this);
}

const AtomicString& HTMLButtonElement::type() const
{
    return getAttribute(typeAttr);
}

void HTMLButtonElement::parseMappedAttribute(MappedAttribute* attr)
{
    if (attr->name() == typeAttr) {
        if (equalIgnoringCase(attr->value(), "submit"))
            m_type = SUBMIT;
        else if (equalIgnoringCase(attr->value(), "reset"))
            m_type = RESET;
        else if (equalIgnoringCase(attr->value(), "button"))
            m_type = BUTTON;
    } else if (attr->name() == alignAttr) {
        // Don't map 'align' attribute.  This matches what Firefox and IE do, but not Opera.
        // See http://bugs.webkit.org/show_bug.cgi?id=12071
    } else if (attr->name() == onfocusAttr) {
        setHTMLEventListener(focusEvent, attr);
    } else if (attr->name() == onblurAttr) {
        setHTMLEventListener(blurEvent, attr);
    } else
        HTMLGenericFormElement::parseMappedAttribute(attr);
}

void HTMLButtonElement::defaultEventHandler(Event* evt)
{
    if (evt->type() == DOMActivateEvent && !disabled()) {
        if (form() && m_type == SUBMIT) {
            m_activeSubmit = true;
            form()->prepareSubmit(evt);
            m_activeSubmit = false; // in case we were canceled
        }
        if (form() && m_type == RESET)
            form()->reset();
    }

    if (evt->type() == keypressEvent && evt->isKeyboardEvent()) {
        String key = static_cast<KeyboardEvent*>(evt)->keyIdentifier();

        // Do the same things <input type=button/reset/submit> would do.
        if (key == "Enter" && m_type == BUTTON) {
            if (form())
                form()->submitClick(evt);
            evt->setDefaultHandled();
            return;
        }
        if (key == "Enter" || key == "U+0020") {
            dispatchSimulatedClick(evt);
            evt->setDefaultHandled();
            return;
        }
    }

    HTMLGenericFormElement::defaultEventHandler(evt);
}

bool HTMLButtonElement::isSuccessfulSubmitButton() const
{
    // HTML spec says that buttons must have names to be considered successful.
    // However, other browsers do not impose this constraint.
    return m_type == SUBMIT && !disabled();
}

bool HTMLButtonElement::isActivatedSubmit() const
{
    return m_activeSubmit;
}

void HTMLButtonElement::setActivatedSubmit(bool flag)
{
    m_activeSubmit = flag;
}

bool HTMLButtonElement::appendFormData(FormDataList& formData, bool)
{
    if (m_type != SUBMIT || name().isEmpty() || !m_activeSubmit)
        return false;
    formData.appendData(name(), value());
    return true;
}

void HTMLButtonElement::accessKeyAction(bool sendToAnyElement)
{   
    // send the mouse button events iff the caller specified sendToAnyElement
    dispatchSimulatedClick(0, sendToAnyElement);
}

String HTMLButtonElement::accessKey() const
{
    return getAttribute(accesskeyAttr);
}

void HTMLButtonElement::setAccessKey(const String &value)
{
    setAttribute(accesskeyAttr, value);
}

String HTMLButtonElement::value() const
{
    return getAttribute(valueAttr);
}

void HTMLButtonElement::setValue(const String &value)
{
    setAttribute(valueAttr, value);
}
    
} // namespace
