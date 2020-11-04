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


//Texture2D txDiffuse : register(t0);
//Texture2D txNormal : register(t1);
//Texture2D txSpecularMetal : register(t2);
//Texture2D txRoughness : register(t3);
//TextureCube cubeMap : register(t4);
//
//
//SamplerState samLinear : register(s0);
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
  float4 Pos : SV_POSITION;
  float4 worldPos : COLOR0;
  float4 Norm : COLOR1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
  PS_INPUT output = (PS_INPUT)0;
  matrix boneTrans = boneTransform[input.BonesIDs[0]] * input.Weights[0];
  boneTrans += boneTransform[input.BonesIDs[1]] * input.Weights[1];
  boneTrans += boneTransform[input.BonesIDs[2]] * input.Weights[2];
  boneTrans += boneTransform[input.BonesIDs[3]] * input.Weights[3];
  float4 position = mul(input.Pos, boneTrans);

  output.worldPos = mul(position, World);
  output.Pos = mul(position, World);
  output.Pos = mul(output.Pos, View);
  output.Pos = mul(output.Pos, Projection);
  output.Norm = normalize(mul(input.Norm, boneTrans));
  output.Norm = normalize(mul(output.Norm, World));
  //output.TBN[0] = normalize(mul(input.Tang, boneTrans)).xyz;
  //output.TBN[0] = normalize(mul(float4(output.TBN[0], 0), World)).xyz;
  //output.TBN[2] = normalize(mul(input.Norm, boneTrans)).xyz;
  //output.TBN[2] = normalize(mul(float4(output.TBN[2], 0), World)).xyz;
  //output.TBN[1] = normalize(cross(output.TBN[0], output.TBN[2]));
  //output.Tex = input.Tex;
  return output;
}

struct PS_OUTPUT
{
  float4 Positions : SV_Target0;
  float4 Normal : SV_Target1;
};
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUTPUT PS(PS_INPUT input)
{
  PS_OUTPUT output = (PS_OUTPUT)0;
  output.Positions = (input.worldPos);
  //output.Positions = float4(0, 1, 0, 1);
  output.Normal = float4(input.Norm.xyz, 1);
  //output.Normal = float4(1,0,0, 1);
 
  return output;
}
