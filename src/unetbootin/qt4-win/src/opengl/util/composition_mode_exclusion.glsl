// Dca' = (Sca.Da + Dca.Sa - 2.Sca.Dca) + Sca.(1 - Da) + Dca.(1 - Sa)
// Da'  = Sa + Da - Sa.Da
vec4 composite(vec4 src, vec4 dst)
{
    vec4 result;
    result.rgb = (src.rgb * dst.a + dst.rgb * src.a - 2 * src.rgb * dst.rgb) + src.rgb * (1 - dst.a) + dst.rgb * (1 - src.a);
    result.a = src.a + dst.a - src.a * dst.a;
    return result;
}
