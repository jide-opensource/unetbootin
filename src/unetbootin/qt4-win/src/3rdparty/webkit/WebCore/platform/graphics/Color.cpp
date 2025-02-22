/*
 * Copyright (C) 2003, 2004, 2005, 2006 Apple Computer, Inc. All rights reserved.
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

#include "config.h"
#include "Color.h"

#include "DeprecatedString.h"
#include "PlatformString.h"
#include <math.h>
#include <wtf/Assertions.h>
#include <wtf/MathExtras.h>

#include "ColorData.c"

using namespace std;

namespace WebCore {

const RGBA32 lightenedBlack = 0xFF545454;
const RGBA32 darkenedWhite = 0xFFABABAB;

RGBA32 makeRGB(int r, int g, int b)
{
    return 0xFF000000 | max(0, min(r, 255)) << 16 | max(0, min(g, 255)) << 8 | max(0, min(b, 255));
}

RGBA32 makeRGBA(int r, int g, int b, int a)
{
    return max(0, min(a, 255)) << 24 | max(0, min(r, 255)) << 16 | max(0, min(g, 255)) << 8 | max(0, min(b, 255));
}

static double calcHue(double temp1, double temp2, double hueVal)
{
    if (hueVal < 0.0)
        hueVal++;
    else if (hueVal > 1.0)
        hueVal--;
    if (hueVal * 6.0 < 1.0)
        return temp1 + (temp2 - temp1) * hueVal * 6.0;
    if (hueVal * 2.0 < 1.0)
        return temp2;
    if (hueVal * 3.0 < 2.0)
        return temp1 + (temp2 - temp1) * (2.0 / 3.0 - hueVal) * 6.0;
    return temp1;
}

// Explanation of this algorithm can be found in the CSS3 Color Module
// specification at http://www.w3.org/TR/css3-color/#hsl-color with further
// explanation available at http://en.wikipedia.org/wiki/HSL_color_space 

// all values are in the range of 0 to 1.0
RGBA32 makeRGBAFromHSLA(double hue, double saturation, double lightness, double alpha)
{
    const double scaleFactor = nextafter(256.0, 0.0);

    if (!saturation) {
        int greyValue = static_cast<int>(lightness * scaleFactor);
        return makeRGBA(greyValue, greyValue, greyValue, static_cast<int>(alpha * scaleFactor));
    }

    double temp2 = lightness < 0.5 ? lightness * (1.0 + saturation) : lightness + saturation - lightness * saturation;
    double temp1 = 2.0 * lightness - temp2;
    
    return makeRGBA(static_cast<int>(calcHue(temp1, temp2, hue + 1.0 / 3.0) * scaleFactor), 
                    static_cast<int>(calcHue(temp1, temp2, hue) * scaleFactor),
                    static_cast<int>(calcHue(temp1, temp2, hue - 1.0 / 3.0) * scaleFactor),
                    static_cast<int>(alpha * scaleFactor));
}

// originally moved here from the CSS parser
bool Color::parseHexColor(const String& name, RGBA32& rgb)
{
    int len = name.length();
    if (len == 3 || len == 6) {
        bool ok;
        int val = name.deprecatedString().toInt(&ok, 16);
        if (ok) {
            if (len == 6) {
                rgb = 0xFF000000 | val;
                return true;
            }
            // #abc converts to #aabbcc according to the specs
            rgb = 0xFF000000
                | (val & 0xF00) << 12 | (val & 0xF00) << 8
                | (val & 0xF0) << 8 | (val & 0xF0) << 4
                | (val & 0xF) << 4 | (val & 0xF);
            return true;
        }
    }
    return false;
}

int differenceSquared(const Color& c1, const Color& c2)
{
    int dR = c1.red() - c2.red();
    int dG = c1.green() - c2.green();
    int dB = c1.blue() - c2.blue();
    return dR * dR + dG * dG + dB * dB;
}

Color::Color(const String& name)
{
    if (name.startsWith("#"))
        m_valid = parseHexColor(name.substring(1), m_color);
    else
        setNamedColor(name);
}

Color::Color(const char* name)
{
    if (name[0] == '#')
        m_valid = parseHexColor(&name[1], m_color);
    else {
        const NamedColor* foundColor = findColor(name, strlen(name));
        m_color = foundColor ? foundColor->RGBValue : 0;
        m_color |= 0xFF000000;
        m_valid = foundColor;
    }
}

String Color::name() const
{
    if (alpha() < 0xFF)
        return String::format("#%02X%02X%02X%02X", red(), green(), blue(), alpha());
    return String::format("#%02X%02X%02X", red(), green(), blue());
}

void Color::setNamedColor(const String& name)
{
    DeprecatedString dname = name.deprecatedString();
    const NamedColor* foundColor = dname.isAllASCII() ? findColor(dname.latin1(), dname.length()) : 0;
    m_color = foundColor ? foundColor->RGBValue : 0;
    m_color |= 0xFF000000;
    m_valid = foundColor;
}

Color Color::light() const
{
    // Hardcode this common case for speed.
    if (m_color == black)
        return lightenedBlack;
    
    const float scaleFactor = nextafterf(256.0f, 0.0f);

    float r, g, b, a;
    getRGBA(r, g, b, a);

    float v = max(r, max(g, b));

    if (v == 0.0f)
        // Lightened black with alpha.
        return Color(0x54, 0x54, 0x54, alpha());

    float multiplier = min(1.0f, v + 0.33f) / v;

    return Color(static_cast<int>(multiplier * r * scaleFactor),
                 static_cast<int>(multiplier * g * scaleFactor),
                 static_cast<int>(multiplier * b * scaleFactor),
                 alpha());
}

Color Color::dark() const
{
    // Hardcode this common case for speed.
    if (m_color == white)
        return darkenedWhite;
    
    const float scaleFactor = nextafterf(256.0f, 0.0f);

    float r, g, b, a;
    getRGBA(r, g, b, a);

    float v = max(r, max(g, b));
    float multiplier = max(0.0f, (v - 0.33f) / v);

    return Color(static_cast<int>(multiplier * r * scaleFactor),
                 static_cast<int>(multiplier * g * scaleFactor),
                 static_cast<int>(multiplier * b * scaleFactor),
                 alpha());
}

static int blendComponent(int c, int a)
{
    // We use white.
    float alpha = a / 255.0f;
    int whiteBlend = 255 - a;
    c -= whiteBlend;
    return static_cast<int>(c / alpha);
}

const int cStartAlpha = 153; // 60%
const int cEndAlpha = 204; // 80%;
const int cAlphaIncrement = 17; // Increments in between.

Color Color::blend(const Color& source) const
{
    if (!alpha() || !source.hasAlpha())
        return source;

    if (!source.alpha())
        return *this;

    int d = 255 * (alpha() + source.alpha()) - alpha() * source.alpha();
    int a = d / 255;
    int r = (red() * alpha() * (255 - source.alpha()) + 255 * source.alpha() * source.red()) / d;
    int g = (green() * alpha() * (255 - source.alpha()) + 255 * source.alpha() * source.green()) / d;
    int b = (blue() * alpha() * (255 - source.alpha()) + 255 * source.alpha() * source.blue()) / d;
    return Color(r, g, b, a);
}

Color Color::blendWithWhite() const
{
    // If the color contains alpha already, we leave it alone.
    if (hasAlpha())
        return *this;

    Color newColor;
    for (int alpha = cStartAlpha; alpha <= cEndAlpha; alpha += cAlphaIncrement) {
        // We have a solid color.  Convert to an equivalent color that looks the same when blended with white
        // at the current alpha.  Try using less transparency if the numbers end up being negative.
        int r = blendComponent(red(), alpha);
        int g = blendComponent(green(), alpha);
        int b = blendComponent(blue(), alpha);
        
        newColor = Color(r, g, b, alpha);

        if (r >= 0 && g >= 0 && b >= 0)
            break;
    }
    return newColor;
}

void Color::getRGBA(float& r, float& g, float& b, float& a) const
{
    r = red() / 255.0f;
    g = green() / 255.0f;
    b = blue() / 255.0f;
    a = alpha() / 255.0f;
}

void Color::getRGBA(double& r, double& g, double& b, double& a) const
{
    r = red() / 255.0;
    g = green() / 255.0;
    b = blue() / 255.0;
    a = alpha() / 255.0;
}

} // namespace WebCore
