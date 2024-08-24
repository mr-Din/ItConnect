#include <QCoreApplication>
#include <QFileDialog>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>
#include <QStandardPaths>
#include <QWidget>

#pragma once

namespace utility {
inline void addShadowCurrentObj(QWidget* wgt, double offset = 2, double radius = 10)
{
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setBlurRadius(radius);
    shadowEffect->setOffset(offset, offset);
    shadowEffect->setColor(Qt::black);
    wgt->setGraphicsEffect(shadowEffect);
}

template<typename Obj>
void addShadowToObj (QWidget* parent, double offset = 2, double radius = 10)
{
    for (auto* child : parent->findChildren<Obj>())
    {
        if (child->objectName() != "lbl_photo")
            addShadowCurrentObj(child, offset, radius);
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

 // Функция для создания скругленного изображения
inline QPixmap createRoundedPixmap(const QPixmap &pixmap, int radius)
{
    QPixmap roundedPixmap(pixmap.size());
    roundedPixmap.fill(Qt::transparent);

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(roundedPixmap.rect(), radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, pixmap);

    return roundedPixmap;
}
} // utility
