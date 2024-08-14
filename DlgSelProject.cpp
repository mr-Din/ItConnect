#include "DlgSelProject.h"
#include "ui_DlgSelProject.h"

#include <QScroller>
#include <QScreen>
#include <QVBoxLayout>
#include <algorithm>

DlgSelProject::DlgSelProject(std::vector<std::shared_ptr<Project>>& all_project, std::shared_ptr<User> manager, QWidget *parent)
    : QDialog(parent)
    , ui(std::make_unique<Ui_DlgSelProject>())
    , m_all_project(all_project)
    , m_manager(manager)
{
    ui->setupUi(this);
#ifdef Q_OS_ANDROID
//    QSize screenSize = QGuiApplication::primaryScreen()->size();
//    int width = screenSize.width() * 0.98;  // Диалог будет занимать 90% ширины экрана
//    int height = screenSize.height() * 0.9;  // Диалог будет занимать 90% высоты экрана
//    setFixedSize(width, height);
#endif

    showMaximized();
    QScroller::grabGesture(ui->scrollArea, QScroller::TouchGesture);
    initWgtsProjects();
}

DlgSelProject::~DlgSelProject()
{
}

void DlgSelProject::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);  // Вызов базового класса
    update();  // Принудительная перерисовка
}

void DlgSelProject::initWgtsProjects()
{
    QVBoxLayout* lt = new QVBoxLayout();

    //    lt->addWidget(new WgtWorker(m_cur_user));
    // Заполняем проекты, который привязаны к текущему менеджеру
    for (const auto proj : m_all_project)
    {
        if (m_manager->getId() == proj->getManagerId())
        {
            auto wgt = new WgtProject(proj, nullptr, false, true);
            connect(wgt, &WgtProject::sigSelProject, this, &DlgSelProject::sigSelProject);
            connect(wgt, &WgtProject::sigSelProject, this, &DlgSelProject::accept);
            lt->addWidget(wgt);
        }
    }

    lt->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    ui->scrollAreaWidgetContents->setLayout(lt);

}

//void DlgSelProject::onSelProject()
//{
//    if (WgtProject* sndr = qobject_cast<WgtProject*>(sender()); sndr)
//    {
//        emit sigSelProject(sndr->getProject()->getId());
//    }
//}

