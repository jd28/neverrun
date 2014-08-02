#include <QDebug>
#include <QMessageBox>

#include "directconnectdialog.h"
#include "ui_directconnectdialog.h"

#include "../nwnmasterserver.h"
#include "../options.h"
#include "../util.h"

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
    static QRegExp domain(".*\\..*:[0-9]{1,6}");

    int res2 = domain.indexIn(addr);

    close();
    if(!isValidServerAddress(addr) && res2 == -1) {
        errorMessage("Invalid IP Address/Hostname");
        return;
    }

    if(ui->addToMaster->isChecked()) {
        qDebug() << AddServer(addr.toStdString().c_str());
    }

    options_->addDirectConnect(addr);
    ui->address->insertItem(0, addr);

    for (int i = 1; i < ui->address->count(); ++i) {
        if (ui->address->itemText(i) == addr) {
            ui->address->removeItem(i);
            break;
        }
    }

    if (ui->address->count() > 10) {
        ui->address->removeItem(10);
    }

    ui->address->setCurrentIndex(0);

    options_->addServerToCategory("History", addr);
    emit addServer(addr);
    emit requestPlayServer(addr, pass, dm);
}

void DirectConnectDialog::play() {
    doServer(ui->address->lineEdit()->text(), ui->password->text(), false);
}

void DirectConnectDialog::dm() {
    doServer(ui->address->lineEdit()->text(), ui->password->text(), true);
}