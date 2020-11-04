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
  // Position of the light
  float3 lightPos = float3(1000, 0, 0);
  float lightIntesity = 5.0f;

  float3 lightColor = float3(1, 1, 1);
  float3 surfColor = float3(1, 1, 1);  //Material color

  //Computes the direction of the light to This Pixel
  float3 lightDir = lightPos - input.PosWorld;
  float distance = length(lightDir);
  lightDir = lightDir / distance;
  distance = distance * distance;

  //Get Diffuse texture value
  float4 diffuseText = txDiffuse.Sample(samLinear, input.Tex);

  //Get Normal teture value
  float4 normalText = txNormal.Sample(samLinear, input.Tex);

  //Compute surface normal
  float3 normal = 2.0f * normalText.xyz - 1.0f;
  normal = normalize(mul(normal, input.TBN));

  //Compute the view Direction
  float3 viewDir = normalize(View[3].xyz - input.PosWorld);

  //Compute the view Direction
  float kD = Lamber_Diffuse(lightDir, normal);

  //Blinn
  float3 Reflected = reflect(-lightDir, normal);

  //Specular component
  float3 H = normalize(lightDir + viewDir);

  //computes the specular incidence (HdN)
  float kS = max(0.0f, dot(H, normal));

  float intensity = pow(kS, 128.0f);

  //Computes final diffuse
  float3 finalDiffuse = surfColor * kD * lightColor * diffuseText.xyz * lightIntesity;

  //Computes final Specular
  float3 finalSpecular = kS * intensity * lightColor;

  //Computes the dinal color without alpha channel
  float3 kA = float3(0.05f, 0.05f, 0.05f);
  float3 finalColor = finalDiffuse + finalSpecular + kA;

  return float4(finalColor.xyz,1);
}
