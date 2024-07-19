#pragma once

#include "Event.h"

namespace Ivory {
	class IVORY_API MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y) : mouse_x(x), mouse_y(y) {}

		inline float get_x() const { return mouse_x; }
		inline float get_y() const { return mouse_y; }

		std::string to_string() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: (" << mouse_x << ", " << mouse_y << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float mouse_x, mouse_y;
	};

	class IVORY_API MouseScrollEvent : public Event {
	public:
		MouseScrollEvent(float x, float y) : offset_x(x), offset_y(y) {}

		inline float get_offset_x() const { return offset_x; }
		inline float get_offset_y() const { return offset_y; }

		std::string to_string() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: (" << offset_x << ", " << offset_y << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float offset_x, offset_y;
	};

	class IVORY_API MouseButtonEvent : public Event {
	public:
		inline int get_mouse_button() const { return mouse_button; }
		
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button) : mouse_button(button) {}
		int mouse_button;
	};

	class IVORY_API MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

		std::string to_string() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << mouse_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventCategoryMouse | EventCategoryInput)
	};

	class IVORY_API MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		std::string to_string() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << mouse_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventCategoryMouse | EventCategoryInput)
	};
}