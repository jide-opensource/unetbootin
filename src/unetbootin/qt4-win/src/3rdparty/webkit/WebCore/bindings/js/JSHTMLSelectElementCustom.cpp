/*
 * Copyright (C) 2007 Apple, Inc.
 * Copyright (C) 2007 Alexey Proskuryakov (ap@webkit.org)
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
 */

#include "config.h"
#include "JSHTMLSelectElementCustom.h"

#include "ExceptionCode.h"
#include "HTMLNames.h"
#include "HTMLOptionElement.h"
#include "HTMLSelectElement.h"
#include "JSHTMLOptionElement.h"
#include "kjs_html.h"

namespace WebCore {

using namespace KJS;
using namespace HTMLNames;

JSValue* JSHTMLSelectElement::remove(ExecState* exec, const List& args)
{
    HTMLSelectElement& select = *static_cast<HTMLSelectElement*>(impl());

    // we support both options index and options objects
    HTMLElement* element = toHTMLElement(args[0]);
    if (element && element->hasTagName(optionTag))
        select.remove(static_cast<HTMLOptionElement*>(element)->index());
    else
        select.remove(args[0]->toInt32(exec));

    return jsUndefined();
}

void selectIndexSetter(HTMLSelectElement* select, KJS::ExecState* exec, unsigned index, KJS::JSValue* value)
{
    if (value->isUndefinedOrNull())
        select->remove(index);
    else {
        ExceptionCode ec = 0;
        HTMLOptionElement* option = toHTMLOptionElement(value);
        if (!option)
            ec = TYPE_MISMATCH_ERR;
        else
            select->setOption(index, option, ec);
        setDOMException(exec, ec);
    }
}

void JSHTMLSelectElement::indexSetter(KJS::ExecState* exec, unsigned index, KJS::JSValue* value, int attr)
{
    selectIndexSetter(static_cast<HTMLSelectElement*>(impl()), exec, index, value);
}

}
