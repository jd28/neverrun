#ifndef DIRECTCONNECTDIALOG_H
#define DIRECTCONNECTDIALOG_H

#include <QDialog>

class QMessageBox;

class Options;

namespace Ui {
class DirectConnectDialog;
}

class DirectConnectDialog : public QDialog
{
    Q_OBJECT

    Ui::DirectConnectDialog *ui;
    Options *options_;
    QMessageBox *error_;

    void doServer(const QString &addr, const QString &pass, bool dm);

private slots:
    void play();
    void dm();

public:
    explicit DirectConnectDialog(Options *opts, QWidget *parent = 0);
    ~DirectConnectDialog();

    void ensureFocus();

signals:
    void requestPlayServer(QString address, QString password, bool dm);
    void addServer(QString address);

};

#endif // DIRECTCONNECTDIALOG_H
