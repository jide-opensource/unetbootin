/* ANSI-C code produced by gperf version 3.0.3 */
/* Command-line: gperf -c -a -L ANSI-C -G -D -E -C -o -t --key-positions='*' -NfindSVGVal -Hhash_val -Wwordlist_val -D -s 3 ksvgcssvalues.gperf  */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "ksvgcssvalues.gperf"

/* This file is automatically generated from ksvgcssvalues.in by cssmakevalues, do not edit */
#include "ksvgcssvalues.h"
#line 5 "ksvgcssvalues.gperf"
struct css_val {
    const char* name;
    int id;
};

static const struct css_val *findSVGVal (register const char *str, register unsigned int len);
enum
  {
    VAL_TOTAL_KEYWORDS = 169,
    VAL_MIN_WORD_LENGTH = 2,
    VAL_MAX_WORD_LENGTH = 20,
    VAL_MIN_HASH_VALUE = 2,
    VAL_MAX_HASH_VALUE = 681
  };

/* maximum key range = 680, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash_val (register const char *str, register unsigned int len)
{
  static const unsigned short asso_values[] =
    {
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682,  75, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682,   5,   5,  50,
        0,   0,  70,   0,  30,  20, 145,   5,   0,  20,
        0,   0,  55,  30,   0,   0,   0,  10, 225, 240,
       15, 155,  40,   0, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682, 682, 682, 682,
      682, 682, 682, 682, 682, 682, 682
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[19]];
      /*FALLTHROUGH*/
      case 19:
        hval += asso_values[(unsigned char)str[18]];
      /*FALLTHROUGH*/
      case 18:
        hval += asso_values[(unsigned char)str[17]];
      /*FALLTHROUGH*/
      case 17:
        hval += asso_values[(unsigned char)str[16]];
      /*FALLTHROUGH*/
      case 16:
        hval += asso_values[(unsigned char)str[15]];
      /*FALLTHROUGH*/
      case 15:
        hval += asso_values[(unsigned char)str[14]];
      /*FALLTHROUGH*/
      case 14:
        hval += asso_values[(unsigned char)str[13]];
      /*FALLTHROUGH*/
      case 13:
        hval += asso_values[(unsigned char)str[12]];
      /*FALLTHROUGH*/
      case 12:
        hval += asso_values[(unsigned char)str[11]];
      /*FALLTHROUGH*/
      case 11:
        hval += asso_values[(unsigned char)str[10]];
      /*FALLTHROUGH*/
      case 10:
        hval += asso_values[(unsigned char)str[9]];
      /*FALLTHROUGH*/
      case 9:
        hval += asso_values[(unsigned char)str[8]];
      /*FALLTHROUGH*/
      case 8:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
        hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
      case 6:
        hval += asso_values[(unsigned char)str[5]];
      /*FALLTHROUGH*/
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]+1];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

