#ifndef GLOPTIONS_H
#define GLOPTIONS_H

#include <QGLWidget>
#include <QPixmap>

class GlOptions
{
public:
    GlOptions();
    ~GlOptions();

    void BindTexture();
    void DrawCube(int id);
    void DrawCube();
    void Init(QGLWidget* parent);

    void Cube(int id);
    void Cube();

private:
    GLuint lst_index;
    GLuint tex_id;

    enum Face{ UP = 1, DOWN = 3, LEFT = 0, RIGHT = 2, BACK = 4, FRONT = 5, TOP = 1, BOTTOM = 3 };

    void Cube(float*** tx);

    void SetTexCoords(float*** tx, int face, int start_x, int start_y);
};

#endif // GLOPTIONS_H
