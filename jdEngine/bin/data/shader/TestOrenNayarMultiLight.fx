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


Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
Texture2D txSpecularMetal : register(t2);
Texture2D txRoughness : register(t3);
SamplerState samLinear : register(s0);

struct lightStruct
{
  float4 lightDirection;
  float4 lightPosition;
  float4 lightColor;
  uint type;//0= directional 1=point 2= spot
  uint numberOfLights;
};

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

cbuffer cbLights : register (b4)
{
  lightStruct lights[20];
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
PS_INPUT VS(VS_INPUT input)
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
  output.TBN[1] = normalize(cross(output.TBN[0], output.TBN[2]));
  output.Tex = input.Tex;
  return output;
}


//Normal distribution Function -GGX
float ndf_GGX(float NdH, float roughness)
{
  float alpha = roughness * roughness;
  float alphaSqr = alpha * alpha;

  float denom = (NdH * NdH) * (alphaSqr - 1.0f) + 1.0f;
  return alphaSqr / (M_PI * denom * denom);
}

//schlick Aproximation of the fresnel factor
float3 fresnelSchlick(float3 F0, float cosTheta)
{
  return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

//geometri atenuation
float ga_SchlickG1(float cosTheta, float k)
{
  return cosTheta / (cosTheta * (1.0f - k) + k);
}

float ga_SchlickGGX(float cosLi, float cosLo, float roughness)
{
  float r = roughness + 1.0f;
  float k = (r * r) / 8.0f;
  return ga_SchlickG1(cosLi, k) * ga_SchlickG1(cosLo, k);
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
  float4 finalColor = txDiffuse.Sample(samLinear, input.Tex.xy);
  float4 normalColor = txNormal.Sample(samLinear, input.Tex.xy);
  float4 specularColor = txSpecularMetal.Sample(samLinear, input.Tex.xy);
  float4 roughnessColor = txRoughness.Sample(samLinear, input.Tex.xy);
  float metallic = specularColor.r;
  float roughness = roughnessColor.r;
  //absortion factor

  //conver color to linear space
  finalColor.xyz = pow(abs(finalColor.xyz), 2.2f);
  float3 result = (0,0,0);
  //Factor de fresnel en angulo 0
  float3 F0 = lerp(0.04f, finalColor.xyz, metallic);

  //return float4(F0,1);

  //convert normal to world space
  float3 normal = 2.0f * normalColor.xyz - 1.0f;
  normal = normalize(mul(normal, input.TBN));
  //float normal = input.TBN[2].xyz;

  //Compute light and view vectors

  float3 viewDir = normalize(viewPosition.xyz - input.PosWorld);

  for (int i = 0; i < lights[0].numberOfLights; i++)
  {
    float3 lightDir = -normalize(lights[i].lightDirection.xyz);

    //Diffuse incidence
    float NdL = max(0.0f, dot(lightDir, normal));

    //Half vector
    float3 H = normalize(lightDir + viewDir);

    //specular incidence
    float NdH = max(0.0f, dot(normal, H));
    float NdV = max(0.0f, dot(normal, viewDir));
    float HdV = max(0.0f, dot(H, viewDir));

    float D = ndf_GGX(NdH, roughness);
    float3 F = fresnelSchlick(F0,HdV);
    float G = ga_SchlickGGX(NdL, NdV, roughness);

    float3 specularBRDF = (D * F * G) / max(EPSILON,4.0F * NdL * NdV);

    float3 kD = lerp(float3(1,1,1), float3(0,0,0), metallic.xxx);
    float3 diffuseBRDF = finalColor.xyz * kD * lights[i].lightColor.xyz;
    result += pow(abs(diffuseBRDF.xyz + specularBRDF) * NdL, 1.0f / 2.2f);
  }

  //result /= lights[0].numberOfLights;
  return float4(result, finalColor.w);
}
