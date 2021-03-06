//
// Created by insane on 12.07.20.
//

#include "Manager.h"

Manager::Manager() {
    settings = new QSettings("carpi", "manager");
    loader = new ModuleLoader(settings->value(KEY_MODULE_DIRECTORY).toString());
    //Logger::debug("Manager", "configuration file is here: " + settings->fileName());

    mainWindow = new MainWindow();
    vTabWidget = new VTabWidget();
    setupUI();
}

Manager::~Manager() {
    delete vTabWidget;
    delete loader;
    delete mainWindow;
    delete settings;
}

void Manager::setupUI() {
    if(Utils::isPi()) {
        //Logger::debug("Manager", "running on raspberry pi");
        //Logger::info("Manager", "setting fullscreen");
        mainWindow->setWindowState(Qt::WindowFullScreen);
        //Logger::info("Manager", "disabling cursor");
        mainWindow->setCursor(Qt::BlankCursor);
    }

    loader->loadAll();

    attachTabs();
    vTabWidget->show();

    mainWindow->setCentralWidget(vTabWidget);
    mainWindow->show();
}

void Manager::attachTabs() {
    //Logger::debug("Manager", "attaching tabs");
    const auto& widgets = loader->getWidgets();
    for(const auto& p : widgets.keys()) {
        if(settings->value(p, true).toBool()){
            //Logger::debug("Manager", "attaching '" + p->second + "'");
            vTabWidget->addTab(widgets.value(p), p);
        }
    }
}

void Manager::setDefaultSettings() {
    if(settings->contains(KEY_GROUP_GENERAL)) return;
    // Logger::debug("Manager", "setting default settings");
    settings->beginGroup(KEY_GROUP_GENERAL);
#ifdef CARPI_DEBUG
    settings->setValue(KEY_MODULE_DIRECTORY, "./");
#else
    settings->setValue(KEY_MODULE_DIRECTORY, "/usr/local/lib/carpi/");
#endif
}
