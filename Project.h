#pragma once
#include "User.h"

#include <QWidget>

class Project
{
public:
    Project(int id, QString title, QString description, QString status, QString photo, int manager_id);

public:
    QString getTitle() const;
    QString getDescription() const;
    QString getStatus() const;
    QString getPhotoPath() const;
    int getManagerId() const;
    int getId() const;
    QString getEmail() const;

private:
    void fillManager();

private:
    int m_id;
    QString m_title;
    QString m_description;
    QString m_status;
    QString m_photo;
    int m_manager_id;
    std::shared_ptr<User> m_manager;
};

