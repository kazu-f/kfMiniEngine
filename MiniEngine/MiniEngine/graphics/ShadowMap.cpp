#include "stdafx.h"
#include "ShadowMap.h"

namespace Engine {
	void CShadowMap::Init(SShadowMapConfig& config)
	{
		m_isEnable = config.isEnable;
		if (!m_isEnable) return;		//無効だったら戻る。

		//シャドウマップの範囲を設定する。
		for (int i = 0; i < NUM_SHADOW_MAP; i++)
		{
			m_shadowAreas[i] = config.shadowAreas[i];
			m_depthOffset[i] = config.depthOffset[i];
		}

		//シャドウマップの解像度の設定。
		int wh[NUM_SHADOW_MAP][2] = {
			{config.shadowMapWidth,config.shadowMapHeight},					//近距離
			{config.shadowMapWidth,config.shadowMapHeight >> 1},		//中距離。
			{config.shadowMapWidth >> 1,config.shadowMapHeight >> 1}	//遠距離。
		};
		//クリアカラーを作成。
		float clearColor[] = {
			1.0f,1.0f,1.0f,1.0f
		};
		//シャドウマップ用のレンダーターゲット作成。
		for (int shadowMapNo = 0; shadowMapNo < NUM_SHADOW_MAP;shadowMapNo++) {
			m_shadowMaps[shadowMapNo].Create(
				wh[shadowMapNo][0],		//幅
				wh[shadowMapNo][1],		//高さ
				1,						//
				1,						//
				DXGI_FORMAT_R32_FLOAT,	//カラーフォーマット。
				DXGI_FORMAT_D32_FLOAT,	//デプスステンシルフォーマット。
				clearColor				//クリアカラー。
			);
		}

		//定数バッファを初期化する。
		m_shadowCb.Init(sizeof(m_shadowCbEntity), nullptr);
	}
	void CShadowMap::RenderToShadowMap(RenderContext& rc)
	{
		//無効だったら返す。
		if (!m_isEnable) return;

		//シャドウマップ用の定数バッファを更新。
		m_shadowCb.CopyToVRAM(m_shadowCbEntity);
		//レンダリングステップをシャドウマップ作成に変更。
		rc.SetRenderStep(EnRenderStep::enRenderStep_CreateDirectionalShadowMap);

		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			//レンダリングターゲットの設定？
			rc.SetRenderTarget(m_shadowMaps[i].GetRTVCpuDescriptorHandle(), m_shadowMaps[i].GetDSVCpuDescriptorHandle());
			const float clearColor = 1.0f;
			const float value[] = { clearColor,clearColor,clearColor,clearColor };
			rc.ClearRenderTargetView(m_shadowMaps[i].GetRTVCpuDescriptorHandle(), value);
			rc.ClearDepthStencilView(m_shadowMaps[i].GetDSVCpuDescriptorHandle(), clearColor);
			//レンダリングターゲットとして使用可能になるまで待つ。
			rc.WaitUntilToPossibleSetRenderTarget(m_shadowMaps[i]);
			//影をドロー
			for (auto& caster : m_shadowCasters) {
				caster->Draw(rc,m_LVPMatrix[i]);
			}
			rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMaps[i]);
			g_graphicsEngine->EndRenderShadowMap();
			g_graphicsEngine->BeginRenderShadowMap();
		}

		//シャドウキャスター登録をクリア。
		ClearShadowCaster();
	}
	void CShadowMap::WaitEndRenderToShadowMap(RenderContext& rc)
	{

		////影の描画終わり。
		//for (int i = 0; i < NUM_SHADOW_MAP; i++) {
		//	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMaps[i]);
		//}
	}
	void CShadowMap::Update()
	{
		//シャドウマップが無効。
		if (m_isEnable == false) {
			return;
		}
		//シーンのレンダリングに使用しているカメラを使ってライトの回転を求める。
		Vector3 cameraDir = g_camera3D->GetForward();
		if (fabs(cameraDir.x) < FLT_EPSILON && fabsf(cameraDir.z) < FLT_EPSILON) {
			//ほぼ真上を向いている。
			return;
		}
		//ライトビュー行列の回転成分を計算する。
		Vector3 lightViewForward = m_lightDirection;
		Vector3 lightViewUp;
		if (fabsf(lightViewForward.y) > 0.999f) {
			//ライトがほぼ真上を向いている
			lightViewUp.Cross(lightViewForward, Vector3::Right);
		}
		else {
			lightViewUp.Cross(lightViewForward, Vector3::Up);
		}
		lightViewUp.Normalize();	//正規化。
		Vector3 lightViewRight;		//ライトの右方向。
		lightViewRight.Cross(lightViewUp, lightViewForward);
		lightViewRight.Normalize();

		Matrix lightViewRot;	//ライトビューの向きを決める。
		//ライトビューの横を設定。
		lightViewRot.m[0][0] = lightViewRight.x;
		lightViewRot.m[0][1] = lightViewRight.y;
		lightViewRot.m[0][2] = lightViewRight.z;
		lightViewRot.m[0][3] = 0.0f;
		//ライトビューの上を設定。
		lightViewRot.m[1][0] = lightViewUp.x;
		lightViewRot.m[1][1] = lightViewUp.y;
		lightViewRot.m[1][2] = lightViewUp.z;
		lightViewRot.m[1][3] = 0.0f;
		//ライトビューの前を設定。
		lightViewRot.m[2][0] = lightViewForward.x;
		lightViewRot.m[2][1] = lightViewForward.y;
		lightViewRot.m[2][2] = lightViewForward.z;
		lightViewRot.m[2][3] = 0.0f;

		//ライトビューの高さを計算。するようにする。
		float lightHeight = g_camera3D->GetTarget().y + m_lightHeight;

		float nearPlaneZ = 0.0f;	//近平面。
		float farPlaneZ;			//遠平面。
		Vector3 cameraUp;			//カメラの上方向
		cameraUp.Cross(g_camera3D->GetRight(), g_camera3D->GetForward());
		//カスケードシャドウのための処理。
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			farPlaneZ = nearPlaneZ + m_shadowAreas[i];		//近平面+シャドウの範囲。
			Matrix mLightView = Matrix::Identity;			//ライトビュー。
			float halfViewAngle = g_camera3D->GetViewAngle() * 0.5f;		//画角の半分。
			//視推台の8頂点をライト空間に変換してAABBを求めて、正射影の幅、高さを求める。
			float w, h;						//高さ,幅
			float far_z = -1.0f;			//視推台の奥行。
			Vector3 v[8];					//視推台の頂点
			{
				float t = tan(halfViewAngle);
				Vector3 toUpperNear, toUpperFar;			//視推台の近平面、遠平面の上方向ベクトル。
				toUpperNear = cameraUp * t * nearPlaneZ;
				toUpperFar = cameraUp * t * farPlaneZ;
				t *= g_camera3D->GetAspect();		//アスペクト比をかけている？
				//近平面の中央座標を計算。
				Vector3 nearPlaneCenterPos = g_camera3D->GetPosition() + cameraDir * nearPlaneZ;
				//近平面の中央座標 + (カメラの右単位ベクトル * 縦横の比率(tan) * 近平面までの距離。) + 近平面の上方向のベクトル。
				v[0] = nearPlaneCenterPos + g_camera3D->GetRight() * t * nearPlaneZ + toUpperNear;		//近平面の右上。
				v[1] = v[0] - toUpperNear * 2.0f;														//近平面の右下。

				v[2] = nearPlaneCenterPos + g_camera3D->GetRight() * -t * nearPlaneZ + toUpperNear;		//近平面の左上。
				v[3] = v[2] - toUpperNear * 2.0f;														//近平面の左下。

				//遠平面の中央座標を計算。
				Vector3 farPlaneCenterPos = g_camera3D->GetPosition() + cameraDir * farPlaneZ;

				v[4] = farPlaneCenterPos + g_camera3D->GetRight() * t * farPlaneZ + toUpperFar;			//遠平面の右上。
				v[5] = v[4] - toUpperFar * 2.0f;														//遠平面の右下。

				v[6] = farPlaneCenterPos + g_camera3D->GetRight() * -t * farPlaneZ + toUpperFar;		//遠平面の左上。
				v[7] = v[6] - toUpperFar * 2.0f;														//遠平面の左下。

				//ライト行列を作成。
				Vector3 viewFrustumCenterPosition = (nearPlaneCenterPos + farPlaneCenterPos) * 0.5f;	//近平面と遠平面の中央座標の間。
				Vector3 lightPos = CalcLightPosition(lightHeight, viewFrustumCenterPosition);			//カメラの位置を計算。

				mLightView = lightViewRot;

				mLightView.m[3][0] = lightPos.x;		//ライトの座標を平行移動成分に代入。
				mLightView.m[3][1] = lightPos.y;		//
				mLightView.m[3][2] = lightPos.z;		//
				mLightView.m[3][3] = 1.0f;
				mLightView.Inverse(mLightView);			//ライトビュー完成。
				//視推台を構成する8頂点をライト空間に座標変換して、AABBを求める。
				Vector3 vMax = { -FLT_MAX,-FLT_MAX ,-FLT_MAX };
				Vector3 vMin = { FLT_MAX ,FLT_MAX ,FLT_MAX };
				for (auto& vInLight : v) {
					mLightView.Apply(vInLight);
					vMax.Max(vInLight);
					vMin.Min(vInLight);
				}
				w = vMax.x - vMin.x;
				h = vMax.y - vMin.y;
				far_z = vMax.z;
				//視推台の頂点を全て写すプロジェクション行列を作る。
				Matrix proj;
				proj.MakeOrthoProjectionMatrix(
					w,
					h,
					far_z / 100.0f,
					far_z
				);
				m_LVPMatrix[i] = mLightView * proj;									//ライトビュープロジェクション行列を作成。
				m_shadowCbEntity.mLVP[i] = m_LVPMatrix[i];							//定数バッファ用に記録。
				m_shadowCbEntity.shadowAreaDepthInViewSpace[i] = farPlaneZ * 0.8f;	//境界線辺りを上手く描画するために、エリアを少し狭める？
				m_shadowCbEntity.depthOffset[i] = m_depthOffset[i];					//深度値オフセット。
				nearPlaneZ = farPlaneZ;
			}
		}

	}
	Vector3 CShadowMap::CalcLightPosition(float lightHeight, Vector3 viewFrustomCenterPosition)
	{
		//分割された視推台を写すためのライト高さを計算する。
		float alpha = (lightHeight - viewFrustomCenterPosition.y) / m_lightDirection.y;
		Vector3 lightPos = viewFrustomCenterPosition + m_lightDirection * alpha;
		return lightPos;
	}
}
