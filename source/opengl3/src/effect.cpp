/**
 * @file effect.cpp
 * @author HenryAWE
 * @brief Shader effects
 */

#include <sr/graphic/opengl3/effect.hpp>
#include <string>


const char generic_vs[] = 
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

const char grayscale_fs[] =
R"(#version 330 core
in vec2 texcoord;
out vec4 out_color;
uniform sampler2D tex;
void main(){
    vec4 texel = texture(tex, texcoord);
    float average = 0.2126 * texel.r + 0.7152 * texel.g + 0.0722 * texel.b;
    out_color = vec4(average, average, average, texel.a);
})";

const char convolution_fs_template[] =
R"(#version 330 core
in vec2 texcoord;
out vec4 out_color;
uniform sampler2D tex;
const float offset = 1.0f/300.0f;
void main(){
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset),
        vec2( 0.0f,    offset),
        vec2( offset,  offset),
        vec2(-offset,  0.0f),
        vec2( 0.0f,    0.0f),
        vec2( offset,  0.0f),
        vec2(-offset, -offset),
        vec2( 0.0f,   -offset),
        vec2( offset, -offset)
    );

    float kernel[9] = float[](
        %f, %f, %f,
        %f, %f, %f,
        %f, %f, %f
    );

    vec4 sample[9];
    for(int i = 0; i < 9; ++i)
    {
        sample[i] = texture(tex, texcoord.st + offsets[i]);
    }
    vec4 color = vec4(0.0f);
    for(int i = 0; i < 9; ++i)
        color += sample[i] * kernel[i];

    out_color = color;
})";

namespace srose::gpu::opengl3
{
    namespace detail
    {
        std::string GenerateFragShaderFromConvolutionMatrix(const float (&kernel)[9])
        {
            auto print = [&kernel](char* buf, std::size_t buf_size)->int
            {
                return std::snprintf(
                    buf, buf_size,
                    convolution_fs_template,
                    kernel[0], kernel[1], kernel[2],
                    kernel[3], kernel[4], kernel[5],
                    kernel[6], kernel[7], kernel[8]
                );
            };
            std::string result;
            result.resize(print(nullptr, 0)+1);
            print(result.data(), result.size());

            return std::move(result);
        }
    } // namespace detail
    

    ShaderProgram CreateEffect(EffectID id)
    {
        ShaderProgram sh;
        sh.Generate();

        switch(id)
        {
        case EffectID::INVERSION:
            sh.Compile(generic_vs, inversion_fs);
            break;
        
        case EffectID::GRAYSCALE:
            sh.Compile(generic_vs, grayscale_fs);
            break;

        case EffectID::SHARPEN:
            sh.Compile(
                generic_vs,
                detail::GenerateFragShaderFromConvolutionMatrix({
                    -1, -1, -1,
                    -1,  9, -1,
                    -1, -1, -1
                }).c_str()
            );
            break;

        case EffectID::BLUR:
            sh.Compile(
                generic_vs,
                detail::GenerateFragShaderFromConvolutionMatrix({
                    1.0f / 16, 2.0f / 16, 1.0f / 16,
                    2.0f / 16, 4.0f / 16, 2.0f / 16,
                    1.0f / 16, 2.0f / 16, 1.0f / 16
                }).c_str()
            );
            break;
        }

        return std::move(sh);
    }
} // namespace srose::gpu::opengl3
