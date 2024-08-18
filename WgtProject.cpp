#include "WgtProject.h"
#include "ui_WgtProject.h"
#include "InitDataBase.h"
#include "DlgSelSkills.h"
#include "util.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>
#include <QScrollBar>

WgtProject::WgtProject(std::shared_ptr<Project> proj, shrd_map_skills skills, bool is_account, bool is_selected, QWidget *parent_sa,  QWidget *parent)
    : QFrame(parent)
    , ui(std::make_unique<Ui_WgtProject>())
    , m_project(proj)
    , m_all_skills(skills)
    , m_is_account(is_account)
    , m_is_selected(is_selected)
    , parent(parent)
    , m_sa(qobject_cast<QScrollArea*>(parent_sa))
    , m_counter(0)
    , m_timer(new QTimer(this))
{
    ui->setupUi(this);
    fillSkills();
    fillUi();
    setupSigSlot();

    connect(m_timer, &QTimer::timeout, this, &WgtProject::animate);
}

WgtProject::~WgtProject()
{}

std::shared_ptr<Project> WgtProject::getProject() const
{
    return m_project;
}

void WgtProject::onShowProject(int id)
{
    if (id == m_project->getId())
    {
        m_timer->start(400);
    }
}

void WgtProject::fillUi()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->lbl_title_2->setText(m_project->getTitle());
    ui->lbl_info_2->setText(m_project->getDescription());
    ui->lbl_email_2->setText(m_project->getEmail());
    ui->lbl_status_2->setText(m_project->getStatus());

    ui->stackedWidget_Del->setVisible(m_is_account);
    ui->btn_edit->setVisible(m_is_account);
    ui->btn_add_skill->setVisible(m_is_account);
    ui->btn_add_worker->setVisible(m_is_selected);

    ui->stackedWidget_Del->setCurrentIndex(0);

    utility::addShadowCurrentObj(this);
    utility::addShadowToObj<QLabel*>(this);
    utility::addShadowToObj<QPushButton*>(this);

//    ui->lbl_title_2->setText(m_user->getLogin());
//    ui->lbl_status_2->setText(m_user->getProjectId() == 0 ? "Свободен" : "Занят");
    /// TODO брать изображение из базы данных (либо адрес картинки из бд)
    QString path_to_photo = ":/icons/noimage.svg";
    setPhoto(path_to_photo);
}

void WgtProject::fillSkills()
{
    QSqlQuery skills_to_proj_query;
    skills_to_proj_query.prepare("SELECT * FROM skills_to_project WHERE project_id = ?");
    skills_to_proj_query.addBindValue(m_project->getId());
    std::vector<int>skills_ids;
    if (!skills_to_proj_query.exec())
    {
        qDebug()<<"Error SQL skills_to_proj_query query!";
    }
    while (skills_to_proj_query.next())
    {
        skills_ids.push_back(skills_to_proj_query.value(2).toInt());
    }

    QString skills_query_str = "SELECT * FROM skills WHERE id IN (";
    for (int i = 0; i < skills_ids.size(); ++i)
    {
        skills_query_str += QString::number(skills_ids[i]);
        if (i < skills_ids.size() - 1)
            skills_query_str += ",";
    }
    skills_query_str += ");";

    skills_to_proj_query.prepare(skills_query_str);

    if (!skills_to_proj_query.exec())
    {
        qDebug()<<"Error SQL skills_to_proj_query query!";
    }
    while (skills_to_proj_query.next())
    {
        m_skills_ids[skills_to_proj_query.value(1).toString()] = skills_to_proj_query.value(0).toString();
    }
    updSkills();
}

void WgtProject::updSkills()
{
    QString skills_txt;
    for (auto it = m_skills_ids.begin(); it != m_skills_ids.end(); ++it)
    {
        skills_txt.push_back(it->first);

        if (std::next(it) != m_skills_ids.end())
            skills_txt.push_back(", ");
    }

    ui->lbl_skills_2->setText(skills_txt);
}

