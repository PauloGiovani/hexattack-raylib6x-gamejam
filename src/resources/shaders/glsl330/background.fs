#version 330

// Raylib input attributes
in vec2 fragTexCoord;
in vec4 fragColor;

// C++ uniform variables
uniform float time;
uniform vec2 resolution;

// Pixel final color
out vec4 finalColor;

// ---

// Number generator
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

// ---

// Noise generator
float noise(vec2 st) {

    vec2 i = floor(st);
    vec2 f = fract(st);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Interpolation by cubic hermite curve
    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;

}

// ---

// Fractal noise, using brownian motion, to nebula texture
float fbm(vec2 st) {

    float value = 0.0;
    float amplitude = 0.5;

    for (int i = 0; i < 4; i++) {
        value += amplitude * noise(st);
        st *= 2.0;
        amplitude *= 0.5;
    }

    return value;

}

// ---

// Main function
void main() {

    // Normalized screen coordinates (-1.0 to 1.0)
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    uv.x *= resolution.x / resolution.y;

    // ---

    // Layer 01: nebula

    vec2 nebulaUV = uv * 3.0;

    // Domain warping, using FBM
    vec2 warp = vec2(
        fbm(nebulaUV + vec2(time * 0.05, time * 0.03)),
        fbm(nebulaUV + vec2(-time * 0.04, time * 0.06))
    );

    // Final nebula noise
    float n = fbm(nebulaUV + warp * 2.0);

    // Purple
    //vec3 colorPurple = vec3(0.5, 0.1, 0.8);
    //vec3 colorPurple = vec3(0.35, 0.05, 0.70);
    vec3 colorPurple = vec3(0.55, 0.00, 1.00);

    // Blue
    //vec3 colorBlue = vec3(0.30, 0.40, 0.90);
    vec3 colorBlue   = vec3(0.20, 0.45, 1.00);

    // White
    //vec3 colorWhite  = vec3(0.75, 0.82, 1.0);
    //vec3 colorWhite  = vec3(0.65, 0.80, 1.0);
    //vec3 colorWhite  = vec3(0.85, 0.85, 0.95);
    vec3 colorWhite  = vec3(1.00);

    // Color mix
    float colorMix = (sin(time * 0.4 + n * 2.0) + 1.0) * 0.5;

    // Mix the colors
    //vec3 baseColor = mix(colorPurple, colorWhite, colorMix);

    // Mix purple and blue
    vec3 baseColor = mix(colorPurple, colorBlue, colorMix);

    // Mix result with white
    //baseColor = mix(baseColor, colorWhite, colorMix);
    baseColor = mix(baseColor, colorWhite, colorMix * 0.8);

    baseColor *= 1.5; 

    // Apply color to nebula
    //vec3 nebulaColor = baseColor * smoothstep(0.0, 1.0, n) * 0.08;
    vec3 nebulaColor = baseColor * pow(n, 1.8) * 0.18;

    // ---
    
    // Layer 02: stars

    // Density
    vec2 starUV = uv * 50.0;

    // Cell coordinates
    vec2 cell = floor(starUV);

    // Local coordinates
    vec2 local = fract(starUV);

    float star = 0.0;

    // Chance of star
    float starChance = 0.85;

    // Eight cells technique
    for (int y = -1; y <= 1; y++) {
        
        for (int x = -1; x <= 1; x++) {

            // Get the neighbor
            vec2 neighbor = vec2(float(x), float(y));

            // Set neighnor ID
            vec2 id = cell + neighbor;

            // Does this cell contain a star?
            float exists = step(
                starChance,
                random(id + vec2(151.0, 63.0))
            );

            if (exists > 0.0) {

                // Random position
                vec2 pos = vec2(
                    random(id + vec2(13.1, 91.7)),
                    random(id + vec2(71.3, 42.5))
                );

                // Local coordinates
                vec2 p = neighbor + pos - local;

                // Random size
                float radius = mix(
                    0.015,
                    0.045,
                    random(id + vec2(211.7, 89.3))
                );

                // Star shape

                float d = length(p);

                // Central core
                float core = smoothstep(radius, 0.0, d);

                // Horizontal ray
                float rayX = smoothstep(radius * 0.25, 0.0, abs(p.y));
                rayX *= smoothstep(radius * 4.0, 0.0, abs(p.x));

                // Vertical ray
                float rayY = smoothstep(radius * 0.25, 0.0, abs(p.x));
                rayY *= smoothstep(radius * 4.0, 0.0, abs(p.y));

                // Soft halo
                float halo = smoothstep(radius * 3.0, 0.0, d) * 0.20;

                // Final star
                float shape = core;
                shape += (rayX + rayY) * 0.45;
                shape += halo;

                // Prevent over-brightening
                shape = min(shape, 1.0);

                // Blink
                float phase = random(id + vec2(311.0, 27.0)) * 100.0;

                float speed = mix(
                    0.15,
                    0.35,
                    random(id + vec2(411.0, 57.0))
                );

                float t = fract(time * speed + phase);

                float blink = sin(t * 3.14159265359);
                blink = pow(blink, 4.0);

                // Brightness
                float brightness = mix(
                    0.5,
                    1.2,
                    random(id + vec2(511.0, 17.0))
                );

                star += shape * blink * brightness;
            }
        }
    }

    star = clamp(star, 0.0, 1.0);
    
    // ---

    // Layer 03: final

    // Black background
    vec3 backgroundColor = vec3(0.0, 0.0, 0.0);
    vec3 finalRGB = backgroundColor + nebulaColor + vec3(star);
    
    // Smooth edges
    vec2 v = gl_FragCoord.xy / resolution.xy;
    v = v * 2.0 - 1.0; 
    float vignette = 1.0 - smoothstep(0.6, 1.5, length(v));
    
    // Final color
    finalColor = vec4(finalRGB * vignette, 1.0);

}