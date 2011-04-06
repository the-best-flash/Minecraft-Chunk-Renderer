#include <QtGui>
#include <QtOpenGL>

#include "glwidget.h"

glWidget::glWidget(GlOptions* opts, list<Chunk*>* lst, QWidget *parent) :
    QGLWidget(parent),
    chunks(lst),
    tempPos(0,0,0),
    CameraPos(0, 0, 0),
    tempRot(0, 0, 0),
    Rotation(0, 0, 0),
    Center(0, 0, 0),
    Mode(Chunk::OPEN_GL_RENDER),
    inputMode(ROTATE),
    oldSize(0),
    oldMode(Chunk::OPEN_GL_RENDER),
    View(TOP_DOWN),
    Zoom(-10),
    glOpts(opts)
{

}

glWidget::~glWidget()
{
    glDeleteLists(renderList, 1);
}

void glWidget::initializeGL()
{
    glOpts->Init(this);

    renderList = glGenLists(1);

    glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glEnable(GL_NORMALIZE);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_MULTISAMPLE);
    //GLfloat lightPosition[4] = { CameraPos.x, CameraPos.y, CameraPos.z, 1.0 };
    GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };

    static const float Gray25[4] = {0.05f, 0.05f, 0.05f, 1.0f};
    glLightfv ( GL_LIGHT0, GL_AMBIENT, Gray25 );

    static const float White[4] = {0.75f, 0.75f, 0.75f, 1.0f};
    glLightfv ( GL_LIGHT0, GL_DIFFUSE, White );

    static const float Black[4] = {0, 0, 0, 1};
    glLightfv ( GL_LIGHT0, GL_SPECULAR, Black );

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void glWidget::paintGL()
{
    static float prev_z = 0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);

    static const float White[4] = {1, 1, 1, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, White);

    glLoadIdentity();

    float3 p = CameraPos + tempPos;

    if(prev_z != p.z)
    {
        GLfloat lightPosition[4] = { 0, 0, (View == TOP_DOWN? -p.z : 0), 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        prev_z = p.z;
    }

    if(View == TOP_DOWN)
        glTranslatef(p.x, p.z, Zoom);

    float3 r = Rotation;
    r += tempRot;

    glRotatef(r.x, 1, 0, 0);
    glRotatef(r.y, 0, 1, 0);
    glRotatef(r.z, 0, 0, 1);

    if(View == TOP_DOWN)
    {
        glTranslatef(-Center.x, Center.y + p.y, -Center.z);
    }
    else
    {
        glTranslatef(-Center.x + p.x, Center.y + p.y, -Center.z + p.z);
    }

    if(chunks != NULL)
    {
        if((int)chunks->size() != oldSize || Mode != oldMode)
        {
            glNewList(renderList, GL_COMPILE);

            glRotatef(90.0, 1.0, 0.0, 0.0);

            glTranslatef(-0.5, -128/16.0f / 2, -0.5);

            for(list<Chunk*>::iterator it = chunks->begin(); it != chunks->end(); it++)
            {
                (*it)->Render(Mode);
            }

            glEndList();

            oldSize = chunks->size();
            oldMode = Mode;
        }


        glCallList(renderList);
    }
}

void glWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glOrtho(-5, 5, -5, 5, -100, 100);
    gluPerspective(40.0f, width / (float)height, 1/32.0f, 40);

    glMatrixMode(GL_MODELVIEW);
}

QSize glWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize glWidget::sizeHint() const
{
    return QSize(400, 400);
}

void glWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}
//! [9]

