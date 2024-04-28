#pragma once
#include "DUShader.h"

namespace Dune::Renderer
{
    class DURenderer
    {
    public:
        static void BeginShaderMode(const DUShader& shader);
        static void EndShaderMode();
    };
}