static const struct css_val wordlist_val[] =
  {
#line 179 "ksvgcssvalues.gperf"
    {"lr", SVGCSS_VAL_LR},
#line 134 "ksvgcssvalues.gperf"
    {"tan", SVGCSS_VAL_TAN},
#line 154 "ksvgcssvalues.gperf"
    {"srgb", SVGCSS_VAL_SRGB},
#line 107 "ksvgcssvalues.gperf"
    {"orangered", SVGCSS_VAL_ORANGERED},
#line 42 "ksvgcssvalues.gperf"
    {"darkred", SVGCSS_VAL_DARKRED},
#line 35 "ksvgcssvalues.gperf"
    {"darkgreen", SVGCSS_VAL_DARKGREEN},
#line 16 "ksvgcssvalues.gperf"
    {"azure", SVGCSS_VAL_AZURE},
#line 152 "ksvgcssvalues.gperf"
    {"stroke", SVGCSS_VAL_STROKE},
#line 180 "ksvgcssvalues.gperf"
    {"rl", SVGCSS_VAL_RL},
#line 33 "ksvgcssvalues.gperf"
    {"darkgoldenrod", SVGCSS_VAL_DARKGOLDENROD},
#line 40 "ksvgcssvalues.gperf"
    {"darkorange", SVGCSS_VAL_DARKORANGE},
#line 44 "ksvgcssvalues.gperf"
    {"darkseagreen", SVGCSS_VAL_DARKSEAGREEN},
#line 153 "ksvgcssvalues.gperf"
    {"all", SVGCSS_VAL_ALL},
#line 122 "ksvgcssvalues.gperf"
    {"salmon", SVGCSS_VAL_SALMON},
#line 31 "ksvgcssvalues.gperf"
    {"darkblue", SVGCSS_VAL_DARKBLUE},
#line 133 "ksvgcssvalues.gperf"
    {"steelblue", SVGCSS_VAL_STEELBLUE},
#line 90 "ksvgcssvalues.gperf"
    {"magenta", SVGCSS_VAL_MAGENTA},
#line 58 "ksvgcssvalues.gperf"
    {"gainsboro", SVGCSS_VAL_GAINSBORO},
#line 45 "ksvgcssvalues.gperf"
    {"darkslateblue", SVGCSS_VAL_DARKSLATEBLUE},
#line 43 "ksvgcssvalues.gperf"
    {"darksalmon", SVGCSS_VAL_DARKSALMON},
#line 66 "ksvgcssvalues.gperf"
    {"indigo", SVGCSS_VAL_INDIGO},
#line 135 "ksvgcssvalues.gperf"
    {"thistle", SVGCSS_VAL_THISTLE},
#line 128 "ksvgcssvalues.gperf"
    {"slateblue", SVGCSS_VAL_SLATEBLUE},
#line 38 "ksvgcssvalues.gperf"
    {"darkmagenta", SVGCSS_VAL_DARKMAGENTA},
#line 181 "ksvgcssvalues.gperf"
    {"tb", SVGCSS_VAL_TB},
#line 65 "ksvgcssvalues.gperf"
    {"indianred", SVGCSS_VAL_INDIANRED},
#line 60 "ksvgcssvalues.gperf"
    {"gold", SVGCSS_VAL_GOLD},
#line 68 "ksvgcssvalues.gperf"
    {"khaki", SVGCSS_VAL_KHAKI},
#line 132 "ksvgcssvalues.gperf"
    {"springgreen", SVGCSS_VAL_SPRINGGREEN},
#line 171 "ksvgcssvalues.gperf"
    {"hanging", SVGCSS_VAL_HANGING},
#line 61 "ksvgcssvalues.gperf"
    {"goldenrod", SVGCSS_VAL_GOLDENROD},
#line 110 "ksvgcssvalues.gperf"
    {"palegreen", SVGCSS_VAL_PALEGREEN},
#line 109 "ksvgcssvalues.gperf"
    {"palegoldenrod", SVGCSS_VAL_PALEGOLDENROD},
#line 15 "ksvgcssvalues.gperf"
    {"aquamarine", SVGCSS_VAL_AQUAMARINE},
#line 23 "ksvgcssvalues.gperf"
    {"cadetblue", SVGCSS_VAL_CADETBLUE},
#line 54 "ksvgcssvalues.gperf"
    {"dodgerblue", SVGCSS_VAL_DODGERBLUE},
#line 105 "ksvgcssvalues.gperf"
    {"oldlace", SVGCSS_VAL_OLDLACE},
#line 124 "ksvgcssvalues.gperf"
    {"seagreen", SVGCSS_VAL_SEAGREEN},
#line 37 "ksvgcssvalues.gperf"
    {"darkkhaki", SVGCSS_VAL_DARKKHAKI},
#line 176 "ksvgcssvalues.gperf"
    {"lr-tb", SVGCSS_VAL_LR_TB},
#line 136 "ksvgcssvalues.gperf"
    {"tomato", SVGCSS_VAL_TOMATO},
#line 150 "ksvgcssvalues.gperf"
    {"painted", SVGCSS_VAL_PAINTED},
#line 165 "ksvgcssvalues.gperf"
    {"after-edge", SVGCSS_VAL_AFTER_EDGE},
#line 48 "ksvgcssvalues.gperf"
    {"darkturquoise", SVGCSS_VAL_DARKTURQUOISE},
#line 24 "ksvgcssvalues.gperf"
    {"chartreuse", SVGCSS_VAL_CHARTREUSE},
#line 29 "ksvgcssvalues.gperf"
    {"crimson", SVGCSS_VAL_CRIMSON},
#line 30 "ksvgcssvalues.gperf"
    {"cyan", SVGCSS_VAL_CYAN},
#line 17 "ksvgcssvalues.gperf"
    {"beige", SVGCSS_VAL_BEIGE},
#line 142 "ksvgcssvalues.gperf"
    {"nonzero", SVGCSS_VAL_NONZERO},
#line 177 "ksvgcssvalues.gperf"
    {"rl-tb", SVGCSS_VAL_RL_TB},
#line 108 "ksvgcssvalues.gperf"
    {"orchid", SVGCSS_VAL_ORCHID},
#line 117 "ksvgcssvalues.gperf"
    {"plum", SVGCSS_VAL_PLUM},
#line 144 "ksvgcssvalues.gperf"
    {"accumulate", SVGCSS_VAL_ACCUMULATE},
#line 125 "ksvgcssvalues.gperf"
    {"seashell", SVGCSS_VAL_SEASHELL},
#line 26 "ksvgcssvalues.gperf"
    {"coral", SVGCSS_VAL_CORAL},
#line 13 "ksvgcssvalues.gperf"
    {"aliceblue", SVGCSS_VAL_ALICEBLUE},
#line 41 "ksvgcssvalues.gperf"
    {"darkorchid", SVGCSS_VAL_DARKORCHID},
#line 178 "ksvgcssvalues.gperf"
    {"tb-rl", SVGCSS_VAL_TB_RL},
#line 166 "ksvgcssvalues.gperf"
    {"central", SVGCSS_VAL_CENTRAL},
#line 25 "ksvgcssvalues.gperf"
    {"chocolate", SVGCSS_VAL_CHOCOLATE},
#line 158 "ksvgcssvalues.gperf"
    {"crispedges", SVGCSS_VAL_CRISPEDGES},
#line 57 "ksvgcssvalues.gperf"
    {"forestgreen", SVGCSS_VAL_FORESTGREEN},
#line 28 "ksvgcssvalues.gperf"
    {"cornsilk", SVGCSS_VAL_CORNSILK},
#line 115 "ksvgcssvalues.gperf"
    {"peru", SVGCSS_VAL_PERU},
#line 111 "ksvgcssvalues.gperf"
    {"paleturquoise", SVGCSS_VAL_PALETURQUOISE},
#line 19 "ksvgcssvalues.gperf"
    {"blanchedalmond", SVGCSS_VAL_BLANCHEDALMOND},
#line 89 "ksvgcssvalues.gperf"
    {"linen", SVGCSS_VAL_LINEN},
#line 126 "ksvgcssvalues.gperf"
    {"sienna", SVGCSS_VAL_SIENNA},
#line 95 "ksvgcssvalues.gperf"
    {"mediumseagreen", SVGCSS_VAL_MEDIUMSEAGREEN},
#line 155 "ksvgcssvalues.gperf"
    {"linearrgb", SVGCSS_VAL_LINEARRGB},
#line 92 "ksvgcssvalues.gperf"
    {"mediumblue", SVGCSS_VAL_MEDIUMBLUE},
#line 172 "ksvgcssvalues.gperf"
    {"mathematical", SVGCSS_VAL_MATHEMATICAL},
#line 161 "ksvgcssvalues.gperf"
    {"miter", SVGCSS_VAL_MITER},
#line 64 "ksvgcssvalues.gperf"
    {"hotpink", SVGCSS_VAL_HOTPINK},
#line 36 "ksvgcssvalues.gperf"
    {"darkgrey", SVGCSS_VAL_DARKGREY},
#line 88 "ksvgcssvalues.gperf"
    {"limegreen", SVGCSS_VAL_LIMEGREEN},
#line 96 "ksvgcssvalues.gperf"
    {"mediumslateblue", SVGCSS_VAL_MEDIUMSLATEBLUE},
#line 127 "ksvgcssvalues.gperf"
    {"skyblue", SVGCSS_VAL_SKYBLUE},
#line 34 "ksvgcssvalues.gperf"
    {"darkgray", SVGCSS_VAL_DARKGRAY},
#line 47 "ksvgcssvalues.gperf"
    {"darkslategrey", SVGCSS_VAL_DARKSLATEGREY},
#line 78 "ksvgcssvalues.gperf"
    {"lightgreen", SVGCSS_VAL_LIGHTGREEN},
#line 46 "ksvgcssvalues.gperf"
    {"darkslategray", SVGCSS_VAL_DARKSLATEGRAY},
#line 130 "ksvgcssvalues.gperf"
    {"slategrey", SVGCSS_VAL_SLATEGREY},
#line 169 "ksvgcssvalues.gperf"
    {"ideographic", SVGCSS_VAL_IDEOGRAPHIC},
#line 82 "ksvgcssvalues.gperf"
    {"lightseagreen", SVGCSS_VAL_LIGHTSEAGREEN},
#line 129 "ksvgcssvalues.gperf"
    {"slategray", SVGCSS_VAL_SLATEGRAY},
#line 18 "ksvgcssvalues.gperf"
    {"bisque", SVGCSS_VAL_BISQUE},
#line 156 "ksvgcssvalues.gperf"
    {"optimizespeed", SVGCSS_VAL_OPTIMIZESPEED},
#line 73 "ksvgcssvalues.gperf"
    {"lightblue", SVGCSS_VAL_LIGHTBLUE},
#line 170 "ksvgcssvalues.gperf"
    {"alphabetic", SVGCSS_VAL_ALPHABETIC},
#line 86 "ksvgcssvalues.gperf"
    {"lightsteelblue", SVGCSS_VAL_LIGHTSTEELBLUE},
#line 103 "ksvgcssvalues.gperf"
    {"moccasin", SVGCSS_VAL_MOCCASIN},
#line 116 "ksvgcssvalues.gperf"
    {"pink", SVGCSS_VAL_PINK},
#line 81 "ksvgcssvalues.gperf"
    {"lightsalmon", SVGCSS_VAL_LIGHTSALMON},
#line 50 "ksvgcssvalues.gperf"
    {"deeppink", SVGCSS_VAL_DEEPPINK},
#line 175 "ksvgcssvalues.gperf"
    {"reset-size", SVGCSS_VAL_RESET_SIZE},
#line 151 "ksvgcssvalues.gperf"
    {"fill", SVGCSS_VAL_FILL},
#line 173 "ksvgcssvalues.gperf"
    {"use-script", SVGCSS_VAL_USE_SCRIPT},
#line 174 "ksvgcssvalues.gperf"
    {"no-change", SVGCSS_VAL_NO_CHANGE},
#line 98 "ksvgcssvalues.gperf"
    {"mediumturquoise", SVGCSS_VAL_MEDIUMTURQUOISE},
#line 32 "ksvgcssvalues.gperf"
    {"darkcyan", SVGCSS_VAL_DARKCYAN},
#line 164 "ksvgcssvalues.gperf"
    {"before-edge", SVGCSS_VAL_BEFORE_EDGE},
#line 97 "ksvgcssvalues.gperf"
    {"mediumspringgreen", SVGCSS_VAL_MEDIUMSPRINGGREEN},
#line 160 "ksvgcssvalues.gperf"
    {"butt", SVGCSS_VAL_BUTT},
#line 69 "ksvgcssvalues.gperf"
    {"lavender", SVGCSS_VAL_LAVENDER},
#line 120 "ksvgcssvalues.gperf"
    {"royalblue", SVGCSS_VAL_ROYALBLUE},
#line 74 "ksvgcssvalues.gperf"
    {"lightcoral", SVGCSS_VAL_LIGHTCORAL},
#line 100 "ksvgcssvalues.gperf"
    {"midnightblue", SVGCSS_VAL_MIDNIGHTBLUE},
#line 131 "ksvgcssvalues.gperf"
    {"snow", SVGCSS_VAL_SNOW},
#line 143 "ksvgcssvalues.gperf"
    {"evenodd", SVGCSS_VAL_EVENODD},
#line 101 "ksvgcssvalues.gperf"
    {"mintcream", SVGCSS_VAL_MINTCREAM},
#line 21 "ksvgcssvalues.gperf"
    {"brown", SVGCSS_VAL_BROWN},
#line 91 "ksvgcssvalues.gperf"
    {"mediumaquamarine", SVGCSS_VAL_MEDIUMAQUAMARINE},
#line 93 "ksvgcssvalues.gperf"
    {"mediumorchid", SVGCSS_VAL_MEDIUMORCHID},
#line 71 "ksvgcssvalues.gperf"
    {"lawngreen", SVGCSS_VAL_LAWNGREEN},
#line 121 "ksvgcssvalues.gperf"
    {"saddlebrown", SVGCSS_VAL_SADDLEBROWN},
#line 80 "ksvgcssvalues.gperf"
    {"lightpink", SVGCSS_VAL_LIGHTPINK},
#line 49 "ksvgcssvalues.gperf"
    {"darkviolet", SVGCSS_VAL_DARKVIOLET},
#line 39 "ksvgcssvalues.gperf"
    {"darkolivegreen", SVGCSS_VAL_DARKOLIVEGREEN},
#line 139 "ksvgcssvalues.gperf"
    {"wheat", SVGCSS_VAL_WHEAT},
#line 94 "ksvgcssvalues.gperf"
    {"mediumpurple", SVGCSS_VAL_MEDIUMPURPLE},
#line 106 "ksvgcssvalues.gperf"
    {"olivedrab", SVGCSS_VAL_OLIVEDRAB},
#line 70 "ksvgcssvalues.gperf"
    {"lavenderblush", SVGCSS_VAL_LAVENDERBLUSH},
#line 20 "ksvgcssvalues.gperf"
    {"blueviolet", SVGCSS_VAL_BLUEVIOLET},
#line 137 "ksvgcssvalues.gperf"
    {"turquoise", SVGCSS_VAL_TURQUOISE},
#line 55 "ksvgcssvalues.gperf"
    {"firebrick", SVGCSS_VAL_FIREBRICK},
#line 162 "ksvgcssvalues.gperf"
    {"bevel", SVGCSS_VAL_BEVEL},
#line 51 "ksvgcssvalues.gperf"
    {"deepskyblue", SVGCSS_VAL_DEEPSKYBLUE},
#line 145 "ksvgcssvalues.gperf"
    {"new", SVGCSS_VAL_NEW},
#line 140 "ksvgcssvalues.gperf"
    {"whitesmoke", SVGCSS_VAL_WHITESMOKE},
#line 112 "ksvgcssvalues.gperf"
    {"palevioletred", SVGCSS_VAL_PALEVIOLETRED},
#line 59 "ksvgcssvalues.gperf"
    {"ghostwhite", SVGCSS_VAL_GHOSTWHITE},
#line 159 "ksvgcssvalues.gperf"
    {"geometricprecision", SVGCSS_VAL_GEOMETRICPRECISION},
#line 168 "ksvgcssvalues.gperf"
    {"text-after-edge", SVGCSS_VAL_TEXT_AFTER_EDGE},
#line 167 "ksvgcssvalues.gperf"
    {"text-before-edge", SVGCSS_VAL_TEXT_BEFORE_EDGE},
#line 53 "ksvgcssvalues.gperf"
    {"dimgrey", SVGCSS_VAL_DIMGREY},
#line 102 "ksvgcssvalues.gperf"
    {"mistyrose", SVGCSS_VAL_MISTYROSE},
#line 52 "ksvgcssvalues.gperf"
    {"dimgray", SVGCSS_VAL_DIMGRAY},
#line 146 "ksvgcssvalues.gperf"
    {"currentcolor", SVGCSS_VAL_CURRENTCOLOR},
#line 79 "ksvgcssvalues.gperf"
    {"lightgrey", SVGCSS_VAL_LIGHTGREY},
#line 72 "ksvgcssvalues.gperf"
    {"lemonchiffon", SVGCSS_VAL_LEMONCHIFFON},
#line 77 "ksvgcssvalues.gperf"
    {"lightgray", SVGCSS_VAL_LIGHTGRAY},
#line 85 "ksvgcssvalues.gperf"
    {"lightslategrey", SVGCSS_VAL_LIGHTSLATEGREY},
#line 84 "ksvgcssvalues.gperf"
    {"lightslategray", SVGCSS_VAL_LIGHTSLATEGRAY},
#line 83 "ksvgcssvalues.gperf"
    {"lightskyblue", SVGCSS_VAL_LIGHTSKYBLUE},
#line 157 "ksvgcssvalues.gperf"
    {"optimizequality", SVGCSS_VAL_OPTIMIZEQUALITY},
#line 14 "ksvgcssvalues.gperf"
    {"antiquewhite", SVGCSS_VAL_ANTIQUEWHITE},
#line 163 "ksvgcssvalues.gperf"
    {"optimizelegibility", SVGCSS_VAL_OPTIMIZELEGIBILITY},
#line 118 "ksvgcssvalues.gperf"
    {"powderblue", SVGCSS_VAL_POWDERBLUE},
#line 138 "ksvgcssvalues.gperf"
    {"violet", SVGCSS_VAL_VIOLET},
#line 75 "ksvgcssvalues.gperf"
    {"lightcyan", SVGCSS_VAL_LIGHTCYAN},
#line 56 "ksvgcssvalues.gperf"
    {"floralwhite", SVGCSS_VAL_FLORALWHITE},
#line 99 "ksvgcssvalues.gperf"
    {"mediumvioletred", SVGCSS_VAL_MEDIUMVIOLETRED},
#line 62 "ksvgcssvalues.gperf"
    {"greenyellow", SVGCSS_VAL_GREENYELLOW},
#line 149 "ksvgcssvalues.gperf"
    {"visiblestroke", SVGCSS_VAL_VISIBLESTROKE},
#line 123 "ksvgcssvalues.gperf"
    {"sandybrown", SVGCSS_VAL_SANDYBROWN},
#line 67 "ksvgcssvalues.gperf"
    {"ivory", SVGCSS_VAL_IVORY},
#line 114 "ksvgcssvalues.gperf"
    {"peachpuff", SVGCSS_VAL_PEACHPUFF},
#line 27 "ksvgcssvalues.gperf"
    {"cornflowerblue", SVGCSS_VAL_CORNFLOWERBLUE},
#line 119 "ksvgcssvalues.gperf"
    {"rosybrown", SVGCSS_VAL_ROSYBROWN},
#line 141 "ksvgcssvalues.gperf"
    {"yellowgreen", SVGCSS_VAL_YELLOWGREEN},
#line 63 "ksvgcssvalues.gperf"
    {"honeydew", SVGCSS_VAL_HONEYDEW},
#line 147 "ksvgcssvalues.gperf"
    {"visiblepainted", SVGCSS_VAL_VISIBLEPAINTED},
#line 148 "ksvgcssvalues.gperf"
    {"visiblefill", SVGCSS_VAL_VISIBLEFILL},
#line 87 "ksvgcssvalues.gperf"
    {"lightyellow", SVGCSS_VAL_LIGHTYELLOW},
#line 76 "ksvgcssvalues.gperf"
    {"lightgoldenrodyellow", SVGCSS_VAL_LIGHTGOLDENRODYELLOW},
#line 22 "ksvgcssvalues.gperf"
    {"burlywood", SVGCSS_VAL_BURLYWOOD},
#line 113 "ksvgcssvalues.gperf"
    {"papayawhip", SVGCSS_VAL_PAPAYAWHIP},
#line 104 "ksvgcssvalues.gperf"
    {"navajowhite", SVGCSS_VAL_NAVAJOWHITE}
  };

