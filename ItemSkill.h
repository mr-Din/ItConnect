#pragma once

#include <QPushButton>
#include <QObject>

class ItemSkill : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(bool is_selected MEMBER m_is_selected)
public:
    ItemSkill(int id = -1, QString skill = "", QWidget *parent = nullptr);

public:
    const bool isSelected() const;
    const int getId() const;
    const QString getSkillTitle() const;

private:
    void resizeEvent(QResizeEvent* event) override;

private:
    bool m_is_selected;
    int m_id;
    QString m_skill;
};

