#include "../Core/Shader_Loader.h"  //! Correction 1/12/2015
#include <iostream>
#include <fstream>
#include <vector>
 
using namespace Core;
 
Shader_Loader::Shader_Loader(void){}
Shader_Loader::~Shader_Loader(void){}
 
std::string Shader_Loader::ReadShader(char *filename)
{
	std::string shaderCode;
    std::ifstream file(filename, std::ios::in);
 
    if(!file.good())
    {
		std::cout<<"Can't read file "<<filename<<std::endl;
        std::terminate();
    }
 
    file.seekg(0, std::ios::end);
    shaderCode.resize((unsigned int)file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&shaderCode[0], shaderCode.size());
    file.close();
    return shaderCode;
}

GLuint Shader_Loader::CreateShader(GLenum shaderType, std::string source, char* shaderName)
{
	int compile_result = 0;
 
    GLuint shader = glCreateShader(shaderType); // create an empty shader object
    const char *shader_code_ptr = source.c_str(); // 
    const int shader_code_size = source.size();

    glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size); //  it loads the shader object with the code; count is usually 
	// set to 1, because you normally have one character array, shader_code is the array of code and length is normally set to 
	// NULL (thus, it will read code from the array until it reaches NULL)
    glCompileShader(shader); // compiles the code
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result); //  Check for errors and output them to the console
 
    //check for errors
    if (compile_result == GL_FALSE)
    {
		int info_log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> shader_log(info_log_length);
        glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
        std::cout << "ERROR compiling shader: " << shaderName << std::endl << &shader_log[0] << std::endl;
        return 0;
    }
    return shader;
}

GLuint Shader_Loader::CreateProgram(char* vertexShaderFilename, char* fragmentShaderFilename)
{
	//read the shader files and save the code
    std::string vertex_shader_code = ReadShader(vertexShaderFilename);
    std::string fragment_shader_code = ReadShader(fragmentShaderFilename);
 
    GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
    GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");
 
    int link_result = 0;
    //create the program handle, attatch the shaders and link it
    GLuint program = glCreateProgram();
	// attatch the shader to the program
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
	// link the program
    glLinkProgram(program);
	// check if the shader was linked properly
    glGetProgramiv(program, GL_LINK_STATUS, &link_result); //output possible errors to the console. Checking and catching shader 
	// and linking program errors is extremely important, helping you saving time and many headaches.

    // check for link errors
    if (link_result == GL_FALSE)
    {
		int info_log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> program_log(info_log_length);
        glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
        std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
        return 0;
    }
return program;
}