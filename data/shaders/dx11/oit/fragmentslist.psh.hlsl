#include <common.h.hlsl>
#include <pscommon.h.hlsl>

struct ListNode
{
	uint packedColor;
	uint depthAndCoverage;
	uint next;
};
globallycoherent RWTexture2D<uint> headBuffer;
globallycoherent RWStructuredBuffer<ListNode> fragmentsList;

uint packColor(float4 color)
{
	return (uint(color.r * 255) << 24) | (uint(color.g * 255) << 16) | (uint(color.b * 255) << 8) | uint(color.a * 255);
}

[earlydepthstencil]
float4 main(VS_OUTPUT input, uint coverage : SV_COVERAGE, bool frontFace : SV_IsFrontFace) : SV_TARGET
{
	float4 color = computeColorTransparent(input, frontFace);
	uint newHeadBufferValue = fragmentsList.IncrementCounter();
	if (newHeadBufferValue == 0xffffffff) { return float4(0, 0, 0, 0); }
	
	uint2 upos = uint2(input.position.xy);
	uint previosHeadBufferValue;
	InterlockedExchange(headBuffer[upos], newHeadBufferValue, previosHeadBufferValue);
	
	uint currentDepth = f32tof16(input.worldPos.w);
	ListNode node;
	node.packedColor = packColor(float4(color.rgb, color.a));
	node.depthAndCoverage = currentDepth | (coverage << 16);
	node.next = previosHeadBufferValue;
	fragmentsList[newHeadBufferValue] = node;
	
	return float4(0, 0, 0, 0);
}