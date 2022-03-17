// グローバル
Texture2D g_texDecal : register(t0);        // テクスチャはレジスタt(n)
SamplerState g_samLinear : register(s0);    // サンプラーはレジスタs(n)

cbuffer global
{
    matrix g_mWVP;      // ワールドから射影までの変換行列
    float4 g_vDiffuse;  // ディフューズ色
    float g_fAlpha;     // 透明度
};

// 構造体
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

//
// バーテックスシェーダー
//
VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(Pos, g_mWVP);
    output.Tex = Tex;

    return output;
}

//
// ピクセルシェーダー
//
float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 tex = g_vDiffuse * g_texDecal.Sample(g_samLinear, input.Tex);
    if (tex.a <= 0.5f) discard; // 背景透過処理
    return tex * g_fAlpha;

    //return g_texDecal.Sample(g_samLinear, input.Tex);
}