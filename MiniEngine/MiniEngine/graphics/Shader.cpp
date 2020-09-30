#include "stdafx.h"
#include "Shader.h"
#include <stierr.h>

namespace {
	const char* g_vsShaderModelName = "vs_5_0";	//頂点シェーダーのシェーダーモデル名。
	const char* g_psShaderModelName = "ps_5_0";	//ピクセルシェーダーのシェーダモデル名。
	const char* g_csShaderModelName = "cs_5_0";	//コンピュートシェーダーのシェーダーモデル名。
}
void Shader::Load(const wchar_t* filePath, const char* entryFuncName, const char* shaderModel)
{
	ID3DBlob* errorBlob;
#ifdef _DEBUG
	// Enable better shader debugging with the graphics debugging tools.
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT compileFlags = 0;
#endif
	auto hr = D3DCompileFromFile(filePath, nullptr, nullptr, entryFuncName, shaderModel, compileFlags, 0, &m_blob, &errorBlob);
	
	if (FAILED(hr)) {
		if (hr == STIERR_OBJECTNOTFOUND) {
			std::wstring errorMessage;
			errorMessage = L"指定されたfxファイルが開けませんでした。";
			errorMessage += filePath;
			MessageBoxW(nullptr, errorMessage.c_str(), L"エラー", MB_OK);
		}
		if (errorBlob) {
			static char errorMessage[10 * 1024];
			sprintf_s(errorMessage, "filePath : %s, %s", "hoge", (char*)errorBlob->GetBufferPointer());
			MessageBoxA(NULL, errorMessage, "シェーダーコンパイルエラー", MB_OK);
			return;
		}
	}
}
void Shader::LoadPS(const wchar_t* filePath, const char* entryFuncName)
{
	Load(filePath, entryFuncName, g_psShaderModelName);
}
void Shader::LoadVS(const wchar_t* filePath, const char* entryFuncName)
{
	Load(filePath, entryFuncName, g_vsShaderModelName);
}
void Shader::LoadCS(const wchar_t* filePath, const char* entryFuncName)
{
	Load(filePath, entryFuncName, g_csShaderModelName);
}


