#pragma once

#include <QDialog>
#include "ItemSkill.h"
#include "WgtProject.h"

class Ui_DlgSelSkills;

class DlgSelSkills : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSelSkills(std::vector<QString> &added_skills, shrd_map_skills all_skills = nullptr, QWidget *parent = nullptr);
    ~DlgSelSkills();

private:
    void fillSkills();
    void initWgtsSkill();

private:
    void onFillSelectedSkills();

private:
    std::unique_ptr<Ui_DlgSelSkills> ui;
//    std::vector<ItemSkill *> m_skills;
//    std::map<int, QString> m_skills_ids;
    shrd_map_skills m_all_skills;
    std::vector<QString> &m_added_skills;
    std::vector<ItemSkill*> m_wgts_all_skills;
};

