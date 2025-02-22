/*
 * Copyright (C) 2004 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef RUNTIME_ROOT_H_
#define RUNTIME_ROOT_H_

#include "interpreter.h"
#if PLATFORM(MAC)
#include "jni_jsobject.h"
#endif
#include "protect.h"

#include <wtf/HashSet.h>
#include <wtf/Noncopyable.h>

namespace KJS {

class RuntimeObjectImp;
    
namespace Bindings {

class RootObject;

typedef PassRefPtr<RootObject> (*CreateRootObjectFunction)(void* nativeHandle);
typedef HashCountedSet<JSObject*> ProtectCountSet;

extern RootObject* findRootObject(JSObject*);
extern RootObject* findRootObject(Interpreter*);

class RootObject : Noncopyable
{
friend class JavaJSObject;
public:
    static PassRefPtr<RootObject> create(const void* nativeHandle, PassRefPtr<Interpreter> interpreter);

    void ref() { m_refCount++; }
    void deref()
    {
        if (--m_refCount == 0)
            delete this;
    }

    bool isValid() { return m_isValid; }
    void invalidate();
    
    void gcProtect(JSObject*);
    void gcUnprotect(JSObject*);
    bool gcIsProtected(JSObject*);

    const void* nativeHandle() const;
    Interpreter* interpreter() const;

#if PLATFORM(MAC)
    // Must be called from the thread that will be used to access JavaScript.
    static void setCreateRootObject(CreateRootObjectFunction);
    static CreateRootObjectFunction createRootObject() {
        return _createRootObject;
    }
    
    static CFRunLoopRef runLoop() { return _runLoop; }
    static CFRunLoopSourceRef performJavaScriptSource() { return _performJavaScriptSource; }
    
    static void dispatchToJavaScriptThread(JSObjectCallContext *context);
#endif

    void addRuntimeObject(RuntimeObjectImp*);
    void removeRuntimeObject(RuntimeObjectImp*);
private:
    RootObject(const void* nativeHandle, PassRefPtr<Interpreter> interpreter);
    ~RootObject();
    
    unsigned m_refCount;
    bool m_isValid;
    
    const void* m_nativeHandle;
    RefPtr<Interpreter> m_interpreter;
    ProtectCountSet m_protectCountSet;

    HashSet<RuntimeObjectImp*> m_runtimeObjects;
    
#if PLATFORM(MAC)
    static CreateRootObjectFunction _createRootObject;
    static CFRunLoopRef _runLoop;
    static CFRunLoopSourceRef _performJavaScriptSource;
#endif
};

} // namespace Bindings

} // namespace KJS

#endif
