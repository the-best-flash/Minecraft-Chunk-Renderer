#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    glOpts = new GlOptions();

    lst = new list<Chunk*>();

    //ui->GLwidget = new glWidget(lst, this->centralWidget());

    Renderer = new glWidget(glOpts, lst, this->centralWidget());

    //QHBoxLayout *mainLayout = new QHBoxLayout;
    this->centralWidget()->layout()->addWidget(Renderer);

    this->connect(this->ui->actionClear_Chunks, SIGNAL(triggered()), this, SLOT(ClearChunks()));
    this->connect(this->ui->actionLoad_Chunk, SIGNAL(triggered()), this, SLOT(LoadChunk()));
    this->connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

    this->connect(this->ui->actionRender_All, SIGNAL(triggered()), this, SLOT(RenderAll()));
    this->connect(this->ui->actionRender_Top, SIGNAL(triggered()), this, SLOT(RenderTop()));
    this->connect(this->ui->actionRender_Caves, SIGNAL(triggered()), this, SLOT(RenderCaves()));
    this->connect(this->ui->actionRenderCavesLightmap, SIGNAL(triggered()), this, SLOT(RenderCavesLightmap()));
    this->connect(this->ui->actionRenderAllTextured, SIGNAL(triggered()), this, SLOT(RenderAllTex()));
    this->connect(this->ui->actionRenderTopTextured, SIGNAL(triggered()), this, SLOT(RenderTopTex()));
    this->connect(this->ui->actionRenderCavesTextured, SIGNAL(triggered()), this, SLOT(RenderCavesTex()));

    this->connect(this->ui->actionRotate, SIGNAL(triggered()), this, SLOT(RotateMode()));
    this->connect(this->ui->actionRotate, SIGNAL(triggered()), this->Renderer, SLOT(RotateMode()));

    this->connect(this->ui->actionMove, SIGNAL(triggered()), this, SLOT(MoveMode()));
    this->connect(this->ui->actionMove, SIGNAL(triggered()), this->Renderer, SLOT(MoveMode()));

    this->connect(this->ui->actionZoomIn, SIGNAL(triggered()), this->Renderer, SLOT(ZoomIn()));
    this->connect(this->ui->actionZoomOut, SIGNAL(triggered()), this->Renderer, SLOT(ZoomOut()));

    this->connect(this->ui->actionSelectChunk, SIGNAL(triggered()), this, SLOT(SelectChunk()));

    this->connect(this->ui->actionFirstPerson, SIGNAL(triggered()), this, SLOT(FirstPersonView()));
    this->connect(this->ui->actionFirstPerson, SIGNAL(triggered()), this->Renderer, SLOT(FirstPerson()));

    this->connect(this->ui->actionTopDown, SIGNAL(triggered()), this, SLOT(TopDownView()));
    this->connect(this->ui->actionTopDown, SIGNAL(triggered()), this->Renderer, SLOT(TopDown()));

    chunkSelection = new ChunkSelection("", lst, glOpts, this);

    this->connect(chunkSelection, SIGNAL(SelectChunk(void*)), this, SLOT(ChunkSelected(void*)));

    fileDialog = new QFileDialog(this, "Load Chunk", QString(), "Chunk File (*.dat)");
    folderDialog = new QFileDialog(this, "Load Map");

    folderDialog->setFileMode(QFileDialog::DirectoryOnly);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete glOpts;

    for(list<Chunk*>::iterator it = lst->begin(); it != lst->end(); it++)
    {
        (*it)->free();
        delete *it;
    }

    delete lst;
}

void MainWindow::LoadChunk()
{
    //QString filename = QFileDialog::getOpenFileName(this, "Load Chunk", QString(), "Chunk File (*.dat)");

    if(fileDialog->exec())
    {
        QString filename = fileDialog->selectedFiles().at(0);

        Chunk* c = new Chunk(glOpts, filename.toStdString());

        lst->push_back(c);

        Renderer->Center.x = c->x_pos;
        Renderer->Center.y = c->z_pos;

        Renderer->updateGL();
    }
}

void MainWindow::ClearChunks()
{
    for(list<Chunk*>::iterator it = lst->begin(); it != lst->end(); it++)
    {
        (*it)->free();
        delete *it;
    }

    lst->clear();

    Renderer->updateGL();
}

void MainWindow::RenderAll()
{
    Renderer->SetMode(Chunk::OPEN_GL_RENDER);

    this->ui->actionRender_Caves->setChecked(false);
    this->ui->actionRender_Top->setChecked(false);
    this->ui->actionRender_All->setChecked(true);
    this->ui->actionRenderCavesLightmap->setChecked(false);
    this->ui->actionRenderAllTextured->setChecked(false);
    this->ui->actionRenderCavesTextured->setChecked(false);
    this->ui->actionRenderTopTextured->setChecked(false);
}

