#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mineSearcher.h"

using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Saper");
        setFixedSize(400, 450);

        mineField= new Field(this);
        setCentralWidget(mineField);
    }

private:
    Ui::MainWindow *ui;
    Field* mineField;
};
#endif // MAINWINDOW_H
