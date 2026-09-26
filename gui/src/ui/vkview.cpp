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

#include "vkview.h"
#include <QApplication>
#include <QBoxLayout>
#include <cmath>
#include <cstring>
#include <vector>

static const float farfarAway = 100000.0F;
static const int rulerLength = 200;

// Embedded SPIR-V for simple line/triangle rendering with vertex colors
// Vertex Shader SPIR-V
static const uint32_t vertShaderSpv[] = {
	0x07230203, 0x00010000, 0x0008000b, 0x00000027, 0x00000000, 0x00020011, 0x00000001, 0x0006000b,
	0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001,
	0x0009000f, 0x00000000, 0x00000004, 0x6e69616d, 0x00000000, 0x0000000d, 0x00000019, 0x00000024,
	0x00000025, 0x00030003, 0x00000002, 0x000001c2, 0x00040005, 0x00000004, 0x6e69616d, 0x00000000,
	0x00060005, 0x0000000b, 0x505f6c67, 0x65567265, 0x78657472, 0x00000000, 0x00060006, 0x0000000b,
	0x00000000, 0x505f6c67, 0x7469736f, 0x006e6f69, 0x00070006, 0x0000000b, 0x00000001, 0x505f6c67,
	0x746e696f, 0x657a6953, 0x00000000, 0x00070006, 0x0000000b, 0x00000002, 0x435f6c67, 0x4470696c,
	0x61747369, 0x0065636e, 0x00070006, 0x0000000b, 0x00000003, 0x435f6c67, 0x446c6c75, 0x61747369,
	0x0065636e, 0x00030005, 0x0000000d, 0x00000000, 0x00070005, 0x00000011, 0x66696e55, 0x426d726f,
	0x65666675, 0x6a624f72, 0x00746365, 0x00040006, 0x00000011, 0x00000000, 0x0070766d, 0x00030005,
	0x00000013, 0x006f6275, 0x00040005, 0x00000019, 0x6f506e69, 0x00000073, 0x00050005, 0x00000024,
	0x67617266, 0x6f6c6f43, 0x00000072, 0x00040005, 0x00000025, 0x6f436e69, 0x00726f6c, 0x00030047,
	0x0000000b, 0x00000002, 0x00050048, 0x0000000b, 0x00000000, 0x0000000b, 0x00000000, 0x00050048,
	0x0000000b, 0x00000001, 0x0000000b, 0x00000001, 0x00050048, 0x0000000b, 0x00000002, 0x0000000b,
	0x00000003, 0x00050048, 0x0000000b, 0x00000003, 0x0000000b, 0x00000004, 0x00030047, 0x00000011,
	0x00000002, 0x00040048, 0x00000011, 0x00000000, 0x00000005, 0x00050048, 0x00000011, 0x00000000,
	0x00000007, 0x00000010, 0x00050048, 0x00000011, 0x00000000, 0x00000023, 0x00000000, 0x00040047,
	0x00000013, 0x00000021, 0x00000000, 0x00040047, 0x00000013, 0x00000022, 0x00000000, 0x00040047,
	0x00000019, 0x0000001e, 0x00000000, 0x00040047, 0x00000024, 0x0000001e, 0x00000000, 0x00040047,
	0x00000025, 0x0000001e, 0x00000001, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002,
	0x00030016, 0x00000006, 0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000004, 0x00040015,
	0x00000008, 0x00000020, 0x00000000, 0x0004002b, 0x00000008, 0x00000009, 0x00000001, 0x0004001c,
	0x0000000a, 0x00000006, 0x00000009, 0x0006001e, 0x0000000b, 0x00000007, 0x00000006, 0x0000000a,
	0x0000000a, 0x00040020, 0x0000000c, 0x00000003, 0x0000000b, 0x0004003b, 0x0000000c, 0x0000000d,
	0x00000003, 0x00040015, 0x0000000e, 0x00000020, 0x00000001, 0x0004002b, 0x0000000e, 0x0000000f,
	0x00000000, 0x00040018, 0x00000010, 0x00000007, 0x00000004, 0x0003001e, 0x00000011, 0x00000010,
	0x00040020, 0x00000012, 0x00000002, 0x00000011, 0x0004003b, 0x00000012, 0x00000013, 0x00000002,
	0x00040020, 0x00000014, 0x00000002, 0x00000010, 0x00040017, 0x00000017, 0x00000006, 0x00000003,
	0x00040020, 0x00000018, 0x00000001, 0x00000017, 0x0004003b, 0x00000018, 0x00000019, 0x00000001,
	0x0004002b, 0x00000006, 0x0000001b, 0x3f800000, 0x00040020, 0x00000021, 0x00000003, 0x00000007,
	0x00040020, 0x00000023, 0x00000003, 0x00000017, 0x0004003b, 0x00000023, 0x00000024, 0x00000003,
	0x0004003b, 0x00000018, 0x00000025, 0x00000001, 0x00050036, 0x00000002, 0x00000004, 0x00000000,
	0x00000003, 0x000200f8, 0x00000005, 0x00050041, 0x00000014, 0x00000015, 0x00000013, 0x0000000f,
	0x0004003d, 0x00000010, 0x00000016, 0x00000015, 0x0004003d, 0x00000017, 0x0000001a, 0x00000019,
	0x00050051, 0x00000006, 0x0000001c, 0x0000001a, 0x00000000, 0x00050051, 0x00000006, 0x0000001d,
	0x0000001a, 0x00000001, 0x00050051, 0x00000006, 0x0000001e, 0x0000001a, 0x00000002, 0x00070050,
	0x00000007, 0x0000001f, 0x0000001c, 0x0000001d, 0x0000001e, 0x0000001b, 0x00050091, 0x00000007,
	0x00000020, 0x00000016, 0x0000001f, 0x00050041, 0x00000021, 0x00000022, 0x0000000d, 0x0000000f,
	0x0003003e, 0x00000022, 0x00000020, 0x0004003d, 0x00000017, 0x00000026, 0x00000025, 0x0003003e,
	0x00000024, 0x00000026, 0x000100fd, 0x00010038
};

