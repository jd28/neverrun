#include <QDebug>
#include <QHostInfo>
#include <QMessageBox>
#include <QSettings>
#include <QColorDialog>
#include <QFileDialog>
#include <QFileInfo>

#include "neverrun_settings.h"
#include "ui_neverrun_settings.h"

#include "../nwnmasterserver.h"
#include "../options.h"
#include "../util.h"

NeverrunSettingsDialog::NeverrunSettingsDialog(Options *opts, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NeverrunSettings),
    options_(opts)
{
    loading_ = true;
    ui->setupUi(this);
    setWindowFlags(Qt::SplashScreen);
    setModal(true);
    ui->nrBlacklist->addItems(options_->getBlacklist());
    ui->nrDefaultLoader->setText(options_->getDefaultLoader());
    ui->nrDefaultToolset->setText(options_->getDefaultToolset());
    ui->nrUpdateInBackground->setChecked(options_->getUpdateBackground());

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini((options_->getNWNPath() + "/nwnplayer.ini").toStdString(), pt);

    loadNWNGameOptions(pt);
    loadNWNChatColorSettings(pt);
    loadNWNTrapColorSettings(pt);
    loadCPPSettings(pt);
    loadNWNiniSettings();
    loadNWNPatchIniSettings();
    loading_ = false;
}

NeverrunSettingsDialog::~NeverrunSettingsDialog(){
    delete ui;
}

void NeverrunSettingsDialog::on_nrUpdateInBackground_toggled(bool checked){
    options_->setUpdateBackground(checked);
}

#define SET_NWPLAYER_INI_VALUE(entry, val) \
    if(loading_) { return; } \
    auto file = (options_->getNWNPath() + "/nwnplayer.ini").toStdString(); \
    boost::property_tree::ptree pt; \
    boost::property_tree::ini_parser::read_ini(file, pt);\
    pt.put(entry, val);\
    boost::property_tree::ini_parser::write_ini(file, pt)

