#pragma once
#include "VulkanBase.h"
class VulkanGame : public VulkanBase
{
public:
	VulkanGame();
	~VulkanGame();
	void Init();
	void Update();
	void Render();
};

