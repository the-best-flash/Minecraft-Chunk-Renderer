#ifndef CHUNKSELECTION_H
#define CHUNKSELECTION_H

#include <QDialog>
#include <QRubberBand>
#include <QMouseEvent>
#include <QDir>
#include <QScrollBar>
#include <QMessageBox>

#include "il.h"
#include "point.h"
#include "FileEnumeration.h"
#include "Chunk.h"
#include "gloptions.h"

namespace Ui {
    class ChunkSelection;
}

class ChunkSelection : public QDialog
{
    Q_OBJECT

public:
    explicit ChunkSelection(QString fileName, list<Chunk*>* cLst, GlOptions* glOpts, QWidget *parent = 0);
    ~ChunkSelection();

    void SetFilename(QString filename);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    void ChangeFilename(QString filename);

public slots:
    void OkClick();

signals:
    void SelectChunk(void*);

private:
    Ui::ChunkSelection *ui;

    Point<float> min, max;
    Point<int> click;

    QRubberBand* rb;

    list<Chunk*>* lst;
    QString filename;
    GlOptions* opts;
};

#endif // CHUNKSELECTION_H
