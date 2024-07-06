#ifndef MINESEARCHER_H
#define MINESEARCHER_H
#include <QGridLayout>
#include <QLabel>
#include <QPair>
#include <QWidget>
#include <QPushButton>

class Field: public QWidget{
    Q_OBJECT
public:
    Field(QWidget* parent = nullptr) ;
    ~Field();

private:
    void fillNums();
    void makeAMoove(const int x, const int y);
    void openAll();
    bool checkAlive();
    void openZero(const int x, const int y);
    void checkWin(bool& isRunning);
    void openAllMines();
    bool eventFilter(QObject* obj, QEvent* event);
    QVector<QPushButton*> buttons;
    QVector<QPair<int, int>> coordOfMineCells;
    bool alive = true;
    int amountOfMines = 10;
    QLabel* minesLabel;
    QLabel* flagsLabel;
    int flagsCount = 0;
    int lastClickTime = 0;
private slots:
    void handleRightClick(int index);

};

#endif // MINESEARCHER_H