// Fragment Shader SPIR-V
static const uint32_t fragShaderSpv[] = {
	0x07230203, 0x00010000, 0x0008000b, 0x00000013, 0x00000000, 0x00020011, 0x00000001, 0x0006000b,
	0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001,
	0x0007000f, 0x00000004, 0x00000004, 0x6e69616d, 0x00000000, 0x00000009, 0x0000000c, 0x00030010,
	0x00000004, 0x00000007, 0x00030003, 0x00000002, 0x000001c2, 0x00040005, 0x00000004, 0x6e69616d,
	0x00000000, 0x00050005, 0x00000009, 0x4374756f, 0x726f6c6f, 0x00000000, 0x00050005, 0x0000000c,
	0x67617266, 0x6f6c6f43, 0x00000072, 0x00040047, 0x00000009, 0x0000001e, 0x00000000, 0x00040047,
	0x0000000c, 0x0000001e, 0x00000000, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002,
	0x00030016, 0x00000006, 0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000004, 0x00040020,
	0x00000008, 0x00000003, 0x00000007, 0x0004003b, 0x00000008, 0x00000009, 0x00000003, 0x00040017,
	0x0000000a, 0x00000006, 0x00000003, 0x00040020, 0x0000000b, 0x00000001, 0x0000000a, 0x0004003b,
	0x0000000b, 0x0000000c, 0x00000001, 0x0004002b, 0x00000006, 0x0000000e, 0x3f800000, 0x00050036,
	0x00000002, 0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005, 0x0004003d, 0x0000000a,
	0x0000000d, 0x0000000c, 0x00050051, 0x00000006, 0x0000000f, 0x0000000d, 0x00000000, 0x00050051,
	0x00000006, 0x00000010, 0x0000000d, 0x00000001, 0x00050051, 0x00000006, 0x00000011, 0x0000000d,
	0x00000002, 0x00070050, 0x00000007, 0x00000012, 0x0000000f, 0x00000010, 0x00000011, 0x0000000e,
	0x0003003e, 0x00000009, 0x00000012, 0x000100fd, 0x00010038
};

VKViewRenderer::VKViewRenderer(VKViewWindow *window)
	: m_window(window)
{
}

VKViewRenderer::~VKViewRenderer()
{
}

void VKViewRenderer::initResources()
{
	m_device = m_window->device();
	m_devFuncs = m_window->vulkanInstance()->deviceFunctions(m_device);

	buildGeometry();
	createRenderPass();
	createPipeline();
	createBuffers();
	createDescriptorSet();
}

void VKViewRenderer::initSwapChainResources()
{
}

void VKViewRenderer::releaseSwapChainResources()
{
}

