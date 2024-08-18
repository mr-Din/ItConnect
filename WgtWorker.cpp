#include "WgtWorker.h"
#include "InitDataBase.h"
#include "ui_WgtWorker.h"
#include "Project.h"
#include "util.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>

WgtWorker::WgtWorker(std::shared_ptr<User> user, std::vector<std::shared_ptr<Project>>& projects, bool is_editable, bool is_adding_proj, QWidget *parent)
    : QFrame(parent)
    , ui(std::make_unique<Ui_WgtWorker>())
    , m_user(user)
    , m_is_editable(is_editable)
    , m_is_adding_proj(is_adding_proj)
    , m_all_projects(projects)
{
    ui->setupUi(this);
    fillUi();
    setupSigSlot();
}

WgtWorker::~WgtWorker()
{
}

void WgtWorker::onUpdProject(int proj_id)
{
    m_user->setProjectId(proj_id);
    fillUi();
}

void WgtWorker::fillUi()
{

    /*
    btn_edit - показать только в разделе Аккаунт
    btn_add_to_project - показать, если m_cur_user = manager и status - свободен

*/
    ui->stackedWidget->setCurrentIndex(0);
    ui->lbl_login_2->setText(m_user->getLogin());
    ui->lbl_email_2->setText(m_user->getEmail());
    ui->lbl_info_2->setText(m_user->getDescription());
    ui->btn_edit->setVisible(m_is_editable);
    ui->lbl_status_2->setText(m_user->getProjectId() == 0 ? "Свободен" : "Занят");

    ui->btn_add_to_project->setEnabled(m_user->getProjectId() == 0);
    ui->stackedWidget_Del->setVisible(m_user->getProjectId() != 0 && m_is_editable);
    ui->stackedWidget_Del->setCurrentIndex(0);

    ui->btn_project->setVisible(m_user->getProjectId() != 0);

    ui->btn_project->setText(getProjectName());
    ui->btn_add_to_project->setVisible(m_is_adding_proj);

    utility::addShadowCurrentObj(this);
    utility::addShadowToObj<QLabel*>(this);
    utility::addShadowToObj<QPushButton*>(this);

    setPhoto(m_user->getPhotoPath());
}

void WgtWorker::setupSigSlot()
{
    connect(ui->btn_edit, &QPushButton::clicked, this, &WgtWorker::onEdit);
    connect(ui->btn_ok, &QPushButton::clicked, this, &WgtWorker::onDone);
    connect(ui->btn_exit, &QPushButton::clicked, this, [&](){ui->stackedWidget->setCurrentIndex(0);} );
    connect(ui->btn_change_photo, &QPushButton::clicked, this, &WgtWorker::onChangePhoto);
    connect(ui->btn_add_to_project, &QPushButton::clicked, this, &WgtWorker::onSelectProject);
    connect(ui->btn_project, &QPushButton::clicked, this, [=]{ emit sigShowProject(m_user->getProjectId()); });

    connect(ui->btn_del_proj, &QPushButton::clicked, this, [this]{ onDelProjectConfirm(1); });
    connect(ui->btn_del_confirm, &QPushButton::clicked, this, &WgtWorker::onDelProject);
    connect(ui->btn_del_cancel, &QPushButton::clicked, this, [this]{ onDelProjectConfirm(0); });

}

void WgtWorker::setPhoto(const QString &path_to_photo)
{
#ifdef Q_OS_ANDROID
    QString destinationDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
#else
    QString destinationDir = QCoreApplication::applicationDirPath();;
#endif

    QPixmap photo(destinationDir+path_to_photo);
    if (photo.isNull())
        photo.load(":/icons/user-outline-on.svg");
    QPixmap scaled_photo = photo.scaledToWidth(PHOTO_WIDTH, Qt::SmoothTransformation);
    ui->lbl_photo->setPixmap(scaled_photo);
}

QString WgtWorker::getProjectName() const
{
    auto it = std::find_if(m_all_projects.begin(), m_all_projects.end(), [=](std::shared_ptr<Project> proj){
        return (proj->getId() == m_user->getProjectId());
    });
    return (it != m_all_projects.end() ? it->get()->getTitle() : "");
}

void WgtWorker::onEdit()
{
    ui->le_login->setText(ui->lbl_login_2->text());
    ui->te_info->setPlainText(ui->lbl_info_2->text());
    ui->stackedWidget->setCurrentIndex(1);
}

void WgtWorker::onDone()
{
    QString login = ui->le_login->text();
    QString description = ui->te_info->toPlainText();
    /// TODO Если пусто, выдават сообщение
    if (login.isEmpty())
    {
        QMessageBox::warning(nullptr, "Предупреждение", "Логин не может быть пустым!", QMessageBox::Ok);
        return;
    }

    // Вносим изменения в базу данных!

    QSqlQuery q;
    if (!q.prepare(UPDATE_USER_SQL))
        q.lastError();

    updateUser(q, login, description, m_user->getId());

    ui->lbl_login_2->setText(login);
    ui->lbl_info_2->setText(description);
    ui->stackedWidget->setCurrentIndex(0);
    emit sigUpdateMain();
}

void WgtWorker::onChangePhoto()
{
    auto name = utility::changePhoto(this, "user" + QString::number(m_user->getId()));
    if (name.isEmpty())
        return;

    setPhoto(name);
    QSqlQuery q;
    updateUserPhoto(q, name, m_user->getId());
}

void WgtWorker::onSelectProject()
{
    emit sigSelectProject(m_user->getId());
}

void WgtWorker::onDelProjectConfirm(int pg)
{
    ui->stackedWidget_Del->setCurrentIndex(pg);
}

void WgtWorker::onDelProject()
{
    emit sigDelProject();
    m_user->setProjectId(0);
    fillUi();
}
