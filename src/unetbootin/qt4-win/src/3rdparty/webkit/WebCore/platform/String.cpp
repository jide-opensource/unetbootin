/**
 * This file is part of the DOM implementation for KDE.
 *
 * (C) 1999 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006 Apple Computer, Inc.
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
#include "PlatformString.h"

#include "CString.h"
#include "DeprecatedString.h"
#include "TextEncoding.h"
#include <kjs/identifier.h>
#include <wtf/Vector.h>
#include <stdarg.h>
#include <stdio.h>

using KJS::Identifier;
using KJS::UString;

namespace WebCore {

String::String(const UChar* str, unsigned len)
{
    if (!str)
        return;
    
    if (len == 0)
        m_impl = StringImpl::empty();
    else
        m_impl = new StringImpl(str, len);
}

String::String(const UChar* str)
{
    if (!str)
        return;
        
    int len = 0;
    while (str[len] != UChar(0))
        len++;
    
    if (len == 0)
        m_impl = StringImpl::empty();
    else
        m_impl = new StringImpl(str, len);
}

String::String(const DeprecatedString& str)
{
    if (str.isNull())
        return;
    
    if (str.isEmpty())
        m_impl = StringImpl::empty();
    else 
        m_impl = new StringImpl(reinterpret_cast<const UChar*>(str.unicode()), str.length());
}

String::String(const char* str)
{
    if (!str)
        return;

    int l = strlen(str);
    if (l == 0)
        m_impl = StringImpl::empty();
    else
        m_impl = new StringImpl(str, l);
}

String::String(const char* str, unsigned length)
{
    if (!str)
        return;

    if (length == 0)
        m_impl = StringImpl::empty();
    else
        m_impl = new StringImpl(str, length);
}

void String::append(const String &str)
{
    if (str.m_impl) {
        if (!m_impl) {
            // ### FIXME!!!
            m_impl = str.m_impl;
            return;
        }
        m_impl = m_impl->copy();
        m_impl->append(str.m_impl.get());
    }
}

void String::append(char c)
{
    if (!m_impl)
        m_impl = new StringImpl(&c, 1);
    else {
        m_impl = m_impl->copy();
        m_impl->append(c);
    }
}

void String::append(UChar c)
{
    if (!m_impl)
        m_impl = new StringImpl(&c, 1);
    else {
        m_impl = m_impl->copy();
        m_impl->append(c);
    }
}

String operator+(const String& a, const String& b)
{
    if (a.isEmpty())
        return b.copy();
    if (b.isEmpty())
        return a.copy();
    String c = a.copy();
    c += b;
    return c;
}

String operator+(const String& s, const char* cs)
{
    return s + String(cs);
}

String operator+(const char* cs, const String& s)
{
    return String(cs) + s;
}

void String::insert(const String& str, unsigned pos)
{
    if (!m_impl)
        m_impl = str.m_impl->copy();
    else
        m_impl->insert(str.m_impl.get(), pos);
}

void String::insert(const UChar* str, unsigned length, unsigned pos)
{
    if (!m_impl)
        m_impl = new StringImpl(str, length);
    else
        m_impl->insert(str, length, pos);
}

UChar String::operator[](unsigned i) const
{
    if (!m_impl || i >= m_impl->length())
        return 0;
    return m_impl->characters()[i];
}

unsigned String::length() const
{
    if (!m_impl)
        return 0;
    return m_impl->length();
}

void String::truncate(unsigned len)
{
    if (m_impl)
        m_impl->truncate(len);
}

void String::remove(unsigned pos, int len)
{
    if (m_impl)
        m_impl->remove(pos, len);
}

String String::substring(unsigned pos, unsigned len) const
{
    if (!m_impl) 
        return String();
    return m_impl->substring(pos, len);
}

String String::lower() const
{
    if (!m_impl)
        return String();
    return m_impl->lower();
}

String String::upper() const
{
    if (!m_impl)
        return String();
    return m_impl->upper();
}

String String::stripWhiteSpace() const
{
    if (!m_impl)
        return String();
    return m_impl->stripWhiteSpace();
}

String String::simplifyWhiteSpace() const
{
    if (!m_impl)
        return String();
    return m_impl->simplifyWhiteSpace();
}

String String::foldCase() const
{
    if (!m_impl)
        return String();
    return m_impl->foldCase();
}

bool String::percentage(int& result) const
{
    if (!m_impl || !m_impl->length())
        return false;

    if ((*m_impl)[m_impl->length() - 1] != '%')
       return false;

    result = DeprecatedConstString(reinterpret_cast<const DeprecatedChar*>(m_impl->characters()), m_impl->length() - 1).string().toInt();
    return true;
}

const UChar* String::characters() const
{
    if (!m_impl)
        return 0;
    return m_impl->characters();
}

const UChar* String::charactersWithNullTermination()
{
    if (!m_impl)
        return 0;
    return m_impl->charactersWithNullTermination();
}

DeprecatedString String::deprecatedString() const
{
    if (!m_impl)
        return DeprecatedString::null;
    if (!m_impl->characters())
        return DeprecatedString("", 0);
    return DeprecatedString(reinterpret_cast<const DeprecatedChar*>(m_impl->characters()), m_impl->length());
}

String String::format(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    Vector<char, 256> buffer;

    // Do the format once to get the length.
#if COMPILER(MSVC)
    int result = _vscprintf(format, args);
#else
    char ch;
    int result = vsnprintf(&ch, 1, format, args);
    // We need to call va_end() and then va_start() again here, as the
    // contents of args is undefined after the call to vsnprintf
    // according to http://man.cx/snprintf(3)
    //
    // Not calling va_end/va_start here happens to work on lots of
    // systems, but fails e.g. on 64bit Linux.
    va_end(args);
    va_start(args, format);
#endif

    if (result == 0)
        return String("");
    if (result < 0)
        return String();
    unsigned len = result;
    buffer.resize(len + 1);
    
    // Now do the formatting again, guaranteed to fit.
    vsnprintf(buffer.data(), buffer.size(), format, args);

    va_end(args);
    
    return new StringImpl(buffer.data(), len);
}

String String::number(int n)
{
    return String::format("%d", n);
}

String String::number(unsigned n)
{
    return String::format("%u", n);
}

String String::number(long n)
{
    return String::format("%ld", n);
}

String String::number(unsigned long n)
{
    return String::format("%lu", n);
}

String String::number(long long n)
{
#if PLATFORM(WIN_OS)
    return String::format("%I64i", n);
#else
    return String::format("%lli", n);
#endif
}

String String::number(unsigned long long n)
{
#if PLATFORM(WIN_OS)
    return String::format("%I64u", n);
#else
    return String::format("%llu", n);
#endif
}
    
String String::number(double n)
{
    return String::format("%.6lg", n);
}

int String::toInt(bool* ok) const
{
    if (!m_impl) {
        if (ok)
            *ok = false;
        return 0;
    }
    return m_impl->toInt(ok);
}

int64_t String::toInt64(bool* ok) const
{
    if (!m_impl) {
        if (ok)
            *ok = false;
        return 0;
    }
    return m_impl->toInt64(ok);
}

uint64_t String::toUInt64(bool* ok) const
{
    if (!m_impl) {
        if (ok)
            *ok = false;
        return 0;
    }
    return m_impl->toUInt64(ok);
}

double String::toDouble(bool* ok) const
{
    if (!m_impl) {
        if (ok)
            *ok = false;
        return 0;
    }
    return m_impl->toDouble(ok);
}

float String::toFloat(bool* ok) const
{
    if (!m_impl) {
        if (ok)
            *ok = false;
        return 0.0f;
    }
    return m_impl->toFloat(ok);
}

String String::copy() const
{
    if (!m_impl)
        return String();
    return m_impl->copy();
}

bool String::isEmpty() const
{
    return !m_impl || !m_impl->length();
}

Length* String::toCoordsArray(int& len) const 
{
    return m_impl ? m_impl->toCoordsArray(len) : 0;
}

Length* String::toLengthArray(int& len) const 
{
    return m_impl ? m_impl->toLengthArray(len) : 0;
}

Vector<String> String::split(const String& separator, bool allowEmptyEntries) const
{
    Vector<String> result;
    
    int startPos = 0;
    int endPos;
    while ((endPos = find(separator, startPos)) != -1) {
        if (allowEmptyEntries || startPos != endPos)
            result.append(substring(startPos, endPos - startPos));
        startPos = endPos + separator.length();
    }
    if (allowEmptyEntries || startPos != (int)length())
        result.append(substring(startPos));
    
    return result;
}

Vector<String> String::split(UChar separator, bool allowEmptyEntries) const
{
    Vector<String> result;
  
    return split(String(&separator, 1), allowEmptyEntries);
}

#ifndef NDEBUG
Vector<char> String::ascii() const
{
    if (m_impl) 
        return m_impl->ascii();
    
    const char* nullMsg = "(null impl)";
    Vector<char, 2048> buffer;
    for (int i = 0; nullMsg[i]; ++i)
        buffer.append(nullMsg[i]);
    
    buffer.append('\0');
    return buffer;
}
#endif

CString String::latin1() const
{
    return Latin1Encoding().encode(characters(), length());
}
    
CString String::utf8() const
{
    return UTF8Encoding().encode(characters(), length());
}

String String::fromUTF8(const char* string, size_t size)
{
    return UTF8Encoding().decode(string, size);
}

String String::fromUTF8(const char* string)
{
    return UTF8Encoding().decode(string, strlen(string));
}


bool operator==(const String& a, const DeprecatedString& b)
{
    unsigned l = a.length();
    if (l != b.length())
        return false;
    if (!memcmp(a.characters(), b.unicode(), l * sizeof(UChar)))
        return true;
    return false;
}

String::String(const Identifier& str)
{
    if (str.isNull())
        return;
    
    if (str.isEmpty())
        m_impl = StringImpl::empty();
    else 
        m_impl = new StringImpl(reinterpret_cast<const UChar*>(str.data()), str.size());
}

String::String(const UString& str)
{
    if (str.isNull())
        return;
    
    if (str.isEmpty())
        m_impl = StringImpl::empty();
    else 
        m_impl = new StringImpl(reinterpret_cast<const UChar*>(str.data()), str.size());
}

String::operator Identifier() const
{
    if (!m_impl)
        return Identifier();
    return Identifier(reinterpret_cast<const KJS::UChar*>(m_impl->characters()), m_impl->length());
}

String::operator UString() const
{
    if (!m_impl)
        return UString();
    return UString(reinterpret_cast<const KJS::UChar*>(m_impl->characters()), m_impl->length());
}

String String::newUninitialized(size_t length, UChar*& characterBuffer)
{
    return StringImpl::newUninitialized(length, characterBuffer);
}

String String::adopt(Vector<UChar>& buffer)
{
    // For an empty buffer, construct an empty string, not a null string,
    // and use a standard constructor so we get the shared empty string.
    if (buffer.isEmpty())
        return "";
    return StringImpl::adopt(buffer);
}

} // namespace WebCore

#ifndef NDEBUG
// For debugging only -- leaks memory
WebCore::String* string(const char* s)
{
    return new WebCore::String(s);
}
#endif
