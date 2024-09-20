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

using namespace Tusk;

namespace Ivory {
    class ScriptVM {
    public:
        static std::shared_ptr<ClassObject> load_script(const std::filesystem::path& path);
        static Emulator s_emulator;
        static ErrorHandler s_handler;

        static std::unordered_map<std::string, std::shared_ptr<Tusk::ClassObject>> s_class_map;
        //static std::unordered_map<std::string, std::shared_ptr<Tusk::InstanceObject>> m_instance_map;
        static Result run(const std::string& in);
    };
}