void VKViewRenderer::releaseResources()
{
	if(!m_devFuncs)
		return;

	if(m_vertexBuffer != VK_NULL_HANDLE) {
		m_devFuncs->vkDestroyBuffer(m_device, m_vertexBuffer, nullptr);
		m_devFuncs->vkFreeMemory(m_device, m_vertexBufferMemory, nullptr);
		m_vertexBuffer = VK_NULL_HANDLE;
	}

	if(m_uniformBuffer != VK_NULL_HANDLE) {
		m_devFuncs->vkDestroyBuffer(m_device, m_uniformBuffer, nullptr);
		m_devFuncs->vkFreeMemory(m_device, m_uniformBufferMemory, nullptr);
		m_uniformBuffer = VK_NULL_HANDLE;
	}

	if(m_descriptorPool != VK_NULL_HANDLE) {
		m_devFuncs->vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);
		m_descriptorPool = VK_NULL_HANDLE;
	}

	if(m_descriptorSetLayout != VK_NULL_HANDLE) {
		m_devFuncs->vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayout, nullptr);
		m_descriptorSetLayout = VK_NULL_HANDLE;
	}

	if(m_pipeline != VK_NULL_HANDLE) {
		m_devFuncs->vkDestroyPipeline(m_device, m_pipeline, nullptr);
		m_pipeline = VK_NULL_HANDLE;
	}

	if(m_pipelineLayout != VK_NULL_HANDLE) {
		m_devFuncs->vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
		m_pipelineLayout = VK_NULL_HANDLE;
	}

	if(m_renderPass != VK_NULL_HANDLE) {
		m_devFuncs->vkDestroyRenderPass(m_device, m_renderPass, nullptr);
		m_renderPass = VK_NULL_HANDLE;
	}
}

void VKViewRenderer::buildGeometry()
{
	m_vertices.clear();

	const float printX = static_cast<float>(m_window->printX);
	const float printY = static_cast<float>(m_window->printY);
	const float printWidth = static_cast<float>(m_window->printWidth);
	const float printLength = static_cast<float>(m_window->printLength);
	const float printHeight = static_cast<float>(m_window->printHeight);

	// Axes
	if(m_window->showAxes) {
		const float c = fmaxf(m_window->camera.getPositionY() / 2.0F, static_cast<float>(rulerLength));
		// X axis
		m_vertices.push_back({-c, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F});
		m_vertices.push_back({+c, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F});
		// Y axis
		m_vertices.push_back({0.0F, -c, 0.0F, 0.0F, 0.0F, 0.0F});
		m_vertices.push_back({0.0F, +c, 0.0F, 0.0F, 0.0F, 0.0F});
		// Z axis
		m_vertices.push_back({0.0F, 0.0F, -c, 0.0F, 0.0F, 0.0F});
		m_vertices.push_back({0.0F, 0.0F, +c, 0.0F, 0.0F, 0.0F});
	}

	// Cross / Origin indicator
	if(m_window->showCross) {
		const float n = 0.2F;
		// X line - red
		m_vertices.push_back({printX, printY, n, 1.0F, 0.0F, 0.0F});
		m_vertices.push_back({printX + 10.0F, printY, n, 1.0F, 0.0F, 0.0F});
		// Y line - green
		m_vertices.push_back({printX, printY, n, 0.0F, 1.0F, 0.0F});
		m_vertices.push_back({printX, printY + 10.0F, n, 0.0F, 1.0F, 0.0F});
		// Z line - blue
		m_vertices.push_back({printX - n, printY - n, n, 0.0F, 0.0F, 1.0F});
		m_vertices.push_back({printX - n, printY - n, 10.0F, 0.0F, 0.0F, 1.0F});
	}

	// Base Grid & Outline
	if(m_window->showBase) {
		const float z = 0.0F;
		// Base Outline Box
		m_vertices.push_back({printX, printY, z, 0.8F, 0.8F, 0.8F});
		m_vertices.push_back({printX + printWidth, printY, z, 0.8F, 0.8F, 0.8F});

		m_vertices.push_back({printX + printWidth, printY, z, 0.8F, 0.8F, 0.8F});
		m_vertices.push_back({printX + printWidth, printY + printLength, z, 0.8F, 0.8F, 0.8F});

		m_vertices.push_back({printX + printWidth, printY + printLength, z, 0.8F, 0.8F, 0.8F});
		m_vertices.push_back({printX, printY + printLength, z, 0.8F, 0.8F, 0.8F});

		m_vertices.push_back({printX, printY + printLength, z, 0.8F, 0.8F, 0.8F});
		m_vertices.push_back({printX, printY, z, 0.8F, 0.8F, 0.8F});
	}

	// Print Area Box
	if(m_window->showPrintArea) {
		m_vertices.push_back({printX, printY, 0.0F, 0.8F, 0.8F, 0.8F});
		m_vertices.push_back({printX, printY, printHeight, 0.8F, 0.8F, 0.8F});

		m_vertices.push_back({printX + printWidth, printY + printLength, 0.0F, 0.8F, 0.8F, 0.8F});
		m_vertices.push_back({printX + printWidth, printY + printLength, printHeight, 0.8F, 0.8F, 0.8F});

		m_vertices.push_back({printX, printY + printLength, 0.0F, 0.8F, 0.8F, 0.8F});
		m_vertices.push_back({printX, printY + printLength, printHeight, 0.8F, 0.8F, 0.8F});

		m_vertices.push_back({printX + printWidth, printY, 0.0F, 0.8F, 0.8F, 0.8F});
		m_vertices.push_back({printX + printWidth, printY, printHeight, 0.8F, 0.8F, 0.8F});
	}

	m_vertexCount = static_cast<uint32_t>(m_vertices.size());
}

