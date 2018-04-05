#ifndef DELETE_CONFIRMATION_H
#define DELETE_CONFIRMATION_H

#include <QDialog>

namespace Ui {
class Delete_confirmation;
}

class Delete_confirmation : public QDialog
{
    Q_OBJECT

public:
    explicit Delete_confirmation(QWidget *parent = 0);
    ~Delete_confirmation();

private:
    Ui::Delete_confirmation *ui;
    void set_label_text(QString name);
    QString configuration_name;

public slots:
    void set_configuration_name(QString name);

signals:
    delete_configuration(QString name);
private slots:
    void on_buttonBox_accepted();
};

#endif // DELETE_CONFIRMATION_H
