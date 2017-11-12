#ifndef HISTO_H
#define HISTO_H
#include<QWidget>
#include<QPainter>



class Histo : public QWidget
{
Q_OBJECT
private:
    QPainter p;
public:
    Histo(QWidget *parent = 0);



    void paintEvent(QPaintEvent *);
};

#endif // HISTO_H



