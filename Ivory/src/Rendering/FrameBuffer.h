#pragma once
#include <cstdint>
#include <memory>

namespace Ivory {
	struct FrameBufferSpecification {
		uint32_t width, height;
		uint32_t samples = 1;

		bool swap_chain_target = false;
	};

	class FrameBuffer {
	public:
		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t get_color_attachment_rendererID() const = 0;

		virtual const FrameBufferSpecification& get_spec() const = 0;
		virtual FrameBufferSpecification& get_spec() = 0;
		static std::shared_ptr<FrameBuffer> create(const FrameBufferSpecification& spec);
	};
}