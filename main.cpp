#include "MainWindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
//    // Настройка формата для использования OpenGL ES
//    QSurfaceFormat format;
//    format.setRenderableType(QSurfaceFormat::OpenGLES);
//    format.setVersion(3, 0); // Используем OpenGL ES 3.0 (или нужную версию)
//    format.setProfile(QSurfaceFormat::CoreProfile);
//    QSurfaceFormat::setDefaultFormat(format);
    QApplication a(argc, argv);
//    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);

    MainWindow w;
    w.show();

//    qApp->processEvents();
    return a.exec();
}
