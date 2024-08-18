#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "InitDataBase.h"
#include "WgtWorker.h"
#include "DlgSelProject.h"
#include "util.h"


#include <QMessageBox>
#include <QPalette>
#include <QScroller>
#include <QScrollBar>
#include <QGraphicsDropShadowEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui_MainWindow>())
    , m_type_user(QString())
{
    ui->setupUi(this);
    setupSigSlot();
    ui->combo_type->addItems({"Разработчик", "Проект менеджер"});
    fillUi();
    prepareApp();
    setStyle();

    for (auto sa : {ui->sa_account, ui->sa_projects, ui->sa_workers, ui->scrollArea}) {
        QScroller::grabGesture(sa, QScroller::TouchGesture);
    }

    demoEnter();
    utility::addShadowToObj<QLabel*>(this);
    utility::addShadowToObj<QPushButton*>(this);

//    QPalette palette;
//    palette.setBrush(ui->scrollAreaWidgetContents->backgroundRole(),
//                     QBrush(QPixmap(":/icons/main.png").scaled(ui->scrollAreaWidgetContents->size(),
//                                                               Qt::KeepAspectRatioByExpanding,
//                                                               Qt::SmoothTransformation)));
//    ui->scrollAreaWidgetContents->setAutoFillBackground(true);
//    ui->scrollAreaWidgetContents->setPalette(palette);
}

MainWindow::~MainWindow()
{
}

//bool MainWindow::event(QEvent *event)
//{
//    qDebug()<<"event"<<event->type();
//    return QMainWindow::event(event);
//}

void MainWindow::paintEvent(QPaintEvent *event)
{
//    QPixmap pixmap;
//    pixmap.load(":/icons/main.png");
//    QPainter paint(ui->lbl_description);
//    int widWidth = ui->scrollAreaWidgetContents->width();
//    int widHeight = ui->scrollAreaWidgetContents->height();
//    pixmap = pixmap.scaled(widWidth, widHeight, Qt::KeepAspectRatioByExpanding);
//    paint.drawPixmap(0, 0, pixmap);

    QMainWindow::paintEvent(event);  // Вызов базового класса
    update();  // Принудительная перерисовка
}

void MainWindow::setupSigSlot()
{
    connect(ui->btn_reg,      &QPushButton::clicked, this, &MainWindow::registration);
    connect(ui->btn_enter,    &QPushButton::clicked, this, &MainWindow::authentication);
    connect(ui->btn_logout,   &QPushButton::clicked, this, &MainWindow::logout);
    connect(ui->btn_add_proj, &QPushButton::clicked, this, &MainWindow::addNewProject);
}

void MainWindow::fillUi()
{
    if (m_type_user.isEmpty())
    {
        ui->tabWidget->setCurrentIndex(MAIN);
        ui->sa_projects_content->setDisabled(true);
        ui->sa_workers_content->setDisabled(true);
        ui->st_wgt_account->setCurrentIndex(0);
    }
    else if (m_type_user == "worker")
    {
        ui->sa_projects_content->setDisabled(false);
        ui->sa_workers_content->setDisabled(true);
        ui->tabWidget->setCurrentIndex(ACCOUNT);
        ui->st_wgt_account->setCurrentIndex(1);
        ui->btn_add_proj->hide();
    }
    else if (m_type_user == "manager")
    {
        ui->sa_projects_content->setDisabled(true);
        ui->sa_workers_content->setDisabled(false);
        ui->tabWidget->setCurrentIndex(ACCOUNT);
        ui->st_wgt_account->setCurrentIndex(1);
        ui->btn_add_proj->show();
    }
    clearLE();
}

void MainWindow::setStyle()
{
    qDebug() << "Set stylesheet";
//    QFile styleFile(":/files/ItConnect.qss");
    // тёмная тема
    QFile styleFile(":/files/ItConnect_dark.qss");
    styleFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleFile.readAll());
    setStyleSheet(styleSheet);
}

void MainWindow::prepareApp()
{
    initDb();
    fillUsers();
    fillProjects();
    fillSkills();

    fillWgtUsers();
    fillWgtProjects();
}

void MainWindow::fillUsers()
{
    m_workers.clear();
    m_managers.clear();
    QSqlQuery users_query;
    users_query.prepare("SELECT * FROM users");
    if (!users_query.exec())
    {
        qDebug()<<"Error SQL users query!";
        return;
    }
    while(users_query.next())
    {
        int id = users_query.value(0).toInt();
        QString login = users_query.value(1).toString();
        QString password = users_query.value(2).toString();
        QString email = users_query.value(3).toString();
        QString description = users_query.value(4).toString();
        QString type = users_query.value(5).toString();
        int project_id = users_query.value(6).toInt();

        auto user = std::make_shared<User>(id, login, password, email, description, type, project_id);
        if (type == "worker")
        {
            m_workers.emplace_back(user);
        }
        else
        {
            m_managers.emplace_back(user);
        }
    }
}

