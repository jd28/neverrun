#include "fvupdatewindow.h"
#include "ui_fvupdatewindow.h"
#include "fvupdater.h"
#include "fvavailableupdate.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>


FvUpdateWindow::FvUpdateWindow(FvUpdater *updater, QWidget *parent, bool skipVersionAllowed, bool remindLaterAllowed) :
    QWidget(parent, Qt::CustomizeWindowHint),
    m_ui(new Ui::FvUpdateWindow),
    updater_(updater)
{
    m_ui->setupUi(this);
    m_ui->releaseNotesWebView->page()->settings()->setUserStyleSheetUrl(QUrl("qrc:/web/styles/clearness-dark.css"));
    m_appIconScene = 0;

    if(!skipVersionAllowed)
        m_ui->skipThisVersionButton->hide();
    if(!remindLaterAllowed)
        m_ui->remindMeLaterButton->hide();

    // Delete on close
    setAttribute(Qt::WA_DeleteOnClose, true);

    // Set the "new version is available" string
    QString newVersString = m_ui->newVersionIsAvailableLabel->text().arg(QApplication::applicationName());
    m_ui->newVersionIsAvailableLabel->setText(newVersString);

    // Connect buttons
    connect(m_ui->installUpdateButton, SIGNAL(clicked()),
            updater, SLOT(InstallUpdate()));
    connect(m_ui->skipThisVersionButton, SIGNAL(clicked()),
            updater, SLOT(SkipUpdate()));
    connect(m_ui->remindMeLaterButton, SIGNAL(clicked()),
            updater, SLOT(RemindMeLater()));
}

FvUpdateWindow::~FvUpdateWindow()
{
    m_ui->releaseNotesWebView->stop();
    delete m_ui;
}

bool FvUpdateWindow::UpdateWindowWithCurrentProposedUpdate()
{
    FvAvailableUpdate* proposedUpdate = updater_->GetProposedUpdate();
    if (! proposedUpdate) {
        return false;
    }

    QString downloadString = m_ui->wouldYouLikeToDownloadLabel->text()
            .arg(QApplication::applicationName(), proposedUpdate->GetEnclosureVersion(), QApplication::applicationVersion());
    m_ui->wouldYouLikeToDownloadLabel->setText(downloadString);

    m_ui->releaseNotesWebView->stop();
    m_ui->releaseNotesWebView->load(proposedUpdate->GetReleaseNotesLink());

    return true;
}

void FvUpdateWindow::closeEvent(QCloseEvent* event)
{
    updater_->updaterWindowWasClosed();
    event->accept();
}
