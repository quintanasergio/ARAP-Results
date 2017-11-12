#ifndef DIALOGTABLE_H
#define DIALOGTABLE_H

#include <QDialog>
#include <QStandardItemModel>


namespace Ui {
class DialogTable;
}

class DialogTable : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTable(QWidget *parent = 0);
    void createTable(QString csvfile);
    void setDir(QString dir);
    ~DialogTable();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogTable *ui;
    QStandardItemModel *csvModel;

};

#endif // DIALOGTABLE_H