void VKViewRenderer::createRenderPass()
{
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = m_window->colorFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	m_devFuncs->vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass);
}

void VKViewRenderer::createPipeline()
{
	VkShaderModuleCreateInfo vertInfo = {};
	vertInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vertInfo.codeSize = sizeof(vertShaderSpv);
	vertInfo.pCode = vertShaderSpv;

	VkShaderModule vertShaderModule;
	m_devFuncs->vkCreateShaderModule(m_device, &vertInfo, nullptr, &vertShaderModule);

	VkShaderModuleCreateInfo fragInfo = {};
	fragInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	fragInfo.codeSize = sizeof(fragShaderSpv);
	fragInfo.pCode = fragShaderSpv;

	VkShaderModule fragShaderModule;
	m_devFuncs->vkCreateShaderModule(m_device, &fragInfo, nullptr, &fragShaderModule);

	VkPipelineShaderStageCreateInfo vertStageInfo = {};
	vertStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertStageInfo.module = vertShaderModule;
	vertStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragStageInfo = {};
	fragStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragStageInfo.module = fragShaderModule;
	fragStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = {vertStageInfo, fragStageInfo};

	VkVertexInputBindingDescription bindingDescription = {};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	VkVertexInputAttributeDescription attributeDescriptions[2] = {};
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, x);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, r);

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = 2;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_NONE;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;

	VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
	VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;

	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	m_devFuncs->vkCreateDescriptorSetLayout(m_device, &layoutInfo, nullptr, &m_descriptorSetLayout);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &m_descriptorSetLayout;

	m_devFuncs->vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout);

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = m_pipelineLayout;
	pipelineInfo.renderPass = m_renderPass;
	pipelineInfo.subpass = 0;

	m_devFuncs->vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline);

	m_devFuncs->vkDestroyShaderModule(m_device, fragShaderModule, nullptr);
	m_devFuncs->vkDestroyShaderModule(m_device, vertShaderModule, nullptr);
}

