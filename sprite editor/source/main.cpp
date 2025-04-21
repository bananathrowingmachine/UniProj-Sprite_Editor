#include "mainwindow.h"
#include "model.h"

#include <QApplication>

int main(int argc, char *argv[]){
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
    QApplication a(argc, argv);
    Model m;
    MainWindow w(&m);
    w.setWindowTitle("Sprite Editor");
    w.show();
    return a.exec();//test branch
}
