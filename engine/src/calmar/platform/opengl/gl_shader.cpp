#include "gl_shader.hpp"

#include "calmar/core/util.hpp"
#include "calmar/core/logging.hpp"

#include <glad/glad.h>

namespace calmar {
    glShader::glShader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
        std::vector<render_id> shaders = {};

        std::string vertexSource = util::getFileContents(vertexPath);
        std::string fragmentSource = util::getFileContents(fragmentPath);

        render_id vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        render_id fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        shaders.push_back(vertexShader);
        shaders.push_back(fragmentShader);

        if (geometryPath != "") {
            std::string geometrySource = util::getFileContents(geometryPath);
            render_id geometryShader = compileShader(GL_GEOMETRY_SHADER, geometrySource);
            shaders.push_back(geometryShader);
        }
        linkProgram(shaders);
    }
    glShader::~glShader() {
        glDeleteProgram(mId);
    }

    void glShader::bind() {
        glUseProgram(mId);
    }
    void glShader::unbind() {
        glUseProgram(0);
    }
    void glShader::deleteId() {
        glDeleteProgram(mId);
    }

    render_id glShader::compileShader(unsigned int shaderType, const std::string& shaderSource) {
        int compileSuccess;
        char infoLog[512];

        render_id shader;

        shader = glCreateShader(shaderType);
        const char* shaderSourceCstr = shaderSource.c_str();
        glShaderSource(shader, 1, &shaderSourceCstr, nullptr);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);

        std::string shaderName = "";
        switch (shaderType) {
            case GL_VERTEX_SHADER:
                shaderName = "vertex";
                break;
            case GL_FRAGMENT_SHADER:
                shaderName = "fragment";
                break;
            case GL_GEOMETRY_SHADER:
                shaderName = "geometry";
                break;
            default:
                shaderName = "unknown";
                break;
        }
        if (!compileSuccess) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);

            CALMAR_ERROR("Failed to compile {0} shader", shaderName);
            CALMAR_TRACE("{0}", infoLog);
        } else {
            CALMAR_INFO("Successfully compiled {0} shader", shaderName);
        }

        return shader;
    }

    void glShader::linkProgram(const std::vector<render_id>& shaders) {
        int linkSuccess;
        char infoLog[512];

        mId = glCreateProgram();

        for (render_id shader : shaders) {
            glAttachShader(mId, shader);
        }
        glLinkProgram(mId);

        glGetProgramiv(mId, GL_LINK_STATUS, &linkSuccess);

        if (!linkSuccess) {
            glGetProgramInfoLog(mId, 512, nullptr, infoLog);

            CALMAR_ERROR("Failed to link shader program {0}", mId);
            CALMAR_TRACE("{0}", infoLog);
        } else {
            CALMAR_INFO("Successfully linked shader program {0}", mId);
        }

        for (render_id shader : shaders) {
            glDeleteShader(shader);
        }
    }

}  // namespace calmar
