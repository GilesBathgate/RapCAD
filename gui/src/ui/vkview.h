/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VKVIEW_H
#define VKVIEW_H

#include "bedappearance.h"
#include "camera.h"
#include "renderer.h"
#include "viewdirections.h"
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QVulkanDeviceFunctions>
#include <QVulkanFunctions>
#include <QVulkanInstance>
#include <QVulkanWindow>
#include <QVulkanWindowRenderer>
#include <QWheelEvent>
#include <QWidget>

class VKViewWindow;

class VKViewRenderer : public QVulkanWindowRenderer
{
public:
	explicit VKViewRenderer(VKViewWindow *window);
	~VKViewRenderer() override;

	void initResources() override;
	void initSwapChainResources() override;
	void releaseSwapChainResources() override;
	void releaseResources() override;
	void startNextFrame() override;

private:
	struct Vertex {
		float x, y, z;
		float r, g, b;
	};

	struct UniformBufferObject {
		float mvp[16];
	};

	void buildGeometry();
	void updateVertexBuffer();
	void createRenderPass();
	void createPipeline();
	void createBuffers();
	void createDescriptorSet();
	void updateUniformBuffer();

	VKViewWindow *m_window;
	VkDevice m_device;
	QVulkanDeviceFunctions *m_devFuncs = nullptr;

	VkRenderPass m_renderPass = VK_NULL_HANDLE;
	VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
	VkPipeline m_pipeline = VK_NULL_HANDLE;
	VkPipeline m_bgPipeline = VK_NULL_HANDLE;

	VkBuffer m_bgVertexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory m_bgVertexBufferMemory = VK_NULL_HANDLE;

	VkBuffer m_vertexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory m_vertexBufferMemory = VK_NULL_HANDLE;
	VkDeviceSize m_vertexBufferSize = 0;
	uint32_t m_vertexCount = 0;

	VkBuffer m_uniformBuffer = VK_NULL_HANDLE;
	VkDeviceMemory m_uniformBufferMemory = VK_NULL_HANDLE;
	VkBuffer m_bgUniformBuffer = VK_NULL_HANDLE;
	VkDeviceMemory m_bgUniformBufferMemory = VK_NULL_HANDLE;

	VkDescriptorSet m_bgDescriptorSet = VK_NULL_HANDLE;

	VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;
	VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;
	VkDescriptorSet m_descriptorSet = VK_NULL_HANDLE;

	std::vector<Vertex> m_vertices;
};

class VKViewWindow : public QVulkanWindow
{
	Q_OBJECT
public:
	explicit VKViewWindow();
	~VKViewWindow() override = default;

	QVulkanWindowRenderer *createRenderer() override;

	Camera camera;
	bool showAxes;
	bool showCross;
	bool showBase;
	bool showPrintArea;
	bool showRulers;
	bool showEdges;
	bool skeleton;
	int printX;
	int printY;
	int printWidth;
	int printLength;
	int printHeight;
	BedAppearance appearance;
	Renderer *render;

	QMatrix4x4 projection;
	QMatrix4x4 modelview;
};

class VKView : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(VKView)
public:
	explicit VKView(QWidget *parent = nullptr);
	~VKView() override;

	void setRenderer(Renderer *r);
	void setCompiling(bool value);
	void setBedAppearance(BedAppearance v);
	void preferencesUpdated();
	void setPrintOrigin(int x, int y);
	void setPrintVolume(int w, int l, int h);
	const Camera &getCamera() const;
	void setCamera(const Camera &c);

public slots:
	void setShowAxes(bool axes);
	void setShowRulers(bool rulers);
	void setShowBase(bool base);
	void setShowPrintArea(bool print);
	void setSkeleton(bool skel);
	void setShowEdges(bool edges);
	void changeViewDirection(ViewDirections d);

private:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	bool eventFilter(QObject *watched, QEvent *event) override;

	QVulkanInstance m_vkInstance;
	VKViewWindow *m_vkWindow;
	QWidget *m_vkContainer;

	bool mouseDrag;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
	QPoint last;
#else
	QPointF last;
#endif
};

#endif // VKVIEW_H
