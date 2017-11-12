#ifndef ANT_H
#define ANT_H

#include<QPainter>
#include<QGraphicsItem>
#include<QGraphicsScene>

class ant : public QGraphicsItem
{
public:
    ant(int semilla, int ejecucion, QList<int> intPath, float tExe);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QList<int> camino;
    void drawLines();
    void removeLines();
    qreal angle;
    QList<QPointF> puntos;
    void preparePath();
    void makePointList();

    int getStage() const;

    void setStage(int value);

    void setExperimento(const QString &value);

    QString getTablePath();
protected:
    void advance(int phase);

private:

    int stage;
    int seed;
    int exe;
    float timeExe;
    bool contacto;
    QString experimento;
    void DoCollision();
};
#endif // ANT_H
