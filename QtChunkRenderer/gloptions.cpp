#include "gloptions.h"

#include <QDir>

GlOptions::GlOptions()
{
    //Init(parent);
}

GlOptions::~GlOptions()
{
    glDeleteLists(lst_index, 257);
}

void GlOptions::Init(QGLWidget* parent)
{
    lst_index = glGenLists(1 + 256);

    //tex_id = parent->bindTexture(QString("terrain.png"));

    QDir dir;
    QString img = dir.absoluteFilePath("terrain.png");

    QPixmap px(img);

    tex_id = parent->bindTexture(px);

    glNewList(lst_index, GL_COMPILE);
        Cube();
    glEndList();

    for(int i = 0; i < 256; i++)
    {
        glNewList(lst_index + i + 1, GL_COMPILE);
            Cube(i);
        glEndList();
    }
}

void GlOptions::DrawCube()
{
    glCallList(lst_index);
}

void GlOptions::BindTexture()
{
    glBindTexture(GL_TEXTURE_2D, tex_id);
}

void GlOptions::DrawCube(int id)
{
    glCallList(lst_index + 1 + id);
}

void GlOptions::Cube()
{
    float size = 1/16.0f;

    static GLfloat n[6][3] =
    {
      {-1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0},
      {0.0, 0.0, 1.0},
      {0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] =
    {
      {0, 1, 2, 3},
      {3, 2, 6, 7},
      {7, 6, 5, 4},
      {4, 5, 1, 0},
      {5, 6, 2, 1},
      {7, 4, 0, 3}
    };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    glBegin(GL_QUADS);

    for (i = 5; i >= 0; i--) {
      glNormal3fv(&n[i][0]);
      glVertex3fv(&v[faces[i][0]][0]);
      glVertex3fv(&v[faces[i][1]][0]);
      glVertex3fv(&v[faces[i][2]][0]);
      glVertex3fv(&v[faces[i][3]][0]);
    }

    glEnd();
}

void GlOptions::Cube(float*** tx)
{
    float size = 1/16.0f;

    glNewList(lst_index, GL_COMPILE);
    static GLfloat n[6][3] =
    {
      {-1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0},
      {0.0, 0.0, 1.0},
      {0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] =
    {
      {0, 1, 2, 3},
      {3, 2, 6, 7},
      {7, 6, 5, 4},
      {4, 5, 1, 0},
      {5, 6, 2, 1},
      {7, 4, 0, 3}
    };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    glBegin(GL_QUADS);

    for (i = 5; i >= 0; i--) {
      glNormal3fv(&n[i][0]);
      glTexCoord2fv(tx[i][0]);
      glVertex3fv(&v[faces[i][0]][0]);

      glTexCoord2fv(tx[i][1]);
      glVertex3fv(&v[faces[i][1]][0]);

      glTexCoord2fv(tx[i][2]);
      glVertex3fv(&v[faces[i][2]][0]);

      glTexCoord2fv(tx[i][3]);
      glVertex3fv(&v[faces[i][3]][0]);
    }

    glEnd();
}

void GlOptions::SetTexCoords(float*** tx, int face, int start_x, int start_y)
{
    const float px = 1/1024.0f;
    const float offset = px/2;

    start_y = 1024 - start_y;
    //start_x++;

    if(face <= 3)
    {
        tx[face][(2 + face) % 4][0] = start_x * px + offset;
        tx[face][(2 + face) % 4][1] = start_y * px - offset;

        tx[face][(3 + face) % 4][0] = (start_x + 63) * px + offset;
        tx[face][(3 + face) % 4][1] = start_y * px - offset;

        tx[face][(0 + face) % 4][0] = (start_x + 63) * px + offset;
        tx[face][(0 + face) % 4][1] = (start_y - 63) * px - offset;

        tx[face][(1 + face) % 4][0] = start_x * px + offset;
        tx[face][(1 + face) % 4][1] = (start_y - 63) * px - offset;
    }
    else if(face == 5)
    {
        tx[face][3][0] = start_x * px + offset;
        tx[face][3][1] = start_y * px - offset;

        tx[face][0][0] = (start_x + 63) * px + offset;
        tx[face][0][1] = start_y * px - offset;

        tx[face][1][0] = (start_x + 63) * px + offset;
        tx[face][1][1] = (start_y - 63) * px - offset;

        tx[face][2][0] = start_x * px + offset;
        tx[face][2][1] = (start_y - 63) * px - offset;
    }
    else if(face == 4)
    {
        tx[face][1][0] = start_x * px + offset;
        tx[face][1][1] = start_y * px - offset;

        tx[face][2][0] = (start_x + 63) * px + offset;
        tx[face][2][1] = start_y * px - offset;

        tx[face][3][0] = (start_x + 63) * px + offset;
        tx[face][3][1] = (start_y - 63) * px - offset;

        tx[face][0][0] = start_x * px + offset;
        tx[face][0][1] = (start_y - 63) * px - offset;
    }
}

void GlOptions::Cube(int id)
{
    //float tx[6][4][2] = {{{0}}};

    float*** tx = new float**[6];

    for(int j = 0; j < 6; j++)
    {
        tx[j] = new float*[4];

        for(int k = 0; k < 4; k++)
            tx[j][k] = new float[2];
    }


    int start_x = 0;
    int start_y;

    switch(id)
    {
    case 1: //Stone
        start_x = 64;
        start_y = 0;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 2: //Grass
        start_x = 192;
        start_y = 0;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, UP, 0, 0);
        SetTexCoords(tx, DOWN, 128, 0);
        break;

    case 3: //Dirt
        start_x = 128;
        start_y = 0;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 4: //Cobblestone
        start_x = 0;
        start_y = 64;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 5: //Wood
        start_x = 256;
        start_y = 0;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 7: //Bedrock
        start_x = 64;
        start_y = 64;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 8: //Water
    case 9: //Water moving
        start_x = 832;
        start_y = 768;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 10: //Lava
    case 11: //Lava Moving
        start_x = 832;
        start_y = 896;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 12: //Sand
        start_x = 128;
        start_y = 64;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 13: //Gravel
        start_x = 192;
        start_y = 64;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 14: //Gold Ore
        start_x = 0;
        start_y = 128;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 15: //Iron Ore
        start_x = 64;
        start_y = 128;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 16: //Coal
        start_x = 128;
        start_y = 128;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 17: //Tree
        start_x = 256;
        start_y = 64;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, UP, 320, 64);
        SetTexCoords(tx, DOWN, 320, 64);
        break;

    case 18: //Leaves
        start_x = 256;
        start_y = 192;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 19: //Sponge?
        start_x = 0;
        start_y = 192;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 35: //Wool
        start_x = 0;
        start_y = 256;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 41: //Gold
        start_x = 448;
        start_y = 128;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, UP, start_x, 64);
        SetTexCoords(tx, DOWN, start_x, 192);
        break;

    case 42: //Iron
        start_x = 384;
        start_y = 128;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, UP, start_x, 64);
        SetTexCoords(tx, DOWN, start_x, 192);
        break;

    case 43: //Double Stone Slab
        start_x = 320;
        start_y = 0;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, UP, 384, 0);
        SetTexCoords(tx, DOWN, 384, 0);
        break;

    case 44: //Stone Slab
        start_x = 320;
        start_y = 0;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, UP, 384, 0);
        SetTexCoords(tx, DOWN, 384, 0);
        break;

    case 45: //Brick
        start_x = 448;
        start_y = 0;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 46: //TNT
        start_x = 512;
        start_y = 0;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, UP, 576, 0);
        SetTexCoords(tx, DOWN, 640, 0);
        break;

    case 47: //Bokshelf
        start_x = 256;
        start_y = 0;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, FRONT, 192, 128);
        SetTexCoords(tx, BACK, 192, 128);
        break;

    case 48: //Moss Stone
        start_x = 256;
        start_y = 128;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 49: //Obsidian
        start_x = 320;
        start_y = 128;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 50: //Torch
        start_x = 0;
        start_y = 384;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 51: //Fire
        return;

    case 52: //Monster Spawner
        start_x = 64;
        start_y = 256;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 53: //Wooden Stairs
        start_x = 256;
        start_y = 0;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 54: //Chest
        start_x = 640;
        start_y = 64;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, TOP, start_x -  64, start_y);
        SetTexCoords(tx, BOTTOM, start_x -  64, start_y);
        SetTexCoords(tx, FRONT, start_x +  64, start_y);
        break;

    case 56: //Diamond Ore
        start_x = 128;
        start_y = 192;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 57: //Diamond
        start_x = 512;
        start_y = 128;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, UP, start_x, 64);
        SetTexCoords(tx, DOWN, start_x, 192);
        break;

    case 58: //Workbench
        start_x = 704;
        start_y = 192;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, FRONT, start_x + 64, start_y);
        SetTexCoords(tx, BACK, start_x + 64, start_y);
        SetTexCoords(tx, TOP, start_x, start_y - 64);
        SetTexCoords(tx, BOTTOM, start_x, start_y - 64);
        break;

    case 60: //Soil
        Cube();
        return;

    case 61: //Furnace
        start_x = 832;
        start_y = 128;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, FRONT, start_x - 64, start_y);
        SetTexCoords(tx, TOP, start_x + 64, start_y + 64);
        SetTexCoords(tx, BOTTOM, start_x + 64, start_y + 64);
        break;

    case 62: //Burning Furnace
        start_x = 832;
        start_y = 128;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, FRONT, start_x, start_y + 64);
        SetTexCoords(tx, TOP, start_x + 64, start_y + 64);
        SetTexCoords(tx, BOTTOM, start_x + 64, start_y + 64);
        break;

    case 66: //Minecart Tracks
        start_x = 0;
        start_y = 512;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 67: //Cobblestone Stairs
        start_x = 0;
        start_y = 64;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 70: //Stone Pressure Plate
        start_x = 320;
        start_y = 0;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, UP, 384, 0);
        SetTexCoords(tx, DOWN, 384, 0);
        break;

    case 72: //Wooden Pressure Plate
        start_x = 256;
        start_y = 0;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 73: //Redstone Ore
        start_x = 192;
        start_y = 192;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 74: //Glowing Redstone Ore
        start_x = 192;
        start_y = 192;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 75: //Redstone Torch (off)
        start_x = 192;
        start_y = 384;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 76: //Redstone Torch (on)
        start_x = 192;
        start_y = 448;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 78: //Snow
    case 80: //Snow Block
        start_x = 128;
        start_y = 256;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 79: //Ice
        start_x = 192;
        start_y = 256;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 81: //Cactus
        start_x = 384;
        start_y = 256;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, TOP, start_x - 64, start_y);
        SetTexCoords(tx, DOWN, start_x + 64, start_y);
        break;

    case 82: //Clay
        start_x = 512;
        start_y = 256;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 84: //Jukebox
        start_x = 640;
        start_y = 256;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, TOP, start_x + 64, start_y);
        break;

    case 86: //Pumpkin
        start_x = 384;
        start_y = 448;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, FRONT, start_x + 64, start_y);
        SetTexCoords(tx, TOP, start_x, start_y - 64);
        break;

    case 87: //Netherrack
        start_x = 448;
        start_y = 384;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 88: //Soul Sand
        start_x = 512;
        start_y = 384;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 89: //GlowStone
        start_x = 576;
        start_y = 384;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }
        break;

    case 91: //Jack-O-Latern
        start_x = 384;
        start_y = 448;
        for(int i = 0; i < 6; i++)
        {
            SetTexCoords(tx, i, start_x, start_y);
        }

        SetTexCoords(tx, FRONT, start_x + 128, start_y);
        SetTexCoords(tx, TOP, start_x, start_y - 64);
        break;

    default:
        Cube();
        return;
    }

    Cube(tx);

    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            delete [] tx[i][j];
        }

        delete [] tx[i];
    }

    delete [] tx;
}
