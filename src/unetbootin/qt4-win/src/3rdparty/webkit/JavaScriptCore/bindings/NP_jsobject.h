/*
 * Copyright (C) 2004, 2006 Apple Computer, Inc.  All rights reserved.
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

#ifndef NP_JSOBJECT_H
#define NP_JSOBJECT_H

#if !PLATFORM(DARWIN) || !defined(__LP64__)

#include "npruntime.h"
#include <wtf/Forward.h>

namespace KJS {
    class JSObject;
    namespace Bindings {
        class RootObject;
    }
}

extern NPClass* NPScriptObjectClass;

struct JavaScriptObject
{
    NPObject object;
    KJS::JSObject* imp;
    KJS::Bindings::RootObject* originRootObject;
    KJS::Bindings::RootObject* rootObject;
};

NPObject* _NPN_CreateScriptObject(NPP npp, KJS::JSObject*, PassRefPtr<KJS::Bindings::RootObject> originRootObject, PassRefPtr<KJS::Bindings::RootObject> rootObject);
NPObject* _NPN_CreateNoScriptObject(void);

#endif
#endif
