#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float time; // Elapsed time for dynamic effects
uniform int renderWidth = 800; // Dynamic render width
uniform int renderHeight = 800; // Dynamic render height
uniform float scanlineEffectIntensity = 0.45f; // Intensity of scanline effect
uniform float curvatureDistortion = 0.2f; // Amount of curvature distortion
uniform float vignetteStrength = 0.8f; // Strength of vignette effect
uniform vec2 colorOffset = vec2(0.0015f, 0.0015f);
uniform float borderSize = 0.05f;

// Output fragment color
out vec4 finalColor;

void main()
{
    // Calculate UVs
    vec2 uv = fragTexCoord;

    // Reduce UV area according to the border size
    uv = uv * (1.0 - borderSize * 2.0) + borderSize;
    
    // Apply scanlines effect
    float scanline = sin(uv.y * renderHeight * scanlineEffectIntensity + time * 5.0) * 0.05 + 0.95;
    
    // Simulate CRT curvature
    vec2 crtUV = uv - 0.5;
    crtUV.x *= renderWidth / renderHeight;
    float dist = length(crtUV);
    uv += crtUV * dist * dist * curvatureDistortion;
    
    // Vignetting effect
    float vignette = 1.0 - smoothstep(0.0, 0.5, dist);
    vignette = mix(vignetteStrength, 1.0, vignette); // Apply strength modifier
    
    // Phosphor screen effect: Slightly offset the color channels
    vec3 color = vec3(
        texture(texture0, uv + vec2(-colorOffset.x, 0.0)).r, // Red channel offset
        texture(texture0, uv).g, // Green channel is centered
        texture(texture0, uv + vec2(colorOffset.y, 0.0)).b // Blue channel offset
    );
    
    color = color * scanline * vignette; // Apply scanline and vignette
    
    finalColor = vec4(color, 1.0);
}