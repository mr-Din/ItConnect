#pragma once

#include "Project.h"
#include "User.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QFrame>

class Ui_WgtProject;

using shrd_map_skills = std::shared_ptr<std::map<int, QString>>;

class WgtProject : public QFrame
{
    Q_OBJECT
    const static int PHOTO_WIDTH = 80;
public:
    explicit WgtProject(std::shared_ptr<Project> proj, shrd_map_skills skills, bool is_account = false, bool is_selected = false, QWidget *parent = nullptr);
    ~WgtProject();

public:
    std::shared_ptr<Project> getProject() const;

public slots:
    void onShowProject(int id);

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
    void animate();

signals:
    void sigDelProject(int id_proj);
    void sigSelProject(int id_proj);
    void sigUpdateAll();

private:
    std::unique_ptr<Ui_WgtProject> ui;
    std::shared_ptr<Project> m_project;
    std::map<QString, QString> m_skills_ids;    // текущие скиллы, добавленные из бд
    bool m_is_account;
    bool m_is_selected;
    QScrollArea* m_sa;
    shrd_map_skills m_all_skills;               // все скиллы
    std::vector<QString> m_del_buff_skills;     // буфер удалённых скиллов
    std::vector<QString> m_added_skills;        // скиллы добавленные из диалога выбора скиллов
    QTimer *m_timer;  // Таймер для анимации
    int m_counter;  // Счетчик тиков таймера
    QString m_style;
};

