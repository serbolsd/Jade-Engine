//--------------------------------------------------------------------------------------
// File: Tutorial072.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D txIn0 : register(t0);
Texture2D txIn1 : register(t1);

SamplerState samLinear : register(s0);


//cbuffer cbNeverChanges : register(b0)
//{
//  matrix View;
//  matrix ViewInv;
//};
//
//cbuffer cbChangeOnResize : register(b1)
//{
//  matrix Projection;
//  matrix ProjectionInv;
//  matrix ViewProjection;
//  matrix ViewProjectionInv;
//};
//
//cbuffer cbChangesEveryFrame : register(b2)
//{
//  matrix World;
//  matrix WorldInv;
//  matrix WorldView;
//  matrix WorldViewInv;
//  matrix WorldProj;
//  matrix WorldProjInv;
//  matrix WorldViewProj;
//  matrix WorldViewProjInv;
//  float4 viewPosition;
//  float4 vMeshColor;
//};
//
//cbuffer cbBonesTransform : register (b3)
//{
//  matrix boneTransform[200];
//};

//cbuffer cbLights : register (b4)
//{
//  lightStruct lights[20];
//};

//cbuffer cbAO : register (b5)
//{
//  float g_sample_radius;
//  float g_scale;
//  float g_Bias;
//  float g_intensity;
//};

//cbuffer cbBright : register(b6)
//{
//  //int BrightLod;                               // Luminance mip lvl for bright
//  //float4 BrightLod_BloomThreshold;                        // Bloom clamp
//};

cbuffer cbMipLevels : register(b7)
{
  //int BrightLod;                               // Luminance mip lvl for bright
  int mipLevle0;                        // Bloom clamp
  int mipLevle1;                        // Bloom clamp
  int mipLevle2;                        // Bloom clamp
  int mipLevle3;                        // Bloom clamp
};


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
  float4 Pos : POSITION;
  float4 Norm : NORMAL0;
  float4 Tang : TANGENT0;
  float2 Tex : TEXCOORD0;
  float4 Color : COLOR0;
  uint4 BonesIDs : BLENDINDICES0;
  float4 Weights : BLENDWEIGHT0;
};

struct PS_INPUT
{
  float4 Pos    : SV_POSITION;
  float2 Tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT)0;
  output.Pos = input.Pos;
  output.Tex = input.Tex;
  return output;
}

struct PS_OUTPUT
{
  float4 Color : SV_Target0;
};
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

PS_OUTPUT Add(PS_INPUT input)
{
  PS_OUTPUT output = (PS_OUTPUT)0;
  output.Color = 0.5f * (txIn0.SampleLevel(samLinear, input.Tex, mipLevle0) + txIn1.SampleLevel(samLinear, input.Tex, mipLevle1));
  return output;
}

PS_OUTPUT Blur(PS_INPUT input)
{
  PS_OUTPUT output = (PS_OUTPUT)0;
  const float2 OFFSETS[] =
#ifdef HORIZONTAL
  {float2(-4,0),float2(-3,0), float2(-2,0), float2(-1,0), float2(0,0), float2(1,0), float2(2,0), float2(3,0), float2(4,0)};
#else
  {float2(0, -4),float2(0, -3), float2(0, -2), float2(0, -1), float2(0,0), float2(0, 1), float2(0, 2), float2(0, 3), float2(0, 4)};
#endif
  static const float g_BlurWeights [] = {
    0.004815026f,
    0.028716039f,
    0.102818575f,
    0.221024189f,
    0.28525234f,
    0.221024189f,
    0.102818575f,
    0.028716039f,
    0.004815026f,
  };
  //txLuminance.Sample(samLinear, Input.Tex);
  //float Brigth = txLuminance.SampleLevel(samLinear, input.Tex, BrightLod_BloomThreshold.x).x;
  //Brigth = max(Brigth - BrightLod_BloomThreshold.y, 0.0f);
  output.Color = txIn0.Load(float3(input.Pos.xy + OFFSETS[0], mipLevle0 + 1)) * g_BlurWeights[0];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[1], mipLevle0 + 1)) * g_BlurWeights[1];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[2], mipLevle0 + 1)) * g_BlurWeights[2];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[3], mipLevle0 + 1)) * g_BlurWeights[3];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[4], mipLevle0 + 1)) * g_BlurWeights[4];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[5], mipLevle0 + 1)) * g_BlurWeights[5];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[6], mipLevle0 + 1)) * g_BlurWeights[6];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[7], mipLevle0 + 1)) * g_BlurWeights[7];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[8], mipLevle0 + 1)) * g_BlurWeights[8];
  //float4 color = float4(255,0,255,1);

  return output;
}

