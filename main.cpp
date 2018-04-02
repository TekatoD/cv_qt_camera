#include <iostream>
#include <QtWidgets/QApplication>
#include "cv_camera.h"


int main(int argc, char** argv) {
    QApplication app(argc, argv);
    cv_camera w;
    w.show();
    app.exec();
    return 0;
}