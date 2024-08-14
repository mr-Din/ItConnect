#pragma once

#include <QDialog>
#include "ItemSkill.h"
#include "WgtProject.h"

class Ui_DlgSelProject;

class DlgSelProject : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSelProject(std::vector<std::shared_ptr<Project>>& all_project, std::shared_ptr<User> manager, QWidget *parent = nullptr);
    ~DlgSelProject();

private:
    void paintEvent(QPaintEvent *event) override;
    void initWgtsProjects();

//private slots:
//    void onSelProject();

signals:
    void sigSelProject(int id);

private:
    std::unique_ptr<Ui_DlgSelProject> ui;
    std::vector<std::shared_ptr<Project>>& m_all_project;
    std::shared_ptr<User> m_manager;
    std::vector<WgtProject*> m_wgtd_all_project;
};