PS_OUTPUT BlurH(PS_INPUT input)
{
  PS_OUTPUT output = (PS_OUTPUT)0;
  const float2 OFFSETS[] =
  { float2(-4,0),float2(-3,0), float2(-2,0), float2(-1,0), float2(0,0), float2(1,0), float2(2,0), float2(3,0), float2(4,0) };

  static const float g_BlurWeights[] = {
    0.004815026f,
    0.028716039f,
    0.102818575f,
    0.221024189f,
    0.28525234f,
    0.221024189f,
    0.102818575f,
    0.028716039f,
    0.004815026f,
  };
  //txLuminance.Sample(samLinear, Input.Tex);
  //float Brigth = txLuminance.SampleLevel(samLinear, input.Tex, BrightLod_BloomThreshold.x).x;
  //Brigth = max(Brigth - BrightLod_BloomThreshold.y, 0.0f);
  output.Color = txIn0.Load(float3(input.Pos.xy + OFFSETS[0], mipLevle0)) * g_BlurWeights[0];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[1], mipLevle0)) * g_BlurWeights[1];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[2], mipLevle0)) * g_BlurWeights[2];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[3], mipLevle0)) * g_BlurWeights[3];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[4], mipLevle0)) * g_BlurWeights[4];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[5], mipLevle0)) * g_BlurWeights[5];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[6], mipLevle0)) * g_BlurWeights[6];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[7], mipLevle0)) * g_BlurWeights[7];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[8], mipLevle0)) * g_BlurWeights[8];
  //float4 color = float4(255,0,255,1);

  return output;
}

PS_OUTPUT BlurV(PS_INPUT input)
{
  PS_OUTPUT output = (PS_OUTPUT)0;
  const float2 OFFSETS[] =
  {float2(0, -4), float2(0, -3), float2(0, -2), float2(0, -1), float2(0, 0), float2(0, 1), float2(0, 2), float2(0, 3), float2(0, 4)};

  static const float g_BlurWeights[] = {
    0.004815026f,
    0.028716039f,
    0.102818575f,
    0.221024189f,
    0.28525234f,
    0.221024189f,
    0.102818575f,
    0.028716039f,
    0.004815026f,
  };
  //txLuminance.Sample(samLinear, Input.Tex);
  //float Brigth = txLuminance.SampleLevel(samLinear, input.Tex, BrightLod_BloomThreshold.x).x;
  //Brigth = max(Brigth - BrightLod_BloomThreshold.y, 0.0f);
  output.Color = txIn0.Load(float3(input.Pos.xy + OFFSETS[0], mipLevle0)) * g_BlurWeights[0];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[1], mipLevle0)) * g_BlurWeights[1];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[2], mipLevle0)) * g_BlurWeights[2];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[3], mipLevle0)) * g_BlurWeights[3];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[4], mipLevle0)) * g_BlurWeights[4];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[5], mipLevle0)) * g_BlurWeights[5];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[6], mipLevle0)) * g_BlurWeights[6];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[7], mipLevle0)) * g_BlurWeights[7];
  output.Color += txIn0.Load(float3(input.Pos.xy + OFFSETS[8], mipLevle0)) * g_BlurWeights[8];
  //float4 color = float4(255,0,255,1);

  return output;
}