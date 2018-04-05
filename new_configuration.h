#ifndef NEW_CONFIGURATION_H
#define NEW_CONFIGURATION_H

#include <QDialog>
#include "QDir"

namespace Ui {
class new_configuration;
}

class new_configuration : public QDialog
{
    Q_OBJECT

public:
    explicit new_configuration(QWidget *parent = 0);
    ~new_configuration();

signals:
    void send_new_configuration_name(QString new_configuration_name);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::new_configuration *ui;
    bool check_if_file_exists(QString filename);
};

#endif // NEW_CONFIGURATION_H
