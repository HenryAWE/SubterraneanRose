/**
 * @file gl_assert.h
 * @author HenryAWE
 * @brief Assertion for OpenGL3 module
 */

#ifndef SROSE_GPU_OPENGL3_gl_assert_h_
#define SROSE_GPU_OPENGL3_gl_assert_h_

#include <glad/glad.h>
#include <assert.h>
#include <SDL_video.h>

#define SR_ASSERT_CTX() assert(SDL_GL_GetCurrentContext())
#define SR_ASSERT_GL() assert(glGetError() == GL_NO_ERROR)

#endif
