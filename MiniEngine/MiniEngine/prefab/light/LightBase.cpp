#include "stdafx.h"
#include "LightBase.h"
#include "graphics/light/LightManager.h"

namespace Engine {

	namespace prefab {
		bool CLightBase::Start()
		{
			GraphicsEngine()->GetLightManager()->AddLight(this);
			return StartSub();
		}
		void CLightBase::OnDestroy()
		{
			GraphicsEngine()->GetLightManager()->RemoveLight(this);
			OnDestroySub();
		}
	}

}