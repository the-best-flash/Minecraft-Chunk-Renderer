#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <iostream>
#include <string>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//#include <GL/glut.h>

#include <QGLWidget>

#include "nbt.h"

#include "Color.h"

#include "gloptions.h"

#include "point.h"

class Chunk
{
private:
    nbt_byte_array *blocks;
    nbt_byte_array *height;
    nbt_byte_array *light;
    Color *img;
    nbt_file *nf;
    bool *visible;
    
public:
    int32_t x_pos, z_pos;
    
    enum RenderMode{ TOP_DOWN_LOW_RES, TOP_DOWN_LOW_RES_HEIGHT,
                     TOP_DOWN_LOW_RES_LIGHT, OPEN_GL_RENDER,
                     OPEN_GL_RENDER_TOP, OPEN_GL_RENDER_CAVES,
                     OPEN_GL_RENDER_CAVES_LIGHT, OPEN_GL_RENDER_TEX,
                     OPEN_GL_RENDER_TOP_TEX, OPEN_GL_RENDER_CAVES_TEX,
                     CAVES_LOW_RES, CAVES_LOW_RES_HEIGHT, CAVES_LOW_RES_LIGHT};

    Chunk(GlOptions* opts);
    Chunk(GlOptions* opts, string filename);
    Chunk(Chunk &c);
    ~Chunk();
    void free();
    void Load(string filename);
    void Render(int mode, void *params = NULL);
    void GetImage(Color* dest, int width, int height, int min_x, int min_z);
    void ComputeVisibility();
    
private:
    Color GetBlockColor(int blockId);
    Color GetLightColor(int illumination);
    bool IsDrawable(int blockID);
    bool IsOpaque(int blockID);
    void DrawBlock(int blockId);
    void DrawBlock(Color color);

    GlOptions* GLopt;
    
};

#endif 
