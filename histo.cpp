#include "histo.h"
#include <QPainter>

Histo::Histo(QWidget *obj)
{

}



void Histo::paintEvent(QPaintEvent * )
{
     QPainter painter(this);


     int nejec =23;
     int histo[nejec];

     for(int i = 0; i<nejec; i++){
         histo[i]= nejec-i;
     }

     int maxx =500;
     int maxy =250;
     int max =0;

     for(int i=0;i<nejec;i++){
         if(max<histo[i]){
             max = histo[i];
            }
     }

     painter.setPen(QColor(100,100,100));
     painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
     painter.drawLine(50,50,50,maxy);
     painter.drawLine(50,maxy,maxx,maxy);

     int maxw = maxx - 50 - ((maxx-50)%nejec);
     painter.drawText(25, 50, "RTT");
     painter.drawText(500, 260, "Ejecuciones");


     for(int i = 0; i<nejec; i++){
         painter.drawRect(50+(i)*(maxw)/nejec, maxy-((maxy-50)*histo[i])/max, (maxw)/nejec, ((maxy-50)*histo[i])/max);
//         painter.drawLine(50+(i+1)*(maxx-50)/nejec,maxy,50+(i+1)*(maxx-50)/nejec,maxy-((maxy-50)*histo[i])/max);
     }

}