void WgtProject::setupSigSlot()
{
    connect(ui->btn_del_proj, &QPushButton::clicked, this, [this]{ onDelProjectConfirm(1); });
    connect(ui->btn_del_confirm, &QPushButton::clicked, this, &WgtProject::onDelProject);
    connect(ui->btn_del_cancel, &QPushButton::clicked, this, [this]{ onDelProjectConfirm(0); });
    connect(ui->btn_edit, &QPushButton::clicked, this, &WgtProject::onEdit);
    connect(ui->btn_ok, &QPushButton::clicked, this, &WgtProject::onDone);
    connect(ui->btn_exit, &QPushButton::clicked, this, &WgtProject::onReject);
    connect(ui->btn_change_photo, &QPushButton::clicked, this, &WgtProject::onChangePhoto);

    connect(ui->btn_add_skill, &QPushButton::clicked, this, &WgtProject::onOpenDlgSelSkills);

    connect(ui->btn_add_worker, &QPushButton::clicked, this, [this]{ emit sigSelProject(m_project->getId()); });
}

void WgtProject::setPhoto(const QString &path_to_photo)
{
    QPixmap photo(path_to_photo);
    if (photo.isNull())
        photo.load(":/icons/noimage.svg");
    QPixmap scaled_photo = photo.scaledToWidth(PHOTO_WIDTH, Qt::SmoothTransformation);
    ui->lbl_photo->setPixmap(scaled_photo);
}

void WgtProject::clearLayout(QWidget *wgt)
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

void WgtProject::addSkillWgt(const QString& skill, QVBoxLayout* lt)
{
    QPushButton* btn_skill = new QPushButton(skill);
    btn_skill->setIcon(QIcon(":/icons/trash-can-gray.svg"));
    btn_skill->setIconSize(QSize(40,40));
    btn_skill->setMinimumHeight(50);
    btn_skill->setMaximumWidth(300);

    utility::addShadowCurrentObj(btn_skill);

    connect(btn_skill, &QPushButton::clicked, this, &WgtProject::onDelSkill);
    lt->addWidget(btn_skill);
    lt->setAlignment(Qt::AlignCenter);

    QFontMetrics fm(btn_skill->font());
    QString elidedText = fm.elidedText(skill, Qt::ElideRight, btn_skill->width() - 100);
    btn_skill->setText(elidedText);
}

void WgtProject::delSkillFromDB(const QString& id)
{
    // Удаление скилла из ДБ
    QSqlQuery del_skill_query;
    del_skill_query.prepare("DELETE FROM skills_to_project WHERE skill_id = ?");
    del_skill_query.bindValue(0, id);
    if (!del_skill_query.exec())
    {
        qDebug()<<"Error SQL del_skill_query query!";
    }
}

void WgtProject::onFillSkillsOnEdit()
{
    clearLayout(ui->wgt_skills);

    QVBoxLayout* lt = new QVBoxLayout();
    for (const auto& [skill, id] : m_skills_ids)
    {
        addSkillWgt(skill, lt);
    }
    lt->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    //    lt->setSpacing(0);
    ui->wgt_skills->setLayout(lt);
}

void WgtProject::onDelProjectConfirm(int pg)
{
    auto scrollPos = m_sa->verticalScrollBar()->value();
    ui->stackedWidget_Del->setCurrentIndex(pg);
    m_sa->verticalScrollBar()->setValue(scrollPos);
}

void WgtProject::onDelProject()
{
    // Удаление из базы
    QSqlQuery q;
    deleteProject(q, m_project->getId());

    // Удаление виджета
    emit sigDelProject(m_project->getId());

    onDelProjectConfirm(0);
}

void WgtProject::onEdit()
{
    ui->le_title->setText(ui->lbl_title_2->text());
    ui->te_info->setPlainText(ui->lbl_info_2->text());
    onFillSkillsOnEdit();
    auto scrollPos = m_sa->verticalScrollBar()->value();
    ui->stackedWidget->setCurrentIndex(1);
    m_sa->verticalScrollBar()->setValue(scrollPos);
}

void WgtProject::onReject()
{
    auto scrollPos = m_sa->verticalScrollBar()->value();
    ui->stackedWidget->setCurrentIndex(0);
    m_sa->verticalScrollBar()->setValue(scrollPos);
    m_del_buff_skills.clear();
    m_added_skills.clear();
    fillSkills();
}

