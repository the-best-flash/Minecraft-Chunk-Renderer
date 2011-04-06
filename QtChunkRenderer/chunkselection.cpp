#include "chunkselection.h"
#include "ui_chunkselection.h"

ChunkSelection::ChunkSelection(QString fileName, list<Chunk*>* cLst, GlOptions* glOpts, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChunkSelection),
    lst(cLst),
    filename(fileName),
    opts(glOpts)
{
    ui->setupUi(this);

    this->connect(ui->Cancel, SIGNAL(clicked()), this, SLOT(close()));
    this->connect(ui->Ok, SIGNAL(clicked()), this, SLOT(OkClick()));

    rb = new QRubberBand(QRubberBand::Rectangle, this->ui->pic);
    rb->setVisible(false);

    //if(filename != "")
        //ChangeFilename(filename);
}

ChunkSelection::~ChunkSelection()
{
    delete ui;
}

void ChunkSelection::mousePressEvent(QMouseEvent *event)
{
    int x = event->x() - ui->scrollArea->pos().x() - ui->pic->pos().x() + ui->scrollArea->horizontalScrollBar()->value();
    int y = event->y() - ui->scrollArea->pos().y() - ui->pic->pos().y() + ui->scrollArea->verticalScrollBar()->value();

    rb->setGeometry(x - (x % 16), y - (y % 16), 16, 16);
    rb->setVisible(true);

    click.x = x - (x % 16);
    click.y = y - (y % 16);
}

void ChunkSelection::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x() - ui->scrollArea->pos().x() - ui->pic->pos().x() + ui->scrollArea->horizontalScrollBar()->value();
    int y = event->y() - ui->scrollArea->pos().y() - ui->pic->pos().y() + ui->scrollArea->verticalScrollBar()->value();

    Point<int> p(x, y);

    int w, h;

    if((p.x - click.x) % 16 == 0)
    {
        w = (p.x - click.x);
    }
    else
    {
        if(p.x - click.x > 0)
        {
            w = ((p.x - click.x) - ((p.x - click.x) % 16)) + 16;
        }
        else
        {
            w = (abs(p.x - click.x) - (abs(p.x - click.x) % 16)) + 32;
            w = -w;
        }
    }

    if((p.y - click.y) % 16 == 0)
    {
        h = (p.y - click.y);
    }
    else
    {
        if(p.y - click.y > 0)
        {
            h = ((p.y - click.y) - ((p.y - click.y) % 16)) + 16;
        }
        else
        {
            h = (abs(p.y - click.y) - (abs(p.y - click.y) % 16)) + 32;
            h = -h;
        }
    }

    rb->setGeometry((w > 0? click.x : p.x - (p.x % 16)), (h > 0? click.y : p.y - (p.y % 16)), abs(w), abs(h));
    rb->setVisible(true);
}

void ChunkSelection::mouseReleaseEvent(QMouseEvent *event)
{
    int x = event->x() - ui->scrollArea->pos().x() - ui->pic->pos().x() + ui->scrollArea->horizontalScrollBar()->value();
    int y = event->y() - ui->scrollArea->pos().y() - ui->pic->pos().y() + ui->scrollArea->verticalScrollBar()->value();

    Point<int> p(x, y);

    int w, h;

    if((p.x - click.x) % 16 == 0)
    {
        w = (p.x - click.x);
    }
    else
    {
        if(p.x - click.x > 0)
        {
            w = ((p.x - click.x) - ((p.x - click.x) % 16)) + 16;
        }
        else
        {
            w = (abs(p.x - click.x) - (abs(p.x - click.x) % 16)) + 32;
            w = -w;
        }
    }

    if((p.y - click.y) % 16 == 0)
    {
        h = (p.y - click.y);
    }
    else
    {
        if(p.y - click.y > 0)
        {
            h = ((p.y - click.y) - ((p.y - click.y) % 16)) + 16;
        }
        else
        {
            h = (abs(p.y - click.y) - (abs(p.y - click.y) % 16)) + 32;
            h = -h;
        }
    }

    rb->setGeometry((w > 0? click.x : p.x - (p.x % 16)), (h > 0? click.y : p.y - (p.y % 16)), abs(w), abs(h));
    rb->setVisible(true);
}

void ChunkSelection::OkClick()
{
    if(rb->isVisible())
    {
        Point<int> start, end, offset(min.x, min.z);

        start.x = rb->geometry().x() / 16;
        start.y = (ui->pic->pixmap()->height() - rb->geometry().y()) / 16;

        start += offset;

        start.y = max.z - start.y + min.z + 1;

        end.x = start.x + rb->geometry().width() / 16;
        end.y = start.y + rb->geometry().height() / 16;

        QDir dir(filename);

        Chunk* center = NULL;

        for(int x = start.x; x < end.x; x++)
        {
            char a[20] = { 0 };

            QDir xPath(dir.path());

            xPath.cd(QString(itoa((x > 0? x % 64 : (x + 65536) % 64), a, 36)));

            for(int y = start.y; y < end.y; y++)
            {
                QDir path(xPath.path());

                char b[20] = { 0 };

                path.cd(QString(itoa((y > 0? (y % 64) : ((y + 65536) % 64)), b, 36)));

                char c[20] = { 0 };
                char d[20] = { 0 };

                QString name = QString("c.") + (x < 0? "-" : "") + itoa(abs(x), c, 36) + "." + (y < 0? "-" : "") + itoa(abs(y), d, 36) + ".dat";

                if(path.exists(name))
                {
                    QString chunkName = path.filePath(name);

                    Chunk* ch = new Chunk(opts, chunkName.toStdString());

                    lst->push_back(ch);

                    if(x == (end.x - start.x) / 2 + start.x && y == (end.y - start.y) / 2 + start.y)
                        center = ch;
                }
            }
        }


        if(center != NULL)
            emit SelectChunk((void*)center);
    }

    this->close();
}

void ChunkSelection::SetFilename(QString fileName)
{
    rb->setVisible(false);

    if(fileName != filename)
        ChangeFilename(fileName);
}

void ChunkSelection::ChangeFilename(QString fileName)
{
    filename = fileName;

    QDir dir(filename);

    QPixmap px(dir.filePath("World.png"));

    ui->pic->setPixmap(px);

    ui->pic->setMaximumSize(px.width(), px.height());

    rb->setVisible(false);

    bool first = true;

    list<string> pos1 = GetDirectories(filename.toStdString());

    for(list<string>::iterator it = pos1.begin(); it != pos1.end(); it++)
    {
        list<string> pos2 = GetDirectories(*it);

        for(list<string>::iterator it2 = pos2.begin(); it2 != pos2.end(); it2++)
        {
            list<string> chunks = GetFiles(*it2);

            for(list<string>::iterator f = chunks.begin(); f != chunks.end(); f++)
            {
                Chunk c(NULL, *f);

                if(first)
                {
                    min.x = c.x_pos;
                    min.z = c.z_pos;

                    max.x = c.x_pos;
                    max.z = c.z_pos;

                    first = false;
                }
                else
                {
                    if(c.x_pos < min.x)
                        min.x = c.x_pos;

                    if(c.z_pos < min.z)
                        min.z = c.z_pos;

                    if(c.x_pos > max.x)
                        max.x = c.x_pos;

                    if(c.z_pos > max.z)
                        max.z = c.z_pos;
                }

                c.free();
            }
        }
    }
}
