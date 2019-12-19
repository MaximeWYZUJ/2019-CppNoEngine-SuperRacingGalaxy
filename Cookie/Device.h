#pragma once
#include <queue>
#include "Vector2.h"
#include "DeviceEvents.h"

namespace Cookie
{
	struct Color;

	enum class CdsMode
	{
		Windowed,
		FullScreen
	};

	enum class BufferFlag
	{
		Vertex = 0x1L,
		Index = 0x2L
	};
	
	enum class GpuMemoryType
	{
		ReadWrite = 0,
		ReadOnly = 1,
		Dynamic = 2,
		Staging = 3
	};

	enum class CpuAccessFlag
	{
		Nothing = 0,
		Read = 0x10000L,
		Write = 0x20000L
	};

	struct BufferDescription
	{
		uint32_t size;
		BufferFlag buffer;
		GpuMemoryType GpuMemory;
		CpuAccessFlag CpuAccess;
	};

	class Device
	{
	public:
		struct BufferPointer { intptr_t p; };
		struct Texture2D { intptr_t p; };
		struct RenderTarget { intptr_t p; };
		struct ShaderResource { intptr_t p; };
		
		virtual ~Device() = default;

		// Todo: Remove these 4 methods from this class
		virtual bool Update() = 0;
		virtual void PostUpdate();
		virtual int64_t GetTimeSpecific() const = 0;
		virtual double GetTimeIntervalsInSec(int64_t start, int64_t stop) const = 0;
		virtual int Init(CdsMode cdsMode) = 0;

		// Events
		virtual std::vector<DeviceEvent> const& GetEvents() const;

		// Window
		virtual bool HasFocus() const;

		// Wrappers
		virtual BufferPointer CreateBuffer(BufferDescription const& bufferDescription, void const* data) = 0;
		virtual Texture2D CreateTexture2D(Vector2<> size) = 0;
		virtual RenderTarget CreateRenderTarget(Texture2D texture) = 0;
		virtual ShaderResource CreateShaderResource(Texture2D texture) = 0;
		virtual void SetTopology() = 0;
		virtual void SetVertexBuffer(BufferPointer ) = 0;
		virtual void SetIndexBuffer() = 0;
		virtual void SetRenderTargets(std::vector<RenderTarget> renderTargets) = 0;
		virtual void ClearRenderTargets(std::vector<RenderTarget> renderTargets, Color clearColor) = 0;

		virtual void Release(Texture2D texture) = 0;
		virtual void Release(RenderTarget texture) = 0;
		virtual void Release(ShaderResource texture) = 0;

		virtual void Draw(int32_t nbIndices) = 0;
		virtual void Clear(Color const& clearColor) = 0;
		virtual void Present() = 0;

		virtual void EnableAlphaBlend() = 0;
		virtual void DisableAlphaBlend() = 0;

		virtual void enableZBuffer() = 0;
		virtual void disableZBuffer() = 0;
		
		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
	protected:
		uint32_t screenWidth = 0;
		uint32_t screenHeight = 0;
		bool hasFocus = false;
		std::vector<DeviceEvent> events;
	};
}
