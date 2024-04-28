#include "DURenderer.h"

void Dune::Renderer::DURenderer::BeginShaderMode(const DUShader& shader)
{
    ::BeginShaderMode(shader.GetRawShader());
}

void Dune::Renderer::DURenderer::EndShaderMode()
{
    ::EndShaderMode();
}
