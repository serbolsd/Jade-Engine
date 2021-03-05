//--------------------------------------------------------------------------------------
// File: BillBoard.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txBright : register(t0);

SamplerState samLinear : register(s0);


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
  matrix depthMVP;
  float4 viewPosition;
  float4 vMeshColor;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
  float4 Pos : POSITION;
  float2 Size : SIZE;
};

struct VS_OUTPUT
{
  float4 Center : POSITION;
  float2 Size : SIZE;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS_Billboard(VS_INPUT input)
{
  VS_OUTPUT output = (VS_OUTPUT)0;
  output.Center = input.Pos;
  output.Size = input.Size;
  return output;
}
//////////////////////////////////////////////////////////////////////////////////////////////
//Geometry shader
//////////////////////////////////////////////////////////////////////////////////////////////
struct GS_OUTPUT
{
  float4 PosH : SV_POSITION;
  float3 PosW : POSITION;
  float3 NormalW : NORMAL;
  float2 TexCoord : TEXCOORD;
  //uint PrimID : SV_PrimitiveID;
};

[maxvertexcount(4)]
void GS_Billboard(point VS_OUTPUT input[1], inout TriangleStream<GS_OUTPUT> OutputStream) {
  float3 up = float3(0.0f, 1.0f, 0.0f);

  float3 look = (viewPosition - input[0].Center).xyz;
  //look = float3(0, 0, -1);
  look.y = 0.0f; // y-axis aligned, so project to xz-plane

  look = -normalize(look);

  float3 right = cross(look, up);

  //
  // Compute triangle strip vertices (quad) in world space.
  //
  //float size = 1000;
  float halfWidth = 0.5f * input[0].Size.x;
  //float halfWidth = 0.5f * size;
  float halfHeight = 0.5f * input[0].Size.y;
  //float halfHeight = 0.5f * size;
  float4 v[4];
  float3 center = input[0].Center.xyz;
  v[0] = float4(input[0].Center + halfWidth * right - halfHeight * up, 1.0f);
  v[1] = float4(input[0].Center + halfWidth * right + halfHeight * up, 1.0f);
  v[2] = float4(input[0].Center - halfWidth * right - halfHeight * up, 1.0f);
  v[3] = float4(input[0].Center - halfWidth * right + halfHeight * up, 1.0f);

  //
  // Transform quad vertices to world space and output
  // them as a triangle strip.
  //
  float2 texC[4] = { float2(0.0f, 1.0f),
                     float2(0.0f, 0.0f),
                     float2(1.0f, 1.0f),
                     float2(1.0f, 0.0f) };

  GS_OUTPUT output;

  [unroll]
  for (int i = 0; i < 4; ++i) {
    output.PosH = mul(v[i], World);
    output.PosH = mul(output.PosH, View);
    output.PosH = mul(output.PosH, Projection);
    output.PosW = v[i].xyz;
    output.NormalW = look;
    output.TexCoord = texC[i];
    //output.PrimID = primID;
    OutputStream.Append(output);
  }

}

struct PS_OUTPUT
{
  float4 Color : SV_Target0;
};
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUTPUT PS_Billboard(GS_OUTPUT input)
{
  PS_OUTPUT output = (PS_OUTPUT)0;

  output.Color = float4(255, 0, 0, 255);

  return output;
}