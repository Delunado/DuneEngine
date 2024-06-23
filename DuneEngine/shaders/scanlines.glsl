#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float time; // Elapsed time for dynamic effects

// Screen parameters
uniform int renderWidth = 800; // Dynamic render width
uniform int renderHeight = 800; // Dynamic render height

// Effect intensities and parameters
uniform float scanlineEffectIntensity = 0.45f; // Intensity of scanline effect
uniform float scanlineFrequency = 700.0; // Frequency of scanlines
uniform float scanlineSpeed = 25.0; // Speed of scanline movement
uniform float scanlineHeight = 0.075; // Height of the scanlines

uniform float curvatureDistortion = 0.02f; // Amount of curvature distortion
uniform float curvatureFactor = 0.025; // Curvature effect factor
uniform float borderSize = 0.05f; // Size of border

uniform float vignetteStrength = 0.8f; // Strength of vignette effect
uniform float vignetteStart = 0.1; // Start of vignette effect
uniform float vignetteEnd = 0.8; // End of vignette effect

uniform vec2 colorOffset = vec2(0.001f, 0.001f); // Color channel offset

uniform float noiseIntensity = 0.075f; // Intensity of noise effect
uniform float noiseSpeed = 0.001f; // Speed of noise movement

uniform float flickerIntensity = 0.0065f; // Intensity of screen flicker effect

uniform vec3 colorTint = vec3(1.0, 0.95, 0.92); // Color tint for aged look

// Output fragment color
out vec4 finalColor;

// Function to generate pseudo-random numbers
float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233)) + time) * 43758.5453);
}

void main()
{
    float adjustedTime = time * 0.001;

    // Calculate UVs
    vec2 uv = fragTexCoord;

    // Apply border size reduction
    uv = uv * (1.0 - borderSize * 2.0) + borderSize;

    // Simulate CRT curvature and barrel distortion
    vec2 crtUV = (uv - 0.5) * 2.0;
    crtUV.x *= renderWidth / renderHeight;
    float dist = dot(crtUV, crtUV);
    uv += crtUV * (curvatureFactor * dist);

    // Apply vignetting effect
    float vignette = 1.0 - smoothstep(vignetteStart, vignetteEnd, dist);
    vignette = mix(vignetteStrength, 1.0, vignette);

    // Animate RGB offsets
    vec2 animatedColorOffset = colorOffset * vec2(sin(adjustedTime), cos(adjustedTime));

    // Phosphor screen effect: Slightly offset the color channels
    vec3 color = vec3(
        texture(texture0, uv + vec2(-animatedColorOffset.x, 0.0)).r, // Red channel offset
        texture(texture0, uv).g, // Green channel centered
        texture(texture0, uv + vec2(animatedColorOffset.y, 0.0)).b // Blue channel offset
    );

    // Add dynamic noise effect
    float noise = (rand(uv * 10.0) - 0.5) * noiseIntensity;
    color += noise;

    // Apply moving scanlines effect
    float scanline = sin((uv.y + adjustedTime * scanlineSpeed) * scanlineFrequency * scanlineEffectIntensity) * scanlineHeight + (1.0 - scanlineHeight);
    color *= scanline;

    // Add screen flicker
    float flicker = sin(adjustedTime * 60.0) * flickerIntensity + 1.0 - flickerIntensity;
    color *= flicker;

    // Apply vignette
    color *= vignette;

    // Apply subtle color tint for aged look
    color *= colorTint;

    finalColor = vec4(color, 1.0);
}