#pragma once

#include "graphics/GraphicsConfig.h"

namespace Engine {

	struct SInitParam {
		int frameBuffer_W = 0;
		int frameBuffer_H = 0;
		unsigned char gameObjectPrioMax = 20;
		SGraphicsConfig graphicsConf;
	};

}

