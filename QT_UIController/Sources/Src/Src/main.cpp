/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

main

***************************************************************************/
#include <QtWidgets/QApplication>
#include <QGuiApplication>
#include <QList>
#include <QQmlApplicationEngine>
#include <Header/informationpanel.h>
#include <QQmlContext>
#include <Header/dashboard.h>
#include <Header/login.h>
#include <Header/sysconfigdata.h>
#include <Header/recipe.h>
#include <QDate>
#include <QTime>
#include <QTranslator>
#include "Header/recipeparameter.h"

#include "Header/hornscandata.h"
#include "Header/hornrecipedata.h"
#include "Header/historydata.h"
#include "Header/hornresultdata.h"
#include "Header/eventrecord.h"
#include "Header/eventhandler.h"
#include <iostream>
#include <QtPlatformHeaders/QWindowsWindowFunctions>
#include "Header/productionsetupinfo.h"
#include "Header/WeldGraph.h"
#include "Header/RecipeInformation.h"
#include "Header/Production.h"
#include "Header/usermodel.h"
#include "Header/configurationmanagement.h"
#include "Header/authoritylevelmanagement.h"

#include "Header/customvalidator.h"
#include "Header/errorhandling.h"
#include "Header/menuhandler.h"
#include "Header/helperenums.h"
#include "Header/globalsettings.h"
#include "Header/graphtableview.h"
#include "Header/init.h"
#include "Header/ultrasonictest.h"
#include "Header/checkbox.h"
#include "Header/configuration.h"
#include "Header/splash.h"
#include "Header/alarmnotification.h"
#include "Header/alarmfromsc.h"
#include "Header/seekscandata.h"
#include "Header/seekrecipedata.h"
#include "Header/seekhistorydata.h"
#include "Header/seekresultdata.h"
#include "Header/stackrecipedata.h"
#include "Header/ipconfiguration.h"
#include <Header/graph.h>
#include "Header/alarmconfiguration.h"
#include "Header/machinedetails.h"
#include "Header/leftmenumodel.h"
#include "Header/customeventfilter.h"
#include "Header/actuatorcalibration.h"
#include "Header/userio.h"
#include "Header/authorityoptionmodel.h"
#include "Header/firmwareupgrade.h"
#include "Header/analyticsalarm.h"
#include "Header/trendsgraph.h"
#include "Header/analyticsproduction.h"
#include "Header/horndown.h"
#include "Header/checkbox.h"
#include "Header/datawizard.h"
#include "Header/customcalendar.h"
#include "Header/bransondata.h"
#include "Header/smartstack.h"
#include <QtGlobal>
#include <QMutex>
#include <QObject>
#include <QDir>

#include "Header/customchartview.h"
#include "Header/customvalueaxis.h"
#include "Header/customxyseries.h"

QQmlContext* context;
static SysConfigData *sysconfig = nullptr;
BransonData *bransonDataObj;
unsigned int psFreqGlobalVar = 0 ;
CommunicationInterface CIObj;
NewPassword *m_NewPassword ;
CustomEventFilter *filter;
Recipe *recipe;
GraphTableView *graphtableview;
int dwCurrentUnit = 0;
UserPrivilege* userPrivilageObj;

Login *login;
EventHandler Eventlogger;

void messageHandler(QtMsgType type,
                    const QMessageLogContext &context,
                    const QString &message)
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyyMMddhhmmss");
    static QMutex mutex;
    mutex.lock();
    static QString fileName = "./logs/" + current_date + "UICLogs.txt";
    QFile file(fileName);
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << qFormatLogMessage(type, context, message)<< "\r\n";
    file.flush();
    file.close();
    mutex.unlock();
}

