#include <QCoreApplication>
#include <QFileDialog>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QStandardPaths>
#include <QWidget>

#pragma once

namespace utility {
inline void addShadowCurrentObj(QWidget* wgt)
{
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setBlurRadius(5);
    shadowEffect->setOffset(2, 2);
    shadowEffect->setColor(Qt::black);
    wgt->setGraphicsEffect(shadowEffect);
}

template<typename Obj>
void addShadowToObj (QWidget* parent)
{
    for (auto* child : parent->findChildren<Obj>())
    {
        addShadowCurrentObj(child);
    }
}

inline QString changePhoto(QWidget* wgt, QString name)
{
    QString path_to_photo = QFileDialog::getOpenFileName(wgt, "Выберите изображение", "", "Изображения (*.png *.jpg *.jpeg *.bmp *.gif)");

    if (path_to_photo.isEmpty())
        return "";

    QFileInfo fileInfo(path_to_photo);
    QString fileExtension = fileInfo.suffix();

    QString newFileName = "/" + name + "." + fileExtension;

// Каталог, куда будет сохранено изображение
#ifdef Q_OS_ANDROID
    QString destinationDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
#else
    QString destinationDir = QCoreApplication::applicationDirPath();;
#endif

    QString destinationPath = destinationDir + newFileName;

    if (QFile::exists(destinationPath) && path_to_photo != destinationPath) {
        QFile::remove(destinationPath);
    }
    // Копируем файл
    if (QFile::copy(path_to_photo, destinationPath)) {
        return newFileName;
    } else if (path_to_photo == destinationPath) {
        return newFileName;
    } else {
        QMessageBox::warning(wgt, "Ошибка", "Не удалось скопировать файл.");
        return "";
    }
}
} // utility
