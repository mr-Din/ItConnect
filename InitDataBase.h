#pragma once

#include <QtSql>

inline QVariant addUser(QSqlQuery &q, const QString &login, const QString &password, const QString &email,
             const QString &description, const QString &type, const QVariant &project_id)
{
    q.addBindValue(login);
    q.addBindValue(password);
    q.addBindValue(email);
    q.addBindValue(description);
    q.addBindValue(type);
    q.addBindValue(project_id);
    q.exec();
    return q.lastInsertId();
}

inline QVariant addProject(QSqlQuery &q, const QString &title, const QString &description, const QVariant &manager_id)
{
    q.addBindValue(title);
    q.addBindValue(description);
    q.addBindValue(manager_id);
    q.exec();
    return q.lastInsertId();
}

inline void addSkillsToProject(QSqlQuery &q, const QVariant &project_id, const QVariant &skill_id)
{
    q.addBindValue(project_id);
    q.addBindValue(skill_id);
    q.exec();
}

inline QVariant addSkill(QSqlQuery &q, const QString &title)
{
    q.addBindValue(title);
    q.exec();
    return q.lastInsertId();
}

inline void updateUser(QSqlQuery &q, const QString &login, const QString &description, const QVariant &id)
{
    q.bindValue(":login", login);
    q.bindValue(":description", description);
    q.bindValue(":id", id);
    q.exec();
}

inline void updateUserProject(QSqlQuery &q, const QVariant &id, const QVariant &project_id)
{
    q.bindValue(":id", id);
    q.bindValue(":project_id", project_id);
    q.exec();
}

inline void updateProject(QSqlQuery &q, const QString &title, /*const QString &email,*/ const QString &description, const QVariant &id)
{
    q.bindValue(":title", title);
//    q.bindValue(":email", email);
    q.bindValue(":description", description);
    q.bindValue(":id", id);
    q.exec();
}

inline void deleteProject(QSqlQuery &q, const QVariant &id)
{
    q.bindValue(":id", id);
    q.exec();
}

const auto USER_SQL = QLatin1String(R"(
    create table users(id integer primary key, login varchar, password varchar,
                       email varchar, description varchar, type varchar, project_id integer)
    )");

const auto PROJECT_SQL = QLatin1String(R"(
    create table projects(id integer primary key, title varchar, description varchar,
                        manager_id integer)
    )");

const auto SKILL_SQL =  QLatin1String(R"(
    create table skills(id integer primary key, title varchar)
    )");

const auto SKILLS_TO_PROJ_SQL =  QLatin1String(R"(
    create table skills_to_project(id integer primary key, project_id integer, skill_id integer)
    )");



const auto INSERT_USER_SQL = QLatin1String(R"(
    insert into users(login, password, email, description, type, project_id) values(?, ?, ?, ?, ?, ?)
    )");

const auto INSERT_PROJECT_SQL = QLatin1String(R"(
    insert into projects(title, description, manager_id)
                      values(?, ?, ?)
    )");

const auto INSERT_SKILL_SQL = QLatin1String(R"(
    insert into skills(title) values(?)
    )");

const auto INSERT_SKILLS_TO_PROJ_SQL = QLatin1String(R"(
    insert into skills_to_project(project_id, skill_id) values(?, ?)
    )");

const auto UPDATE_USER_SQL = QLatin1String(R"(
    UPDATE users SET login = :login, description = :description WHERE id = :id
    )");

const auto UPDATE_USER_PROJ_SQL = QLatin1String(R"(
    UPDATE users SET project_id = :project_id WHERE id = :id
    )");

const auto UPDATE_PROJECT_SQL = QLatin1String(R"(
    UPDATE projects SET title = :title, description = :description WHERE id = :id
    )");

const auto DELETE_PROJECT_SQL = QLatin1String(R"(
    DELETE FROM projects WHERE id = :id
    )");

inline QSqlError initDb()
{
#ifdef Q_OS_ANDROID
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/database.db";
#else
    QString path = QCoreApplication::applicationDirPath() + "/database.db";
#endif
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);


    if (!db.open())
        return db.lastError();

    QStringList tables = db.tables();
    if (tables.contains("users", Qt::CaseInsensitive)
        && tables.contains("projects", Qt::CaseInsensitive))
        return QSqlError();

    QSqlQuery q;
    if (!q.exec(USER_SQL))
        return q.lastError();
    if (!q.exec(PROJECT_SQL))
        return q.lastError();
    if (!q.exec(SKILL_SQL))
        return q.lastError();
    if (!q.exec(SKILLS_TO_PROJ_SQL))
        return q.lastError();

    if (!q.prepare(INSERT_SKILL_SQL))
        return q.lastError();
    QVariant skill1 = addSkill(q, QLatin1String("Embedded Developer (C/C++)"));
    QVariant skill2 = addSkill(q, QLatin1String("QA-engineer"));
    QVariant skill3 = addSkill(q, QLatin1String("Backend Developer (Python/Django)"));

    addSkill(q, QLatin1String("Backend Developer (Go)"));
    addSkill(q, QLatin1String("Frontend Developer (React/Vue.js/Angular)"));
    addSkill(q, QLatin1String("Mobile Developer (iOS/Android/Flutter/Kotlin/Swift)"));
    addSkill(q, QLatin1String("UX/UI designer"));
    addSkill(q, QLatin1String("Machine Learning engineer"));
    addSkill(q, QLatin1String("Big Data engineer"));
    addSkill(q, QLatin1String("DevOps-engineer"));
    addSkill(q, QLatin1String("Data Scientist"));
    addSkill(q, QLatin1String("Security engineer"));
    addSkill(q, QLatin1String("Blockchain Developer"));
    addSkill(q, QLatin1String("VR/AR Developer"));
    addSkill(q, QLatin1String("Fullstack Developer (MERN stack)"));


    if (!q.prepare(INSERT_USER_SQL))
        return q.lastError();
    QVariant manager1 = addUser(q, QLatin1String("manager1"), QLatin1String("123"),
                                QLatin1String("manager1@mail.ru"), QLatin1String(""), QLatin1String("manager"), 0);

    QVariant user1 = addUser(q, QLatin1String("user1"), QLatin1String("123"),
                                QLatin1String("user1@mail.ru"), QLatin1String("description..."), QLatin1String("worker"), 0);

    if (!q.prepare(INSERT_PROJECT_SQL))
        return q.lastError();
    QVariant proj1 = addProject(q, QLatin1String("1. Project Phoenix"), QLatin1String("Платформа для автоматизации управления удалёнными командами. Включает функции отслеживания времени, управления задачами и видеоконференций."), manager1);

    if (!q.prepare(INSERT_SKILLS_TO_PROJ_SQL))
        return q.lastError();
    addSkillsToProject(q, proj1, skill1);
    addSkillsToProject(q, proj1, skill2);


    return QSqlError();
}