static const short lookupVal[] =
  {
     -1,  -1,   0,  -1,  -1,  -1,  -1,  -1,   1,   2,
     -1,  -1,  -1,  -1,   3,  -1,  -1,   4,  -1,   5,
      6,   7,   8,   9,  -1,  10,  -1,  11,  12,  -1,
     -1,  13,  -1,  14,  15,  -1,  -1,  16,  -1,  17,
     -1,  -1,  -1,  18,  -1,  19,  20,  21,  -1,  22,
     -1,  23,  24,  -1,  25,  -1,  -1,  -1,  -1,  26,
     27,  28,  29,  -1,  30,  -1,  -1,  -1,  -1,  31,
     -1,  -1,  -1,  32,  -1,  33,  -1,  -1,  -1,  34,
     35,  -1,  36,  37,  38,  39,  40,  41,  -1,  -1,
     42,  -1,  -1,  43,  -1,  44,  -1,  45,  -1,  46,
     47,  -1,  48,  -1,  -1,  49,  50,  -1,  -1,  51,
     52,  -1,  -1,  53,  -1,  54,  -1,  -1,  -1,  55,
     56,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     57,  -1,  58,  -1,  59,  60,  61,  -1,  62,  63,
     -1,  -1,  -1,  64,  -1,  -1,  -1,  -1,  -1,  65,
     66,  -1,  -1,  -1,  -1,  -1,  67,  -1,  -1,  68,
     -1,  -1,  -1,  -1,  69,  70,  -1,  71,  -1,  -1,
     72,  -1,  73,  74,  75,  76,  -1,  77,  78,  -1,
     -1,  -1,  -1,  79,  -1,  80,  -1,  -1,  81,  82,
     -1,  83,  -1,  84,  85,  -1,  86,  -1,  87,  88,
     89,  -1,  -1,  -1,  90,  -1,  -1,  -1,  91,  92,
     -1,  93,  -1,  94,  -1,  95,  -1,  -1,  -1,  96,
     97,  -1,  -1,  -1,  98,  99,  -1,  -1, 100,  -1,
     -1, 101, 102,  -1, 103,  -1,  -1,  -1, 104, 105,
    106,  -1, 107,  -1, 108,  -1,  -1, 109,  -1, 110,
    111, 112, 113,  -1, 114,  -1,  -1,  -1,  -1,  -1,
     -1, 115,  -1,  -1, 116, 117,  -1,  -1,  -1, 118,
    119,  -1, 120,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1, 121,  -1,  -1,  -1, 122,  -1,
    123,  -1,  -1,  -1, 124,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1, 125, 126,  -1,  -1,  -1,  -1,
     -1, 127,  -1, 128,  -1, 129,  -1,  -1, 130,  -1,
    131,  -1,  -1, 132,  -1, 133, 134, 135,  -1, 136,
     -1,  -1, 137,  -1,  -1,  -1,  -1, 138,  -1, 139,
     -1,  -1, 140,  -1, 141,  -1,  -1,  -1,  -1, 142,
     -1,  -1,  -1,  -1, 143,  -1,  -1,  -1,  -1,  -1,
     -1,  -1, 144,  -1,  -1, 145,  -1, 146, 147,  -1,
     -1,  -1,  -1,  -1,  -1, 148, 149,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1, 150,  -1, 151,  -1,  -1,  -1,
    152,  -1,  -1,  -1,  -1,  -1, 153,  -1,  -1,  -1,
     -1,  -1,  -1, 154,  -1, 155,  -1,  -1,  -1,  -1,
    156,  -1,  -1,  -1, 157,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1, 158,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1, 159,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1, 160,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 161, 162,
     -1,  -1,  -1,  -1,  -1,  -1, 163,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1, 164,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
    165,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1, 166, 167,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1, 168
  };

