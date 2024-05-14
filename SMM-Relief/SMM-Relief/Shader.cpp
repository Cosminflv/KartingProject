#include "Shader.h"

void Shader::Set(const std::string& sourceFilePath, const std::string& shadowVertexFilePath, const std::string& shadowFragmentFilePath)
{
	ShaderSource source = ParseShader(sourceFilePath);
	ShaderSource shadowSource = ParseShader(shadowVertexFilePath, shadowFragmentFilePath);

	// Create main shader program
	ShaderIndex = CreateShader(source.VertexSource, source.FragmentSource);

	// Create shadow mapping shader program
	unsigned int shadowShaderIndex = CreateShader(shadowSource.ShadowVertexSource, shadowSource.ShadowFragmentSource);

	// Attach shadow mapping shaders to main shader program
	glAttachShader(ShaderIndex, shadowShaderIndex);

	// Link the shader program
	glLinkProgram(ShaderIndex);

	// Delete the shadow mapping shaders as they are no longer needed
	//glDeleteShader(shadowVertexShader);
	//glDeleteShader(shadowFragmentShader);
}

void Shader::Set(const std::string& sourceFilePath)
{
	ShaderSource source = ParseShader(sourceFilePath);
	ShaderIndex = CreateShader(source.VertexSource, source.FragmentSource);
}

void Shader::Use()
{
	glUseProgram(ShaderIndex);
}

void Shader::Delete()
{
	glDeleteProgram(ShaderIndex);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ShaderIndex, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ShaderIndex, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, const float& value) const
{
	glUniform1f(glGetUniformLocation(ShaderIndex, name.c_str()), value);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ShaderIndex, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ShaderIndex, name.c_str()), x, y, z);
}

ShaderSource Shader::ParseShader(const std::string& filePath)
{
	std::ifstream fin(filePath);

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(fin, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

ShaderSource Shader::ParseShader(const std::string& filePath, const std::string& shadowFilePath)
{
	std::ifstream fin(filePath);
	std::ifstream shadowFin(shadowFilePath);

	std::string line;
	std::stringstream ss[4]; // One for each shader stage
	ShaderType type = ShaderType::NONE;

	// Read main shader file
	while (getline(fin, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[static_cast<int>(type)] << line << '\n';
		}
	}

	// Read shadow mapping shader file
	while (getline(shadowFin, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[static_cast<int>(type)] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str(), ss[2].str(), ss[3].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)malloc(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile shader";
		std::cout << message << '\n';
		glDeleteShader(id);
		return 0;
	}
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}
