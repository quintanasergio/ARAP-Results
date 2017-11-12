#ifndef NODE_H
#define NODE_H

#include<QPainter>
#include<QGraphicsItem>
#include<QGraphicsScene>

class node : public QGraphicsItem
{
public:
    node(int StartX, int StartY, int id);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void relocate(int xval, int yval);
    int ID;


protected:
    void advance(int phase);

private:
    void DoCollision();
};

#endif // NODE_H
