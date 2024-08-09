#pragma once

#include "Project.h"
#include "User.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

class Ui_WgtProject;

using shrd_map_skills = std::shared_ptr<std::map<int, QString>>;

class WgtProject : public QWidget
{
    Q_OBJECT
    const static int PHOTO_WIDTH = 200;
public:
    explicit WgtProject(std::shared_ptr<Project> proj, shrd_map_skills skills, bool is_account = false, QWidget *parent = nullptr);
    ~WgtProject();

private:
    void fillUi();
    void fillSkills();
    void updSkills();
    void setupSigSlot();
    void setPhoto(const QString &path_to_photo);
    void clearLayout(QWidget *wgt);
    void addSkillWgt(const QString& skill, QVBoxLayout* lt);
    void delSkillFromDB(const QString& id);

private slots:
    void onFillSkillsOnEdit();
    void onDelProjectConfirm(int pg);
    void onDelProject();
    void onEdit();
    void onReject();
    void onDone();
    void onChangePhoto();
    void onDelSkill();
    void onOpenDlgSelSkills();
    void onAddNewSkills();
    void onAddNewSkillsToBD();
    void onClearAddedSkills();

signals:
    void sigDelProject();

private:
    std::unique_ptr<Ui_WgtProject> ui;
    std::shared_ptr<Project> m_project;
    std::map<QString, QString> m_skills_ids;    // текущие скиллы, добавленные из бд
    bool m_is_account;
    QScrollArea* m_sa;
    shrd_map_skills m_all_skills;               // все скиллы
    std::vector<QString> m_del_buff_skills;     // буфер удалённых скиллов
    std::vector<QString> m_added_skills;        // скиллы добавленные из диалога выбора скиллов
};

