#ifndef ANIMATION_H
#define ANIMATION_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "ant.h"

namespace Ui {
class animation;
}

class animation : public QDialog
{
    Q_OBJECT

public:
    explicit animation(QString Exp, QWidget *parent = 0);
    ~animation();

    void setExperimento(const QString &value);

private slots:

    void returnPath();

    void on_pushButton_clicked();

private:
    Ui::animation *ui;
    QGraphicsScene *scene;
    QGraphicsScene *scene2;
    QTimer *timer;
    ant *hormiga;
    QString experimento;

};

#endif // ANIMATION_H
