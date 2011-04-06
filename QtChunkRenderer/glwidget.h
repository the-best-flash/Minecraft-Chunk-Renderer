#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

#include <list>
using namespace std;

#include "Chunk.h"
#include "point.h"

#include "gloptions.h"

typedef Point<float> float3;

class glWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit glWidget(GlOptions* opts, list<Chunk*>* lst, QWidget *parent = 0);
    ~glWidget();

    void keyPress(QKeyEvent *event);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);


private:
    enum InputMode{ ROTATE, MOVE };
    enum ViewMode{ TOP_DOWN, FIRST_PERSON };

    list<Chunk*> *chunks;

    //The position of the camera
    float3 tempPos;
    float3 CameraPos;
    float3 tempRot;
    float3 Rotation;

    QPoint lastPos;

public:
    float3 Center;

private:
    int Mode;
    InputMode inputMode;
    int renderList;
    int oldSize;
    int oldMode;
    int View;
    int oldView;
    float Zoom;
    GlOptions* glOpts;

signals:

public slots:
    void SetMode(int mode);
    void ZoomIn();
    void ZoomOut();
    void RotateMode();
    void MoveMode();
    void TopDown();
    void FirstPerson();

};

#endif // GLWIDGET_H
