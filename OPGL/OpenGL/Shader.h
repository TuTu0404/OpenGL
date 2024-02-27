//预处理命令，只有在没被包含过的情况才编译，防止链接冲突
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>; // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class Shader
{
public:
    // 程序ID
    unsigned int ID;

    // 构造器读取并构建着色器
    Shader(const char* vertexPath, const char* fragmentPath) {
        string vertexCode;
        string fragmentCode;
        ifstream vShaderFile;
        ifstream fShaderFile;

        vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

        try
        {
            //打开文件
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            stringstream vShaderStream, fShaderStream;
            //读取文件
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            //关闭
            vShaderFile.close();
            fShaderFile.close();
            //转换
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (ifstream::failure& e)
        {
            cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        //编译着色器
        int success;
        char infoLog[512];
        unsigned int vertex, fragement;
        vertex = creatVertexShader(&vertex, &success, infoLog,vShaderCode);
        fragement = creatFragmentShader(&fragement,&success,infoLog,fShaderCode);
        ID = linkProgram(vertex, fragement,&success,infoLog);

        glDeleteShader(vertex);
        glDeleteShader(fragement);
    }

   
    // 使用/激活程序
    void use()
    {
        glUseProgram(ID);
    }

    // uniform工具函数
    void setBool(const string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

private:
    unsigned int creatVertexShader(unsigned int* vertexShader, int* success, char* infoLog, const char* vertexShaderSource) {
        *vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(*vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(*vertexShader);
        //检测着色器对象是否编译成功

        glGetShaderiv(*vertexShader, GL_COMPILE_STATUS, success);
        if (!*success) {
            glGetShaderInfoLog(*vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        return *vertexShader;
    }

    unsigned int creatFragmentShader(unsigned int* fragmentShader, int* success, char* infoLog, const char* fragmentShaderSource) {
        //片段着色器
        *fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(*fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(*fragmentShader);
        // check for shader compile errors
        glGetShaderiv(*fragmentShader, GL_COMPILE_STATUS, success);
        if (!*success)
        {
            glGetShaderInfoLog(*fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        return *fragmentShader;
    }

    unsigned int linkProgram(unsigned int& vertexShader, unsigned int& fragmentShader, int* success, char* infoLog) {
        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, success);
        if (!*success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        return shaderProgram;
    }
};

#endif
