#include "node.h"
#include<QTime>
#include<QDebug>
#include<typeinfo>

node::node(int StartX, int StartY, int id)
{
    setPos(mapToParent(StartX, StartY));
    ID = id;
    setData(0,ID);
}

QRectF node::boundingRect() const
{
    return QRect(0,0,20,20);
}

void node::paint(QPainter *painter, const QStyleOptionGraphicsItem */* unused */, QWidget */* unused */)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::gray);

    //deteccion de colisiones

    if(scene()->collidingItems(this).isEmpty())
    {
        //no colision
        brush.setColor(QColor(17,152,174));
    }
    else
    {
        //colision
        brush.setColor(QColor(17,152,174));
    }



    painter->setBrush(brush);
    painter->drawRect(rec);
    painter->drawText(rec,Qt::AlignCenter,QString::number(data(0).toInt()));


}

void node::relocate(int xval, int yval)
{
    setPos(xval, yval);
}

void node::advance(int phase)
{
    if(!phase) return;

    update();

    setPos(mapToParent(0,0));
}

void node::DoCollision()
{

}