void WgtProject::onDone()
{
    QString title = ui->le_title->text();
    QString description = ui->te_info->toPlainText();
    /// TODO Если пусто, выдават сообщение
    if (title.isEmpty())
    {
        QMessageBox::warning(nullptr, "Предупреждение", "Название не может быть пустым!", QMessageBox::Ok);
        return;
    }

    // Вносим изменения в базу данных!

    QSqlQuery q;
    if (!q.prepare(UPDATE_PROJECT_SQL))
        q.lastError();

    updateProject(q, title, /*email,*/ description, m_project->getId());
    /// TODO обновить данные в таблице skills_to_project (id проекта - id навыка)

    ui->lbl_title_2->setText(title);
    ui->lbl_info_2->setText(description);
    for (const QString& skill_txt : m_del_buff_skills)
    {
        for (const auto&[id, skill] : *m_all_skills) {
            if (skill == skill_txt) {
                delSkillFromDB(QString::number(id));
                break;
            }
        }
    }

    m_del_buff_skills.clear();
    onAddNewSkillsToBD();
    fillSkills();

    auto scrollPos = m_sa->verticalScrollBar()->value();
    ui->stackedWidget->setCurrentIndex(0);
    m_sa->verticalScrollBar()->setValue(scrollPos);
    emit sigUpdateAll();
}

void WgtProject::onChangePhoto()
{
    QString path_to_photo = QFileDialog::getOpenFileName(this, "Выберите изображение", "", "Изображения (*.png *.jpg *.jpeg *.bmp *.gif)");

    setPhoto(path_to_photo);
}

void WgtProject::onDelSkill()
{
    if (QPushButton* sndr = qobject_cast<QPushButton*>(sender()); sndr)
    {
        m_del_buff_skills.push_back(sndr->text());
        QString skill_name;
        for (const auto&[id, skill] : *m_all_skills) {
            if (skill == sndr->text()) {
                skill_name = QString::number(id);
            }
        };
        m_added_skills.erase(std::remove_if(m_added_skills.begin(), m_added_skills.end(), [&](const QString& s)
                                            {
                                                return s == skill_name;
                                            })
                             , m_added_skills.end());
        m_skills_ids.erase(sndr->text());
        onFillSkillsOnEdit();
    }
}

void WgtProject::onOpenDlgSelSkills()
{
    shrd_map_skills unadded_skills = std::make_shared<std::map<int, QString>>();
    for (const auto& [id, skill] : *m_all_skills)
    {
        bool is_unadded = !(m_skills_ids.count(skill))
                          && std::find(m_added_skills.cbegin(), m_added_skills.cend(), QString::number(id)) == m_added_skills.cend();
        bool is_deleted = std::find(m_del_buff_skills.cbegin(), m_del_buff_skills.cend(), skill) != m_del_buff_skills.cend();
        if (is_unadded || is_deleted)
            (*unadded_skills)[id] = skill;
    }

    DlgSelSkills dlg(m_added_skills, unadded_skills, this);
    dlg.setGeometry(parent->geometry());
    connect(&dlg, &DlgSelSkills::accepted, this, &WgtProject::onAddNewSkills);
    connect(&dlg, &DlgSelSkills::rejected, this, &WgtProject::onClearAddedSkills);
    if (dlg.exec())
        return;
}

void WgtProject::onAddNewSkills()
{
    for (const auto& id : m_added_skills)
    {
        m_skills_ids[m_all_skills->at(id.toInt())] = id;

        m_del_buff_skills.erase(std::remove_if(m_del_buff_skills.begin(), m_del_buff_skills.end(), [&](const QString& s)
                                               {
                                                   return s == m_all_skills->at(id.toInt());
                                               })
                                , m_del_buff_skills.end());
    }
    onFillSkillsOnEdit();
}

void WgtProject::onAddNewSkillsToBD()
{
    QSqlQuery q;
    if (!q.prepare(INSERT_SKILLS_TO_PROJ_SQL))
        return;

    for (const QString& id : m_added_skills)
        addSkillsToProject(q, m_project->getId(), id);

    onClearAddedSkills();
    /// TODO Необходимо реализовать добавление кнопок скиллов (в разделе Редактирования)
    /// Учитывать то, что заново добавленные скиллы могут удалить (не добавлять в буффер m_del_buff_skills)
    /// При подтверждении (onDone) добавить новые скиллы в раздел Проект.
//    fillSkills();

}

void WgtProject::onClearAddedSkills()
{
    m_added_skills.clear();
}

void WgtProject::animate()
{
    if (m_counter % 2 == 0) {
//        setStyleSheet("background-color: #495366;");
        setStyleSheet("QWidget#pg_proj_date {background-color: #495366; margin: -2px -2px -2px -2px; border: 4px solid #2e3440; border-radius: 10px;}");
    } else {
        setStyleSheet(m_style);
    }

    ++m_counter;

    if (m_counter >= 4)
    {
        m_timer->stop();  // Останавливаем таймер
        setStyleSheet(m_style);  // Восстанавливаем исходный стиль
        m_counter = 0;
    }
}
