//--------------------------------------------------------------------------------------
// File: Tutorial072.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer cbNeverChanges : register( b0 )
{
    matrix View;
};

cbuffer cbChangeOnResize : register( b1 )
{
    matrix Projection;
};

cbuffer cbChangesEveryFrame : register( b2 )
{
    matrix World;
    float4 vMeshColor;
};

cbuffer cbBonesTransform : register ( b3 )
{
  matrix boneTransform[400];
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Norm : NORMAL0;
    float4 Tang : TANG0;
    float2 Tex : TEXCOORD0;
    float4 Color : COLOR0;
    uint4 BonesIDs : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 PosWorld : COLOR0;
    float2 Tex : TEXCOORD0;
    float3 Normal : NORMAL0;
};


float Lambert_Diffuse(in float3 lightDir, in float3 surfNormal) {
  float f = dot(lightDir, surfNormal); // This is for toon shading
  return f;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    matrix boneTrans = boneTransform[input.BonesIDs[0]] * input.Weights[0];
    boneTrans += boneTransform[input.BonesIDs[1]] * input.Weights[1];
    boneTrans += boneTransform[input.BonesIDs[2]] * input.Weights[2];
    boneTrans += boneTransform[input.BonesIDs[3]] * input.Weights[3];
    float4 position = mul(input.Pos, boneTrans);
    output.Pos = mul( position, World );
    output.PosWorld = mul( position, World).xyz;
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Tex = input.Tex;
    output.Normal = mul(input.Norm, World).xyz;
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
  float3 LightPos = float3(10000, 0, 0);

  float3 surfColor = float3(1, 0,1);

  float3 LightDir = normalize(LightPos - input.PosWorld.xyz);

  float kD = Lambert_Diffuse(LightDir, input.Normal.xyz);

  float3 finalColor = surfColor * kD * 0.8f;

  float3 texColor = txDiffuse.Sample(samLinear, input.Tex).xyz * finalColor;

    return float4(texColor.xyz,1);
}
