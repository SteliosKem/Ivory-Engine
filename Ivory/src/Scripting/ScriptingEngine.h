#pragma once
#include <string>
#include "Core/Timestep.h"
#include "VirtualMachine.h"
#include "TuskScript.h"

using namespace Tusk;

namespace Ivory {
	class ScriptHandler {
    public:
        static std::shared_ptr<TuskScript> add_script(const std::filesystem::path& path) {
            std::shared_ptr<ClassObject> class_obj = ScriptVM::load_script(path);
            if (!class_obj)
                return nullptr;
            std::shared_ptr<TuskScript> script = std::make_shared<TuskScript>(class_obj);
            
            m_scripts.push_back(script);
            return script;
        }

        static void start_runtime() {
            for (auto& it : m_scripts) {
                it->make_instance();
                //m_scripts.push_back({it.second});
            }
        }

        static void on_create() {
            for(auto& script : m_scripts)
                script->on_create();
        }
        static void on_update() {
            for (auto& script : m_scripts)
                script->on_update(Timestep());
        }
        static void on_destroy() {
            for (auto& script : m_scripts)
                script->on_destroy();
        }

        static void end_runtime() {
            //m_scripts.clear();
        }
    private:
        static std::vector<std::shared_ptr<TuskScript>> m_scripts;
	};
}