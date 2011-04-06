#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

#include "glwidget.h"

#include "gloptions.h"

#include "chunkselection.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void LoadChunk();
    void ClearChunks();
    void RenderAll();
    void RenderTop();
    void RenderCaves();
    void RenderCavesLightmap();
    void RenderAllTex();
    void RenderTopTex();
    void RenderCavesTex();
    void RotateMode();
    void MoveMode();
    void TopDownView();
    void FirstPersonView();
    void SelectChunk();
    void ChunkSelected(void* chunk);

protected:
    void keyPressEvent(QKeyEvent *);

private:
    Ui::MainWindow *ui;

    list<Chunk*>* lst;
    glWidget *Renderer;
    GlOptions* glOpts;

    QFileDialog* fileDialog;
    QFileDialog* folderDialog;

    ChunkSelection* chunkSelection;
};

#endif // MAINWINDOW_H
