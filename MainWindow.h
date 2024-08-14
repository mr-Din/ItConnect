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
        PROJECTS,
        WORKERS,
        MAIN
    };


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
//    bool event(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
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
    void clearProjectFromUsers(int id_proj);

    void demoEnter();

private slots:
    void authentication();
    void registration();
    void logout();
    void addNewProject();

    void onDlgSelProject(int id_user);
    void onAddUserToProject(int id_proj, int id_user);
    void onShowProject(int id);

signals:
    void sigShowProject(int id);
    void sigUpdCurrentAccount(int proj_id);


private:
    std::unique_ptr<Ui_MainWindow> ui;
    QString m_type_user;
    std::vector<std::shared_ptr<User>> m_workers;
    std::vector<std::shared_ptr<User>> m_managers;
    std::vector<std::shared_ptr<Project>> m_projects;
    std::shared_ptr<User> m_cur_user;
    shrd_map_skills m_all_skills;
};
