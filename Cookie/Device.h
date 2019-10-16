#pragma once

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
		using BufferPointer = intptr_t;
		
		virtual ~Device() = default;

		// Todo: Remove these 4 methods from this class
		virtual bool Run() = 0;
		virtual int64_t GetTimeSpecific() const = 0;
		virtual double GetTimeIntervalsInSec(int64_t start, int64_t stop) const = 0;
		virtual int Init(CdsMode cdsMode, HMODULE hModule) = 0;

		virtual BufferPointer CreateBuffer(BufferDescription const& bufferDescription, void const* data) = 0;
				
		// Mesh
		virtual void SetTopology() = 0;
		virtual void SetVertexBuffer(BufferPointer ) = 0;
		virtual void SetIndexBuffer() = 0;
		
		virtual void Draw(int32_t nbIndices) = 0;
		
		virtual void Clear(Color const& clearColor) = 0;
		virtual void Present() = 0;

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
	protected:
		uint32_t screenWidth;
		uint32_t screenHeight;
	};
}