void MainWindow::RenderTop()
{
    Renderer->SetMode(Chunk::OPEN_GL_RENDER_TOP);

    this->ui->actionRender_All->setChecked(false);
    this->ui->actionRender_Caves->setChecked(false);
    this->ui->actionRender_Top->setChecked(true);
    this->ui->actionRenderCavesLightmap->setChecked(false);
    this->ui->actionRenderAllTextured->setChecked(false);
    this->ui->actionRenderCavesTextured->setChecked(false);
    this->ui->actionRenderTopTextured->setChecked(false);
}

void MainWindow::RenderCaves()
{
    Renderer->SetMode(Chunk::OPEN_GL_RENDER_CAVES);

    this->ui->actionRender_All->setChecked(false);
    this->ui->actionRender_Top->setChecked(false);
    this->ui->actionRender_Caves->setChecked(true);
    this->ui->actionRenderCavesLightmap->setChecked(false);
    this->ui->actionRenderAllTextured->setChecked(false);
    this->ui->actionRenderCavesTextured->setChecked(false);
    this->ui->actionRenderTopTextured->setChecked(false);
}

void MainWindow::RenderCavesLightmap()
{
    Renderer->SetMode(Chunk::OPEN_GL_RENDER_CAVES_LIGHT);

    this->ui->actionRenderCavesLightmap->setChecked(true);
    this->ui->actionRender_All->setChecked(false);
    this->ui->actionRender_Top->setChecked(false);
    this->ui->actionRender_Caves->setChecked(false);
    this->ui->actionRenderAllTextured->setChecked(false);
    this->ui->actionRenderCavesTextured->setChecked(false);
    this->ui->actionRenderTopTextured->setChecked(false);
}

void MainWindow::RenderAllTex()
{
    Renderer->SetMode(Chunk::OPEN_GL_RENDER_TEX);

    this->ui->actionRender_Caves->setChecked(false);
    this->ui->actionRender_Top->setChecked(false);
    this->ui->actionRender_All->setChecked(false);
    this->ui->actionRenderCavesLightmap->setChecked(false);
    this->ui->actionRenderAllTextured->setChecked(true);
    this->ui->actionRenderCavesTextured->setChecked(false);
    this->ui->actionRenderTopTextured->setChecked(false);
}

void MainWindow::RenderTopTex()
{
    Renderer->SetMode(Chunk::OPEN_GL_RENDER_TOP_TEX);

    this->ui->actionRender_Caves->setChecked(false);
    this->ui->actionRender_Top->setChecked(false);
    this->ui->actionRender_All->setChecked(false);
    this->ui->actionRenderCavesLightmap->setChecked(false);
    this->ui->actionRenderAllTextured->setChecked(false);
    this->ui->actionRenderCavesTextured->setChecked(false);
    this->ui->actionRenderTopTextured->setChecked(true);
}

void MainWindow::RenderCavesTex()
{
    Renderer->SetMode(Chunk::OPEN_GL_RENDER_CAVES_TEX);

    this->ui->actionRender_Caves->setChecked(false);
    this->ui->actionRender_Top->setChecked(false);
    this->ui->actionRender_All->setChecked(false);
    this->ui->actionRenderCavesLightmap->setChecked(false);
    this->ui->actionRenderAllTextured->setChecked(false);
    this->ui->actionRenderCavesTextured->setChecked(true);
    this->ui->actionRenderTopTextured->setChecked(false);
}

void MainWindow::RotateMode()
{
    this->ui->actionRotate->setChecked(true);
    this->ui->actionMove->setChecked(false);
}

void MainWindow::MoveMode()
{
    this->ui->actionMove->setChecked(true);
    this->ui->actionRotate->setChecked(false);
}

void MainWindow::TopDownView()
{
    this->ui->actionTopDown->setChecked(true);
    this->ui->actionFirstPerson->setChecked(false);
}

void MainWindow::FirstPersonView()
{
    this->ui->actionTopDown->setChecked(false);
    this->ui->actionFirstPerson->setChecked(true);
}

void MainWindow::SelectChunk()
{
    if(folderDialog->exec())
    {
        QString filename = folderDialog->selectedFiles().at(0);

        QDir dir(filename);

        if(dir.exists("World.png"))
        {
            chunkSelection->SetFilename(filename);

            chunkSelection->show();
        }
        else
        {
            QMessageBox::information(this, "No Map Image Found", "No \"World.png\" image found in \"" + filename + "\".\n Use QtMapRenderer to generate an image in the map folder.", QMessageBox::Ok);
        }
    }
}

void MainWindow::ChunkSelected(void *chunk)
{
    Chunk* c = (Chunk*)chunk;

    Renderer->Center.x = c->x_pos;
    Renderer->Center.y = c->z_pos;

    Renderer->updateGL();
}

void MainWindow::keyPressEvent(QKeyEvent *k)
{
    Renderer->keyPress(k);
}
