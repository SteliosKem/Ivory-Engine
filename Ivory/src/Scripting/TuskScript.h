#pragma once
#include "Core/Timestep.h"
#include "Tusk/src/Lexer.h"
#include "Tusk/src/Parser.h"
#include "Tusk/src/Compiler.h"
#include "Tusk/src/Emulator.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <unordered_map>
#include "VirtualMachine.h"

namespace Ivory {
	class TuskScript {
	public:
		TuskScript(const std::shared_ptr<Tusk::InstanceObject>& instance) : m_instance{ instance } {}

		void on_create() {
			if (m_instance->public_members.find("on_create") != m_instance->public_members.end())
                ScriptVM::emulator.call_method_external(m_instance, "on_create", 0);
		}
		void on_destroy() {
			if (m_instance->public_members.find("on_destroy") != m_instance->public_members.end())
				ScriptVM::emulator.call_method_external(m_instance, "on_destroy", 0);
		}
		void on_update(Timestep dt) {
			if (m_instance->public_members.find("on_update") != m_instance->public_members.end())
				ScriptVM::emulator.call_method_external(m_instance, "on_update", 0);
		}
	private:
		std::shared_ptr<Tusk::InstanceObject> m_instance;
	};
}