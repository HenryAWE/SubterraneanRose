/**
 * @file effect.cpp
 * @author HenryAWE
 * @brief Shader effects
 */

#include <sr/gpu/opengl3/effect.hpp>

const char inversion_vs[] = 
R"(#version 330 core
layout (location=0) in vec3 in_position;
layout (location=1) in vec2 in_texcoord;
out vec2 texcoord;
void main(){
    gl_Position = vec4(in_position, 1.0f);
    texcoord = in_texcoord;
})";
const char inversion_fs[] =
R"(#version 330 core
in vec2 texcoord;
out vec4 out_color;
uniform sampler2D tex;
void main(){
    vec4 texel = texture(tex, texcoord);
    out_color = vec4(1.0f - texel.rgb, texel.a);
})";

namespace srose::gpu::opengl3
{
    ShaderProgram CreateEffect(EffectID id)
    {
        ShaderProgram sh;
        sh.Generate();

        switch(id)
        {
        case EffectID::INVERSION:
            sh.Compile(inversion_vs, inversion_fs);
            break;
        }

        return std::move(sh);
    }
} // namespace srose::gpu::opengl3
