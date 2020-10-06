#include "stdafx.h"
#include "LightBase.h"
#include "graphics/light/LightManager.h"

namespace Engine {

	namespace prefab {
		bool CLightBase::Start()
		{
			g_graphicsEngine->GetLightManager()->AddLight(this);
			return StartSub();
		}
		void CLightBase::OnDestroy()
		{
			g_graphicsEngine->GetLightManager()->RemoveLight(this);
			OnDstroySub();
		}
	}

}