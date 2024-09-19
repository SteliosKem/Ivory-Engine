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
        static void load_script(const std::filesystem::path& path);
        static Emulator emulator;
        static ErrorHandler handler;

        static std::unordered_map<std::string, std::shared_ptr<Tusk::ClassObject>> m_class_map;
        //static std::unordered_map<std::string, std::shared_ptr<Tusk::InstanceObject>> m_instance_map;
        static void run(const std::string& in);
    };
}