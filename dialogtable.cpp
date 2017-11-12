#include "dialogtable.h"
#include "ui_dialogtable.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>

DialogTable::DialogTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTable)
{
    ui->setupUi(this);

    // Create a data model for the mapping table from a CSV file
    csvModel = new QStandardItemModel(this);

    ui->tableView->setModel(csvModel);
}

DialogTable::~DialogTable()
{
    delete ui;
}


void DialogTable::createTable(QString csvfile){

    QFile file(csvfile);
    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "El archivo no existe";
    } else {
        QTextStream in(&file);
        //Lee hasta el final
        while (!in.atEnd())
        {
            QString line = in.readLine();
            // Adding to the model in line with the elements
            QList<QStandardItem *> standardItemsList;
            // consider that the line separated by semicolons into columns
            for (QString item : line.split(",")) {
                standardItemsList.append(new QStandardItem(item));
            }
            csvModel->insertRow(csvModel->rowCount(), standardItemsList);
        }
        file.close();
    }

}

void DialogTable::setDir(QString dir)
{
    createTable(dir);
}

void DialogTable::on_pushButton_clicked()
{
    done(0);

}
