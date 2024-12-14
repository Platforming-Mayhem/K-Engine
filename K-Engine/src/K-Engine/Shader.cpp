#include "K-Engine/Shader.h"

namespace K 
{
	Shader::Shader() 
	{

	}

	Shader::Shader(int resource)
	{
		HMODULE hModule;
		GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR) & "main", &hModule);
		HRSRC hr = FindResource(hModule, MAKEINTRESOURCE(resource), "SHADER");

		if (hr == NULL)
		{
			std::cout << "Error: Shader File Could Not Be Found!";
		}
		else
		{
			HGLOBAL temp = LoadResource(hModule, hr);
			LPVOID lp = LockResource(temp);

			char* data = static_cast<char*>(lp);

			std::istringstream Shader(data);

			std::string currentLine;
			std::string defaultShaderV;
			std::string defaultShaderF;

			bool writeVertex = false;
			bool writeFragment = false;

			while (std::getline(Shader, currentLine, '\n'))
			{
				if (currentLine.find("//Vertex") != std::string::npos)
				{
					writeVertex = true;
					writeFragment = false;
				}
				else if (currentLine.find("//Fragment") != std::string::npos)
				{
					writeVertex = false;
					writeFragment = true;
				}
				if (writeVertex)
					defaultShaderV += "\n" + currentLine;
				if (writeFragment)
					defaultShaderF += "\n" + currentLine;
			}

			if (!writeVertex && !writeFragment)
			{
				std::cout << "Error: Type for shader not defined, Use //Vertex or //Fragment. \n";
			}
			else
			{
				this->shaderCode[0] = defaultShaderV;
				this->shaderCode[1] = defaultShaderF;
				this->shader = this->CreateShader(this->shaderCode[0], this->shaderCode[1]);
			}
			UnlockResource(temp);
		}
	}

	Shader::Shader(std::string file)
	{
		std::ifstream Shader(ASSET_DIR + file);

		if (!Shader)
		{
			std::cout << "Error: Shader File Could Not Be Found!";
		}
		else
		{
			std::string currentLine;
			std::string defaultShaderV;
			std::string defaultShaderF;

			bool writeVertex = false;
			bool writeFragment = false;

			while (std::getline(Shader, currentLine))
			{
				if (currentLine.find("//Vertex") != std::string::npos)
				{
					writeVertex = true;
					writeFragment = false;
				}
				else if (currentLine.find("//Fragment") != std::string::npos)
				{
					writeVertex = false;
					writeFragment = true;
				}
				if (writeVertex)
					defaultShaderV += "\n" + currentLine;
				if (writeFragment)
					defaultShaderF += "\n" + currentLine;
			}

			if (!writeVertex && !writeFragment)
			{
				std::cout << "Error: Type for shader not defined, Use //Vertex or //Fragment. \n";
			}
			else
			{
				this->shaderCode[0] = defaultShaderV;
				this->shaderCode[1] = defaultShaderF;
				this->shader = this->CreateShader(this->shaderCode[0], this->shaderCode[1]);
			}
		}
	}

	unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, NULL);
		glCompileShader(id);
		//Error handling
		GLint isCompiled;
		glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			std::cout << "Error: Shader could not compile fully" << std::endl;
			GLint maxLength;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
			for (auto letter : errorLog) {
				std::cout << letter;
			}
			glfwTerminate();
			glDeleteShader(id);
		}
		return id;
	}

	unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		int success;
		char infoLog[512];
		unsigned int program = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		// check for linking errors
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		glValidateProgram(program);
		glDeleteShader(vs);
		glDeleteShader(fs);
		return program;
	}
}