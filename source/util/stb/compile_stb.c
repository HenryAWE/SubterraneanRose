/**
 * @file compile_stb.c
 * @author HenryAWE
 * @brief Compile the source in header files of stb library
 */

#define STB_IMAGE_IMPLEMENTATION 1
#define STBI_ONLY_PNG 1
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION 1
#include "stb_image_write.h"
