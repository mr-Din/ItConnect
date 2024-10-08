#include "ItemSkill.h"
#include "util.h"
#include <QStyle>

ItemSkill::ItemSkill(int id, QString skill, QWidget *parent)
    : QPushButton(parent)
    , m_is_selected(false)
    , m_id(id)
    , m_skill(skill)
{
    setText(m_skill);
    connect(this, &QPushButton::clicked, this, [&]{
        m_is_selected = !m_is_selected;
        style()->polish(this);
    });
    utility::addShadowCurrentObj(this);

}

const bool ItemSkill::isSelected() const
{
    return m_is_selected;
}

const int ItemSkill::getId() const
{
    return m_id;
}

const QString ItemSkill::getSkillTitle() const
{
    return m_skill;
}

void ItemSkill::resizeEvent(QResizeEvent *event)
{
    QFontMetrics fm(font());
    QString elidedText = fm.elidedText(m_skill, Qt::ElideRight, parentWidget()->width() - 100);
    setText(elidedText);
    QPushButton::resizeEvent(event);
}
