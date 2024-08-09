#include "WgtWorker.h"
#include "InitDataBase.h"
#include "ui_WgtWorker.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>

WgtWorker::WgtWorker(std::shared_ptr<User> user, bool is_editable, QWidget *parent)
    : QWidget(parent)
    , ui(std::make_unique<Ui_WgtWorker>())
    , m_user(user)
    , m_is_editable(is_editable)
{
    ui->setupUi(this);
    fillUi();
    setupSigSlot();
}

WgtWorker::~WgtWorker()
{
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
    ui->lbl_status_2->setText(m_user->getProjectId() == 0 ? "Свободен" : "Занят");
    ui->btn_edit->setVisible(m_is_editable);
    /// TODO брать изображение из базы данных (либо адрес картинки из бд)
    QString path_to_photo = ":/icons/user-outline-on.svg";
    setPhoto(path_to_photo);
}

void WgtWorker::setupSigSlot()
{
    connect(ui->btn_edit, &QPushButton::clicked, this, &WgtWorker::onEdit);
    connect(ui->btn_ok, &QPushButton::clicked, this, &WgtWorker::onDone);
    connect(ui->btn_exit, &QPushButton::clicked, this, [&](){ui->stackedWidget->setCurrentIndex(0);} );
    connect(ui->btn_change_photo, &QPushButton::clicked, this, &WgtWorker::onChangePhoto);
}

void WgtWorker::setPhoto(const QString &path_to_photo)
{
    QPixmap photo(path_to_photo);
    if (photo.isNull())
        photo.load(":/icons/user-outline-on.svg");
    QPixmap scaled_photo = photo.scaledToWidth(PHOTO_WIDTH, Qt::SmoothTransformation);
    ui->lbl_photo->setPixmap(scaled_photo);
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
}

void WgtWorker::onChangePhoto()
{
    QString path_to_photo = QFileDialog::getOpenFileName(this, "Выберите изображение", "", "Изображения (*.png *.jpg *.jpeg *.bmp *.gif)");

    setPhoto(path_to_photo);
}
