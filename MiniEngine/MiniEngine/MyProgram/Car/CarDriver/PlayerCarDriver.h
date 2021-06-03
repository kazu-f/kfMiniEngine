#pragma once
#include "ICarDriver.h"
class PlayerCarDriver : public ICarDriver
{
public:
	~PlayerCarDriver() {};
	void Update() override final;
};