Eventrecord *record1;
QObject * object;
Production *production;
DashBoard *dashboard;
extern bool loginStatus;
extern bool bAlarmPopupStatusFlag;
extern bool productionPageStatus;
int noOfAlarms;
InformationPanel *PrimaryData;
AlarmNotification *alarmNotify;
AlarmFromSC *alarmDetails;
HornRecipeData *obj_hornRecipeData ;
SeekRecipeData *obj_seekRecipeData ;
UltrasonicTest *obj_ultrasonictest;
checkbox *obj_checkbox;
bool g_bFirmwareUpgradeInProgress = false;
StackRecipeData *obj_stackrecipedata;
HornDown *horndownObj;
UserIO *userIO;
Splash *splash;
QApplication *app;
QTranslator *app_translator;
SmartStack*	obj_smartStack;

static bool s_configImport = qputenv("QML2_IMPORT_PATH", TESTDATA_DIR);
static bool s_configIM = qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

/**
 * @brief qMain
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    //qInstallMessageHandler(messageHandler);
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss:zzz}|%{type}|%{file}|%{function}|%{line}|%{message}");
    qInfo() << "Application started...";
    qmlRegisterUncreatableMetaObject(DataWizardspace::staticMetaObject, "com.branson.wizardsteps", 1, 0, "DataWizardStep", "Access to enums & flags only");

    QString layoutPath(SRC_DIR "/data/layouts");
    qputenv("QT_VIRTUALKEYBOARD_LAYOUT_PATH", qPrintable(layoutPath));

    app = new QApplication(argc, argv);
    app->setWindowIcon(QIcon(":/images/image/Branson_App_Full.ico"));
    QQmlApplicationEngine engine;
    context = engine.rootContext();

    filter = new CustomEventFilter();
    context->setContextProperty("customFilter", filter);

    Configuration *config = Configuration::getInstance();
    Sleep(1000);

    app_translator = new QTranslator();
    config->setSystemLanguage();

    splash = new Splash();
    context ->setContextProperty("config", config);

    sysconfig = SysConfigData::getInstance();
    context->setContextProperty("sysconfig", sysconfig);

    obj_hornRecipeData = new HornRecipeData;

    recipe = new Recipe();

    obj_checkbox = new checkbox("a","b","c","d","e","f",1);

    SetupLimits setupLimit;
    context ->setContextProperty("setupLimitObj", &setupLimit);

    AlarmConfiguration alarmconfig;
    alarmNotify = new AlarmNotification();
    alarmDetails = new AlarmFromSC();

    obj_ultrasonictest = new UltrasonicTest();

    graphtableview = new GraphTableView();
    graphtableview->m_Context=context;

    GlobalSettings objGlobalSettings;
    //objGlobalSettings.handleGlobalSettings();
    userIO = new UserIO();
    context ->setContextProperty(USERIO_CONTEXT_PROPERTY, userIO);
    context ->setContextProperty("qmlGlobalSettingsObject", &objGlobalSettings);
    Graph graph;

    ErrorHandling::registerQmlType();
    Features::registerQmlType();
    HelperEnums::registerQmlType();
    SCStateEnums::registerQmlType();
    OpenedScreenEnums::registerQmlType();
    EventLogging::registerQmlType();
    RuntimeFeatures::registerQmlType();
    FirmwareUpgrade::declareQML();
    AnalyticAlarm analyticalarm;
    DataWizard::registerQmlType();
    DataWizard obj_DataWizard;
    WSRestrictionScreen::registerQmlType();
    DeleteRecipeResponse::registerQmlType();

    context -> setContextProperty("datawizardobject", &obj_DataWizard);
    context -> setContextProperty("analyticalarm", &analyticalarm);

    AnalyticsProdcution analyticsproduction;
    context -> setContextProperty("analyticsproduction",&analyticsproduction);

    alarmNotify->alarmInit();
    context -> setContextProperty("alarmNotify", alarmNotify);

    userPrivilageObj= UserPrivilege::getInstance();

    context ->setContextProperty("userPrivilageObj", userPrivilageObj);

    bransonDataObj = new BransonData;
    context->setContextProperty("bransonDataObj",bransonDataObj);

    TrendsGraph trendsGraphQmlObj;
    context ->setContextProperty("trendsGraphQmlObj", &trendsGraphQmlObj);


    AuthorityOptionModel authorityOptionModelObj;
    context ->setContextProperty("authorityOptionModel", &authorityOptionModelObj);

    Init Initialize;
    context -> setContextProperty("Initialize" , &Initialize);

    record1 = new Eventrecord();
    context -> setContextProperty("userList1Model" , record1);

    LeftMenuData LeftMenuData_obj;
    context->setContextProperty("leftMenuData_obj", &LeftMenuData_obj);

    HistoryData obj_historyData ;

    context -> setContextProperty("Eventlogger" , &Eventlogger);

    SeekHistoryData obj_seekhistoryData ;
    obj_seekRecipeData = new SeekRecipeData();
    context -> setContextProperty("seekhistoryDataObj" , &obj_seekhistoryData);
    context -> setContextProperty("seekRecipeObj" , obj_seekRecipeData);
    QObject::connect(&CIObj , &CommunicationInterface::seekScanMsgData , &obj_seekhistoryData , &SeekHistoryData :: receiveScUiseekReqMsg);

    m_NewPassword = new NewPassword();
    context->setContextProperty("m_NewPassword",QVariant::fromValue(m_NewPassword));

//    obj_checkbox = new checkbox;//("a","b","c","d","e","f",1);
//    obj_checkbox("a","b","c","d","e","f",1);
    obj_checkbox->addcheckboxValue();
    context -> setContextProperty("checkboxObj" , obj_checkbox);

    MachineDetails sysmachinedetails;
    context -> setContextProperty("machineDetails", &sysmachinedetails);

    CustomCalendar customCalendarObj;
    context ->setContextProperty("customCalendarObj", &customCalendarObj);

    qmlRegisterSingletonType(QUrl("qrc:/QmlSource/Style.qml"), "Style", 1, 0, "Style");

    context -> setContextProperty("historyDataObj" , &obj_historyData);
    context -> setContextProperty("hornRecipeObj" , obj_hornRecipeData);
    PrimaryData = new InformationPanel();
    context->setContextProperty("PrimaryData",PrimaryData);
    obj_stackrecipedata = new StackRecipeData();
    context -> setContextProperty("StackRecipeObj", obj_stackrecipedata);

    /* Below list will be filled from the DB and this data will be sent to Recipe parameter model */
    context->setContextProperty("recipe",recipe);
    QObject::connect(&CIObj , &CommunicationInterface::updateWeldHistory , recipe , &Recipe :: updateWeldHistory);
    QObject::connect(&CIObj , &CommunicationInterface::barcodeRecipeRecall , recipe , &Recipe ::recallRecipeByBarcode);
    QObject::connect(&CIObj , &CommunicationInterface::barcodeRecipeRecallAuthCheck , recipe , &Recipe ::getRecipeRecallAuthCheck);
    QObject::connect(&CIObj , &CommunicationInterface::barcodescannerconnectionstatus , recipe , &Recipe ::barcodeConnectionStatus);
    QObject::connect(&CIObj , &CommunicationInterface::barcodeProcessStatus ,recipe , &Recipe :: barcodeProcessInfo);
    QObject::connect(&CIObj , &CommunicationInterface::weldDataDeleteInd , recipe , &Recipe :: weldDataDeleteIndication);

    dashboard = new DashBoard();
    context->setContextProperty("dashboard",dashboard);
    login = new Login();
    context->setContextProperty("login",login);
    horndownObj = new HornDown();
    context->setContextProperty("horndownObj",horndownObj);
    RecipeInformation recipeinfo;
    context->setContextProperty("recipeinfo",&recipeinfo);

    obj_smartStack = new SmartStack();
    context->setContextProperty("obj_smartStack" , obj_smartStack);

    ProductionSetUpInfo recipeSetupInfo;
    context->setContextProperty("recipeSetupInfo",&recipeSetupInfo);

    UserManagement userMangementObj;
    context->setContextProperty("userMangementObj", &userMangementObj);

    production = new Production();
    context->setContextProperty("production",production);

    sysconfig->context=context;
    QObject::connect(&CIObj , &CommunicationInterface::generateReportRes , sysconfig , &SysConfigData::generateReportResPopup);
    QObject::connect(&CIObj , &CommunicationInterface::dBStorageStatusRes ,sysconfig , &SysConfigData::dBMemoryStatusPopup);
    QObject::connect(&CIObj , &CommunicationInterface::vacummReportRes , sysconfig , &SysConfigData::vacuumReportResPopup);
    WeldGraphData obj_weldGraph;

    context->setContextProperty("weldGraphObj" , &obj_weldGraph);
    qmlRegisterType<CustomChartView>("myCustomChartView", 19, 1, "CustomChartView");
    qmlRegisterType<CustomXYSeries>("myCustomXYSeries", 19, 1, "CustomXYSeries");
    qmlRegisterType<CustomValueAxis>("myCustomValueAxis", 19, 1, "CustomValueAxis");
    //horndownObj->context=context;
    QObject::connect(&CIObj , &CommunicationInterface::setupIndication , horndownObj , &HornDown::UpdateStructureFromBL);

    IpConfiguration ipConfigObj;
    context->setContextProperty("ipConfigObj" , &ipConfigObj);



    QObject::connect(&CIObj , &CommunicationInterface::hornScanMsgData , &obj_historyData , &HistoryData :: receiveScUiReqMsg);

    qmlRegisterType<CustomValidator>("CustomValidator", 1,0,"CustomValidator");


    ActuatorCalibration actuatorCalibrationItem;
    //    auto timeOutSetInMin = (objGlobalSettings.idealLogoffTime()).toInt();
    //    auto timeOutInMilliseconds = 60000 * timeOutSetInMin;
    //    filter.setTimeOut(timeOutInMilliseconds);
    app->installEventFilter(filter);

    context->setContextProperty("actuatorCalibrationObject",&actuatorCalibrationItem);

    FirmwareUpgrade firmwareUpgradeTask;
    context->setContextProperty("firmwareUpgradeObject",&firmwareUpgradeTask);

    engine.load(QUrl(QStringLiteral("qrc:/QmlSource/main.qml")));
    object = engine.rootObjects().value(0);

    QWindowsWindowFunctions::setHasBorderInFullScreen((QWindow *)engine.rootObjects().first(),true);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app->exec();
}

