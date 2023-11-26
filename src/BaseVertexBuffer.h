#pragma once

#include "Resources/ResourceBase.h"

class CommandBuffer;

class BaseVertexBuffer : public ResourceBase {
public:
	BaseVertexBuffer(const std::string& name);
	virtual ~BaseVertexBuffer(){}
	virtual void bind(CommandBuffer* commandBuffer = nullptr) = 0;
};