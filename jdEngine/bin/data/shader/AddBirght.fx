//--------------------------------------------------------------------------------------
// File: Tutorial072.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txBright : register(t0);
Texture2D txBlurH : register(t1);
Texture2D txBlurV : register(t2);

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
PS_OUTPUT AddBright(PS_INPUT input)
{
  PS_OUTPUT output = (PS_OUTPUT)0;
  
  float4 texBright = txBright.Sample(samLinear, input.Tex);

  float4 texBlurH = txBlurH.Sample(samLinear, input.Tex);

  float4 texBlurV = txBlurV.Sample(samLinear, input.Tex);

  output.Color = (texBlurH + texBlurV) + texBright;

  return output;
}
