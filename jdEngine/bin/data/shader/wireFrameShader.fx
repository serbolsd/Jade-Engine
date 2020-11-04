//--------------------------------------------------------------------------------------
// File: Tutorial072.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------


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
  float4 Color : COLOR0;
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

  output.Pos = mul(position, World);
  output.Pos = mul(output.Pos, View);
  output.Pos = mul(output.Pos, Projection);
  output.Color = input.Color;

  return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
    return input.Color;
}
