#pragma once

#ifndef BASE_VERTEX_BUFFER_H
#define BASE_VERTEX_BUFFER_H

#include "Resources/ResourceBase.h"

class CommandBuffer;

class BaseVertexBuffer : public ResourceBase {
public:
	BaseVertexBuffer(const std::string& name);
	virtual ~BaseVertexBuffer(){}
	virtual void bind(CommandBuffer* commandBuffer = nullptr) = 0;
};

#endif