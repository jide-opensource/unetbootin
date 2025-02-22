/*
 * Copyright 2005 Frerich Raabe <raabe@kde.org>
 * Copyright (C) 2006 Apple Computer, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef XPathExpression_h
#define XPathExpression_h

#if ENABLE(XPATH)

#include "Shared.h"
#include <wtf/Forward.h>

namespace WebCore {

    typedef int ExceptionCode;

    class Node;
    class String;
    class XPathNSResolver;
    class XPathResult;

    namespace XPath {
        class Expression;
    }

    class XPathExpression : public Shared<XPathExpression> {
    public:
        ~XPathExpression();
        static PassRefPtr<XPathExpression> createExpression(const String& expression, XPathNSResolver*, ExceptionCode&);
        PassRefPtr<XPathResult> evaluate(Node* contextNode, unsigned short type, XPathResult*, ExceptionCode&);
            
    private:
        XPath::Expression* m_topExpression;
    };

}

#endif // ENABLE(XPATH)

#endif // XPathExpression_h
