#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <optional>
#include <set>
#include <algorithm>
#include <fstream>
#include <assert.h>
#include <array>
#include "Logger.h"

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities = {};
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);
		return attributeDescriptions;
	}
};

class VulkanBase
{
const int WIDTH = 800;
const int HEIGHT = 600;
const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};
const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif
const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
};

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
	VkDebugUtilsMessageTypeFlagsEXT messageType, 
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
	void* pUserData) {
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	}
	return VK_FALSE;
}

static std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::in | std::ios::ate);
	if (file.is_open()) {
		size_t shaderSize = file.tellg();
		file.seekg(0, std::ios::beg);
		// copy file to buffer
		std::vector<char> buffer(shaderSize);
		file.read(buffer.data(), shaderSize);
		file.close();
		return buffer;
	}
	else {
		throw std::runtime_error("failed to open file.");
	}
}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<VulkanBase*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}

static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
	std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, pos);
	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, color);
	return attributeDescriptions;
}

public:
	VulkanBase();
	~VulkanBase();
	virtual void Init();
	virtual void Update();
	virtual void Render();
private:
	GLFWwindow* window;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFrameBuffers;
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkCommandPool commandPool;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	size_t currentFrame = 0;
	bool framebufferResized = false;
	/// <summary></summary>
	void initWindow();
	/// <summary></summary>
	void initVulkan();
	/// <summary></summary>
	void createInstance();
	/// <summary></summary>
	void setupDebugCallback();
	/// <summary></summary>
	void createSurface();
	/// <summary></summary>
	void pickPhysicalDevice();
	/// <summary></summary>
	void createLogicalDevice();
	/// <summary></summary>
	void createSwapChain();
	/// <summary></summary>
	void createImageViews();
	/// <summary></summary>
	void createRenderPass();
	/// <summary><para>Creates a graphics pipeline with the specifications we define.</para>The graphics pipeline is a series of operations to convert data into images on the frame buffer. <para>Such as vertex shading, fragment shading and tessellation, as well as other fixed function pipeline operations.</para></summary>
	void createGraphicsPipeline();
	void createFrameBuffers();
	void createCommandPool();
	void createVertexBuffer();
	void createIndexBuffer();
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createCommandBuffers();
	void createSyncObjects();
	void recreateSwapChain();
	/// <summary></summary>
	VkShaderModule createShaderModule(const std::vector<char>& code);
	/// <summary></summary>
	bool isDeviceSuitable(VkPhysicalDevice device);
	/// <summary></summary>
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	/// <summary></summary>
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	/// <summary></summary>
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	/// <summary></summary>
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	/// <summary></summary>
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	/// <summary></summary>
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void cleanupSwapChain();
	/// <summary></summary>
	bool checkValidationLayerSupport();
	/// <summary></summary>
	std::vector<const char*> getRequiredExtensions();
	/// <summary>Primary loop to represent the program state at each frame.</summary>
	void mainLoop();
	/// <summary>Executed at the end of application to deallocate memory used at runtime.</summary>
	void cleanup();
	void drawFrame();
};
