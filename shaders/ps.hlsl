struct PixelShaderInput
{
	float2 twodpos : POS;
	float4 position : SV_POSITION;
	float4 scrpos : SCRPOS;
};


float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 val = float4(input.scrpos.yx, input.scrpos.y * input.scrpos.x, 1);
	return val;
}    