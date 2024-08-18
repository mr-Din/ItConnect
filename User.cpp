#include "User.h"

User::User(int id, QString login, QString password, QString email, QString description, QString type, QString photo, int project_id)
    : m_id(id)
    , m_login(login)
    , m_password(password)
    , m_email(email)
    , m_description(description)
    , m_type(type)
    , m_photo(photo)
    , m_project_id(project_id)
{

}

int User::getId() const
{
    return m_id;
}

QString User::getLogin() const
{
    return m_login;
}

QString User::getPassword() const
{
    return m_password;
}

QString User::getEmail() const
{
    return m_email;
}

QString User::getDescription() const
{
    return m_description;
}

QString User::getType() const
{
    return m_type;
}

QString User::getPhotoPath() const
{
    return m_photo;
}

int User::getProjectId() const
{
    return m_project_id;
}

void User::setProjectId(int id)
{
    m_project_id = id;
}
