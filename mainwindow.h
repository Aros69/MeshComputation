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
    void flip();
    void randomFHighlight();
    void unmarkAll();
    void circulate();
    void naiveInsert();
    void toggleVoronoi();
    void computeVoronoi();
    void delaunize();
    void delaunayInsert();
    void highLightFace();
    void mergeVertices();
    void crust2D();
    void simplify();
private slots:
    void on_refresh_clicked();
    void on_CrustCurve_toggled(bool checked);
    void on_meshFile_currentIndexChanged(const QString &arg1);
    void on_Change_Mesh_released();
    void on_change_Curve_clicked();
    void on_curveFile_currentIndexChanged(const QString &arg1);
};

#endif // MAINWINDOW_H
