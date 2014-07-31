#include <QDebug>
#include <QMessageBox>

#include "directconnectdialog.h"
#include "ui_directconnectdialog.h"

#include "../options.h"
#include "../nwnmasterserver.h"

DirectConnectDialog::DirectConnectDialog(Options *opts, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DirectConnectDialog),
    options_(opts)
{
    ui->setupUi(this);
    setWindowFlags(Qt::SplashScreen);
    setModal(true);

    ui->address->addItems(opts->getDirectConnects());

    connect(ui->playButton, SIGNAL(clicked()), SLOT(play()));
    connect(ui->dmButton, SIGNAL(clicked()), SLOT(dm()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(close()));

    ui->addToMasterWiget->layout()->setContentsMargins(0, 0, 0, 0);
    ui->addToMasterWiget->layout()->setAlignment(ui->addToMaster, Qt::AlignCenter);

    error_ = new QMessageBox(this);
    error_->setWindowFlags(Qt::SplashScreen);
    error_->setText("Error: Invalid IP Address!");
    error_->setModal(true);
}

DirectConnectDialog::~DirectConnectDialog()
{
    delete ui;
}

void DirectConnectDialog::ensureFocus() {
    ui->addToMaster->setChecked(false);
    ui->address->setFocus();
}

void DirectConnectDialog::doServer(const QString& addr, const QString& pass, bool dm) {
    static QRegExp ip("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}:[0-9]{1,6}");

    int res = ip.indexIn(addr);

    close();
    if(res == -1) {
        error_->show();
        return;
    }

    if(ui->addToMaster->isChecked()) {
        qDebug() << AddServer(addr.toStdString().c_str());
    }

    options_->addDirectConnect(addr);
    bool add = true;
    for (int i = 0; i < ui->address->count(); ++i) {
        if (ui->address->itemText(i) == addr) {
            add = false;
            break;
        }
    }
    if (add) {
        ui->address->addItem(addr);
    }
    emit requestPlayServer(addr, pass, false);
}

void DirectConnectDialog::play() {
    doServer(ui->address->lineEdit()->text(), ui->password->text(), false);
}

void DirectConnectDialog::dm() {
    doServer(ui->address->lineEdit()->text(), ui->password->text(), true);
}
