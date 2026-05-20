#include "raylib.h"

#ifndef RAYEXTRA_IMPLEMENTATION
#define RAYEXTRA_IMPLEMENTATION

void GetAllCodepoints(int codepoints[512])
{
    for (int i = 0; i < 95; i++)
        codepoints[i] = 32 + i;
    for (int i = 0; i < 400; i++) codepoints[95 + i] = 0xf000 + i;
}

#endif // RAYEXTRA_IMPLEMENTATION
