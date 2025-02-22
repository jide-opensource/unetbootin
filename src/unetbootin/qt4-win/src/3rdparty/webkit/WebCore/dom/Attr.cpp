/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Peter Kelly (pmk@post.com)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007 Apple Inc. All rights reserved.
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
#include "Attr.h"

#include "Document.h"
#include "Element.h"
#include "ExceptionCode.h"
#include "Text.h"

namespace WebCore {

Attr::Attr(Element* element, Document* docPtr, Attribute* a)
    : ContainerNode(docPtr),
      m_element(element),
      m_attribute(a),
      m_ignoreChildrenChanged(0)
{
    ASSERT(!m_attribute->attr());
    m_attribute->m_impl = this;
    m_attrWasSpecifiedOrElementHasRareData = true;
}

Attr::~Attr()
{
    ASSERT(m_attribute->attr() == this);
    m_attribute->m_impl = 0;
}

void Attr::createTextChild()
{
    ASSERT(refCount());
    if (!m_attribute->value().isEmpty()) {
        RefPtr<Text> textNode = document()->createTextNode(m_attribute->value().impl());

        // This does everything appendChild() would do in this situation (assuming m_ignoreChildrenChanged was set),
        // but much more efficiently.
        textNode->setParent(this);
        setFirstChild(textNode.get());
        setLastChild(textNode.get());
    }
}

String Attr::nodeName() const
{
    return name();
}

Node::NodeType Attr::nodeType() const
{
    return ATTRIBUTE_NODE;
}

const AtomicString& Attr::localName() const
{
    return m_attribute->localName();
}

const AtomicString& Attr::namespaceURI() const
{
    return m_attribute->namespaceURI();
}

const AtomicString& Attr::prefix() const
{
    return m_attribute->prefix();
}

void Attr::setPrefix(const AtomicString &_prefix, ExceptionCode& ec)
{
    checkSetPrefix(_prefix, ec);
    if (ec)
        return;

    m_attribute->setPrefix(_prefix);
}

String Attr::nodeValue() const
{
    return value();
}

void Attr::setValue( const String& v, ExceptionCode& ec)
{
    ec = 0;

    // do not interprete entities in the string, its literal!

    // NO_MODIFICATION_ALLOWED_ERR: Raised when the node is readonly
    if (isReadOnlyNode()) {
        ec = NO_MODIFICATION_ALLOWED_ERR;
        return;
    }

    int e = 0;
    m_ignoreChildrenChanged++;
    removeChildren();
    appendChild(document()->createTextNode(v.impl()), e);
    m_ignoreChildrenChanged--;
    
    m_attribute->setValue(v.impl());
    if (m_element)
        m_element->attributeChanged(m_attribute.get());
}

void Attr::setNodeValue(const String& v, ExceptionCode& ec)
{
    // NO_MODIFICATION_ALLOWED_ERR: taken care of by setValue()
    setValue(v, ec);
}

PassRefPtr<Node> Attr::cloneNode(bool /*deep*/)
{
    RefPtr<Attr> clone = new Attr(0, document(), m_attribute->clone());
    cloneChildNodes(clone.get());
    return clone.release();
}

// DOM Section 1.1.1
bool Attr::childTypeAllowed(NodeType type)
{
    switch (type) {
        case TEXT_NODE:
        case ENTITY_REFERENCE_NODE:
            return true;
        default:
            return false;
    }
}

void Attr::childrenChanged()
{
    Node::childrenChanged();
    
    if (m_ignoreChildrenChanged > 0)
        return;
    
    // FIXME: We should include entity references in the value
    
    String val = "";
    for (Node *n = firstChild(); n; n = n->nextSibling()) {
        if (n->isTextNode())
            val += static_cast<Text *>(n)->data();
    }
    
    m_attribute->setValue(val.impl());
    if (m_element)
        m_element->attributeChanged(m_attribute.get());
}

String Attr::toString() const
{
    String result;

    result += nodeName();

    // FIXME: substitute entities for any instances of " or ' --
    // maybe easier to just use text value and ignore existing
    // entity refs?

    if (firstChild() != NULL) {
        result += "=\"";

        for (Node *child = firstChild(); child != NULL; child = child->nextSibling()) {
            result += child->toString();
        }
        
        result += "\"";
    }

    return result;
}

}
