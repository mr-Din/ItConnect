#pragma once
#include <QWidget>

class User
{
public:
    User(int id, QString login, QString password, QString email, QString description, QString type = "", QString photo = "", int project_id = 0);
public:
    int getId() const;
    QString getLogin() const;
    QString getPassword() const;
    QString getEmail() const;
    QString getDescription() const;
    QString getType() const;
    QString getPhotoPath() const;
    int getProjectId() const;
    void setProjectId(int id);

private:
    int m_id;
    QString m_login;
    QString m_password;
    QString m_email;
    QString m_description;
    QString m_type;
    QString m_photo;
    int m_project_id;
};

