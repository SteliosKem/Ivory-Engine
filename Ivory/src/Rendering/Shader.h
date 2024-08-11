#pragma once
#include <string>
#include <glm/glm.hpp>
#include <memory>

namespace Ivory {
	class Shader {
	public:
		virtual ~Shader() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const std::string& get_name() const = 0;

		static std::shared_ptr<Shader> create(const std::string& file_path);
		static std::shared_ptr<Shader> create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src);
	};

	class ShaderLibrary {
	public:
		void add(const std::shared_ptr<Shader>& shader);
		std::shared_ptr<Shader> load(const std::string& file_path);

		std::shared_ptr<Shader> get(const std::string& name);
	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
	};
}