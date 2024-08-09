#pragma once

#include "User.h"

#include <QWidget>

class Ui_WgtWorker;


class WgtWorker : public QWidget
{
    Q_OBJECT
    const static int PHOTO_WIDTH = 200;
public:
    explicit WgtWorker(std::shared_ptr<User> user, bool is_editable = false, QWidget *parent = nullptr);
    ~WgtWorker();

private:
    void fillUi();
    void setupSigSlot();
    void setPhoto(const QString &path_to_photo);

private slots:
    void onEdit();
    void onDone();
    void onChangePhoto();

private:
    std::unique_ptr<Ui_WgtWorker> ui;
    std::shared_ptr<User> m_user;
    bool m_is_editable;
};

