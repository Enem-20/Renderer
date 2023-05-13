#pragma once

#ifdef OGL
#include <glad/glad.h>
#elif GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#endif

#include "../../../src/ExportPropety.h"
#include "../../src/Resources/ResourceBase.h"

#include <unordered_map>
#include <string>



#include "glm/vec2.hpp"

#include <memory>
#include <string>

class Serializer;

#ifdef GLFW_INCLUDE_VULKAN
	class PhysicalDevice;
	class LogicalDevice;
	class CommandPool;
	class SwapChain;
#endif

class DLLEXPORT Texture2D : public ResourceBase
{
	friend class Serializer;
	friend class DeserializerTexture2D;

public:
	struct DLLEXPORT SubTexture2D
	{
		glm::vec2 leftBottomUV;
		glm::vec2 rightTopUV;

		SubTexture2D(const SubTexture2D& sub);

		SubTexture2D(SubTexture2D&& sub) noexcept;

		SubTexture2D(const glm::vec2 _leftBottomUV, const glm::vec2 _rightTopUV);
		SubTexture2D();

		const glm::vec2& getLeftBottomUV() const;
		const glm::vec2& getRightTopUV() const;

		const SubTexture2D& operator=(const SubTexture2D& second);
	};

	Texture2D() = delete;
	Texture2D& operator=(const Texture2D&) = delete;

	void addSubTexture(const std::string& name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV);
	const SubTexture2D& getSubTexture(const std::string& name) const;
	unsigned int getWidth() const;
	unsigned int getHeight() const;

	std::string path;
	inline static const std::string type = GETTYPE(Texture2D);
private:
	unsigned int m_width;
	unsigned int m_height;

	std::unordered_map<std::string, SubTexture2D> m_subTextures;

#ifdef GLFW_INCLUDE_VULKAN
public:
	Texture2D(const std::string& name, int texWidth, int texHeight, int texChannels, unsigned char* pixels, SwapChain& swapChain, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool);
	Texture2D(const Texture2D& texture2D);
	//Texture2D(Texture2D&& texture2D) noexcept;
	~Texture2D();

	VkImage& getTextureImage();
	VkImageView& getTextureImageView();
	VkSampler& getTextureSampler();
	uint64_t getImageSize() const;

private:
	void createTextureImageView();
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void copyImageToBuffer(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	VkImageView createImageView(VkImage image, VkFormat format);
	void createTextureSampler();

	SwapChain& swapchain;
	PhysicalDevice& physicalDevice;
	LogicalDevice& logicalDevice;
	CommandPool& commandPool;

	VkImageView textureImageView;
	VkSampler textureSampler;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
#endif

#ifdef OGL
public:
	Texture2D(const GLuint width, const GLuint height,
		const unsigned char* data, const unsigned int channels = 4,
		const GLenum filter = GL_LINEAR, const GLenum wrapMode = GL_CLAMP_TO_EDGE);
	//Texture2D() = default;
	Texture2D(Texture2D& texture2d);
	Texture2D& operator=(Texture2D&& texture2d) noexcept;
	Texture2D(Texture2D&& texture2d) noexcept;
	~Texture2D();

	void bind() const;

private:
	GLuint m_ID;
	GLenum m_mode;
#endif
};