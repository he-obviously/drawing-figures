#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QMouseEvent>
#include <QPoint>
#include <QPointF>
#include <fstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

qreal sep = 40;
QPointF start(600, 300);
QRgb incorrect = qRgb(0, 0, 0);
QRgb correct = qRgb(255, 255, 255);

QPointF new_pixel(int x, int y ) {
    x = x - start.x();
    y = y - start.y();
    return {x / sep, y / sep};
}

bool under_line_1(QPointF a) {
    qreal x = a.x();
    qreal y = a.y();
    return y <= -0.25 * x + 3.25;
}

bool above_line_2(QPointF a) {
    qreal x = a.x();
    qreal y = a.y();
    return y >= (x * 1.0) / 3 - 5.0 / 3;
}

bool in_par_1(QPointF a) {
    qreal x = a.x();
    qreal y = a.y();
    return y <= -1.5 * (x * x) + 12 * x - 21;
}

bool out_of_par_2(QPointF a) {
    qreal x = a.x();
    qreal y = a.y();
    return y >= -1.5 * (x * x) - 3 * x - 2;
}

bool out_of_rectangle(QPointF a) {
    qreal x = a.x();
    qreal y = a.y();
    return y >= -0.5 || y <= -1 || x >= -3.5;
}

bool out_of_rhombus(QPointF a) {
    qreal x = a.x();
    qreal y = a.y();
    bool first = y >= 2* x + 1;
    bool second = y >= -2 * x + 5;
    bool third = y <= -2 * x + 3;
    bool fourth = y <= 2 * x - 1;
    return first || second || third || fourth;
}

bool in_circle(QPointF a) {
    qreal x = a.x();
    qreal y = a.y();
    return (x - 3.5) * (x - 3.5) + (y + 5) * (y + 5) <= 6.25;
}

bool above_turned_par(QPointF a) {
    qreal x = a.x();
    qreal y = a.y();
    return (y + 5.5) * (y + 5.5) >= x + 0.5;
}

bool ones(QPointF a) {
    qreal x = a.x();
    qreal y = a.y();
    return (x == int(x) && (y - int(y) < 0.25 || y - int(y) > 0.75)) || (y == int(y) && (x - int(x) < 0.25 || x - int(x) > 0.75));
}

bool segment_1(QPointF a) {
    qreal x = a.x();
    qreal y = a.y();
    return under_line_1(a) && above_line_2(a) && out_of_rhombus(a) && out_of_rectangle(a) && out_of_par_2(a) && !(in_par_1(a));
}

bool segment_2(QPointF a) {
    qreal x = a.x();
    qreal y = a.y();
    return in_par_1(a) && !(under_line_1(a));
}

bool segment_3(QPointF a) {
    qreal x = a.x();
    qreal y = a.y();
    return under_line_1(a) && !(in_par_1(a)) && above_turned_par(a);
}

bool segment_4(QPointF a) {
    qreal x = a.x();
    qreal y = a.y();
    return in_par_1(a) && in_circle(a) && above_turned_par(a);
}

bool segment_5(QPointF a) {
    qreal x = a.x();
    qreal y = a.y();
    return out_of_par_2(a) && !(in_par_1(a)) && !(in_circle(a)) && !(above_line_2(a)) && above_turned_par(a);
}


bool Check(QPointF a) {
    qreal x = a.x();
    qreal y = a.y();
    return segment_1(a) || segment_2(a) || segment_3(a) || segment_4(a) || segment_5(a) || ones(a);
}

void MainWindow::Draw() {
    int W = ui->label->width();
    int H = ui->label->height();
    QImage img(W, H, QImage::Format_RGB32);
    for (int x = 0; x < W; x++) {
        for (int y = 0; y < H; y++) {
            QPointF now = new_pixel(x, y);
            img.setPixel(x, y, Check(now) ? correct : incorrect);
            if ((x - start.x()) * (x - start.x()) + (y - start.y()) * (y - start.y()) <= 25) {
                img.setPixel(x, y, qRgb(255, 0, 0));
            }
            if (abs(x - start.x()) < 3) {
                img.setPixel(x, y, qRgb(255, 0, 0));
            }
            if (abs(y - start.y()) < 3) {
                img.setPixel(x, y, qRgb(255, 0, 0));
            }
        }
    }
    for (int x = start.x(); x < W; x += sep) {
        for (int y = 0; y < H; y++) {
            img.setPixel(x, y, qRgb(255, 0, 0));
        }
    }
    for (int x = start.x(); x >= 0; x -= sep) {
        for (int y = 0; y < H; y++) {
            img.setPixel(x, y, qRgb(255, 0, 0));
        }
    }
    for (int y = start.y(); y < H; y += sep) {
        for (int x = 0; x < W; x++) {
            img.setPixel(x, y, qRgb(255, 0, 0));
        }
    }
    for (int y = start.y(); y >= 0; y -= sep) {
        for (int x = 0; x < W; x++) {
            img.setPixel(x, y, qRgb(255, 0, 0));
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::wheelEvent(QWheelEvent *event) {
    qreal alpha = (event->delta() > 0) ? 1.1 : 0.9;
    sep *= alpha;
    QPointF pos = event->posF();
    start.setX((start.x() - pos.x()) * alpha + pos.x());
    start.setY((start.y() - pos.y()) * alpha + pos.y());
    Draw();
}

QPointF posStart;

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton) {
        posStart = QCursor::pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton) {
        QPointF posCur = QCursor::pos();
        start.setX(start.x() + posCur.x() - posStart.x());
        start.setY(start.y() + posCur.y() - posStart.y());
        posStart = QCursor::pos();
        Draw();
    }
}
