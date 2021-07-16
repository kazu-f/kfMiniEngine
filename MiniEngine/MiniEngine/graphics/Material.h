#pragma once

#include "tkFile/TkmFile.h"


namespace Engine {
	/// <summary>
	/// マテリアル。
	/// </summary>
	class IMaterial {
	public:
		virtual ~IMaterial() {};
		/// <summary>
		/// tkmファイルのマテリアル情報から初期化する。
		/// </summary>
		/// <param name="tkmMat">tkmマテリアル</param>
		virtual void InitFromTkmMaterila(
			const TkmFile::SMaterial& tkmMat,
			const SShaderInitData& shaderData);
		/// <summary>
		/// レンダリングを開始するときに呼び出す関数。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="maxInstance">インスタンスの数</param>
		virtual void BeginRender(RenderContext& rc, int maxInstance = 1) = 0;
	public:
		void SetAlbedoMap(Texture& tex)
		{
			m_albedoMap.InitFromD3DResource(tex.Get());
		}


	public:		//Get関数。
		/// <summary>
		/// アルベドマップを取得。
		/// </summary>
		/// <returns></returns>
		Texture& GetAlbedoMap()
		{
			return m_albedoMap;
		}
		/// <summary>
		/// 法線マップを取得。
		/// </summary>
		/// <returns></returns>
		Texture& GetNormalMap()
		{
			return m_normalMap;
		}
		/// <summary>
		/// スペキュラマップを取得。
		/// </summary>
		/// <returns></returns>
		Texture& GetSpecularMap()
		{
			return m_specularMap;
		}
		Texture& GetReflectionMap()
		{
			return m_reflectionMap;
		}
		/// <summary>
		/// 定数バッファを取得。
		/// </summary>
		/// <returns></returns>
		ConstantBuffer& GetConstantBuffer()
		{
			return m_constantBuffer;
		}
	protected:
		/// <summary>
		/// テクスチャを初期化。
		/// </summary>
		/// <param name="tkmMat"></param>
		void InitTexture(const TkmFile::SMaterial& tkmMat);

	protected:
		/// <summary>
		/// マテリアルパラメータ。
		/// </summary>
		struct SMaterialParam {
			int hasNormalMap;	//法線マップを保持しているかどうかのフラグ。
			int hasSpecMap;		//スペキュラマップを保持しているかどうかのフラグ。
			int hasReflectionMap;		//スペキュラマップを保持しているかどうかのフラグ。
		};
		Texture	m_albedoMap;	//アルベドマップ。
		Texture	m_normalMap;							//法線マップ。
		Texture	m_specularMap;							//スペキュラマップ。
		Texture	m_reflectionMap;						//反射マップ。
		ConstantBuffer m_constantBuffer;				//定数バッファ。
	};
	/// <summary>
	/// スキン無しマテリアル。
	/// </summary>
	class NonSkinMaterial : public IMaterial {
	public:
		/// <summary>
		/// レンダリングを開始するときに呼び出す関数。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="maxInstance">インスタンスの数</param>
		void BeginRender(RenderContext& rc, int maxInstance = 1) override final;

	};
	/// <summary>
	/// スキンありマテリアル。
	/// </summary>
	class SkinMaterial :public IMaterial{
	public:
		/// <summary>
		/// レンダリングを開始するときに呼び出す関数。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="maxInstance">インスタンスの数</param>
		void BeginRender(RenderContext& rc, int maxInstance = 1) override final;
	};
}