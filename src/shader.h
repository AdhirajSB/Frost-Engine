#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>


class Shader{
    public:
    Shader(const std::string &pathVS, const std::string &pathFS){
        std::string vsCode = CreateShader(pathVS);
        std::string fsCode = CreateShader(pathFS);

        unsigned int vertex = CompileShader(GL_VERTEX_SHADER, vsCode);
        unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, fsCode);

        shaderID = glCreateProgram();
        glAttachShader(shaderID, vertex);
        glAttachShader(shaderID, fragment);
        glLinkProgram(shaderID);

        // Check linking errors
        int success;
        glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[1024];
            glGetProgramInfoLog(shaderID, sizeof(infoLog), nullptr, infoLog);
            std::cerr << "ERROR: SHADER PROGRAM LINKING FAILED\n" << infoLog << std::endl;
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            throw std::runtime_error("Shader program linking failed.");
        }

        glDetachShader(shaderID, vertex);
        glDetachShader(shaderID, fragment);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    
    ~Shader(){
        uniformLocationCache.clear();
        glDeleteProgram(shaderID);
    }

    std::string CreateShader(const std::string &path){
        std::ifstream file(path);
        if (!file.is_open()){
            throw std::runtime_error("Failed to open file: " + path);
        }

        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    unsigned int CompileShader(unsigned int type, const std::string &code){
        unsigned int id = glCreateShader(type);
        const char* src = code.c_str();
        glShaderSource(id, 1, &src, NULL);
        glCompileShader(id);

        int success;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[1024];
            glGetShaderInfoLog(id, sizeof(infoLog), nullptr, infoLog);
            std::string shaderType = (type == GL_VERTEX_SHADER) ? "VERTEX" :
                                    (type == GL_FRAGMENT_SHADER) ? "FRAGMENT" :
                                    (type == GL_GEOMETRY_SHADER) ? "GEOMETRY" : "UNKNOWN";

            std::cerr << "ERROR: " << shaderType << " SHADER COMPILATION FAILED\n" << infoLog << std::endl;
            glDeleteShader(id);
            throw std::runtime_error("Shader compilation failed.");
        }

        return id;
    }

    void Use() const{
        glUseProgram(shaderID);
    }
    
    int GetUniformLocation(const std::string& name){
        auto it = uniformLocationCache.find(name);
        if (it != uniformLocationCache.end()){
            return uniformLocationCache[name];
        }

        int location = glGetUniformLocation(shaderID, name.c_str());
        if (location == -1){
            std::cerr << "Warning: uniform " << name << " does not exist!" << std::endl;
        }
        uniformLocationCache[name] = location;

        return location;
    }

    void IsBound() const{
        GLint currentProgram; // GLint instead of unsigned int cuz this shi stupid and wont work otherwise
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

        if (currentProgram != shaderID){
            glUseProgram(shaderID);
        }
    }
    
    void SetMat4(const std::string& name, const glm::mat4 &matrix){
        IsBound();
        unsigned int location = GetUniformLocation(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void SetMat3(const std::string& name, const glm::mat3 &matrix){
        IsBound();
        unsigned int location = GetUniformLocation(name);
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void SetMat2(const std::string& name, const glm::mat2 &matrix){
        IsBound();
        unsigned int location = GetUniformLocation(name);
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void SetVec4(const std::string& name, const glm::vec4 &vector){
        IsBound();
        unsigned int location = GetUniformLocation(name);
        glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
    }

    void SetVec3(const std::string& name, const glm::vec3 &vector){
        IsBound();
        unsigned int location = GetUniformLocation(name);
        glUniform3f(location, vector.x, vector.y, vector.z);
    }

    void SetVec2(const std::string& name, const glm::vec2 &vector){
        IsBound();
        unsigned int location = GetUniformLocation(name);
        glUniform2f(location, vector.x, vector.y);
    }

    void SetFloat(const std::string& name, float value){
        IsBound();
        unsigned int location = GetUniformLocation(name);
        glUniform1f(location, value);
    }

    void SetInt(const std::string& name, int value){
        IsBound();
        unsigned int location = GetUniformLocation(name);
        glUniform1i(location, value);
    }

    void SetBool(const std::string &name, bool value){
        IsBound();
        unsigned int location = GetUniformLocation(name);
        glUniform1i(location, value);
    }

    private:
    unsigned int shaderID;
    std::unordered_map<std::string, int> uniformLocationCache;
};

#endif