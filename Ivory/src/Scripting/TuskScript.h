#pragma once
#include <filesystem>
#include "VirtualMachine.h"

namespace Ivory {
	class TuskScript {
	public:
		TuskScript(std::shared_ptr<Tusk::ClassObject> class_obj) : m_class{ class_obj } {}

		void make_instance() {
			m_instance = std::make_shared<Tusk::InstanceObject>(*m_class);
			m_instance->private_members = m_class->private_members;
			m_instance->public_members = m_class->public_members;
		}
		void on_create() {
			if (m_instance->public_members.find("on_create") != m_instance->public_members.end())
                ScriptVM::s_emulator.call_method_external(m_instance, "on_create", 0);
		}
		void on_destroy() {
			if (m_instance->public_members.find("on_destroy") != m_instance->public_members.end())
				ScriptVM::s_emulator.call_method_external(m_instance, "on_destroy", 0);
		}
		void on_update(Timestep dt) {
			if (m_instance->public_members.find("on_update") != m_instance->public_members.end())
				ScriptVM::s_emulator.call_method_external(m_instance, "on_update", 0);
		}
	private:
		std::shared_ptr<Tusk::ClassObject> m_class;
		std::shared_ptr<Tusk::InstanceObject> m_instance;
	};
}