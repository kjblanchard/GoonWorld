#pragma once
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
typedef SDL_Surface SDL_Surface;

int geInitializeTextSubsystem(const char *fontPath, int fontSize);
/**
 * @brief Creates a surface for a character with a specific RGB.  This translates the grayscale to color
 *
 * @param face the face with the character already preloaded in it.
 * @param letter the letter (unused)
 * @param r Red
 * @param g Green
 * @param b Blue
 * @return SDL_Surface* Surface with the color
 */
SDL_Surface *geCreateSurfaceForCharacter(FT_Face face, char letter, int r, int g, int b);
/**
 * @brief Create a Empty Surface object used as "paper" for drawing letters on
 *
 * @param width
 * @param height
 * @return SDL_Surface* Surface you can use for blitting
 */
SDL_Surface *createEmptySurface(int width, int height);
/**
 * @brief Creates a texture with a bunch of letters blitted on it for you, cannot modify characters after the fact
 *
 * @param word The word that should be written
 * @return SDL_Texture* Texture with surfaces blitted to it.
 */
SDL_Texture *geCreateTextureForString(const char *word, SDL_Color color);
/**
 * @brief Cleans up the text system
 *
 * @return int 1 for success
 */
int geShutdownTextSubsystem();