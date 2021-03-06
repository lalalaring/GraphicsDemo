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

#include "stdafx.h"
#include "standardGpuPrograms.h"

namespace framework
{

const std::string STANDARD_SHADERS_PATH = "data/shaders/gl/win32/standard/";

std::shared_ptr<GpuProgram> StandardGpuPrograms::m_lineRenderer;
std::shared_ptr<GpuProgram> StandardGpuPrograms::m_arrowRenderer;
std::shared_ptr<GpuProgram> StandardGpuPrograms::m_copyDepthBuffer;
std::shared_ptr<GpuProgram> StandardGpuPrograms::m_copyDepthBufferMSAA;
std::shared_ptr<GpuProgram> StandardGpuPrograms::m_skyboxRenderer;

bool StandardGpuPrograms::init()
{
	bool result = true;

	m_lineRenderer.reset(new GpuProgram());
	m_lineRenderer->addShader(STANDARD_SHADERS_PATH + "line.vsh.glsl");
	m_lineRenderer->addShader(STANDARD_SHADERS_PATH + "line.fsh.glsl");
	result &= m_lineRenderer->init();
	if (!result) return false;
	m_lineRenderer->bindUniform<StandardUniforms>(STD_UF::MODELVIEWPROJECTION_MATRIX, "modelViewProjectionMatrix");
	m_lineRenderer->bindUniform<StandardUniforms>(STD_UF::COLOR, "color");

	m_arrowRenderer.reset(new GpuProgram());
	m_arrowRenderer->addShader(STANDARD_SHADERS_PATH + "arrow.vsh.glsl");
	m_arrowRenderer->addShader(STANDARD_SHADERS_PATH + "arrow.gsh.glsl");
	m_arrowRenderer->addShader(STANDARD_SHADERS_PATH + "arrow.fsh.glsl");
	result &= m_arrowRenderer->init();
	if (!result) return false;
	m_arrowRenderer->bindUniform<StandardUniforms>(STD_UF::MODELVIEWPROJECTION_MATRIX, "viewProjectionMatrix");
	m_arrowRenderer->bindUniform<StandardUniforms>(STD_UF::POSITION, "position");
	m_arrowRenderer->bindUniform<StandardUniforms>(STD_UF::ORIENTATION, "orientation");
	m_arrowRenderer->bindUniform<StandardUniforms>(STD_UF::COLOR, "color");

	m_copyDepthBuffer.reset(new GpuProgram());
	m_copyDepthBuffer->addShader(STANDARD_SHADERS_PATH + "copydepth.vsh.glsl");
	m_copyDepthBuffer->addShader(STANDARD_SHADERS_PATH + "copydepth.gsh.glsl");
	m_copyDepthBuffer->addShader(STANDARD_SHADERS_PATH + "copydepth.fsh.glsl");
	result &= m_copyDepthBuffer->init();
	if (!result) return false;
	m_copyDepthBuffer->bindUniform<StandardUniforms>(STD_UF::DEPTH_MAP, "depthMap");

	m_copyDepthBufferMSAA.reset(new GpuProgram());
	m_copyDepthBufferMSAA->addShader(STANDARD_SHADERS_PATH + "copydepth.vsh.glsl");
	m_copyDepthBufferMSAA->addShader(STANDARD_SHADERS_PATH + "copydepth.gsh.glsl");
	m_copyDepthBufferMSAA->addShader(STANDARD_SHADERS_PATH + "copydepth_msaa.fsh.glsl");
	result &= m_copyDepthBufferMSAA->init();
	if (!result) return false;
	m_copyDepthBufferMSAA->bindUniform<StandardUniforms>(STD_UF::DEPTH_MAP, "depthMap");
	m_copyDepthBufferMSAA->bindUniform<StandardUniforms>(STD_UF::SAMPLES_COUNT, "samplesCount");

	m_skyboxRenderer.reset(new framework::GpuProgram());
	m_skyboxRenderer->addShader(STANDARD_SHADERS_PATH + "skybox.vsh.glsl");
	m_skyboxRenderer->addShader(STANDARD_SHADERS_PATH + "skybox.gsh.glsl");
	m_skyboxRenderer->addShader(STANDARD_SHADERS_PATH + "skybox.fsh.glsl");
	result &= m_skyboxRenderer->init();
	m_skyboxRenderer->bindUniform<StandardUniforms>(STD_UF::MODELVIEWPROJECTION_MATRIX, "modelViewProjectionMatrix");
	m_skyboxRenderer->bindUniform<StandardUniforms>(STD_UF::SKYBOX_MAP, "skyboxMap");

	return result;
}

std::shared_ptr<GpuProgram> StandardGpuPrograms::getLineRenderer()
{
	return m_lineRenderer;
}

std::shared_ptr<GpuProgram> StandardGpuPrograms::getArrowRenderer()
{
	return m_arrowRenderer;
}

std::shared_ptr<GpuProgram> StandardGpuPrograms::getDepthBufferCopying(bool msaa)
{
	return msaa ? m_copyDepthBufferMSAA : m_copyDepthBuffer;
}

std::shared_ptr<GpuProgram> StandardGpuPrograms::getSkyboxRenderer()
{
	return m_skyboxRenderer;
}

}