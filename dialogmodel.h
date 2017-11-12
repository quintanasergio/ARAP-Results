#ifndef DIALOGMODEL_H
#define DIALOGMODEL_H

#include <QDialog>
#include <QStandardItemModel>


namespace Ui {
class DialogModel;
}

class DialogModel : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModel(QWidget *parent = 0);
    void createTable(QString csvfile);
    void setDir(QString dir);
    ~DialogModel();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogModel *ui;
    QStandardItemModel *csvModel;
};

#endif // DIALOGMODEL_H
