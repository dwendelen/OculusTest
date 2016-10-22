#include "VulkanRenderPass.h"

#include "VulkanContext.h"
#include <vector>
#include <fstream>

using namespace std;
namespace vulkan
{
    VulkanRenderPass::VulkanRenderPass(VulkanContext& vulkanContext, int width, int height):
        vulkanContext(vulkanContext),
        renderPass(VK_NULL_HANDLE),
        vertexShader(VK_NULL_HANDLE),
        fragmentShader(VK_NULL_HANDLE),
        setLayout(VK_NULL_HANDLE),
        layout(VK_NULL_HANDLE),
        pipeline(VK_NULL_HANDLE),
        width(width),
        height(height)
    {
        //ctor
    }

    vector<char> loadFile(string filename)
    {
        ifstream t(filename, ios::ate | ios::binary);
        if(!t.is_open()) {
            throw new runtime_error("Could not read " + filename);
        }
        size_t size = t.tellg();
        vector<char> buffer(size);
        t.seekg(0);
        t.read(buffer.data(), size);
        t.close();
        return buffer;
    }

    void createShader(string file, VkShaderModule *shader, VkDevice device)
    {
        vector<char> vertexCode = loadFile(file);
        VkShaderModuleCreateInfo shaderInfo = {};
        shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderInfo.pNext = nullptr;
        shaderInfo.flags = 0;
        shaderInfo.pCode = (uint32_t*) vertexCode.data();
        shaderInfo.codeSize = vertexCode.size();

        VkResult r = vkCreateShaderModule(device, &shaderInfo, nullptr, shader);
        if(r != VK_SUCCESS) {
            throw new runtime_error("Could not create vertex shader module");
        }
    }