#ifdef __GNUC__
__inline
#ifdef __GNUC_STDC_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
const struct css_val *
findSVGVal (register const char *str, register unsigned int len)
{
  if (len <= VAL_MAX_WORD_LENGTH && len >= VAL_MIN_WORD_LENGTH)
    {
      register int key = hash_val (str, len);

      if (key <= VAL_MAX_HASH_VALUE && key >= 0)
        {
          register int index = lookupVal[key];

          if (index >= 0)
            {
              register const char *s = wordlist_val[index].name;

              if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
                return &wordlist_val[index];
            }
        }
    }
  return 0;
}
#line 182 "ksvgcssvalues.gperf"


namespace SVG
{


static const unsigned short valList[] = {
    65535,
    55,
    146,
    33,
    6,
    47,
    86,
    65,
    123,
    111,
    166,
    34,
    44,
    59,
    54,
    158,
    62,
    45,
    46,
    14,
    100,
    9,
    78,
    5,
    74,
    38,
    23,
    118,
    10,
    56,
    4,
    19,
    11,
    18,
    81,
    79,
    43,
    117,
    94,
    127,
    137,
    135,
    35,
    125,
    151,
    61,
    17,
    131,
    26,
    30,
    153,
    161,
    73,
    25,
    20,
    156,
    27,
    104,
    122,
    114,
    140,
    88,
    106,
    150,
    165,
    141,
    80,
    139,
    116,
    93,
    84,
    144,
    143,
    142,
    90,
    164,
    75,
    66,
    16,
    112,
    70,
    113,
    120,
    68,
    76,
    102,
    99,
    152,
    107,
    110,
    136,
    91,
    168,
    36,
    121,
    3,
    50,
    32,
    31,
    64,
    130,
    167,
    157,
    63,
    92,
    51,
    148,
    159,
    105,
    115,
    13,
    155,
    37,
    53,
    67,
    77,
    22,
    85,
    82,
    108,
    28,
    15,
    1,
    21,
    40,
    124,
    149,
    119,
    129,
    160,
    48,
    109,
    52,
    128,
    138,
    162,
    163,
    154,
    41,
    96,
    7,
    12,
    2,
    69,
    87,
    145,
    60,
    132,
    103,
    72,
    126,
    147,
    101,
    42,
    58,
    134,
    133,
    83,
    89,
    29,
    71,
    97,
    98,
    95,
    39,
    49,
    57,
    0,
    8,
    24,
    65535
};

const char *getSVGCSSValueID(unsigned short id)
{
    if (!id || id > SVGCSS_VAL_MAX) return "";
    return wordlist_val[valList[id]].name;
}

int getSVGCSSValueID(const char *tagStr, int len)
{
    const struct css_val *valPtr = findSVGVal(tagStr, len);
    if(!valPtr)
      return 0;
    return valPtr->id;
}

}