void AlarmFromSC::emitKey(Qt::Key key)
{
    QObject* receiver = QGuiApplication::focusObject();
    if(!receiver)
    {
        return;
    }
    QKeyEvent pressEvent = QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier, QKeySequence(key).toString());
    QKeyEvent releaseEvent = QKeyEvent(QEvent::KeyRelease, key, Qt::NoModifier);
    QCoreApplication::sendEvent(receiver, &pressEvent);
    QCoreApplication::sendEvent(receiver, &releaseEvent);
}

bool Recipe::getRecipeRecallAuthCheck(QString sRecvBuff)
{
    Q_UNUSED(sRecvBuff)
    bool bValidUser = false;
    //bValidUser = (loginStatus == true);
    //below line ensures user is logge in and also he is operator// still barcode previlige has to be checked.
    bValidUser = (login->getUserLevel().length() > 0 ); //any user logged in
    if(login->getUserLevel()== "Operator")
    {
        bValidUser =  userPrivilageObj->isPermissionAllowed(4, Features::BARCODE_RECIPE_RECALL);
    }
    QString strResponse = QString::number(bValidUser);

    //CIObj.sendMessage(UIC_BARCODE_RECIPE_RECALL_AUTHENTICATION_CHECK_REQ,SCBL_BARCODE_RECIPE_RECALL_AUTHENTICATION_CHECK_RES,strResponse);
    CIObj.sendMessage(SCBL_BARCODE_RECIPE_RECALL_AUTHENTICATION_CHECK_RES,UIC_BARCODE_RECIPE_RECALL_AUTHENTICATION_CHECK_REQ,strResponse);

    return bValidUser;
}
