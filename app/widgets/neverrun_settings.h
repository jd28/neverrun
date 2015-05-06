#ifndef NeverrunSettingsDIALOG_H
#define NeverrunSettingsDIALOG_H

#include <QDialog>
#include <boost/property_tree/ini_parser.hpp>

class QMessageBox;
class QSettings;

class Options;

namespace Ui {
class NeverrunSettings;
}

class NeverrunSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NeverrunSettingsDialog(Options *opts, QWidget *parent = 0);
    ~NeverrunSettingsDialog();

private slots:
    void on_cppDisableLoadModuleFinishHook_toggled(bool checked);
    void on_cppDisableWeaponHooks_toggled(bool checked);
    void on_cppDisableSetLocalStringHook_toggled(bool checked);
    void on_cppDisableGetTotalACSkillModHook_toggled(bool checked);
    void on_cppDisableGetUseMonkAbilitiesHook_toggled(bool checked);
    void on_cppDisableGetCriticalHitRollHook_toggled(bool checked);
    void on_cppDisableResolveAttackHook_toggled(bool checked);
    void on_cppDisableAddEquipItemActionsHook_toggled(bool checked);
    void on_cppDisableSummonAnimalCompanionHook_toggled(bool checked);
    void on_cppDisableSummonFamiliarHook_toggled(bool checked);
    void on_cppDisableSummonAssociateHook_toggled(bool checked);
    void on_cppDisableGetAssociateIdHook_toggled(bool checked);
    void on_cppDisableUseItemHook_toggled(bool checked);
    void on_cppDisableEffectImmunityHook_toggled(bool checked);
    void on_cppDisableResolveDamageShieldsHook_toggled(bool checked);
    void on_cppDisableGetRelativeWeaponSizeHook_toggled(bool checked);
    void on_cppDisableHolyAvengerHook_toggled(bool checked);
    void on_cppDisableToggleModeHook_toggled(bool checked);
    void on_cppDisableResolveAmmunitionHook_toggled(bool checked);
    void on_cppDisableSetActivityHook_toggled(bool checked);
    void on_cppDisableResolveSpecialAttackDamageBonusHook_toggled(bool checked);
    void on_cppDisableGetMinEquipLevelHook_toggled(bool checked);
    void on_cppDisableClearAllPartyInvalidActionsHook_toggled(bool checked);
    void on_cppDisableSpellSlotsInPolymorphHook_toggled(bool checked);

    void on_nrUpdateInBackground_toggled(bool checked);

    void on_nwnShoutColorBtn_clicked();
    void on_nwnFriendlyColorBtn_clicked();
    void on_nwnHostileColorBtn_clicked();
    void on_nrBlacklistRemoveBtn_clicked();
    void on_nwnTalkColorBtn_clicked();
    void on_nwnTellColorBtn_clicked();
    void on_nwnWhisperColorBtn_clicked();
    void on_nwnServerColorBtn_clicked();
    void on_nwnPartyColorBtn_clicked();
    void on_nwnDMColorBtn_clicked();
    void on_nwnDifficultyLevelCB_currentIndexChanged(int index);
    void on_nwnClientPortSB_valueChanged(int arg1);
    void on_nwnLogChat_toggled(bool checked);
    void on_nwnLogEntireChat_toggled(bool checked);
    void on_nwnHideSecondStoryTilesCB_toggled(bool checked);
    void on_nwnVisibleCloaks_toggled(bool checked);
    void on_nwnFullScreen_toggled(bool checked);
    void on_nwnAllowWindowedMode_toggled(bool checked);
    void on_nwnDisableMovies_toggled(bool checked);
    void on_nwnDisableIntroMovies_toggled(bool checked);
    void on_nwnEnameDialogZoom_toggled(bool checked);
    void on_nwnEnableScreenEdgeCameraTurn_toggled(bool checked);

    void on_nwnPatchAddBtn_clicked();

    void on_nwnPatchRemoveBtn_clicked();

private:
    Ui::NeverrunSettings *ui;
    Options *options_;
    bool loading_;

    void loadCPPSettings(const boost::property_tree::ptree& nwn_settings);
    void loadNWNChatColorSettings(const boost::property_tree::ptree& nwn_settings);
    void loadNWNTrapColorSettings(const boost::property_tree::ptree& nwn_settings);
    void loadNWNGameOptions(const boost::property_tree::ptree& nwn_settings);
    void loadNWNiniSettings();
    void loadNWNPatchIniSettings();
    void loadNWNControlOptions(const boost::property_tree::ptree& nwn_settings);
};

#endif // NeverrunSettingsDIALOG_H
