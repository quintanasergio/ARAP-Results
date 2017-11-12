#include "animation.h"
#include "ui_animation.h"
#include "dialogtable.h"
#include "ant.h"
#include "node.h"
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QGraphicsItemAnimation>

animation::animation(QString Exp, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::animation)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);

    ui->graphicsView->setRenderHints(QPainter::Antialiasing);

    scene->setSceneRect(0, 0, 800, 600);

    experimento = Exp;

    QLinearGradient gradient(0, 0, 50, 200);
    gradient.setColorAt(0.0, Qt::green);
    gradient.setColorAt(0.5, Qt::yellow);
    gradient.setColorAt(1.0, Qt::red);

    scene2 = new QGraphicsScene(this);

    ui->graphicsView_2->setScene(scene2);
    ui->graphicsView_2->setRenderHints(QPainter::Antialiasing);
    scene2->setSceneRect(0, 0, 100, 200);

    QGraphicsRectItem *test = new QGraphicsRectItem(0,0,50,200);
    test->setBrush(gradient);
    scene2->addItem(test);

    //numero de nodos (se lee de parametros)
    int nodecount;

    //tiempo de ejecución (se lee de parametros)
    double tEjecucion;

    //Busca el txt de parametros
    QDirIterator it(experimento, QStringList() << "*.txt", QDir::Files, QDirIterator::Subdirectories);
    QString fileexp = it.next();

    //abre archivo de parametros para buscar numero de nodos y tiempo de ejecucion
    QFile file(fileexp);
    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "El archivo no existe";
    } else {
        //Create a thread to retrieve data from a file
        QTextStream in(&file);
        //Reads the data up to the end of file
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if(line.contains("nodos")&& !line.contains("#")){
                nodecount = line.split(' ').last().toInt();
            }

            if(line.contains("tiempo-detener-simulador")&& !line.contains("#")){
                tEjecucion = line.split(' ').last().toDouble();
            }

        }
    }


    // Seed the random generator with current time
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    //crea los nodos, los agrega a la escena y los posiciona
    for (int i = 0; i < nodecount; i++){

        int StartX = 400;
        int StartY = 300;

        node *nodo = new node(StartX, StartY, i+1);

        scene->addItem(nodo);

        while(!scene->itemAt(StartX,StartY,QTransform())->collidingItems(Qt::IntersectsItemShape).isEmpty()){

            StartX = (qrand() % 750);
            StartY = (qrand() % 550);
            nodo->relocate(StartX, StartY);
        }
    }


    int semilla;
    int ejecucion;

    QString aux;

    //Escoje el primer archivo de path y extrae semilla y num ejecucion del nombre de archivo
    QDirIterator it2(experimento, QStringList() << "*.csv", QDir::Files, QDirIterator::Subdirectories);
    QString filep = it2.next();
    int sizeaux = filep.split('_').size();
    semilla = filep.split('_')[sizeaux-2].toInt();
    aux = filep.split('_')[sizeaux-1];
    ejecucion = aux.split('.').first().toInt();

    //error reading file
    QFile file2(filep);
    if (!file2.open(QIODevice::ReadOnly)) {
        qDebug() << file2.errorString();
    }

    //lee camino del archivo
    QStringList pathList;
    QString lastLine;
    while (!file2.atEnd()) {
        QByteArray line = file2.readLine();
        lastLine.clear();
        lastLine.append(line);
        lastLine.remove("\n");
    }
    //guardo lista de nodos
    pathList = lastLine.split(',');
    //remuevo el tiempo de ocurrencia
    pathList.removeFirst();

    QList<int> intPath;
    //guarda los nodos del camino
    for( int i=0; i<pathList.count(); ++i )
    {
        QString aux = pathList[i].split('.')[2];
        intPath.append( aux.toInt() );
    }

    hormiga = new ant(semilla,ejecucion, intPath, tEjecucion);
    hormiga->setExperimento(experimento);

    scene->addItem(hormiga);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), scene, SLOT(advance()));
    timer->start(100);

    int t_anim = 20000;

    //Animación de la hormiga
   QTimeLine *timer2 = new QTimeLine(t_anim);
   timer2->setFrameRange(0, 1000);
   QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
   animation->setItem(hormiga);
   animation->setTimeLine(timer2);

   hormiga->makePointList();

   for(int i = 0; i < intPath.size() ; i++){
       QPointF uno = hormiga->puntos[i];
       qreal frame = (float)(2*i)/(float)(2*intPath.size()-1);
       animation->setPosAt( frame, uno);

       frame = (float)(2*i+1)/(float)(2*intPath.size()-1);
       animation->setPosAt( frame, uno);
    }


   timer2->start();

   QTimer::singleShot(t_anim, this, SLOT(returnPath()));
}

animation::~animation()
{
    delete ui;
}

void animation::returnPath()
{
    hormiga->removeLines();

    hormiga->preparePath();

    QTimeLine *timer2 = new QTimeLine(10000);
    timer2->setFrameRange(0, 1000);
    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
    animation->setItem(hormiga);
    animation->setTimeLine(timer2);

    for(int i = 0; i < hormiga->puntos.size() ; i++){
        QPointF uno = hormiga->puntos[hormiga->puntos.size()-i-1];
        qreal frame = (float)(i)/(float)(hormiga->puntos.size()-1);
        animation->setPosAt( frame, uno);
     }
    timer2->start();

}

void animation::on_pushButton_clicked()
{
    DialogTable *one = new DialogTable();
    one->setWindowTitle("Tabla de Probabilidad");
    one->setDir(hormiga->getTablePath());
    one->show();
}

void animation::setExperimento(const QString &value)
{
    experimento = value;
}
