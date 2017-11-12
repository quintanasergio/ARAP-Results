#include "dialogmodel.h"
#include "ui_dialogmodel.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>

DialogModel::DialogModel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogModel)
{
    ui->setupUi(this);

    // Create a data model for the mapping table from a CSV file
    csvModel = new QStandardItemModel(this);

    ui->tableView->setModel(csvModel);

}

DialogModel::~DialogModel()
{
    delete ui;
}

void DialogModel::createTable(QString csvfile){

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

void DialogModel::setDir(QString dir)
{
    createTable(dir);
}

void DialogModel::on_pushButton_clicked()
{
    done(0);
}
