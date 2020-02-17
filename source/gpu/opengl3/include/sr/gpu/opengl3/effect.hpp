/**
 * @file effect.hpp
 * @author HenryAWE
 * @brief Shader effects
 */

#ifndef SROSE_GPU_OPENGL3_effect_hpp_
#define SROSE_GPU_OPENGL3_effect_hpp_

#include <glad/glad.h>
#include "shader.hpp"


namespace srose::gpu::opengl3
{
    enum class EffectID : int
    {
        INVERSION = 0,
        EffectID_MAX = 0
    };

    ShaderProgram CreateEffect(EffectID id);
} // namespace srose::gpu::opengl3


#endif
