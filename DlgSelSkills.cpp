#include "DlgSelSkills.h"
#include "ui_DlgSelSkills.h"

#include <QScroller>
#include <QVBoxLayout>
#include <algorithm>

DlgSelSkills::DlgSelSkills(std::vector<QString> &added_skills, shrd_map_skills all_skills, QWidget *parent)
    : QDialog(parent)
    , ui(std::make_unique<Ui_DlgSelSkills>())
    , m_all_skills(all_skills)
    , m_added_skills(added_skills)
{
    ui->setupUi(this);
    showMaximized();
    QScroller::grabGesture(ui->scrollArea, QScroller::TouchGesture);
    fillSkills();
    initWgtsSkill();
    connect(this, &DlgSelSkills::accepted, this, &DlgSelSkills::onFillSelectedSkills);
}

DlgSelSkills::~DlgSelSkills()
{
}

void DlgSelSkills::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);  // Вызов базового класса
    update();  // Принудительная перерисовка
}

void DlgSelSkills::fillSkills()
{
    for (const auto& [id, skill] : *m_all_skills)
    {
        // Если уже есть в added_skills, то не добавляем (либо делать заблоченным)
//        if (std::find(m_added_skills.cbegin(), m_added_skills.cend(), QString::number(id)) == m_added_skills.end())
            m_wgts_all_skills.push_back(new ItemSkill(id, skill, this));
    }
}

void DlgSelSkills::initWgtsSkill()
{
    QVBoxLayout* lt = new QVBoxLayout();
    for (const auto& wgt : m_wgts_all_skills)
    {
        lt->addWidget(wgt);
    }
    lt->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
//    setLayout(lt);
    ui->scrollAreaWidgetContents->setLayout(lt);
}

void DlgSelSkills::onFillSelectedSkills()
{
    for (const auto& wgt : m_wgts_all_skills)
    {
        QString id = QString::number(wgt->getId());
        bool is_uniq = (std::find(m_added_skills.cbegin(), m_added_skills.cend(), id) == m_added_skills.cend());
        if (wgt->isSelected() && is_uniq)
        {
            m_added_skills.push_back(id);
        }
        else if (!wgt->isSelected())
        {
            m_added_skills.erase(std::remove_if(m_added_skills.begin(), m_added_skills.end(), [&](const QString& s)
                                 {
                                     return s == id;
                                 }), m_added_skills.end()
            );
        }
    }
}