void VKViewRenderer::createBuffers()
{
	VkDeviceSize bufferSize = sizeof(Vertex) * (m_vertices.empty() ? 1 : m_vertices.size());

	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = bufferSize;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	m_devFuncs->vkCreateBuffer(m_device, &bufferInfo, nullptr, &m_vertexBuffer);

	VkMemoryRequirements memRequirements;
	m_devFuncs->vkGetBufferMemoryRequirements(m_device, m_vertexBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = m_window->hostVisibleMemoryIndex();

	m_devFuncs->vkAllocateMemory(m_device, &allocInfo, nullptr, &m_vertexBufferMemory);
	m_devFuncs->vkBindBufferMemory(m_device, m_vertexBuffer, m_vertexBufferMemory, 0);

	if(!m_vertices.empty()) {
		void *data;
		m_devFuncs->vkMapMemory(m_device, m_vertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, m_vertices.data(), (size_t)bufferSize);
		m_devFuncs->vkUnmapMemory(m_device, m_vertexBufferMemory);
	}

	// Uniform buffer
	VkBufferCreateInfo uboInfo = {};
	uboInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	uboInfo.size = sizeof(UniformBufferObject);
	uboInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	uboInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	m_devFuncs->vkCreateBuffer(m_device, &uboInfo, nullptr, &m_uniformBuffer);

	m_devFuncs->vkGetBufferMemoryRequirements(m_device, m_uniformBuffer, &memRequirements);

	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = m_window->hostVisibleMemoryIndex();

	m_devFuncs->vkAllocateMemory(m_device, &allocInfo, nullptr, &m_uniformBufferMemory);
	m_devFuncs->vkBindBufferMemory(m_device, m_uniformBuffer, m_uniformBufferMemory, 0);
}

void VKViewRenderer::createDescriptorSet()
{
	VkDescriptorPoolSize poolSize = {};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = 1;

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = 1;

	m_devFuncs->vkCreateDescriptorPool(m_device, &poolInfo, nullptr, &m_descriptorPool);

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &m_descriptorSetLayout;

	m_devFuncs->vkAllocateDescriptorSets(m_device, &allocInfo, &m_descriptorSet);

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = m_uniformBuffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(UniformBufferObject);

	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = m_descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = &bufferInfo;

	m_devFuncs->vkUpdateDescriptorSets(m_device, 1, &descriptorWrite, 0, nullptr);
}

void VKViewRenderer::updateUniformBuffer()
{
	const QSize sz = m_window->swapChainImageSize();
	const float w = static_cast<float>(sz.width());
	const float h = static_cast<float>(sz.height() > 0 ? sz.height() : 1);

	QMatrix4x4 proj;
	proj.perspective(45.0F, w / h, +10.0F, +farfarAway);

	QMatrix4x4 modelview;
	m_window->camera.applyTo(modelview);

	QMatrix4x4 mvp = m_window->clipCorrectionMatrix() * proj * modelview;

	UniformBufferObject ubo;
	memcpy(ubo.mvp, mvp.constData(), 16 * sizeof(float));

	void *data;
	m_devFuncs->vkMapMemory(m_device, m_uniformBufferMemory, 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	m_devFuncs->vkUnmapMemory(m_device, m_uniformBufferMemory);
}

void VKViewRenderer::startNextFrame()
{
	VkClearColorValue clearColor = {{0.46F, 0.46F, 0.46F, 1.0F}};
	VkClearValue clearValues[1];
	clearValues[0].color = clearColor;

	const QSize sz = m_window->swapChainImageSize();

	VkRenderPassBeginInfo passBeginInfo = {};
	passBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	passBeginInfo.renderPass = m_renderPass;
	passBeginInfo.framebuffer = m_window->currentFramebuffer();
	passBeginInfo.renderArea.extent = {static_cast<uint32_t>(sz.width()), static_cast<uint32_t>(sz.height())};
	passBeginInfo.clearValueCount = 1;
	passBeginInfo.pClearValues = clearValues;

	VkCommandBuffer cb = m_window->currentCommandBuffer();
	m_devFuncs->vkCmdBeginRenderPass(cb, &passBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	m_devFuncs->vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

	VkViewport viewport = {};
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = static_cast<float>(sz.width());
	viewport.height = static_cast<float>(sz.height());
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	m_devFuncs->vkCmdSetViewport(cb, 0, 1, &viewport);

	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = {static_cast<uint32_t>(sz.width()), static_cast<uint32_t>(sz.height())};
	m_devFuncs->vkCmdSetScissor(cb, 0, 1, &scissor);

	updateUniformBuffer();

	m_devFuncs->vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descriptorSet, 0, nullptr);

	if(m_vertexCount > 0) {
		VkDeviceSize offsets[] = {0};
		m_devFuncs->vkCmdBindVertexBuffers(cb, 0, 1, &m_vertexBuffer, offsets);
		m_devFuncs->vkCmdDraw(cb, m_vertexCount, 1, 0, 0);
	}

	m_devFuncs->vkCmdEndRenderPass(cb);

	m_window->frameReady();
	m_window->requestUpdate();
}

// VKViewWindow implementation
VKViewWindow::VKViewWindow()
	: camera(35.0F, 0.0F, 35.0F, 0.0F, 500.0F, 0.0F),
	  showAxes(true),
	  showCross(true),
	  showBase(true),
	  showPrintArea(true),
	  showRulers(true),
	  showEdges(false),
	  skeleton(false),
	  printX(0),
	  printY(0),
	  printWidth(0),
	  printLength(0),
	  printHeight(0),
	  appearance(BedAppearance::MK42),
	  render(nullptr)
{
}

QVulkanWindowRenderer *VKViewWindow::createRenderer()
{
	return new VKViewRenderer(this);
}

// VKView implementation
VKView::VKView(QWidget *parent)
	: QWidget(parent),
	  m_vkWindow(new VKViewWindow()),
	  mouseDrag(false)
{
	m_vkInstance.setLayers({});
	if(m_vkInstance.create()) {
		m_vkWindow->setVulkanInstance(&m_vkInstance);
	}

	m_vkContainer = QWidget::createWindowContainer(m_vkWindow, this);
	auto *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_vkContainer);
	setLayout(layout);

	setCursor(Qt::CrossCursor);
}

VKView::~VKView()
{
	delete m_vkWindow->render;
}

const Camera &VKView::getCamera() const
{
	return m_vkWindow->camera;
}

void VKView::setCamera(const Camera &c)
{
	m_vkWindow->camera = c;
	m_vkWindow->requestUpdate();
}

void VKView::setPrintOrigin(int x, int y)
{
	m_vkWindow->printX = x;
	m_vkWindow->printY = y;
	m_vkWindow->requestUpdate();
}

void VKView::setPrintVolume(int w, int l, int h)
{
	m_vkWindow->printWidth = w;
	m_vkWindow->printLength = l;
	m_vkWindow->printHeight = h;
	m_vkWindow->requestUpdate();
}

void VKView::setSkeleton(bool skel)
{
	m_vkWindow->skeleton = skel;
	m_vkWindow->requestUpdate();
}

void VKView::setShowEdges(bool edges)
{
	m_vkWindow->showEdges = edges;
	m_vkWindow->requestUpdate();
}

void VKView::changeViewDirection(ViewDirections d)
{
	m_vkWindow->camera.changeViewDirection(d);
	m_vkWindow->requestUpdate();
}

void VKView::setShowAxes(bool axes)
{
	m_vkWindow->showAxes = axes;
	m_vkWindow->requestUpdate();
}

void VKView::setShowRulers(bool rulers)
{
	m_vkWindow->showRulers = rulers;
	m_vkWindow->requestUpdate();
}

void VKView::setShowBase(bool base)
{
	m_vkWindow->showBase = base;
	m_vkWindow->requestUpdate();
}

void VKView::setShowPrintArea(bool print)
{
	m_vkWindow->showPrintArea = print;
	m_vkWindow->requestUpdate();
}

void VKView::setRenderer(Renderer *r)
{
	delete m_vkWindow->render;
	m_vkWindow->render = r;
	m_vkWindow->requestUpdate();
}

void VKView::preferencesUpdated()
{
	if(m_vkWindow->render)
		m_vkWindow->render->preferencesUpdated();
	m_vkWindow->requestUpdate();
}

void VKView::setCompiling(bool value)
{
	if(m_vkWindow->render)
		m_vkWindow->render->setCompiling(value);
	m_vkWindow->requestUpdate();
}

void VKView::setBedAppearance(BedAppearance v)
{
	m_vkWindow->appearance = v;
	m_vkWindow->requestUpdate();
}

void VKView::mousePressEvent(QMouseEvent *event)
{
	mouseDrag = true;
	setCursor(Qt::SizeAllCursor);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
	last = event->globalPos();
#else
	last = event->globalPosition();
#endif
}

void VKView::mouseMoveEvent(QMouseEvent *event)
{
	if(!mouseDrag)
		return;

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
	const QPoint current = event->globalPos();
#else
	const QPointF current = event->globalPosition();
#endif
	const auto dx = static_cast<float>(current.x() - last.x());
	const auto dy = static_cast<float>(current.y() - last.y());
	const bool shift = QApplication::keyboardModifiers() & Qt::ShiftModifier;
	if(event->buttons() & Qt::LeftButton) {
		m_vkWindow->camera.pitch(dy);
		if(shift) {
			m_vkWindow->camera.roll(dx);
		} else {
			m_vkWindow->camera.yaw(dx);
		}
	} else {
		if(shift) {
			m_vkWindow->camera.zoom(-dy);
		} else {
			m_vkWindow->camera.pan(dx, -dy);
		}
	}
	m_vkWindow->requestUpdate();

	last = current;
}

void VKView::mouseReleaseEvent(QMouseEvent *)
{
	mouseDrag = false;
	setCursor(Qt::CrossCursor);
}

void VKView::wheelEvent(QWheelEvent *event)
{
#if QT_VERSION < QT_VERSION_CHECK(5,5,0)
	const int delta = event->delta();
#else
	const int delta = event->angleDelta().y();
#endif
	m_vkWindow->camera.zoom(static_cast<float>(delta) / 12.0F);
	m_vkWindow->requestUpdate();
}
