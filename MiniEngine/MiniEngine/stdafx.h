#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "MiniEngine.h"

const int NUM_DIRECTIONAL_LIGHT = 4;	//ディレクションライトの数。


/// <summary>
/// ライト構造体。
/// </summary>
struct Light {
	SDirectionLight directionalLight[NUM_DIRECTIONAL_LIGHT];	//ディレクションライト。
	Vector3 eyePos;					//カメラの位置。
	float specPow;					//スペキュラの絞り。
	Vector3 ambinetLight;			//環境光。
};