void MainWindow::fillProjects()
{
    m_projects.clear();
    QSqlQuery proj_query;
    proj_query.prepare("SELECT * FROM projects");
    if (!proj_query.exec())
    {
        qDebug()<<"Error SQL project query!";
        return;
    }
    while(proj_query.next())
    {
        int id = proj_query.value(0).toInt();
        QString title = proj_query.value(1).toString();
        QString description = proj_query.value(2).toString();
        QString status = proj_query.value(3).toString();
        int manager_id = proj_query.value(4).toInt();

        auto proj = std::make_shared<Project>(id, title, description, status, manager_id);
        m_projects.emplace_back(proj);
    }
}

void MainWindow::fillSkills()
{
    m_all_skills = std::make_shared<std::map<int, QString>>();
    QSqlQuery skills_query;
    skills_query.prepare("SELECT * FROM skills");
    if (!skills_query.exec())
    {
        qDebug()<<"Error SQL skills query!";
        return;
    }
    while(skills_query.next())
    {
        int id = skills_query.value(0).toInt();
        QString title = skills_query.value(1).toString();
        (*m_all_skills)[id] = title;
    }
}

void MainWindow::fillWgtUsers()
{
    clearLayout(ui->sa_workers_content);

    QVBoxLayout* lt = new QVBoxLayout();
    for (const auto& user : m_workers)
    {
        auto wgt = new WgtWorker(user, m_projects, false, true);
        connect(wgt, &WgtWorker::sigSelectProject, this, &MainWindow::onDlgSelProject);
        connect(wgt, &WgtWorker::sigShowProject, this, &MainWindow::onShowProject);
//        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        lt->addWidget(wgt);
    }
    lt->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
//    lt->setSpacing(0);
    ui->sa_workers_content->setLayout(lt);
}

void MainWindow::fillWgtProjects()
{
    clearLayout(ui->sa_projects_content);

    QVBoxLayout* lt = new QVBoxLayout();
    for (const auto& proj : m_projects)
    {
        auto wgt = new WgtProject(proj, m_all_skills, false, (m_type_user=="worker" && m_cur_user->getProjectId() == 0));
        lt->addWidget(wgt);
        connect(this, &MainWindow::sigShowProject, wgt, &WgtProject::onShowProject);
        connect(wgt, &WgtProject::sigSelProject, this, [=](int id)
            {
                emit sigUpdCurrentAccount(id);
//                onAddUserToProject(id, m_cur_user->getId());
                QTimer::singleShot(0, this, [=]()
                                   {
                                       onAddUserToProject(id, m_cur_user->getId());
                                   });
                ui->tabWidget->setCurrentIndex(ACCOUNT);
            }, Qt::QueuedConnection);
    }
    lt->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    //    lt->setSpacing(0);
    ui->sa_projects_content->setLayout(lt);
}

void MainWindow::fillAccountPage()
{
    clearLayout(ui->sa_account_content);
    QVBoxLayout* lt = new QVBoxLayout();

    auto wgt = new WgtWorker(m_cur_user, m_projects, true);
    wgt->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    lt->addWidget(wgt);
    connect(wgt, &WgtWorker::sigShowProject, this, &MainWindow::onShowProject);
    connect(wgt, &WgtWorker::sigDelProject, this, [=](){ onAddUserToProject(0, m_cur_user->getId()); }, Qt::QueuedConnection);
    connect(this, &MainWindow::sigUpdCurrentAccount, wgt, &WgtWorker::onUpdProject, Qt::QueuedConnection);
    connect(wgt, &WgtWorker::sigUpdateMain, this, [this]{ fillUsers(); fillWgtUsers(); });

    lt->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));
//    lt->addStretch();
    ui->sa_account_content->setLayout(lt);
    ui->btn_logout->setText("Hello, " + m_cur_user->getLogin() + "\nВыйти");
}

