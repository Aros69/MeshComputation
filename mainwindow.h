#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void onWireframe();
    void onPlain();
    void randomFlip();
    void randomFHighlight();
    void unmarkAll();
    void circulate();
    void naiveInsert();
    void toggleVoronoi();
    void computeVoronoi();
    void delaunize();
    void delaunayInsert();
    void highLightFace();
};

#endif // MAINWINDOW_H
