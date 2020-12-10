//--------------------------------------------------------------------------------------
// File: Tutorial072.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D texAddBright : register(t0);
Texture2D texColor : register(t1);

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

cbuffer cbBright : register(b6)
{
  //int BrightLod;                               // Luminance mip lvl for bright
  float4 BrightLod_BloomThreshold;                        // Bloom clamp
};


float3 BasicExposure(float3 inColor)
{
  inColor *= BrightLod_BloomThreshold.z;
  inColor = pow(abs(inColor), 1.0f / 2.2f);

  return inColor;
}

float3 Reinhard(float3 inColor)
{
  inColor *= BrightLod_BloomThreshold.z;
  inColor /= (1.0f + inColor);
  inColor = pow(abs(inColor), 1.0f / 2.2f);

  return inColor;
}
/*
float4 Haarm_Peter_Duiker(in float3 Color, in float exposure)
{
   float3 texColor = Color;
   texColor *= exposure;
   float3 ld = 0.002f;
   float linReference = 0.18;
   float logReference = 444;
   float logGamma = 0.45f;
   float3 LogColor;
   LogColor.rgb = (log10(0.4f * texColor.rgb / linReference) / ld * logGamma + logReference) / 1023.f;
   LogColor.rgb = saturate(LogColor.rgb);
   float FilmLutWidth = 256;
   float Padding = 0.5f / FilmLutWidth;
   float3 retColor;
   retColor.r = tex2D(DefaultSampler, float2(lerp(Padding, 1 - Padding, LogColor.r), 0.5f)).r;
   retColor.g = tex2D(DefaultSampler, float2(lerp(Padding, 1 - Padding, LogColor.g), 0.5f)).r;
   retColor.b = tex2D(DefaultSampler, float2(lerp(Padding, 1 - Padding, LogColor.b), 0.5f)).r;
   return float4(retColor, 1);
}*/

float3 Burgess_Dawson(float3 inColor)
{
  inColor *= BrightLod_BloomThreshold.z;
  float3 x = max(0, inColor - 0.004f);
  inColor = (x * (6.2 * x + 0.5f)) / (x * (6.2f * x + 1.7f) + 0.06f);

  return inColor;
}

float3 Uncharted2Tonemap(float3 inx)
{
  float A = 0.15f;
  float B = 0.5f;
  float C = 0.1f;
  float D = 0.2f;
  float E = 0.02f;
  float F = 0.3f;

  return ((inx * (A * inx + C * B) + D * E) / (inx * (A * inx + B) + D * F)) - E / F;
}

float3 Uncharted2(float3 inColor)
{
  float W = 11.2f;
  inColor *= BrightLod_BloomThreshold.z;
  float ExposureBias = 2.0f;
  float3 curr = Uncharted2Tonemap(ExposureBias * inColor);

  float3 whiteScale = 1.0f / Uncharted2Tonemap(W);
  inColor = curr * whiteScale;

  inColor = pow(abs(inColor), 1 / 2.2);
  return inColor;
}

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
PS_OUTPUT toneMap(PS_INPUT input)
{
  PS_OUTPUT output = (PS_OUTPUT)0;
  float3 Color = texColor.Sample(samLinear, input.Tex).xyz;

  // add HDR
  Color += BrightLod_BloomThreshold.w * texAddBright.Sample(samLinear, input.Tex).xyz;

  if (input.Tex.x > 0.5f)
  {
    if (input.Tex.y > 0.5f)
      Color = Burgess_Dawson(Color);
    else
      Color = BasicExposure(Color);
  }
  if (input.Tex.x < 0.5f)
  {
    if (input.Tex.y > 0.5f)
      Color = Uncharted2(Color);
    else
      Color = Reinhard(Color);
  }

  output.Color = float4(Reinhard(Color),1.0f);

  return output;
}
