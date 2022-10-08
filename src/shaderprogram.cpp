#include"shaderprogram.hpp"
#include<glad/glad.h>
#include<fstream>
#include<iostream>
ShaderProgram::ShaderProgram()
{
    m_ProgramId =glCreateProgram();
}
ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_ProgramId);
}
void ShaderProgram::link()
{
    glLinkProgram(m_ProgramId);
    int isLinked=0;
    char log[512];
    glGetShaderiv(m_ProgramId,GL_LINK_STATUS,&isLinked);

    if(!isLinked){
        glGetProgramInfoLog(m_ProgramId,512,0,log);

        std::cout<<"Program linking error: "<<std::endl<<log<<std::endl;
    }


}
void ShaderProgram::use()
{
    glUseProgram(m_ProgramId);
}

void ShaderProgram::addUniform(const std::string& varName){
    m_UniformVars[varName]= glGetUniformLocation(m_ProgramId,varName.c_str());
}

void ShaderProgram::setFloat(const std::string& varName,float value){
    glUniform1f(m_UniformVars[varName],value);
}

void ShaderProgram::attachShader(const char* fileName,unsigned int shaderType)
{
    unsigned int shaderId = glCreateShader(shaderType);

    std::string sourceCode = getShaderFromFile(fileName);

    const char* chSourceCode = &sourceCode[0];
    glShaderSource(shaderId,1,&chSourceCode,0);
    
    glCompileShader(shaderId);

    int isCompiled=0;
    char log[512];
    glGetShaderiv(shaderId,GL_COMPILE_STATUS,&isCompiled);

    if(!isCompiled){
        glGetShaderInfoLog(shaderId,512,0,log);

        std::cout<<"Shader type: "<<shaderType<<std::endl<<log<<std::endl;
    }

    glAttachShader(m_ProgramId,shaderId);

    glDeleteShader(shaderId);

}
std::string ShaderProgram::getShaderFromFile(const char* fileName)
{
    std::ifstream file(fileName);

    std::string data;
    std::cout << fileName;
    if(file.is_open())
    {   
        
        char readChar;

        while((readChar=file.get())!=EOF)
        {
            data+=readChar;
        }
       
        file.close();
    }
    else {
    std::cout << "okunmadi";

    }
   


    return data;


}