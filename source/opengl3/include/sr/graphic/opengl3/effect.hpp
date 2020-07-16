/**
 * @file effect.hpp
 * @author HenryAWE
 * @brief Shader effects
 */

#ifndef SROSE_GPU_OPENGL3_effect_hpp_
#define SROSE_GPU_OPENGL3_effect_hpp_

#include <glad/glad.h>
#include "shader.hpp"


namespace srose::graphic::opengl3
{
    enum class EffectID : int
    {
        INVERSION = 1,
        GRAYSCALE = 2,
        SHARPEN = 3,
        BLUR = 4,
        EffectID_MAX = 4
    };

    ShaderProgram CreateEffect(EffectID id);
} // namespace srose::graphic::opengl3


#endif
