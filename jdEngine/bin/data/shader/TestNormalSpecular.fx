//--------------------------------------------------------------------------------------
// File: Tutorial072.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
Texture2D txSpecularMetal : register(t2);
Texture2D txRoughness : register(t3);
SamplerState samLinear : register(s0);

cbuffer cbNeverChanges : register( b0 )
{
    matrix View;
    matrix ViewInv;
};

cbuffer cbChangeOnResize : register( b1 )
{
    matrix Projection;
    matrix ProjectionInv;
    matrix ViewProjection;
    matrix ViewProjectionInv;
};

cbuffer cbChangesEveryFrame : register( b2 )
{
    matrix World;
    matrix WorldInv;
    matrix WorldView;
    matrix WorldViewInv;
    matrix WorldProj;
    matrix WorldProjInv;
    matrix WorldViewProj;
    matrix WorldViewProjInv;
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
    float4 Tang : TANGENT0;
    float2 Tex : TEXCOORD0;
    float4 Color : COLOR0;
    uint4 BonesIDs : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 PosWorld : TEXCOORD0;
    float2 Tex : TEXCOORD1;
    float3x3 TBN : TEXCOORD2;
};

float Lamber_Diffuse(in float3 lightDir, in float3 surfNormal)
{
  return max(0.0f, dot(lightDir, surfNormal));
}

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

  output.PosWorld = mul(position, World).xyz;
  output.Pos = mul(position, World);
  output.Pos = mul(output.Pos, View);
  output.Pos = mul(output.Pos, Projection);
  output.TBN[0] = normalize(mul(input.Tang, boneTrans)).xyz;
  output.TBN[0] = normalize(mul(float4(output.TBN[0], 0), World)).xyz;
  output.TBN[2] = normalize(mul(input.Norm, boneTrans)).xyz;
  output.TBN[2] = normalize(mul(float4(output.TBN[2], 0), World)).xyz;
  output.TBN[1] = normalize(-cross(output.TBN[0], output.TBN[2]));
  output.Tex = input.Tex;
  return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
  float4 finalColor = txDiffuse.Sample(samLinear, input.Tex.xy);
  float4 normalColor = txNormal.Sample(samLinear, input.Tex.xy);
  float4 specularColor = txSpecularMetal.Sample(samLinear, input.Tex.xy);
  float metallic = specularColor.r;

  //absortion factor


  //conver color to linear space
  finalColor.xyz = pow(abs(finalColor.xyz), 2.2f);

  //Factor de fresnel en angulo 0
  float3 F0 = lerp(0.04f, finalColor.xyz, metallic);

  //convert normal to world space
  float3 normal = 2.0f * normalColor.xyz - 1.0f;
  normal = normalize(mul(normal, input.TBN));
  //float normal = input.TBN[2].xyz;

  //Compute light and view vectors
  float3 lightDir = -normalize(float3(-1, 0, 1));
  float3 viewDir = normalize(View[3].xyz - input.PosWorld);

  //Diffuse incidence
  float NdL = max(0.0f, dot(lightDir, normal));

  //Half vector
  float3 H = normalize(lightDir + viewDir);

  //specular incidence
  float NdH = max(0.0f, dot(normal, H));

  float kS = pow(NdH, max(1.0f, metallic * 255.0f));
  float3 finalSpec = F0.xyz * kS;

  //return float4(finalSpec,1.0f);
  return float4(pow(abs(finalColor.xyz * NdL + finalSpec),1.0f / 2.2f), finalColor.w);
  //return float4(finalColor.xyz,1);
}
