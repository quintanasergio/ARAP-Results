#include "barra.h"
#include"dialogmodel.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDirIterator>


barra::barra(float w, float h, QString dir)
{
    width = w;
    height = h;
    dirmodel = dir;
    mousehover = false;
    setToolTip("Presione la barra para ver modelo estadístico");

    QFile file(dirmodel);
    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "El archivo no existe";
    } else {
        // Create a thread to retrieve data from a file
        QTextStream in(&file);
        //Reads the data up to the end of file
        while (!in.atEnd())
        {
            QString line = in.readLine();
            // Adding to the model in line with the elements
            if(line.contains("Media de las medias de los tiempos"))
                rtt = line.split(',').last().toFloat();
        }
        file.close();
    }

    setAcceptHoverEvents(true);


}

QRectF barra::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void barra::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();

        QPen pen(Qt::black);
        QBrush brush(QColor(44,91,119));

        if(mousehover==true){ brush.setColor(QColor(17,152,174)); }

        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawRect(rect);

}

void barra::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    mousehover = true;
    QGraphicsItem::hoverEnterEvent(event);
}

void barra::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    mousehover = false;
    QGraphicsItem::hoverLeaveEvent(event);

}

void barra::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
    DialogModel *one = new DialogModel();
    one->setWindowTitle("Modelo Estadístico");
    one->setDir(dirmodel);
    one->show();
}

