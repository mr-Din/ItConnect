#pragma once

#include "Project.h"
#include "User.h"
#include "WgtProject.h"

#include <QMainWindow>
//#include "InitDataBase.h"

class Ui_MainWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    enum {
        ACCOUNT,
        WORKERS,
        PROJECTS,
        MAIN
    };


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupSigSlot();
    void fillUi();
    void setStyle();
    void prepareApp();
    void fillUsers();
    void fillProjects();
    void fillSkills();
    void fillWgtUsers();
    void fillWgtProjects();
    void fillAccountPage();
    void fillManagerPage();
    std::shared_ptr<User> getUser(const QString &email);
    void clearLayout(QWidget *wgt);
    void clearLE();

private slots:
    void authentication();
    void registration();
    void logout();
    void addNewProject();


private:
    std::unique_ptr<Ui_MainWindow> ui;
    QString m_type_user;
    std::vector<std::shared_ptr<User>> m_workers;
    std::vector<std::shared_ptr<User>> m_managers;
    std::vector<std::shared_ptr<Project>> m_projects;
    std::shared_ptr<User> m_cur_user;
    shrd_map_skills m_all_skills;
};
