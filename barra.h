#ifndef BARRA_H
#define BARRA_H

#include <QPainter>
#include <QGraphicsItem>
#include <QDir>


class barra :public QGraphicsItem
{
public:
    barra(float w, float h, QString dir);
    QRectF boundingRect() const;
    //Override paint
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int width;
    int height;
    int numexe;
    float rtt;
    QString dirmodel;
    bool mousehover;
    
protected:

  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    
};

#endif // BARRA_H
