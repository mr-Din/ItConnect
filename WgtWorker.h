#pragma once

#include "Project.h"
#include "User.h"

#include <QWidget>

class Ui_WgtWorker;


class WgtWorker : public QWidget
{
    Q_OBJECT
    const static int PHOTO_WIDTH = 80;
public:
    explicit WgtWorker(std::shared_ptr<User> user, std::vector<std::shared_ptr<Project>>& projects, bool is_editable = false, QWidget *parent = nullptr);
    ~WgtWorker();

private:
    void fillUi();
    void setupSigSlot();
    void setPhoto(const QString &path_to_photo);
    QString getProjectName() const;

private slots:
    void onEdit();
    void onDone();
    void onChangePhoto();
    void onSelectProject();

signals:
    void sigSelectProject(int id);
    void sigShowProject(int id);

private:
    std::unique_ptr<Ui_WgtWorker> ui;
    std::shared_ptr<User> m_user;
    bool m_is_editable;
    std::vector<std::shared_ptr<Project>>& m_all_projects;
};

