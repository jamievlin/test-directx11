struct VertShaderInput
{
	float2 input : VTX_INPUT;
};

struct PixelShaderInput
{
	float2 twodpos : POS;
	float4 position : SV_POSITION;
	float4 scrpos : SCRPOS;
};

PixelShaderInput main(VertShaderInput input)
{
	PixelShaderInput output;
	output.twodpos = input.input;
	output.position = float4(input.input, 0, 1);
	output.scrpos = float4(input.input, 0, 1);
	return output;
}