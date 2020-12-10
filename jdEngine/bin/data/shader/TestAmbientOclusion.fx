//--------------------------------------------------------------------------------------
// File: Tutorial072.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#define M_PI 3.1415926538
#define EPSILON 0.00001


Texture2D txPosition : register(t0);
Texture2D txNormal : register(t1);
Texture2D txDepth : register(t2);
//Texture2D txSpecularMetal : register(t2);
//Texture2D txRoughness : register(t3);
//TextureCube cubeMap : register(t4);
//
//
SamplerState samLinear : register(s0);
//
//struct lightStruct
//{
//  float4 lightDirection;
//  float4 lightPosition;
//  float4 lightColor;
//  uint type;//0= directional 1=point 2= spot
//  uint numberOfLights;
//};

cbuffer cbNeverChanges : register(b0)
{
  matrix View;
  matrix ViewInv;
};

cbuffer cbChangeOnResize : register(b1)
{
  matrix Projection;
  matrix ProjectionInv;
  matrix ViewProjection;
  matrix ViewProjectionInv;
};

cbuffer cbChangesEveryFrame : register(b2)
{
  matrix World;
  matrix WorldInv;
  matrix WorldView;
  matrix WorldViewInv;
  matrix WorldProj;
  matrix WorldProjInv;
  matrix WorldViewProj;
  matrix WorldViewProjInv;
  float4 viewPosition;
  float4 vMeshColor;
};

cbuffer cbBonesTransform : register (b3)
{
  matrix boneTransform[200];
};

//cbuffer cbLights : register (b4)
//{
//  lightStruct lights[20];
//};

cbuffer cbAO : register (b5)
{
  float g_sample_radius;
  float g_scale;
  float g_Bias;
  float g_intensity;
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

float4 getPosition(in float2 uv)
{
  return txPosition.Sample(samLinear, uv);
}

float3 getNormal(in float2 uv)
{
  return normalize(txNormal.Sample(samLinear, uv).xyz);
}

float3 getRandom(in float2 uv)
{
  float noiseX = (frac(sin(dot(uv, float2(15.8989f, 76.132f) * 1.0f)) * 46336.23745f));
  float noiseY = (frac(sin(dot(uv, float2(11.9899f, 62.223f) * 2.0f)) * 34748.34744f));
  float noiseZ = (frac(sin(dot(uv, float2(13.3238f, 63.122f) * 3.0f)) * 59998.47362f));

  return normalize(float3(noiseX, noiseY, noiseZ));
}

float DoAmbientOcclusion(in float2 tcoord, in float2 uv, in float3 p, in float3 cnorm) 
{
  float4 pos = getPosition(tcoord + uv);

  float3 diff = pos.xyz - p;
  const float lengthDiff = length(diff);
  const float3 v = diff / lengthDiff;
  const float d = lengthDiff * g_scale;
  return max(0.0f, dot(cnorm, v) - g_Bias) * (1.0 / (1.0 + d)) * g_intensity;
}

struct PS_OUTPUT
{
  float4 Color : SV_Target0;
};
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUTPUT PS(PS_INPUT input)
{
  PS_OUTPUT output = (PS_OUTPUT)0;

  float depth = txDepth.Sample(samLinear, input.Tex.xy).x;
  
  output.Color = float4 (1, 1, 1, 1);
  if (depth==0)
  {
    return output;
  }

  
  const float2 vec[4] =
  {
    float2(1,0),float2(-1,0),float2(0,1),float2(0,-1)
  };
  
  float4 p = getPosition(input.Tex);

  //clip(p.w < 1.0f ? -1 : 1);

  float3 n = getNormal(input.Tex);
  float3 rand = getRandom(input.Tex);

  float ao = 0.0f;
  float rad = g_sample_radius / p.z;

  int iterations = 4;
  for (int j = 0; j < iterations; j++)
  {
    float2 coord1 = reflect(vec[j], rand) * rad;
    float2 coord2 = float2(coord1.x * 0.707 - coord1.y * 0.707, 
                           coord1.x * 0.707 + coord1.y * 0.707);

    ao += DoAmbientOcclusion(input.Tex, coord1 * 0.25, p.xyz, n);
    ao += DoAmbientOcclusion(input.Tex, coord2 * 0.5,  p.xyz, n);
    ao += DoAmbientOcclusion(input.Tex, coord1 * 0.75, p.xyz, n);
    ao += DoAmbientOcclusion(input.Tex, coord2,        p.xyz, n);
  }

  ao /= (iterations * 4);

  output.Color.rgb = 1 - ao.xxx;

  return output;
  //float4 normalColor = txNormal.Sample(samLinear, input.Tex.xy);
  //float3 normal = 2.0f * normalColor.xyz - 1.0f;
  //normal = normalize(mul(normal, input.TBN));
  //
  //PS_OUTPUT output = (PS_OUTPUT)0;
  //output.Positions = (input.worldPos);
  ////output.Positions = float4(0, 1, 0, 1);
  //output.Normal = float4(normal, 1);
  ////output.Normal = float4(1,0,0, 1);
  //
  //return output;
}
