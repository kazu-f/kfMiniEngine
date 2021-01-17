#pragma once

#pragma comment( lib, "xinput.lib")

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <D3Dcompiler.h>
#include <Windows.h>
#include <wincodec.h>
#include <wincodecsdk.h>
#include <d3d12.h>
#include <map>
#include <functional>
#include <algorithm>
#include <DirectXMath.h>
#include <Xinput.h>
#include <array>
#include <queue>
#include <fstream>
#include <mbstring.h>

#include "d3dx12.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"

#include "policy/Noncopyable.h"
#include "system/debugLog.h"
#include "math/Math.h"
#include "math/Vector.h"
#include "math/Matrix.h"

#include "graphics/GPUBuffer/VertexBuffer.h"
#include "graphics/GPUBuffer/IndexBuffer.h"
#include "graphics/IShaderResource.h"
#include "graphics/IUnorderAccessResource.h"
#include "graphics/Texture.h"
#include "graphics/GPUBuffer/RWStructuredBuffer.h"
#include "graphics/Shader.h"
#include "graphics/PipelineState.h"
#include "graphics/GPUBuffer/ConstantBuffer.h"
#include "graphics/RootSignature.h"
#include "graphics/GraphicsEngine.h"
#include "graphics/DescriptorHeap.h"
#include "graphics/RenderTarget.h"

#include "gameObject/GameObjectManager.h"

#include "graphics/RenderContext_inline.h"
#include "graphics/DescriptorHeap_inline.h"

#include "graphics/2D/Sprite.h"
//#include "graphics/Model.h"
#include "HID/GamePad.h"
#include "util/NameKey.h"
#include "util/CGameTime.h"

#include "physics/PhysicsWorld.h"
#include "tkEngine.h"

#include "graphics/Skeleton.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/animation/AnimationPlayController.h"
#include "prefab/ModelRender.h"
#include "level/Level.h"
#include "graphics/light/LightManager.h"
#include "graphics/PreRender/ShadowMap.h"
#include "graphics/PreRender/GBufferRender.h"
#include "graphics/deferrd/DefferdShading.h"

const UINT FRAME_BUFFER_W = 1280;				//フレームバッファの幅。
const UINT FRAME_BUFFER_H = 720;				//フレームバッファの高さ。

static const int MAX_BONE = 512;				//ボーンの最大数。

using namespace Engine;
