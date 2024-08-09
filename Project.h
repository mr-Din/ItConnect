#pragma once
#include "User.h"

#include <QWidget>

class Project
{
public:
    Project(int id, QString title, QString description, int manager_id);

public:
    QString getTitle() const;
    QString getDescription() const;
    int getManagerId() const;
    int getId() const;
    QString getEmail() const;

private:
    void fillManager();

private:
    int m_id;
    QString m_title;
    QString m_description;
    int m_manager_id;
    std::shared_ptr<User> m_manager;
};