//! [10]
void glWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    int d;

    if (event->buttons() & Qt::LeftButton) {
        switch(inputMode)
        {
        case ROTATE:
            tempRot.z += dx / (float)this->width() * 360;
            tempRot.x += dy / (float)this->height() * 360;
            break;

        case MOVE:
            tempPos.x += dx / (16.0f * 16.0f);
            tempPos.y += -dy / (16.0f * 16.0f);
            break;
        }
    } else if (event->buttons() & Qt::RightButton) {
        switch(inputMode)
        {
        case ROTATE:
            if(View == TOP_DOWN)
            {
                d = (abs(dx) > abs(dy)? dx : -dy);
                tempRot.y += d / (float)this->width() * 360;
            }
            else
            {
                tempPos.z += dy / (16.0f * 16.0f);
            }
            break;

        case MOVE:
            tempPos.z += dy / (16.0f * 16.0f);
            break;
        }
    }
    lastPos = event->pos();

    this->updateGL();
}

void glWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Rotation += tempRot;
    tempRot.x = 0;
    tempRot.y = 0;
    tempRot.z = 0;

    CameraPos += tempPos;
    tempPos.x = 0;
    tempPos.y = 0;
    tempPos.z = 0;

    this->updateGL();
}

void glWidget::SetMode(int mode)
{
    Mode = mode;

    makeCurrent();

    if(mode == Chunk::OPEN_GL_RENDER_CAVES_TEX || mode == Chunk::OPEN_GL_RENDER_TEX || mode == Chunk::OPEN_GL_RENDER_TOP_TEX)
    {
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glOpts->BindTexture();
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
    }

    updateGL();
}

void glWidget::ZoomIn()
{
    Zoom += 1;

    this->updateGL();
}

void glWidget::ZoomOut()
{
    Zoom -= 1;

    this->updateGL();
}

void glWidget::RotateMode()
{
    inputMode = ROTATE;
}

void glWidget::MoveMode()
{
    inputMode = MOVE;
}

void glWidget::TopDown()
{
    View = TOP_DOWN;
    this->updateGL();
}

void glWidget::FirstPerson()
{
    View = FIRST_PERSON;
    this->updateGL();
}

void glWidget::keyPressEvent(QKeyEvent *event)
{
    keyPress(event);
}

void glWidget::keyPress(QKeyEvent *event)
{
    if(View == FIRST_PERSON)
    {
        Point<float> rot = Rotation + tempRot;

        switch(event->key())
        {
        case Qt::Key_W:
            CameraPos += Point<float>( 2 / (16.0f * 16.0f) * sin(rot.x * M_PI / 180) * sin(rot.z * M_PI / 180),
                                       2 / (16.0f * 16.0f) * sin(rot.x * M_PI / 180) * cos(rot.z * M_PI / 180),
                                       2 / (16.0f * 16.0f) * cos(rot.x * M_PI / 180));
            break;

        case Qt::Key_S:
            CameraPos -=  Point<float>( 2 / (16.0f * 16.0f) * sin(rot.x * M_PI / 180) * sin(rot.z * M_PI / 180),
                                        2 / (16.0f * 16.0f) * sin(rot.x * M_PI / 180) * cos(rot.z * M_PI / 180),
                                        2 / (16.0f * 16.0f) * cos(rot.x * M_PI / 180));
            break;

        case Qt::Key_A:
            CameraPos += Point<float>( 2 / (16.0f * 16.0f) * cos((Rotation.z + tempRot.z) * M_PI / 180), -1 / (16.0f * 16.0f) * sin((Rotation.z + tempRot.z) * M_PI / 180), 0);
            break;

        case Qt::Key_D:
            CameraPos -= Point<float>( 2 / (16.0f * 16.0f) * cos((Rotation.z + tempRot.z) * M_PI / 180), -1 / (16.0f * 16.0f) * sin((Rotation.z + tempRot.z) * M_PI / 180), 0);
            break;

        case Qt::Key_Left:
            Rotation.z -= 2 / (float)this->width() * 360;
            break;

        case Qt::Key_Right:
            Rotation.z += 2 / (float)this->width() * 360;
            break;

        case Qt::Key_Up:
            Rotation.x -= 2 / (float)this->height() * 360;
            break;

        case Qt::Key_Down:
            Rotation.x += 2 / (float)this->height() * 360;
            break;
        }

        this->updateGL();
    }
}
