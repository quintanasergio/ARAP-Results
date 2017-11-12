#include "ant.h"
#include "node.h"
#include "typeinfo"
#include <QDebug>
#include <QDirIterator>
#include <QtMath>
#include <QGraphicsItemAnimation>



ant::ant(int semilla, int ejecucion, QList<int> intPath, float tExe)
{
    seed = semilla;
    exe = ejecucion;
    timeExe = tExe;
    stage = 0;
    contacto = false;
    setPos(mapToParent(2000, 2000));
    camino = intPath;
}



QRectF ant::boundingRect() const
{
    return QRect(5,5,10,10);
}

void ant::paint(QPainter *painter, const QStyleOptionGraphicsItem */* unused */, QWidget */* unused */)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::black);

    //deteccion de colisiones

    if(scene()->collidingItems(this).isEmpty())
    {
        //no colision
        brush.setColor(Qt::black);
        contacto=false;
    }
    else
    {
        //colision
        brush.setColor(QColor(44,91,119));

        //if(contacto==false)
        DoCollision();
    }

    painter->setBrush(brush);
    painter->drawEllipse(rec);
}

QString ant::getTablePath(){


    int StartNode = camino.first();
    QString archivo;
    //Lectura de tabla de probabilidades del nodo origen
    archivo = QString("10.1.") + QString::number(StartNode) + QString(".2_") + QString::number(timeExe) +QString("_")+
            QString::number(seed)+QString("_")+ QString::number(exe) + QString(".csv");


    QDirIterator it(experimento, QStringList() << archivo, QDir::Files, QDirIterator::Subdirectories);
    QString filep = it.next();
    return filep;

}

void ant::drawLines()
{
    removeLines();

    int StartNode = camino.first();
    int FinishNode = camino.last();
    QString archivo;
    //Lectura de tabla de probabilidades del nodo origen
    archivo = QString("10.1.") + QString::number(StartNode) + QString(".2_") + QString::number(timeExe) +QString("_")+
            QString::number(seed)+QString("_")+ QString::number(exe) + QString(".csv");


    QDirIterator it(experimento, QStringList() << archivo, QDir::Files, QDirIterator::Subdirectories);
    QString filep = it.next();


    //error reading file
    QFile file(filep);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }


    //lee probabilidades del camino del archivo
    QStringList oddsList;

    QString lastLine;
    int count=0;
    //Guarda línea de probabilidades de de la tabla, correspondiente al nodo destino

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if(StartNode > FinishNode){
            if(count==FinishNode){lastLine.clear(); lastLine.append(line); lastLine.remove("\n");}
        }else if(StartNode < FinishNode){
            if(count==FinishNode-1){lastLine.clear(); lastLine.append(line); lastLine.remove("\n");}
        }
        count++;
    }


    //guardo lista de nodos
    oddsList = lastLine.split(',');
    //remuevo el tiempo de ocurrencia y suma de probabilidades

    oddsList.removeFirst();

    oddsList.removeLast();


    QList<float> listaProbabilidad;
    //guarda las probabilidades de los nodos del camino
    for( int i=0; i<oddsList.count(); ++i )
    {
        listaProbabilidad.append( oddsList[i].toFloat() );
    }


    /* DIBUJAR LINEAS */



    QList<QGraphicsItem *> objetos = scene()->items();

    //recorre todos los objetos de la escena
    foreach(QGraphicsItem* o,objetos){

        QPointF uno = puntos[stage];
        QPointF dos = o->pos();
        QGraphicsItem* actual = scene()->itemAt(dos, QTransform());

        //indice para lista float / numero de nodo
        int index = actual->data(0).toInt();
        float probability=0.0;

        if(index > 0){

        if(StartNode > index){
            probability = listaProbabilidad[actual->data(0).toInt()-1];
        }else if(StartNode < index){
            probability = listaProbabilidad[actual->data(0).toInt()-2];
        }

        //Poner color basado en la probabilidad usando formula
        QPen pen;
        int nodos = listaProbabilidad.size();

        if(probability > 1.0/nodos){
            int x = 255*(1-probability+(double)(1/nodos));
            pen.setColor(QColor(x,255,0));
        }else {
            int x = (255*probability);
            pen.setColor(QColor(255,x,0));
        }


        QGraphicsLineItem* l = scene()->addLine(uno.rx()+10,uno.ry()+10,dos.rx()+10,dos.ry()+10,pen);
        l->setZValue(-1); //envia las lineas hacia el fondo

        }
    }

}

void ant::removeLines()
{
    //borra lineas

    QList<QGraphicsItem*> scene_items = scene()->items();
    for(int i = 0; i < scene_items.size(); i++){
        if(typeid(*(scene_items[i])) == typeid(QGraphicsLineItem) ){
            delete scene_items[i];
        }
    }

}

void ant::preparePath()
{
    //Dibujar el camino de regreso

    for(int i =1; i < puntos.size(); i++){
    QPointF uno = puntos[i];
    QPointF dos = puntos[i-1];

    QPen pen;

    QGraphicsLineItem* l = scene()->addLine(uno.rx()+10,uno.ry()+10,dos.rx()+10,dos.ry()+10,pen);
    l->setZValue(-1); //envia hacia atras

    }
}

void ant::advance(int phase)
{
    if(!phase) return;
}

void ant::setExperimento(const QString &value)
{
    experimento = value;
}

void ant::setStage(int value)
{
    stage = value;
}

int ant::getStage() const
{
    return stage;
}

void ant::DoCollision()
{
    contacto=true;

    //manejo de camino/colisiones

    QList<QGraphicsItem*> colliding_items = collidingItems();
    for(int i = 0; i < colliding_items.size(); i++){
        //revisar si es un nodo
        if(typeid(*(colliding_items[i])) == typeid(node) && stage<puntos.size()){
            //revisar el ID del nodo y actuar acorde
            if(colliding_items[i]->data(0).toInt() == camino[stage]){
                    drawLines();
                    setStage(stage+1);
                    break;
            }
        }
    }
    //remueve lineas al llegar a ultimo nodo
    if(stage==puntos.size() ){
        removeLines();
        stage++;
    }

    //Caso especial del ultimo salto
    if(stage==puntos.size()-1){

        removeLines();

        QPointF uno = puntos[stage-1];
        QPointF dos = puntos[stage];

        QPen pen;

        QGraphicsLineItem* l = scene()->addLine(uno.rx()+10,uno.ry()+10,dos.rx()+10,dos.ry()+10,pen);
        l->setZValue(-1); //envia las lineas hacia el fondo

        return;
    }
}

void ant::makePointList()
{
    //creacion lista de puntos de nodos en el camino
    QList<QGraphicsItem *> objetos = scene()->items();
    for(int i=0; i < camino.size(); i++){
        foreach(QGraphicsItem* o,objetos){
            if(o->data(0).toInt()==camino[i]){
            puntos.append(o->pos());
            }
        }
    }
}
