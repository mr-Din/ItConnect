#pragma once

#include "Project.h"
#include "User.h"

#include <QFrame>

class Ui_WgtWorker;


class WgtWorker : public QFrame
{
    Q_OBJECT
    const static int PHOTO_WIDTH = 80;
public:
    explicit WgtWorker(std::shared_ptr<User> user, std::vector<std::shared_ptr<Project>>& projects, bool is_editable = false, bool is_adding_proj = false, QWidget *parent = nullptr);
    ~WgtWorker();

public slots:
    void onUpdProject(int proj_id);

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
    void onDelProjectConfirm(int pg);
    void onDelProject();

signals:
    void sigSelectProject(int id);
    void sigDelProject();
    void sigShowProject(int id);

private:
    std::unique_ptr<Ui_WgtWorker> ui;
    std::shared_ptr<User> m_user;
    bool m_is_editable;
    bool m_is_adding_proj;
    std::vector<std::shared_ptr<Project>>& m_all_projects;
};

