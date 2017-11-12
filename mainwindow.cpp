#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "animation.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QFile>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing);
    ui->graphicsView->setFixedSize(800,400);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene->setSceneRect(0, 0, 800, 400);

    int cantejec =0;

    args= qApp->arguments();

    //recibimos el nombre de experimento como parámetro
    QString nombreExperimento = args[1];

    QString arapFolder = "/home/sergio/Documentos/simulador-arap-v1.0-master/arap-dir/Experiments";

    expFolder = arapFolder + "/" + nombreExperimento;

    lista = new QStringList();
    //lectura de archivos de modelos de cada experimento
    QDirIterator it(expFolder, QStringList() << "explorer*.csv", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()){
        lista->append(it.next());
    }

    this->setWindowTitle("ARAP Results "+nombreExperimento);

    cantejec=lista->size();
    float histo[cantejec];


    //Extraer RTT de cada archivo para altura de histograma
    for(int i = 0; i<cantejec; i++){

        QString dir;
        dir = lista->at(i);
        QFile file(dir);
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
                    histo[i] = line.split(',').last().toFloat();
            }
            file.close();
        }
    }

    int maxx =700; //Tamaño máximo en X del histograma
    int maxy =300; //Tamaño máximo en Y del histograma
    float max =0; //Máximo valor entre los RTT

    scene->addLine(50,40,50,maxy);
    scene->addLine(50,maxy,maxx+10,maxy);

    QGraphicsTextItem *text1 = scene->addText("RTT(seg)");
    text1->setPos(20,20);//posicion del texto

    QGraphicsTextItem *text2 = scene->addText("Ejecución");
    text2->setPos(maxx+10,maxy);//posicion del texto


    //Elije el máximo valor de RTT dentro de histo
    for(int i=0;i<cantejec;i++){
        if(max<histo[i]){
            max = histo[i];
           }
    }

    QString maxs = QString::number(max);
    maxs.truncate(5);
    QGraphicsTextItem *textmax = scene->addText(maxs);
    textmax->setPos(5,40);//posicion del texto
    QGraphicsTextItem *textcero = scene->addText("0.0");
    textcero->setPos(15,maxy-20);//posicion del texto

    int maxw = maxx - 50 - ((maxx-50)%cantejec);

    //creación de las barras del histograma
    for(int i = 0; i<cantejec; i++){
    barra * bar = new barra((maxw)/cantejec, ((maxy-50)*histo[i])/max, lista->at(i));
        scene->addItem(bar);
        bar->setPos(50+(i)*(maxw)/cantejec, maxy-((maxy-50)*histo[i])/max);
    }

}




MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_2_clicked()
{
    exit(0);
}

void MainWindow::on_pushButton_clicked()
{

    animation *anim = new animation(expFolder);
    anim->setWindowTitle("Enrutado de hormiga de carga");
    anim->show();

}

QString MainWindow::getExperimento() const
{
    return expFolder;
}

void MainWindow::on_pushButton_3_clicked()
{
    QProcess P;
    QString programPath = "/home/sergio/Proyectos/build-arapCompare-Desktop_Qt_5_7_0_GCC_64bit-Debug/arapCompare";
    P.start(programPath);
    exit(0);
}
