#pragma once
#include <raylib.h>

#define PLATFORM_DESKTOP

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif


namespace Dune::Renderer
{
    class DUShader
    {
    public:
        DUShader(const char* vertexShaderPath, const char* fragmentShaderPath);
        DUShader() = default;

        void SetFloat(const char* uniformName, float value) const;

        Shader GetRawShader() const;

    private:
        Shader _shader;
    };
}
