#include "DUShader.h"

Dune::Renderer::DUShader::DUShader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    const char* fixedVertexShaderPath;
    const char* fixedFragmentShaderPath;

    vertexShaderPath == nullptr
        ? fixedVertexShaderPath = nullptr
        : fixedVertexShaderPath = TextFormat(vertexShaderPath, GLSL_VERSION);

    fragmentShaderPath == nullptr
        ? fixedFragmentShaderPath = nullptr
        : fixedFragmentShaderPath = TextFormat(fragmentShaderPath, GLSL_VERSION);

    _shader = LoadShader(fixedVertexShaderPath, fixedFragmentShaderPath);
}

void Dune::Renderer::DUShader::SetFloat(const char* uniformName, float value) const
{
    SetShaderValue(_shader, GetShaderLocation(_shader, uniformName), &value, SHADER_UNIFORM_FLOAT);
}

Shader Dune::Renderer::DUShader::GetRawShader() const
{
    return _shader;
}
