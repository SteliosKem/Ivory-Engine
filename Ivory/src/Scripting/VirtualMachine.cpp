#include "pch.h"
#include "VirtualMachine.h"

namespace Ivory {
    ErrorHandler ScriptVM::s_handler = ErrorHandler();
    Emulator ScriptVM::s_emulator = Emulator(ScriptVM::s_handler);
    std::unordered_map<std::string, std::shared_ptr<Tusk::ClassObject>> ScriptVM::s_class_map = 
        std::unordered_map<std::string, std::shared_ptr<Tusk::ClassObject>>();

    std::shared_ptr<ClassObject> ScriptVM::load_script(const std::filesystem::path& path) {
        std::string script_name = path.stem().string();
        std::ifstream file(path);
        std::stringstream buffer;

        buffer << file.rdbuf();
        if (run(buffer.str()) != Result::OK)
            return nullptr;
        const auto& table = s_emulator.get_global_table();
        if (table.find(script_name) != table.end()) {
            Tusk::Value val = table.at(script_name);
            if (val.is<std::shared_ptr<Tusk::ValueObject>>() && val.get_object_type() == Tusk::ObjectType::CLASS) {
                std::shared_ptr<ClassObject> class_obj = val.get_object<Tusk::ClassObject>();
                s_class_map[script_name] = val.get_object<Tusk::ClassObject>();
                //class_obj->public_members["transform"] = Value();
                return val.get_object<Tusk::ClassObject>();
            }
        }
    }

    Tusk::Result ScriptVM::run(const std::string& in) {
        s_handler.clear();
        Lexer lexer(in, s_handler);
        const std::vector<Token>& tokens = lexer.analyze();
        if (!s_handler.has_errors()) {
            Parser parser(tokens, s_handler);
            const std::shared_ptr<AST>& ast = parser.parse();
            if (!s_handler.has_errors()) {
                Compiler compiler(ast, s_handler);

                const Unit& byte_code = compiler.compile();
                if (!s_handler.has_errors()) {
                    if (s_emulator.run(&byte_code) != Result::OK)
                        for (const Error& error : s_handler.get_errors()) {
                            std::cout << ErrorHandler::string_basic_with_type(error) << '\n';
                        }
                }
            }
        }
        if (s_handler.has_errors()) {
            return Result::RUNTIME_ERROR;
        }
        
        return Result::OK;
    }
}