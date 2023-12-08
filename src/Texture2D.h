#pragma once

#include "API/ExportPropety.h"

#ifdef OGL
#include <glad/glad.h>
#elif  defined(GLFW_INCLUDE_VULKAN)

#endif



#include "Vulkan/DescriptorSetBase.h"
#include "Vulkan/ImageProcessing.h"
#include "Resources/ResourceBase.h"
#include "DependencyResolvers/DependencyList.h"
#include "Tools/VariadicComparer.h"

#include <unordered_map>
#include <string>


#include <memory>
#include <string>
#include <optional>
#include <span>
#include <type_traits>
#include <tuple>

#include <glm/glm.hpp>

class Serializer;

#ifdef GLFW_INCLUDE_VULKAN
class PhysicalDevice;
class LogicalDevice;
class CommandPool;
class SwapChain;
class DescriptorPool;
class CommandBuffer;
class RenderPipeline;
struct VkDescriptorSetLayout_T;
typedef VkDescriptorSetLayout_T* VkDescriptorSetLayout;
struct VkSampler_T;
typedef VkSampler_T* VkSampler;
enum VkFormat;
#endif

class DLLEXPORT Texture2D : public ResourceBase, public ImageProcessing, public DescriptorSetBase
{
	friend class Serializer;
	friend class DeserializerTexture2D;
public:
	struct DLLEXPORT SubTexture2D
	{
		std::shared_ptr<glm::vec2> leftBottomUV;
		std::shared_ptr<glm::vec2> rightTopUV;

		SubTexture2D(const SubTexture2D& sub);

		SubTexture2D(SubTexture2D&& sub) noexcept;

		SubTexture2D(const glm::vec2& _leftBottomUV, const glm::vec2& _rightTopUV);
		SubTexture2D();

		const glm::vec2& getLeftBottomUV() const;
		const glm::vec2& getRightTopUV() const;

		const SubTexture2D& operator=(const SubTexture2D& second);
	};

	Texture2D() = delete;
	Texture2D& operator=(const Texture2D&) = delete;

	void bind(CommandBuffer& commandBuffer, RenderPipeline& renderPipeline, uint32_t currentFrame);

	void addSubTexture(std::string_view name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV);
	const SubTexture2D& getSubTexture(std::string_view name) const;
	unsigned int getWidth() const;
	unsigned int getHeight() const;

	std::string m_path;

	GENERATETYPE(Texture2D)

		static void createDescriptorSetLayout(LogicalDevice& logicalDevice);
	static void destroyDescriptorSetLayout(LogicalDevice& logicalDevice);

private:
	unsigned int m_width;
	unsigned int m_height;

	std::unordered_map<std::string, SubTexture2D> m_subTextures;

#ifdef GLFW_INCLUDE_VULKAN
public:
	Texture2D(std::string_view name, std::string_view relativePath, int texWidth, int texHeight, int texChannels, unsigned char* pixels, std::span<std::string> dependencies);
	Texture2D(const Texture2D& texture2D);
	~Texture2D();

	VkSampler& getTextureSampler();
	uint64_t getImageSize() const;

	static VkDescriptorSetLayout& getDescriptorSetLayout();


	void createDescriptorSets(DescriptorPool& descriptorPool) override;
private:
	void generateMipmaps(VkFormat imageFormat, int32_t texWidth, int32_t texHeight, PhysicalDevice& physicalDevice, CommandPool& commandPool);
	void createTextureImageView();
	void createTextureSampler();

	LogicalDevice& logicalDevice;
	SwapChain& swapchain;

	uint32_t mipLevels;
	VkSampler textureSampler;

	static VkDescriptorSetLayout descriptorSetLayout;
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