//
// Created by insane on 11.07.20.
//

#include "ModuleLoader.h"

ModuleLoader::ModuleLoader(const QString &directory)
{
    this->directory = directory;
}

ModuleLoader::~ModuleLoader()
{
    qDeleteAll(libraries);
    qDeleteAll(widgets);
    libraries.clear();
    widgets.clear();
}

QLibrary* ModuleLoader::load(const QString &name)
{
    auto *lib = new QLibrary(name.toStdString().c_str());
    if(lib->load()) {
        // Logger::debug("ModuleLoader", "loaded '" + name + "'");
        return lib;
    } else {
        // Logger::warning("ModuleLoader", "unable to load '" + name + "'");
        // Logger::error("ModuleLoader", lib->errorString());
    }
    return nullptr;
}

void ModuleLoader::loadAll(){
    QDir dir(this->directory.toStdString().c_str());
    // Logger::debug("ModuleLoader", "loading modules from: " + dir.absolutePath());
    for(const auto& f : dir.entryList(QStringList() << "*.so", QDir::Files))
        libraries.insert(f.split(".")[0].remove("lib"), this->load(dir.absoluteFilePath(f)));

    const auto& lst = sortByDependencies(libraries.keys());
}

QMap<QString, QWidget*> ModuleLoader::getWidgets() {
    return widgets;
}

QString ModuleLoader::getName(QLibrary *lib) {
    return executeReturn<char*>(lib, "getName");
}

QWidget *ModuleLoader::getWidget(QLibrary *lib) {
    return executeReturn<QWidget*>(lib, "create");
}

QStringList ModuleLoader::getDependencies(QLibrary* lib) {
    return executeReturn<QStringList>(lib, "getDependencies");
}

template<typename R>
R ModuleLoader::executeReturn(QLibrary *lib, const QString& functionName) {
    typedef R (*Function)();
    Function f = (Function) lib->resolve(functionName.toStdString().c_str());
    // if(!f) qDebug() << lib->errorString();
    return f();
}

int ModuleLoader::getDefaultIndex(QLibrary *lib) {
    return executeReturn<int>(lib, "getDefaultIndex");
}

QStringList ModuleLoader::getNames() {
    QStringList r;
    for(const auto& p : widgets.keys()) r.append(p);
    return r;
}

QStringList ModuleLoader::getOptions(QLibrary *lib) {
    return executeReturn<QStringList>(lib, "getSettingsKeys");
}

QStringList ModuleLoader::sortByDependencies(const QStringList& libs, int maxTries) {
    QStringList r;
    int tries = 0;
    while(!contains(libs, r) && tries < maxTries){
        for(const auto& k : libraries.keys()){
            auto *m = libraries.value(k);
            const auto& deps = getDependencies(m);
            if(deps.isEmpty() || contains(deps, r)) r.append(k);
            else tries++;
        }
    }
    return r;
}

bool ModuleLoader::contains(const QStringList& n1, const QStringList& n2) {
    for(const auto& n1k : n1){
        if(!n2.contains(n1k)) return false;
    }
    return true;
}
