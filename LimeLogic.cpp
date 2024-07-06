#include "mineSearcher.h"
#include <random>
#include <QDate>
#include <QEvent>
#include <QMouseEvent>
#include <QMessageBox>
using namespace std;

Field::Field(QWidget* parent) : QWidget(parent), lastClickTime(0) {
    setWindowTitle("Saper");
    setFixedSize(400, 450);
    QWidget* centralWidget = new QWidget(this);
    QGridLayout* layout = new QGridLayout(centralWidget);

    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            int index = row * 10 + col;
            QPushButton* button = new QPushButton(this);
            button->setFixedSize(30, 30);
            button->setCheckable(true);
            buttons.push_back(button);
            layout->addWidget(button, col, row);
            button->installEventFilter(this);

        }
    }

    minesLabel = new QLabel(QString("Mines: %1").arg(amountOfMines), this);
    flagsLabel = new QLabel(QString("Flags: %1").arg(amountOfMines), this);

    layout->addWidget(minesLabel, 10, 0, 1, 5);
    layout->addWidget(flagsLabel, 10, 5, 1, 5);

    fillNums();
}

bool Field::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        int index = buttons.indexOf(qobject_cast<QPushButton*>(obj));
        if (mouseEvent->button() == Qt::RightButton) {
            handleRightClick(index);
            return true; // Сообщаем, что событие обработано
        }else if(mouseEvent->button() == Qt::LeftButton){
            int x = index % 10;
            int y = index / 10;
            makeAMoove (x, y);
        }
    }
    return false; // Передаем событие дальше
}

Field::~Field() {
    for (QPushButton* button : buttons) {
        delete button;
    }
    buttons.clear();
    coordOfMineCells.clear();
}

void Field::fillNums() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 99);

    while (coordOfMineCells.size() < amountOfMines) {
        int mineIndex = dist(gen);
        int mineX = mineIndex % 10;
        int mineY = mineIndex / 10;

        bool hasMine = false;
        for (const auto& mine : coordOfMineCells) {
            if (mine.first == mineX && mine.second == mineY) {
                hasMine = true;
                break;
            }
        }

        if (!hasMine) {
            coordOfMineCells.emplace_back(mineX, mineY);
        }
    }
}




void Field::handleRightClick(int index) {
    int x = index % 10;
    int y = index / 10;

    if (!buttons[x + y * 10]->isChecked()) {
        if (buttons[x + y * 10]->icon().isNull()) {
            QIcon icon(R"(C:\Users\legki\source\repos\Minesweaper\flag.jpg)");
            buttons[x + y * 10]->setIcon(icon);
            flagsCount++;
            flagsLabel->setText(QString("Flags: %1").arg(amountOfMines - flagsCount));
            buttons[x + y * 10]->setStyleSheet("background-color: red;");

        } else {
            buttons[x + y * 10]->setIcon(QIcon());
            flagsCount--;
            flagsLabel->setText(QString("Flags: %1").arg(amountOfMines - flagsCount));
        }
    }
}

void Field::makeAMoove(const int x, const int y) {
    if (alive) {
        if (buttons[x + y * 10]->isChecked()) {
            return;
        }
        bool isMine = false;
        for (const auto& mine : coordOfMineCells) {
            if (mine.second == y && mine.first == x) {
                isMine = true;
                break;
            }
        }
        if (isMine) {
            openAllMines();
            alive = false;
            QMessageBox::information(this, "GAME OVER", "You lost!");
        } else {
            qDebug() << x << y;
            openZero(x, y);
            checkWin(alive);
        }
    }
}

bool Field::checkAlive() {
    return alive;
}

void Field::openZero(const int x, const int y) {
    if ((x >= 0) && (x < 10) && (y >= 0) && (y < 10) && !buttons[x + y * 10]->isChecked()) {
        int mineCount = 0;
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (x + i >= 0 && x + i < 10 && y + j >= 0 && y + j < 10) {
                    bool isMine = false;
                    for (const auto& mine : coordOfMineCells) {
                        if (mine.second == y + j && mine.first == x + i) {
                            isMine = true;
                            break;
                        }
                    }
                    if (isMine) {
                        mineCount++;
                    }
                }
            }
        }
        if (mineCount != 0) {
            buttons[x + y * 10]->setText(QString::number(mineCount));
            buttons[x + y * 10]->setChecked(true);
        } else {
            buttons[x + y * 10]->setChecked(true);
        }

        if (mineCount == 0) {
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (x + i >= 0 && x + i < 10 && y + j >= 0 && y + j < 10) {
                        openZero(x + i, y + j);
                    }
                }
            }
        }
    }
}

void Field::openAllMines() {
    for (const auto& mine : coordOfMineCells) {
        QIcon icon(R"(C:\Users\legki\source\repos\Minesweaper\mine.jpg)");
        buttons[mine.first + mine.second * 10]->setIcon(icon);
        buttons[mine.first + mine.second * 10]->setChecked(true);
    }
}

void Field::checkWin(bool& alive) {
    int uncoveredCells = 0;
    int totalCells = 100;

    for (const auto& button : buttons) {
        if (button->isChecked()) {
            uncoveredCells++;
        }
    }

    if (uncoveredCells == totalCells - amountOfMines && flagsCount == amountOfMines) {
        alive = true;
        QMessageBox::information(this, "VICTORY", "Congratulations! You have won the game.");
    }
}
