#include "Project.h"

#include <QSqlQuery>

Project::Project(int id, QString title, QString description, int manager_id)
    : m_id(id)
    , m_title(title)
    , m_description(description)
    , m_manager_id(manager_id)
{
    fillManager();
}

QString Project::getTitle() const
{
    return m_title;
}

QString Project::getDescription() const
{
    return m_description;
}

int Project::getManagerId() const
{
    return m_manager_id;
}

int Project::getId() const
{
    return m_id;
}

QString Project::getEmail() const
{
    if (m_manager)
        return m_manager->getEmail();

    return "";
}

void Project::fillManager()
{
    QSqlQuery users_query;
    users_query.prepare("SELECT * FROM users WHERE id = ?");
    users_query.addBindValue(m_manager_id);
    if (!users_query.exec())
    {
        qDebug()<<"Error SQL users query!";
    }
    while (users_query.next())
    {
        int id = users_query.value(0).toInt();
        QString login = users_query.value(1).toString();
        QString password = users_query.value(2).toString();
        QString email = users_query.value(3).toString();
        QString description = users_query.value(4).toString();
        QString type = users_query.value(5).toString();
        int project_id = users_query.value(6).toInt();

        m_manager = std::make_shared<User>(id, login, password, email, description, type, project_id);
    }
}
