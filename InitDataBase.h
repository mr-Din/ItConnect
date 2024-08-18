#pragma once

#include <QtSql>


const auto USER_SQL = QLatin1String(R"(
    create table users(id integer primary key, login varchar, password varchar,
                       email varchar, description varchar, type varchar, project_id integer)
    )");

const auto PROJECT_SQL = QLatin1String(R"(
    create table projects(id integer primary key, title varchar, description varchar,
                        status varchar, manager_id integer)
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
    insert into projects(title, description, status, manager_id)
                      values(?, ?, ?, ?)
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

inline QVariant addProject(QSqlQuery &q, const QString &title, const QString &description, const QString &status, const QVariant &manager_id)
{
    q.addBindValue(title);
    q.addBindValue(description);
    q.addBindValue(status);
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

inline void deleteSkillsForProject(QSqlQuery &q, const QVariant &project_id)
{
    q.addBindValue(project_id);
    q.exec();
}

inline void deleteProject(QSqlQuery &q, const QVariant &id)
{
    if (q.prepare("DELETE FROM skills_to_project WHERE project_id = :project_id"))
    {
        deleteSkillsForProject(q, id);
    }
    if (q.prepare(DELETE_PROJECT_SQL))
    {
        q.bindValue(":id", id);
        q.exec();
    }
}

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

    QVariant skills[] =
        {
            addSkill(q, QLatin1String("Embedded Developer (C/C++)")),
            addSkill(q, QLatin1String("QA-engineer")),
            addSkill(q, QLatin1String("Backend Developer (Python/Django)")),
            addSkill(q, QLatin1String("Backend Developer (Go)")),
            addSkill(q, QLatin1String("Frontend Developer (React/Vue.js/Angular)")),
            addSkill(q, QLatin1String("Mobile Developer (iOS/Android/Flutter/Kotlin/Swift)")),
            addSkill(q, QLatin1String("UX/UI designer")),
            addSkill(q, QLatin1String("Machine Learning engineer")),
            addSkill(q, QLatin1String("Big Data engineer")),
            addSkill(q, QLatin1String("DevOps-engineer")),
            addSkill(q, QLatin1String("Data Scientist")),
            addSkill(q, QLatin1String("Security engineer")),
            addSkill(q, QLatin1String("Blockchain Developer")),
            addSkill(q, QLatin1String("VR/AR Developer")),
            addSkill(q, QLatin1String("Fullstack Developer (MERN stack)"))
        };


    if (!q.prepare(INSERT_USER_SQL))
        return q.lastError();
    QVariant manager1 = addUser(q, QLatin1String("manager1"), QLatin1String("123"),
                                QLatin1String("manager1@mail.ru"), QLatin1String(""), QLatin1String("manager"), 0);
    QVariant manager2 = addUser(q, QLatin1String("manager2"), QLatin1String("123"),
                                QLatin1String("manager2@mail.ru"), QLatin1String(""), QLatin1String("manager"), 0);
    QVariant manager3 = addUser(q, QLatin1String("manager3"), QLatin1String("123"),
                                QLatin1String("manager3@mail.ru"), QLatin1String(""), QLatin1String("manager"), 0);
    QVariant manager4 = addUser(q, QLatin1String("manager4"), QLatin1String("123"),
                                QLatin1String("manager4@mail.ru"), QLatin1String(""), QLatin1String("manager"), 0);

    QVariant managers[] = { manager1, manager2, manager3, manager4 };
    std::random_device rd;  // Получаем случайное значение от hardware-источника
    std::mt19937 gen(rd()); // Инициализируем генератор случайных чисел
    std::uniform_int_distribution<> distrib(0, 3); // Диапазон [0, 3] для 4-х менеджеров

    QVariant user1 = addUser(q, QLatin1String("user1"), QLatin1String("123"),
                                QLatin1String("user1@mail.ru"), QLatin1String("description..."), QLatin1String("worker"), 0);

    if (!q.prepare(INSERT_PROJECT_SQL))
        return q.lastError();

    QVector<QPair<QLatin1String, QString>> proj_queries
        {
         {QLatin1String("1. Project Phoenix"), QString("Платформа для автоматизации управления удалёнными командами. Включает функции отслеживания времени, управления задачами и видеоконференций.")},
         {QLatin1String("2. AetherConnect"), QString("Система умного дома с использованием IoT для управления бытовыми приборами через мобильное приложение.")},
         {QLatin1String("3. MindMesh"), QString("Платформа для коллективного обучения с элементами геймификации и интеграцией ИИ для персонализированных рекомендаций.")},
         {QLatin1String("4. WaveSecure"), QString("Кибербезопасная система управления корпоративными данными с многофакторной аутентификацией и шифрованием.")},
         {QLatin1String("5. NeoBank"), QString("Виртуальный банк с поддержкой криптовалют, работающий полностью в цифровом пространстве, с возможностью открытия счетов и проведения операций онлайн.")},
         {QLatin1String("6. EcoTrack"), QString("Приложение для отслеживания и снижения углеродного следа пользователя через анализ повседневной активности.")},
         {QLatin1String("7. Vortex AI"), QString("Платформа для анализа больших данных с использованием искусственного интеллекта, предназначенная для предсказательной аналитики в сфере ритейла.")},
         {QLatin1String("8. HealthSync"), QString("Мобильное приложение для мониторинга здоровья с интеграцией с носимыми устройствами и возможностью дистанционного контроля со стороны врачей.")},
         {QLatin1String("9. EventMaster"), QString("Платформа для организации и проведения виртуальных событий и конференций с поддержкой VR/AR.")},
         {QLatin1String("10. SmartFarm"), QString("Система автоматизации сельского хозяйства с использованием IoT и анализа данных для управления урожаем и мониторинга условий в реальном времени.")}
        };

    for (const auto& proj_q : proj_queries)
    {
        int cur_manager = distrib(gen);
        qDebug() << "Current manager: " << cur_manager <<" for project: " << proj_q.first;
        addProject(q, proj_q.first, proj_q.second, "active", managers[cur_manager]);
    }
//    QVariant proj1 = addProject(q, QLatin1String("1. Project Phoenix"), QLatin1String("Платформа для автоматизации управления удалёнными командами. Включает функции отслеживания времени, управления задачами и видеоконференций."), "active", manager1);

    if (!q.prepare(INSERT_SKILLS_TO_PROJ_SQL))
        return q.lastError();

    addSkillsToProject(q, 1, skills[2]);
    addSkillsToProject(q, 1, skills[4]);
    addSkillsToProject(q, 1, skills[9]);

    addSkillsToProject(q, 2, skills[0]);
    addSkillsToProject(q, 2, skills[5]);
    addSkillsToProject(q, 2, skills[6]);

    addSkillsToProject(q, 3, skills[4]);
    addSkillsToProject(q, 3, skills[7]);
    addSkillsToProject(q, 3, skills[10]);

    addSkillsToProject(q, 4, skills[1]);
    addSkillsToProject(q, 4, skills[2]);
    addSkillsToProject(q, 4, skills[11]);

    addSkillsToProject(q, 5, skills[1]);
    addSkillsToProject(q, 5, skills[3]);
    addSkillsToProject(q, 5, skills[5]);
    addSkillsToProject(q, 5, skills[12]);

    addSkillsToProject(q, 6, skills[5]);
    addSkillsToProject(q, 6, skills[6]);
    addSkillsToProject(q, 6, skills[10]);

    addSkillsToProject(q, 7, skills[4]);
    addSkillsToProject(q, 7, skills[7]);
    addSkillsToProject(q, 7, skills[8]);

    addSkillsToProject(q, 8, skills[2]);
    addSkillsToProject(q, 8, skills[5]);
    addSkillsToProject(q, 8, skills[9]);

    addSkillsToProject(q, 9, skills[1]);
    addSkillsToProject(q, 9, skills[9]);
    addSkillsToProject(q, 9, skills[13]);
    addSkillsToProject(q, 9, skills[14]);

    addSkillsToProject(q, 10, skills[0]);
    addSkillsToProject(q, 10, skills[8]);
    addSkillsToProject(q, 10, skills[1]);
    addSkillsToProject(q, 10, skills[10]);

    return QSqlError();
}
