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

#ifndef JSHTMLDocument_h
#define JSHTMLDocument_h

#include "JSDocument.h"

namespace WebCore {

class HTMLDocument;

class JSHTMLDocument : public JSDocument {
    typedef JSDocument Base;
public:
    JSHTMLDocument(PassRefPtr<JSC::Structure>, PassRefPtr<HTMLDocument>);
    static JSC::JSObject* createPrototype(JSC::ExecState*);
    virtual bool getOwnPropertySlot(JSC::ExecState*, const JSC::Identifier& propertyName, JSC::PropertySlot&);
    virtual void put(JSC::ExecState*, const JSC::Identifier& propertyName, JSC::JSValuePtr, JSC::PutPropertySlot&);
    virtual const JSC::ClassInfo* classInfo() const { return &s_info; }
    static const JSC::ClassInfo s_info;

    static PassRefPtr<JSC::Structure> createStructure(JSC::JSValuePtr prototype)
    {
        return JSC::Structure::create(prototype, JSC::TypeInfo(JSC::ObjectType));
    }

    static JSC::JSValuePtr getConstructor(JSC::ExecState*);

    // Custom attributes
    JSC::JSValuePtr all(JSC::ExecState*) const;
    void setAll(JSC::ExecState*, JSC::JSValuePtr);

    // Custom functions
    JSC::JSValuePtr open(JSC::ExecState*, const JSC::ArgList&);
    JSC::JSValuePtr write(JSC::ExecState*, const JSC::ArgList&);
    JSC::JSValuePtr writeln(JSC::ExecState*, const JSC::ArgList&);
private:
    static bool canGetItemsForName(JSC::ExecState*, HTMLDocument*, const JSC::Identifier&);
    static JSC::JSValuePtr nameGetter(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
};


class JSHTMLDocumentPrototype : public JSC::JSObject {
public:
    static JSC::JSObject* self(JSC::ExecState*);
    virtual const JSC::ClassInfo* classInfo() const { return &s_info; }
    static const JSC::ClassInfo s_info;
    virtual bool getOwnPropertySlot(JSC::ExecState*, const JSC::Identifier&, JSC::PropertySlot&);
    static PassRefPtr<JSC::Structure> createStructure(JSC::JSValuePtr prototype)
    {
        return JSC::Structure::create(prototype, JSC::TypeInfo(JSC::ObjectType));
    }
    JSHTMLDocumentPrototype(PassRefPtr<JSC::Structure> structure) : JSC::JSObject(structure) { }
};

// Functions

JSC::JSValuePtr jsHTMLDocumentPrototypeFunctionOpen(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr, const JSC::ArgList&);
JSC::JSValuePtr jsHTMLDocumentPrototypeFunctionClose(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr, const JSC::ArgList&);
JSC::JSValuePtr jsHTMLDocumentPrototypeFunctionWrite(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr, const JSC::ArgList&);
JSC::JSValuePtr jsHTMLDocumentPrototypeFunctionWriteln(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr, const JSC::ArgList&);
JSC::JSValuePtr jsHTMLDocumentPrototypeFunctionClear(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr, const JSC::ArgList&);
JSC::JSValuePtr jsHTMLDocumentPrototypeFunctionCaptureEvents(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr, const JSC::ArgList&);
JSC::JSValuePtr jsHTMLDocumentPrototypeFunctionReleaseEvents(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr, const JSC::ArgList&);
JSC::JSValuePtr jsHTMLDocumentPrototypeFunctionHasFocus(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr, const JSC::ArgList&);
// Attributes

JSC::JSValuePtr jsHTMLDocumentEmbeds(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
JSC::JSValuePtr jsHTMLDocumentPlugins(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
JSC::JSValuePtr jsHTMLDocumentScripts(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
JSC::JSValuePtr jsHTMLDocumentAll(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
void setJSHTMLDocumentAll(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr);
JSC::JSValuePtr jsHTMLDocumentWidth(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
JSC::JSValuePtr jsHTMLDocumentHeight(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
JSC::JSValuePtr jsHTMLDocumentDir(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
void setJSHTMLDocumentDir(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr);
JSC::JSValuePtr jsHTMLDocumentDesignMode(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
void setJSHTMLDocumentDesignMode(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr);
JSC::JSValuePtr jsHTMLDocumentCompatMode(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
JSC::JSValuePtr jsHTMLDocumentActiveElement(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
JSC::JSValuePtr jsHTMLDocumentBgColor(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
void setJSHTMLDocumentBgColor(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr);
JSC::JSValuePtr jsHTMLDocumentFgColor(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
void setJSHTMLDocumentFgColor(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr);
JSC::JSValuePtr jsHTMLDocumentAlinkColor(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
void setJSHTMLDocumentAlinkColor(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr);
JSC::JSValuePtr jsHTMLDocumentLinkColor(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
void setJSHTMLDocumentLinkColor(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr);
JSC::JSValuePtr jsHTMLDocumentVlinkColor(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
void setJSHTMLDocumentVlinkColor(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr);
JSC::JSValuePtr jsHTMLDocumentConstructor(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);

} // namespace WebCore

#endif
