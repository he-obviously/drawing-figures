#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void Draw();
    QImage *pic;
    Ui::MainWindow *ui;
    void mousePressEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
};

#endif // MAINWINDOW_H
