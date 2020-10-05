
static const int NUM_DIRECTIONAL_LIGHT = 4;	//�f�B���N�V�������C�g�̖{���B

//���f���p�̒萔�o�b�t�@
cbuffer ModelCb : register(b0) {
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

//�f�B���N�V�������C�g�B
struct DirectionalLight {
	float3 direction;	//���C�g�̕����B
	float4 color;		//���C�g�̐F�B
};
//���C�g�p�̒萔�o�b�t�@�B
cbuffer LightCb : register(b1) {
	DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];
	float3 eyePos;					//�J�����̎��_�B
	float specPow;					//�X�y�L�����̍i��B
	float3 ambinentLight;			//�����B
};

//�X�L�j���O�p�̒��_�f�[�^���ЂƂ܂Ƃ߁B
struct SSkinVSIn {
	int4  Indices  	: BLENDINDICES0;
	float4 Weights  : BLENDWEIGHT0;
};

//���_�V�F�[�_�[�ւ̓��́B
struct SVSIn {
	float4 pos 		: POSITION;		//���f���̒��_���W�B
	float3 normal	: NORMAL;		//�@���B
	float2 uv 		: TEXCOORD0;	//UV���W�B
	SSkinVSIn skinVert;				//�X�L���p�̒��_�f�[�^�B
};
//�s�N�Z���V�F�[�_�[�ւ̓��́B
struct SPSIn {
	float4 pos 			: SV_POSITION;	//�X�N���[����Ԃł̃s�N�Z���̍��W�B
	float3 normal		: NORMAL;		//�@���B
	float2 uv 			: TEXCOORD0;	//uv���W�B
	float3 worldPos		: TEXCOORD1;	//���[���h��Ԃł̃s�N�Z���̍��W�B
};

//���f���e�N�X�`���B
Texture2D<float4> g_texture : register(t0);
Texture2D<float4> g_normalMap : register(t1);
Texture2D<float4> g_specularMap : register(t2);
//�{�[���s��B
StructuredBuffer<float4x4> boneMatrix : register(t3);

//�T���v���X�e�[�g�B
sampler g_sampler : register(s0);

//�X�L���s����v�Z����֐��B
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;
	float w = 0.0f;
	[unroll]
	for (int i = 0; i < 3; i++)
	{
		skinning += boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
		w += skinVert.Weights[i];
	}

	skinning += boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	return skinning;
}

//���_�V�F�[�_�[�̃R�A�֐��B
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if (hasSkin) {
		m = CalcSkinMatrix(vsIn.skinVert);
	}
	else {
		m = mWorld;
	}

	psIn.pos = mul(m, vsIn.pos);						//���f���̒��_�����[���h���W�n�ɕϊ��B
	psIn.worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos);						//���[���h���W�n����J�������W�n�ɕϊ��B
	psIn.pos = mul(mProj, psIn.pos);						//�J�������W�n����X�N���[�����W�n�ɕϊ��B
	psIn.normal = normalize(mul(m, vsIn.normal));		//�@�������[���h���W�n�ɕϊ��B
	psIn.uv = vsIn.uv;

	return psIn;
}

/// <summary>
/// ���f���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g�B
/// </summary>
SPSIn VSMain(SVSIn vsIn, uniform bool hasSkin)
{
	return VSMainCore(vsIn, false);
}

/*!--------------------------------------------------------------------------------------
 * @brief	�X�L�����胂�f���p�̒��_�V�F�[�_�[�B
-------------------------------------------------------------------------------------- */
SPSIn VSMainSkin(SVSIn vsIn)
{
	return VSMainCore(vsIn, true);
}

/// <summary>
/// ���f���p�̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	float3 lig = 0.0f;
	float metaric = g_specularMap.Sample(g_sampler, psIn.uv).a;
	//////////////////////////////////////////////////////
	// �g�U���˂��v�Z
	//////////////////////////////////////////////////////
	{
		for (int i = 0; i < NUM_DIRECTIONAL_LIGHT; i++) {
			float NdotL = dot(psIn.normal, -directionalLight[i].direction);	//���C�g�̋t�����Ɩ@���œ��ς��v�Z����B
			if (NdotL < 0.0f) {	//���ς̌v�Z���ʂ̓}�C�i�X�ɂȂ�̂ŁAif���Ŕ��肷��B
				NdotL = 0.0f;
			}
			float3 diffuse;
			diffuse = directionalLight[i].color.xyz * (1.0f - metaric) * NdotL; //�g�U���ˌ��𑫂��Z����B

			//return float4( diffuse, 1.0f);		//�Ȃ񂩊g�U���˃��x���������������񂩂���H

			//���C�g�����Ă镨�̂��王�_�Ɍ������ĐL�т�x�N�g�����v�Z����B
			float3 eyeToPixel = eyePos - psIn.worldPos;
			eyeToPixel = normalize(eyeToPixel);

			//���̕��̂ɓ������āA���˂����x�N�g�������߂�B
			float3 reflectVector = reflect(directionalLight[i].direction, psIn.normal);
			//���˂��������ڂɔ�э���ė��Ă��邩�ǂ������A���ς��g���Ē��ׂ�B
			float d = dot(eyeToPixel, reflectVector);
			if (d < 0.0f) {
				d = 0.0f;
			}
			d = pow(d, specPow) * metaric;
			float3 spec = directionalLight[i].color * d * 5.0f;
			//�X�y�L�������˂̌��𑫂��Z����B
			lig += diffuse + spec;
		}
	}

	//////////////////////////////////////////////////////
	// �������v�Z
	//////////////////////////////////////////////////////
	lig += ambinentLight; //�����Z���邾��

	float4 texColor = g_texture.Sample(g_sampler, psIn.uv);
	texColor.xyz *= lig; //�����e�N�X�`���J���[�ɏ�Z����B
	return float4(texColor.xyz, 1.0f);
}
