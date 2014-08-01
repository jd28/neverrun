#ifndef FVUPDATEWINDOW_H
#define FVUPDATEWINDOW_H

#if QT_VERSION >= 0x050000
    #include <QtWidgets/QWidget>
#else
    #include <QWidget>
#endif

class FvUpdater;
class QGraphicsScene;

namespace Ui {
class FvUpdateWindow;
}

class FvUpdateWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FvUpdateWindow(FvUpdater *updater, QWidget *parent, bool skipVersionAllowed, bool remindLaterAllowed);
    ~FvUpdateWindow();

    // Update the current update proposal from FvUpdater
    bool UpdateWindowWithCurrentProposedUpdate();

    void closeEvent(QCloseEvent* event);

private:
    Ui::FvUpdateWindow*	m_ui;
    QGraphicsScene* m_appIconScene;
    FvUpdater *updater_;
};

#endif // FVUPDATEWINDOW_H


