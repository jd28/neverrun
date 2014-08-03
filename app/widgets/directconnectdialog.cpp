#include <QDebug>
#include <QHostInfo>
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
    static QRegExp port("(:[0-9]{1,6})$");
    static QRegExp domain("([^:]+)(:[0-9]{1,6})?$");

    QString p;
    if ( port.indexIn(addr) != -1 ) {
        p = port.cap(0);
    }

    QString address;
    QString dom;

    if(!isValidServerAddress(addr, false)) {
        int res2 = domain.indexIn(addr);
        if( res2 == -1) {
            errorMessage("Invalid IP Address/Hostname");
            return;
        }
        dom = domain.cap(1);
        QHostInfo info = QHostInfo::fromName(dom);
        auto addresses = info.addresses();

        if (addresses.size() == 0 || !isValidServerAddress(addresses[0].toString(), false)) {
            errorMessage("Unable to resolve hostname: " + dom);
            return;
        }
        address = addresses[0].toString();
        address += p;
    }
    else {
        address = addr;
    }

    close();

    if(p.size() == 0) {
        address += ":5121";
    }

    if(ui->addToMaster->isChecked()) {
        qDebug() << AddServer(address.toStdString().c_str());
    }

    QString t;
    if( dom.size() > 0 ) {
        if(p.size() == 0) {
            dom += ":5121";
        }
        else {
            dom += p;
        }
        t = dom;
    }
    else {
        t = address;
    }

    options_->addDirectConnect(t);
    ui->address->insertItem(0, t);

    for (int i = 1; i < ui->address->count(); ++i) {
        if (ui->address->itemText(i) == t) {
            ui->address->removeItem(i);
            break;
        }
    }

    if (ui->address->count() > 10) {
        ui->address->removeItem(10);
    }

    ui->address->setCurrentIndex(0);

    options_->addServerToCategory("History", address);
    emit addServer(address);
    emit requestPlayServer(address, pass, dm);
}

void DirectConnectDialog::play() {
    doServer(ui->address->lineEdit()->text(), ui->password->text(), false);
}

void DirectConnectDialog::dm() {
    doServer(ui->address->lineEdit()->text(), ui->password->text(), true);
}