    void VulkanRenderPass::init()
    {
        VkResult r;
        //SHADERS
        createShader("shaders/vert.spv", &vertexShader, vulkanContext.getDevice());
        createShader("shaders/frag.spv", &fragmentShader, vulkanContext.getDevice());

        VkPipelineShaderStageCreateInfo vertexShaderStage = {};
        vertexShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertexShaderStage.pNext = nullptr;
        vertexShaderStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertexShaderStage.module = vertexShader;
        vertexShaderStage.pName = "main";
        vertexShaderStage.pSpecializationInfo = nullptr;

        VkPipelineShaderStageCreateInfo fragmentShaderStage = {};
        fragmentShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragmentShaderStage.pNext = nullptr;
        fragmentShaderStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragmentShaderStage.module = fragmentShader;
        fragmentShaderStage.pName = "main";
        fragmentShaderStage.pSpecializationInfo = nullptr;

        vector<VkPipelineShaderStageCreateInfo> stages = {vertexShaderStage, fragmentShaderStage};

        //INPUT BINDING
        VkVertexInputBindingDescription vertexBinding = {};
        vertexBinding.binding = 0;
        vertexBinding.stride = 6 * sizeof(float);
        vertexBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        VkVertexInputAttributeDescription vertexAttDescr = {};
        vertexAttDescr.binding = 0;
        vertexAttDescr.format = VK_FORMAT_R32G32B32_SFLOAT;
        vertexAttDescr.location = 0;
        vertexAttDescr.offset = 0;

        VkVertexInputAttributeDescription normalAttDescr = {};
        normalAttDescr.binding = 0;
        normalAttDescr.format = VK_FORMAT_R32G32B32_SFLOAT;
        normalAttDescr.location = 1;
        normalAttDescr.offset = 3 * sizeof(float);

        vector<VkVertexInputAttributeDescription> attributes = {vertexAttDescr, normalAttDescr};

        VkPipelineVertexInputStateCreateInfo inputInfo = {};
        inputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        inputInfo.pNext = nullptr;
        inputInfo.flags = 0;
        inputInfo.vertexAttributeDescriptionCount = attributes.size();
        inputInfo.pVertexAttributeDescriptions = attributes.data();
        inputInfo.vertexBindingDescriptionCount = 1;
        inputInfo.pVertexBindingDescriptions = &vertexBinding;

        //INPUT ASSEMBLY
        VkPipelineInputAssemblyStateCreateInfo assemblyInfo = {};
        assemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        assemblyInfo.pNext = nullptr;
        assemblyInfo.flags = 0;
        assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        assemblyInfo.primitiveRestartEnable = VK_FALSE;

        //VIEWPORT
        VkViewport viewport = {};
        viewport.x = 0;
        viewport.y = 0;
        viewport.height = height;
        viewport.width = width;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset.x = 0;
        scissor.offset.y = 0;
        scissor.extent.height = height;
        scissor.extent.width = width;

        VkPipelineViewportStateCreateInfo viewportInfo = {};
        viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo.pNext = nullptr;
        viewportInfo.flags = 0;
        viewportInfo.viewportCount = 1;
        viewportInfo.pViewports = &viewport;
        viewportInfo.scissorCount = 1;
        viewportInfo.pScissors = &scissor;

        //RASTERISER
        VkPipelineRasterizationStateCreateInfo rasteriserInfo = {};
        rasteriserInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasteriserInfo.pNext = nullptr;
        rasteriserInfo.flags = 0;
        rasteriserInfo.depthClampEnable = VK_FALSE;
        rasteriserInfo.rasterizerDiscardEnable = VK_FALSE;
        rasteriserInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasteriserInfo.cullMode = VK_CULL_MODE_NONE;// VK_CULL_MODE_BACK_BIT;
        rasteriserInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasteriserInfo.depthBiasEnable = VK_FALSE;
        rasteriserInfo.depthBiasConstantFactor = 0.0f;
        rasteriserInfo.depthBiasClamp = 0.0f;
        rasteriserInfo.depthBiasSlopeFactor = 0.0f;
		rasteriserInfo.lineWidth = 1.0f;

        //MULTISAMPLE
        VkPipelineMultisampleStateCreateInfo multisamplingInfo = {};
        multisamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisamplingInfo.sampleShadingEnable = VK_FALSE;
        multisamplingInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisamplingInfo.minSampleShading = 1.0f;
        multisamplingInfo.pSampleMask = nullptr;
        multisamplingInfo.alphaToCoverageEnable = VK_FALSE;
        multisamplingInfo.alphaToOneEnable = VK_FALSE;

        //DEPTH
        VkPipelineDepthStencilStateCreateInfo depthInfo = {};
        depthInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthInfo.pNext = nullptr;
        depthInfo.flags = 0;
		depthInfo.depthTestEnable = VK_TRUE;
		depthInfo.depthWriteEnable = VK_TRUE;
        depthInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        depthInfo.depthBoundsTestEnable = VK_FALSE;
        depthInfo.minDepthBounds = 0.0f;
        depthInfo.maxDepthBounds = 0.0f;
        depthInfo.stencilTestEnable = VK_FALSE;
        depthInfo.front = {};
        depthInfo.back = {};

        //COLOR
        VkPipelineColorBlendAttachmentState attachmentState = {};
		attachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        attachmentState.blendEnable = /*VK_TRUE*/VK_FALSE;
        attachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        attachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        attachmentState.colorBlendOp = VK_BLEND_OP_ADD;
        attachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        attachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        attachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorInfo = {};
        colorInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorInfo.pNext = nullptr;
        colorInfo.flags = 0;
        colorInfo.logicOpEnable = VK_FALSE;
        colorInfo.logicOp = VK_LOGIC_OP_COPY;
        colorInfo.attachmentCount = 1;
        colorInfo.pAttachments = &attachmentState;
        colorInfo.blendConstants[0] = 0.0f;
        colorInfo.blendConstants[1] = 0.0f;
        colorInfo.blendConstants[2] = 0.0f;
        colorInfo.blendConstants[3] = 0.0f;

        //LAYOUT
        VkDescriptorSetLayoutBinding vertexSetBinding = {};
        vertexSetBinding.binding = 0;
        vertexSetBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        vertexSetBinding.descriptorCount = 1;
        vertexSetBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        vertexSetBinding.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutBinding fragmentSetBinding = {};
        fragmentSetBinding.binding = 1;
        fragmentSetBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        fragmentSetBinding.descriptorCount = 1;
        fragmentSetBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragmentSetBinding.pImmutableSamplers = nullptr;

        vector<VkDescriptorSetLayoutBinding> setBindings = {vertexSetBinding, fragmentSetBinding};

        VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
        setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        setLayoutInfo.pNext = nullptr;
        setLayoutInfo.flags = 0;
        setLayoutInfo.bindingCount = setBindings.size();
        setLayoutInfo.pBindings = setBindings.data();

        r = vkCreateDescriptorSetLayout(vulkanContext.getDevice(), &setLayoutInfo, nullptr, &setLayout);
        if(r != VK_SUCCESS) {
            throw new runtime_error("Could not create descriptor set");
        }

        VkPipelineLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        layoutInfo.pNext = nullptr;
        layoutInfo.flags = 0;
        layoutInfo.setLayoutCount = 1;
        layoutInfo.pSetLayouts = &setLayout;
        layoutInfo.pushConstantRangeCount = 0;
        layoutInfo.pPushConstantRanges = nullptr;

        r = vkCreatePipelineLayout(vulkanContext.getDevice(), &layoutInfo, nullptr, &layout);
        if(r != VK_SUCCESS) {
            throw new runtime_error("Could not create layout");
        }

        //RENDER PASSES
        VkAttachmentDescription colorAttachment = {};
        colorAttachment.flags = 0;
        colorAttachment.format = VK_FORMAT_R8G8B8A8_SRGB; //TODO derive from swapchain
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorRef = {};
        colorRef.attachment = 0;
        colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription depthAttachment = {};
        depthAttachment.flags = 0;
        depthAttachment.format = VK_FORMAT_D24_UNORM_S8_UINT; //TODO derive from swapchain
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthRef = {};
        depthRef.attachment = 1;
        depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.flags = 0;
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.inputAttachmentCount = 0; //TODO
        subpass.pInputAttachments = nullptr; //TODO
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorRef;
        subpass.pResolveAttachments = nullptr;
        subpass.pDepthStencilAttachment = &depthRef;
        subpass.preserveAttachmentCount = 0;
        subpass.pResolveAttachments = nullptr;

        vector<VkAttachmentDescription> attachements = {colorAttachment, depthAttachment};
        vector<VkSubpassDescription> subPasses = {subpass};
        vector<VkSubpassDependency> subPassDependencies;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		subPassDependencies.push_back(dependency);

        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.pNext = nullptr;
        renderPassInfo.flags = 0;
        renderPassInfo.attachmentCount = attachements.size();
        renderPassInfo.pAttachments = attachements.data();
        renderPassInfo.subpassCount = subPasses.size();
        renderPassInfo.pSubpasses = subPasses.data();
        renderPassInfo.dependencyCount = subPassDependencies.size();
        renderPassInfo.pDependencies = subPassDependencies.data();

        r = vkCreateRenderPass(vulkanContext.getDevice(), &renderPassInfo, nullptr, &renderPass);
        if(r != VK_SUCCESS) {
            throw new runtime_error("Could not create a rendering pass");
        }

        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.pNext = nullptr;
        pipelineInfo.flags = 0;
        pipelineInfo.stageCount = stages.size();
        pipelineInfo.pStages = stages.data();
        pipelineInfo.pVertexInputState = &inputInfo;
        pipelineInfo.pInputAssemblyState = &assemblyInfo;
        pipelineInfo.pTessellationState = nullptr;
        pipelineInfo.pViewportState = &viewportInfo;
        pipelineInfo.pRasterizationState = &rasteriserInfo;
        pipelineInfo.pMultisampleState = &multisamplingInfo;
		pipelineInfo.pDepthStencilState = &depthInfo;
        pipelineInfo.pColorBlendState = &colorInfo;
        pipelineInfo.pDynamicState = nullptr;
        pipelineInfo.layout = layout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex = -1;

        r = vkCreateGraphicsPipelines(vulkanContext.getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline);
        if(r != VK_SUCCESS) {
            throw new runtime_error("Could not create pipeline");
        }
    }

    VulkanRenderPass::~VulkanRenderPass()
    {
        if(pipeline) {
            vkDestroyPipeline(vulkanContext.getDevice(), pipeline, nullptr);
            pipeline = VK_NULL_HANDLE;
        }
        if(layout) {
            vkDestroyPipelineLayout(vulkanContext.getDevice(), layout, nullptr);
            layout = VK_NULL_HANDLE;
        }
        if(setLayout) {
            vkDestroyDescriptorSetLayout(vulkanContext.getDevice(), setLayout, nullptr);
            setLayout = VK_NULL_HANDLE;
        }

        if(vertexShader) {
            vkDestroyShaderModule(vulkanContext.getDevice(), vertexShader, nullptr);
            vertexShader = VK_NULL_HANDLE;
        }
        if(fragmentShader) {
            vkDestroyShaderModule(vulkanContext.getDevice(), fragmentShader, nullptr);
            fragmentShader = VK_NULL_HANDLE;
        }
        if(renderPass) {
            vkDestroyRenderPass(vulkanContext.getDevice(), renderPass, nullptr);
            renderPass = VK_NULL_HANDLE;
        }
    }
}