void MainWindow::fillManagerPage()
{
    clearLayout(ui->sa_account_content);
    QVBoxLayout* lt = new QVBoxLayout();

//    lt->addWidget(new WgtWorker(m_cur_user));
    // Заполняем проекты, который привязаны к текущему менеджеру
    for (const auto proj : m_projects)
    {
        if (m_cur_user->getId() == proj->getManagerId())
        {
            auto wgt = new WgtProject(proj, m_all_skills, true, false, ui->sa_account, this);
            connect(wgt, &WgtProject::sigDelProject, this, [this](int id_proj)
                {
                    fillProjects();
                    fillWgtProjects();
                    fillManagerPage();
                    clearProjectFromUsers(id_proj);
                }, Qt::QueuedConnection);

            connect(wgt, &WgtProject::sigUpdateAll, this, [this]
                {
                    fillProjects();
                    fillWgtProjects();
                    fillWgtUsers();
                }, Qt::QueuedConnection);

            lt->addWidget(wgt);
        }
    }

    lt->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    ui->sa_account_content->setLayout(lt);
    ui->btn_logout->setText("Hello, " + m_cur_user->getLogin() + "\nВыйти");
}

std::shared_ptr<User> MainWindow::getUser(const QString &email)
{
    QSqlQuery users_query;
    users_query.prepare("SELECT * FROM users WHERE email = ?");
    users_query.addBindValue(email);
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

        auto user = std::make_shared<User>(id, login, password, email, description, type, project_id);
        return user;
    }
    return nullptr;
}

void MainWindow::clearLayout(QWidget *wgt)
{
    if (auto layout = wgt->layout())
    {
        while (layout->count())
        {
            if (auto child = layout->takeAt(0))
            {
                if (child->widget())
                    delete child->widget();

                delete child;
            }
        }
        delete layout;
    }
}

void MainWindow::clearLE()
{
    for (QLineEdit* le : { ui->le_email, ui->le_email_reg, ui->le_login, ui->le_password, ui->le_password, ui->le_password_confirm, ui->le_password_reg})
        le->setText("");
}

void MainWindow::clearProjectFromUsers(int id_proj)
{
    for (auto& user : m_workers)
    {
        if (user->getProjectId() == id_proj)
        {
            onAddUserToProject(0, user->getId());
        }
    }
}

void MainWindow::demoEnter()
{
    ui->le_email->setText("manager1@mail.ru");
    ui->le_password->setText("123");
    authentication();
}

void MainWindow::authentication()
{
    QString email = ui->le_email->text();
    QString password = ui->le_password->text();
    QString errors;
    int error_count = 0;
    if (email.isEmpty())
        errors.push_back(QString::number(++error_count) + ". Введите почту!\n");
    if (password.isEmpty())
        errors.push_back(QString::number(++error_count) + ". Введите пароль!\n");

    // Ищем пользователя по почте
    auto find_worker_email = std::find_if(m_workers.begin(), m_workers.end(),
                                          [&](const std::shared_ptr<User> user)
                                          {
                                              return user->getEmail() == email && user->getPassword() == password;
                                          });

    auto find_manager_email = std::find_if(m_managers.begin(), m_managers.end(),
                                           [&](const std::shared_ptr<User> user)
                                           {
                                               return user->getEmail() == email && user->getPassword() == password;
                                           });

    if (find_worker_email != m_workers.end())
    {
        m_cur_user = *find_worker_email;
        m_type_user = "worker";
        fillAccountPage();
    }
    else if (find_manager_email != m_managers.end())
    {
        m_cur_user = *find_manager_email;
        m_type_user = "manager";
        fillManagerPage();
    }
    else
    {
        errors.push_back(QString::number(++error_count) + ". Пользователя не существует, либо неверный пароль!\n");
    }

    if (!errors.isEmpty())
    {
        QMessageBox::warning(nullptr, "Предупреждение", errors, QMessageBox::Ok);
        return;
    }

    QMessageBox::information(nullptr, "Авторизация", "Добро пожаловать!", QMessageBox::Ok);

//    m_cur_user = m_workers.back();
    fillWgtUsers();
    fillUi();
    fillWgtProjects();
}