void NeverrunSettingsDialog::on_cppDisableSetLocalStringHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableSetLocalStringHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableLoadModuleFinishHook_toggled(bool checked) {
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableLoadModuleFinishHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableWeaponHooks_toggled(bool checked) {
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableWeaponHooks", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableGetTotalACSkillModHook_toggled(bool checked) {
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableGetTotalACSkillModHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableGetUseMonkAbilitiesHook_toggled(bool checked)
{
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableGetUseMonkAbilitiesHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableGetCriticalHitRollHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableGetCriticalHitRollHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableResolveAttackHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableResolveAttackHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableAddEquipItemActionsHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableAddEquipItemActionsHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableSummonAnimalCompanionHook_toggled(bool checked)
{
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableSummonAnimalCompanionHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableSummonFamiliarHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableSummonFamiliarHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableSummonAssociateHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableSummonAssociateHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableGetAssociateIdHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableGetAssociateIdHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableUseItemHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableUseItemHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableEffectImmunityHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableEffectImmunityHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableResolveDamageShieldsHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableResolveDamageShieldsHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableGetRelativeWeaponSizeHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableGetRelativeWeaponSizeHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableHolyAvengerHook_toggled(bool checked)
{
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableHolyAvengerHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableToggleModeHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableToggleModeHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableResolveAmmunitionHook_toggled(bool checked)
{
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableResolveAmmunitionHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableSetActivityHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableSetActivityHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableResolveSpecialAttackDamageBonusHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableResolveSpecialAttackDamageBonusHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableGetMinEquipLevelHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableGetMinEquipLevelHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableClearAllPartyInvalidActionsHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableClearAllPartyInvalidActionsHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_cppDisableSpellSlotsInPolymorphHook_toggled(bool checked){
    SET_NWPLAYER_INI_VALUE("Community Patch.DisableSpellSlotsInPolymorphHook", checked ? 1 : 0);
}

void NeverrunSettingsDialog::loadCPPSettings(const boost::property_tree::ptree &nwn_settings) {
    boost::optional<int> val;
#define CPP_NWPLAYER_INI_BOOL(section, entry) \
    val = nwn_settings.get_optional<int>(section "." #entry); \
    ui->cpp##entry->setChecked(val ? *val : 0)

    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableSetLocalStringHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableLoadModuleFinishHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableWeaponHooks);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableGetTotalACSkillModHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableGetUseMonkAbilitiesHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableGetCriticalHitRollHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableResolveAttackHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableAddEquipItemActionsHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableSummonAnimalCompanionHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableSummonFamiliarHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableSummonAssociateHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableGetAssociateIdHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableUseItemHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableEffectImmunityHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableResolveDamageShieldsHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableGetRelativeWeaponSizeHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableHolyAvengerHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableToggleModeHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableResolveAmmunitionHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableSetActivityHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableResolveSpecialAttackDamageBonusHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableGetMinEquipLevelHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableClearAllPartyInvalidActionsHook);
    CPP_NWPLAYER_INI_BOOL("Community Patch", DisableSpellSlotsInPolymorphHook);
#undef CPP_NWPLAYER_INI_BOOL
}

void NeverrunSettingsDialog::loadNWNGameOptions(const boost::property_tree::ptree &nwn_settings) {
    boost::optional<int> val;
    val = nwn_settings.get_optional<int>("Game Options.Client Port");
    ui->nwnClientPortSB->setValue(val ? *val : 5121);
    val = nwn_settings.get_optional<int>("Game Options.Difficulty Level");
    ui->nwnDifficultyLevelCB->setCurrentIndex(val ? *val : 0);
    val = nwn_settings.get_optional<int>("Game Options.Hide SecondStoryTiles");
    ui->nwnHideSecondStoryTilesCB->setChecked(val ? *val : 0);
    val = nwn_settings.get_optional<int>("Game Options.ClientChatLogging");
    ui->nwnLogChat->setChecked(val ? *val : 0);
    val = nwn_settings.get_optional<int>("Game Options.ClientEntireChatWindowLogging");
    ui->nwnLogEntireChat->setChecked(val ? *val : 0);
    val = nwn_settings.get_optional<int>("Game Options.Visible Cloaks");
    ui->nwnVisibleCloaks->setChecked(val ? *val : 0);
}

void NeverrunSettingsDialog::loadNWNiniSettings() {
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini((options_->getNWNPath() + "/nwn.ini").toStdString(),
                                               pt);
    boost::optional<int> thing;
#define UGH(entry, object) \
    thing = pt.get_optional<int>(entry); \
    ui->object->setChecked(thing ? *thing : 0)

    UGH("Display Options.AllowWindowedMode", nwnAllowWindowedMode);
    UGH("Display Options.FullScreen", nwnFullScreen);
    UGH("Display Options.Disable Intro Movies", nwnDisableIntroMovies);
    UGH("Display Options/Disable Movies", nwnDisableMovies);
#undef UGH
}

void NeverrunSettingsDialog::loadNWNPatchIniSettings() {
    boost::property_tree::ptree pt;
    try {
        boost::property_tree::ini_parser::read_ini((options_->getNWNPath() + "/nwnpatch.ini").toStdString(),
                                                   pt);
    }
    catch(...) {
        return;
    }

    boost::optional<std::string> thing;
    for(int i = 0; i < 100; ++i) {
        QString entry = QString("Patch.PatchFile%1").arg(i, 3, 10, QChar('0'));
        thing = pt.get_optional<std::string>(entry.toStdString());
        if (!thing) { break; }
        if ((*thing).size() == 0) { continue; }
        ui->nwnPatchFiles->addItem(QString::fromStdString(*thing));
    }
}

void NeverrunSettingsDialog::loadNWNControlOptions(const boost::property_tree::ptree &nwn_settings) {
    boost::optional<int> val;
    val = nwn_settings.get_optional<int>("Control Options.Enable ScreenEdgeCameraTurn");
    ui->nwnEnableScreenEdgeCameraTurn->setChecked(val ? *val : 0);
    val = nwn_settings.get_optional<int>("Control Options.EnableDialogZoom");
    ui->nwnEnameDialogZoom->setChecked(val ? *val : 0);
}


#define LOAD_COLOR(section, entry, def_color) \
    variant = nwn_settings.get_optional<std::string>(section "." #entry); \
    cs = QString::fromStdString(variant ? *variant : def_color).split(','); \
    c = QColor(cs[0].toInt(), cs[1].toInt(), cs[2].toInt()); \
    ui->nwn##entry##Btn->setStyleSheet(getBackgroundColorSS(c)); \
    ui->nwn##entry##Text->setText(getBackgroundColorText(c)); \
    ui->nwn##entry##Text->setReadOnly(true)

void NeverrunSettingsDialog::loadNWNChatColorSettings(const boost::property_tree::ptree &nwn_settings) {
    boost::optional<std::string> variant;
    QStringList cs;
    QColor c;

    LOAD_COLOR("Chat Colors", ShoutColor, "255,239,80");
    LOAD_COLOR("Chat Colors", TalkColor, "240,240,240");
    LOAD_COLOR("Chat Colors", WhisperColor, "128,128,128");
    LOAD_COLOR("Chat Colors", TellColor, "32,255,32");
    LOAD_COLOR("Chat Colors", ServerColor, "176,176,176");
    LOAD_COLOR("Chat Colors", PartyColor, "255,102,1");
    LOAD_COLOR("Chat Colors", DMColor, "16,223,255");
}

void NeverrunSettingsDialog::loadNWNTrapColorSettings(const boost::property_tree::ptree &nwn_settings) {
    boost::optional<std::string> variant;
    QStringList cs;
    QColor c;

    LOAD_COLOR("Trap Colors", FriendlyColor, "0,255,0");
    LOAD_COLOR("Trap Colors", HostileColor, "255,0,0");
}

#undef LOAD_COLOR


#define SET_COLOR(section, entry) \
    if(loading_) { return; } \
    auto file = (options_->getNWNPath() + "/nwnplayer.ini").toStdString();\
    boost::property_tree::ptree pt; \
    try { boost::property_tree::ini_parser::read_ini(file, pt); } catch(...) { return; }\
    QColor color = getBackgroundColor(getBackgroundColorFromString(ui->nwn##entry##Text->text())); \
    QString c = getBackgroundColorText(color); \
    if(color.isValid()) { \
        ui->nwn##entry##Text->setText(c); \
        ui->nwn##entry##Btn->setStyleSheet(getBackgroundColorSS(color)); \
    } \
    pt.put(section "." #entry, c.toStdString()); \
    boost::property_tree::ini_parser::write_ini(file, pt)

void NeverrunSettingsDialog::on_nwnShoutColorBtn_clicked(){
    SET_COLOR("Chat Colors", ShoutColor);
}

void NeverrunSettingsDialog::on_nwnTalkColorBtn_clicked() {
    SET_COLOR("Chat Colors", TalkColor);
}

void NeverrunSettingsDialog::on_nwnTellColorBtn_clicked() {
    SET_COLOR("Chat Colors", TellColor);
}

void NeverrunSettingsDialog::on_nwnWhisperColorBtn_clicked() {
    SET_COLOR("Chat Colors", WhisperColor);
}

void NeverrunSettingsDialog::on_nwnServerColorBtn_clicked() {
    SET_COLOR("Chat Colors", ServerColor);
}

void NeverrunSettingsDialog::on_nwnPartyColorBtn_clicked() {
    SET_COLOR("Chat Colors", PartyColor);
}

void NeverrunSettingsDialog::on_nwnDMColorBtn_clicked() {
    SET_COLOR("Chat Colors", DMColor);
}

void NeverrunSettingsDialog::on_nwnFriendlyColorBtn_clicked() {
    SET_COLOR("Trap Colors", FriendlyColor);
}

void NeverrunSettingsDialog::on_nwnHostileColorBtn_clicked() {
    SET_COLOR("Trap Colors", HostileColor);
}

#undef SET_TRAP_COLOR

void NeverrunSettingsDialog::on_nrBlacklistRemoveBtn_clicked() {
    QList<QListWidgetItem*> selected = ui->nrBlacklist->selectedItems();
    foreach(QListWidgetItem* s, selected ) {
        options_->removeFromBlacklist(s->text());
    }
    qDeleteAll(selected);
}

void NeverrunSettingsDialog::on_nwnDifficultyLevelCB_currentIndexChanged(int index) {
    SET_NWPLAYER_INI_VALUE("Game Options.Difficulty Level", index);
}

void NeverrunSettingsDialog::on_nwnClientPortSB_valueChanged(int val) {
    SET_NWPLAYER_INI_VALUE("Profile.Client Port", val);
}

void NeverrunSettingsDialog::on_nwnLogChat_toggled(bool checked) {
    SET_NWPLAYER_INI_VALUE("Game Options.ClientChatLogging", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_nwnLogEntireChat_toggled(bool checked) {
    SET_NWPLAYER_INI_VALUE("Game Options.ClientEntireChatWindowLogging", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_nwnHideSecondStoryTilesCB_toggled(bool checked) {
    SET_NWPLAYER_INI_VALUE("Game Options.Hide SecondStoryTiles", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_nwnVisibleCloaks_toggled(bool checked) {
    SET_NWPLAYER_INI_VALUE("Game Options.Visible Cloaks", checked ? 1 : 0);
}

#define SET_NWN_INI_VALUE(entry, val) \
    if(loading_) { return; } \
    auto file = (options_->getNWNPath() + "/nwn.ini").toStdString(); \
    boost::property_tree::ptree pt; \
    try { boost::property_tree::ini_parser::read_ini(file, pt); } catch(...) { return; }\
    pt.put(entry, val);\
    boost::property_tree::ini_parser::write_ini(file, pt)

void NeverrunSettingsDialog::on_nwnFullScreen_toggled(bool checked) {
    SET_NWN_INI_VALUE("Display Options.FullScreen", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_nwnAllowWindowedMode_toggled(bool checked) {
    SET_NWN_INI_VALUE("Display Options.AllowWindowedMode", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_nwnDisableMovies_toggled(bool checked) {
    SET_NWN_INI_VALUE("Display Options.Disable Movies", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_nwnDisableIntroMovies_toggled(bool checked) {
    SET_NWN_INI_VALUE("Display Options.Disable Intro Movies", checked ? 1 : 0);
}

//#undef SET_NWN_INI_VALUE

//#undef SET_NWPLAYER_INI_VALUE

void NeverrunSettingsDialog::on_nwnEnameDialogZoom_toggled(bool checked) {
    SET_NWPLAYER_INI_VALUE("Control Options.EnableDialogZoom", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_nwnEnableScreenEdgeCameraTurn_toggled(bool checked) {
    SET_NWPLAYER_INI_VALUE("Control Options.Enable ScreenEdgeCameraTurn", checked ? 1 : 0);
}

void NeverrunSettingsDialog::on_nwnPatchAddBtn_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Add Hak to nwnpatch.ini",
                                                    options_->getNWNPath() + "/hak",
                                                    "Hak File (*.hak)");
    if (fileName.size() == 0) {
        return;
    }

    QString base = QFileInfo(fileName).baseName();
    ui->nwnPatchFiles->addItem(base);

    boost::property_tree::ptree pt;
    QString file = (options_->getNWNPath() + "/nwnpatch.ini");
    try {
        boost::property_tree::ini_parser::read_ini(file.toStdString(), pt);
    }
    catch(...) {
        // Log the fact.
    }

    QString entry;
    boost::optional<std::string> thing;
    for(int i = 0; i < 100; ++i) {
        entry = QString("Patch.PatchFile%1").arg(i, 3, 10, QChar('0'));
        thing = pt.get_optional<std::string>(entry.toStdString());
        if (!thing) { break; }
        if((*thing).size() == 0) {
            pt.put(entry.toStdString(), base.toStdString());
            boost::property_tree::ini_parser::write_ini(file.toStdString(), pt);
            return;
        }
    }
    if(entry.size() > 0) {
        pt.put(entry.toStdString(), base.toStdString());
        boost::property_tree::ini_parser::write_ini(file.toStdString(), pt);
    }
}

void NeverrunSettingsDialog::on_nwnPatchRemoveBtn_clicked() {
    boost::property_tree::ptree pt;
    QString file = (options_->getNWNPath() + "/nwnpatch.ini");
    try {
        boost::property_tree::ini_parser::read_ini(file.toStdString(), pt);
    }
    catch(...) {
        // Log the fact.
    }
    qDeleteAll(ui->nwnPatchFiles->selectedItems());

    QString entry;
    boost::optional<std::string> thing;
    // Erase all patch files...
    for(int i = 0; i < 100; ++i) {
        entry = QString("Patch.PatchFile%1").arg(i, 3, 10, QChar('0'));
        thing = pt.get_optional<std::string>(entry.toStdString());
        if (!thing) { break; }
        pt.put(entry.toStdString(), "");
    }

    for(int i = 0; ; ++i) {
        QListWidgetItem *it = ui->nwnPatchFiles->item(i);
        if(!it) { break; }
        entry = QString("Patch.PatchFile%1").arg(i, 3, 10, QChar('0'));
        pt.put(entry.toStdString(), it->text().toStdString());
    }
    boost::property_tree::ini_parser::write_ini(file.toStdString(), pt);
}
