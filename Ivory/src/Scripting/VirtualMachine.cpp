#include "pch.h"
#include "VirtualMachine.h"

namespace Ivory {
    void ScriptVM::load_script(const std::filesystem::path& path) {
        std::string script_name = path.filename().string();
        std::ifstream file(path);
        std::stringstream buffer;

        buffer << file.rdbuf();
        run(buffer.str());
        const auto& table = emulator.get_global_table();
        if (table.find(script_name) != table.end()) {
            Tusk::Value val = table.at(script_name);
            if (val.is<std::shared_ptr<Tusk::ValueObject>>() && val.get_object_type() == Tusk::ObjectType::CLASS) {
                m_class_map[script_name] = val.get_object<Tusk::ClassObject>();
            }
        }
    }

    void ScriptVM::run(const std::string& in) {
        Lexer lexer(in, handler);
        const std::vector<Token>& tokens = lexer.analyze();
        if (!handler.has_errors()) {
            Parser parser(tokens, handler);
            const std::shared_ptr<AST>& ast = parser.parse();
            if (!handler.has_errors()) {
                Compiler compiler(ast, handler);

                const Unit& byte_code = compiler.compile();
                if (!handler.has_errors()) {
                    std::cout << '\n';
                    if (emulator.run(&byte_code) != Result::OK)
                        for (const Error& error : handler.get_errors()) {
                            std::cout << ErrorHandler::string_basic_with_type(error) << '\n';
                        }
                    std::cout << '\n';
                }
            }
        }
        handler.clear();
    }
}