void MainWindow::registration()
{
    QString login = ui->le_login->text();
    QString email = ui->le_email_reg->text();
    QString password = ui->le_password_reg->text();
    QString password_confirm = ui->le_password_confirm->text();
    int type = ui->combo_type->currentIndex();

    auto find_worker_email = std::find_if(m_workers.begin(), m_workers.end(),
                                          [&](const std::shared_ptr<User> user)
                                          {
                                              return user->getEmail() == email;
                                          });

    auto find_manager_email = std::find_if(m_managers.begin(), m_managers.end(),
                                           [&](const std::shared_ptr<User> user)
                                           {
                                               return user->getEmail() == email;
                                           });

    QString errors;
    int error_count = 0;
    if (login.isEmpty())
        errors.push_back(QString::number(++error_count) + ". Логин должен быть заполнен!\n");
    if (email.isEmpty())
        errors.push_back(QString::number(++error_count) + ". Почта должена быть заполнена!\n");
    if (password.isEmpty())
        errors.push_back(QString::number(++error_count) + ". Придумайте пароль!\n");
    if (find_worker_email != m_workers.end() || find_manager_email != m_managers.end())
        errors.push_back(QString::number(++error_count) + ". Пользователь с указанной почтой уже существует!\n");
    if (password != password_confirm)
        errors.push_back(QString::number(++error_count) + ". Пароли не совпадают!\n");
    if (type == -1)
        errors.push_back(QString::number(++error_count) + ". Выберите тип пользователя!\n");

    if (!errors.isEmpty())
    {
        QMessageBox::warning(nullptr, "Предупреждение", errors, QMessageBox::Ok);
        return;
    }

//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    QString path = "D:\\diploma\\IT_Connect\\database\\database.db";
//    db.setDatabaseName(path);

//    if (!db.open())
//        return;

    QSqlQuery q;
    if (!q.prepare(INSERT_USER_SQL))
        q.lastError();
    if (type == 0)
    {
        addUser(q, login, password, email, "empty", "worker", 0);
        /// TODO корректный id
//        auto user = std::make_shared<User>(1, login, password, email, type, 0);
        if (auto user = getUser(email); user)
            m_workers.emplace_back(user);

        QMessageBox::information(nullptr, "Регистрация", "Добро пожаловать!", QMessageBox::Ok);
        m_type_user = "worker";
        m_cur_user = m_workers.back();
        fillWgtUsers();
        fillAccountPage();
    }
    if (type == 1)
    {
        addUser(q, login, password, email, "", "manager", 0);
        if (auto user = getUser(email); user)
            m_managers.emplace_back(user);

        QMessageBox::information(nullptr, "Регистрация", "Добро пожаловать!", QMessageBox::Ok);
        m_type_user = "manager";
        m_cur_user = m_managers.back();
        fillManagerPage();
    }

    fillWgtUsers();
    fillUi();
    fillWgtProjects();
}

void MainWindow::logout()
{
    m_type_user = QString();
    fillUi();
    ui->tabWidget->setCurrentIndex(ACCOUNT);
}

void MainWindow::addNewProject()
{
    // Добавление пустого проекта
    QSqlQuery q;
    if (!q.prepare(INSERT_PROJECT_SQL))
    {
        q.lastError();
    }
    else
    {
        addProject(q, QLatin1String("empty"), QLatin1String("empty"), QLatin1String("active"), m_cur_user->getId());
        fillProjects();
        fillWgtProjects();
        fillManagerPage();
    }
}

void MainWindow::onDlgSelProject(int id_user)
{
    DlgSelProject dlg(m_projects, m_cur_user, this);
    dlg.setGeometry(this->geometry());
    connect(&dlg, &DlgSelProject::sigSelProject, this, [=](int id){ onAddUserToProject(id, id_user); });
    //    connect(&dlg, &DlgSelProject::accepted, this, &WgtWorker::onAddNewSkills);
    //    connect(&dlg, &DlgSelProject::rejected, this, &WgtWorker::onClearAddedSkills);
    if (dlg.exec())
        return;
}

void MainWindow::onAddUserToProject(int id_proj, int id_user)
{

    // Вносим изменения в базу данных!
    QSqlQuery q;
    if (!q.prepare(UPDATE_USER_PROJ_SQL))
        q.lastError();

    updateUserProject(q, id_user, id_proj);

    // Обновить страницу workers
    fillUsers();
    fillWgtUsers();
    fillWgtProjects();
}

void MainWindow::onShowProject(int id)
{
    qDebug() << "show project";
    ui->tabWidget->setCurrentIndex(PROJECTS);
    ui->sa_projects_content->setDisabled(false);


    QList<WgtProject*> widgetList;
    QWidget *container = ui->sa_projects->widget();
    if (container)
    {
        widgetList = container->findChildren<WgtProject*>();
    }
    for (auto wgt : widgetList){
        if (wgt->getProject()->getId() == id)
        {
            // Определяем позицию виджета относительно контейнера
            QPoint widgetPos = wgt->mapTo(ui->sa_projects->widget(), QPoint(0, 0));
            // Обновляем позицию вертикального скролла
            ui->sa_projects->verticalScrollBar()->setValue(widgetPos.y());
        }
    }

    emit sigShowProject(id);
}
