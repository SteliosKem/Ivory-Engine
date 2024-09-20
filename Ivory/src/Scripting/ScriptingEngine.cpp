#include "pch.h"
#include "ScriptingEngine.h"

namespace Ivory {
    std::vector<std::shared_ptr<TuskScript>> ScriptHandler::m_scripts = 
        std::vector<std::shared_ptr<TuskScript>>();
}

/*#include "ScriptingEngine.h"

namespace Ivory {
    void ScriptVM::load_script(const std::filesystem::path& path) {
        std::string script_name = path.filename().string();
        std::ifstream file(path);
        std::stringstream buffer;

        buffer << file.rdbuf();
        run(buffer.str());
        const auto& table = m_emulator.get_global_table();
        if (table.find(script_name) != table.end()) {
            Tusk::Value val = table.at(script_name);
            if (val.is<std::shared_ptr<Tusk::ValueObject>>() && val.get_object_type() == Tusk::ObjectType::CLASS) {
                m_class_map[script_name] = val.get_object<Tusk::ClassObject>();
            }
        }
    }

    void ScriptVM::run(const std::string& in) {
        Lexer lexer(in, m_handler);
        const std::vector<Token>& tokens = lexer.analyze();
        if (!m_handler.has_errors()) {
            Parser parser(tokens, m_handler);
            const std::shared_ptr<AST>& ast = parser.parse();
            if (!m_handler.has_errors()) {
                Compiler compiler(ast, m_handler);

                const Unit& byte_code = compiler.compile();
                if (!m_handler.has_errors()) {
                    std::cout << '\n';
                    if (m_emulator.run(&byte_code) != Result::OK)
                        for (const Error& error : m_handler.get_errors()) {
                            std::cout << ErrorHandler::string_basic_with_type(error) << '\n';
                        }
                    std::cout << '\n';
                }
            }
        }
        m_handler.clear();
    }

    void ScriptVM::execute_on_create() {
        for (auto& it : m_class_map) {
            std::shared_ptr<InstanceObject> instance = std::make_shared<InstanceObject>(*it.second.get());
            instance->private_members = it.second->private_members;
            instance->public_members = it.second->public_members;
            if (instance->public_members.find("on_create") != instance->public_members.end())
                m_emulator.call_method_external(instance, "on_create", 0);
            m_instance_map[it.first] = instance;
        }
    }
    void ScriptVM::execute_on_update() {

    }
    void ScriptVM::execute_on_delete() {

    }
}*/