/*
 * Copyright (c) 2014 Roman Kuznetsov 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#pragma once
#include "structs.h"
#include "vector.h"
#include "quaternion.h"
#include "matrix.h"
#include <vector>
#include <memory>

namespace framework
{

class UniformBuffer;

enum LightType
{
	OmniLight = 0,
	SpotLight,
	DirectLight
};

struct LightSource
{
	LightType type;
	vector3 diffuseColor;
	vector3 ambientColor;
	vector3 specularColor;
	float falloff;
	vector3 position;
	quaternion orientation;
	float angle;

	LightSource() : type(LightType::OmniLight), diffuseColor(1.0f, 1.0f, 1.0f), ambientColor(0.3f, 0.3f, 0.3f),
					specularColor(1.0f, 1.0f, 1.0f), falloff(1000.0f), position(0.0f, 0.0f, 0.0f), angle(60.0f) {} 
};

#pragma pack (push, 1)
struct LightRawData
{
	float positionOrDirection[3];
	unsigned int lightType;
	float diffuseColor[3];
	float falloff;
	float ambientColor[3];
	float angle;
	float specularColor[3];
	unsigned int : 32;
};
#pragma pack (pop)

class Line3D;

class LightManager
{
public:
	LightManager(){}
	~LightManager(){}

	void init();

	size_t getLightSourcesCount() const { return m_lightSources.size(); }
	void addLightSource(const LightSource& lightSource);
	const LightSource& getLightSource(size_t index) { return m_lightSources[index].lightSource; }
	LightRawData getRawLightData(size_t index);

	void renderDebugVisualization(const matrix44& viewProjection);

private:
	struct LightData
	{
		LightSource lightSource;
		std::shared_ptr<Line3D> lineDebugVis;
		LightData() : lineDebugVis(0){}
	};
	std::vector<LightData> m_lightSources;

	std::shared_ptr<UniformBuffer> m_arrowDataBuffer;

	void createDirectLightDebugVisualization(const std::shared_ptr<Line3D>& line);
	void createOmniLightDebugVisualization(const std::shared_ptr<Line3D>& line);
	void createSpotLightDebugVisualization(const std::shared_ptr<Line3D>& line);
};

}