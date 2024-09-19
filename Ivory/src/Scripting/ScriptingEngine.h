#pragma once
#include <string>
#include "Tusk/src/Lexer.h"
#include "Tusk/src/Parser.h"
#include "Tusk/src/Compiler.h"
#include "Tusk/src/Emulator.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <unordered_map>
#include "VirtualMachine.h"
#include "TuskScript.h"

using namespace Tusk;

namespace Ivory {
	class ScriptHandler {
    public:
        static void add_script(const std::filesystem::path& path) {
            ScriptVM::load_script(path);
        }

        static void start_runtime() {
            for (auto& it : ScriptVM::m_class_map) {
                std::shared_ptr<InstanceObject> instance = std::make_shared<InstanceObject>(*it.second.get());
                instance->private_members = it.second->private_members;
                instance->public_members = it.second->public_members;

                m_scripts.push_back({ instance });
            }
        }

        static void on_create() {
            for(auto& script : m_scripts)
                script.on_create();
        }
        static void on_update() {
            for (auto& script : m_scripts)
                script.on_update(Timestep());
        }
        static void on_destroy() {
            for (auto& script : m_scripts)
                script.on_destroy();
        }

        static void end_runtime() {
            m_scripts.clear();
        }
    private:
        static std::vector<TuskScript> m_scripts;
	};
}