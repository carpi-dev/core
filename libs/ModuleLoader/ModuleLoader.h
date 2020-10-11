//
// Created by insane on 11.07.20.
//

#ifndef CARPI_QT_MODULELOADER_H
#define CARPI_QT_MODULELOADER_H

#include <QDir>
#include <QPair>
#include <QWidget>
#include <QString>
#include <QLibrary>
#include <QSettings>
#include <QList>


class ModuleLoader {
private:
    QString directory;

    QMap<QString, QLibrary*> libraries;
    QMap<QString, QWidget*> widgets;

public:
    explicit ModuleLoader(const QString& directory);

    ~ModuleLoader();

    static QLibrary* load(const QString& name);
    void loadAll();
    QStringList sortByDependencies(const QStringList& libraries, int maxTries=10);

    template<typename R> static R executeReturn(QLibrary* lib, const QString& functionName);

    static int getDefaultIndex(QLibrary* lib);
    static QString getName(QLibrary* lib);
    static QWidget* getWidget(QLibrary* lib);
    static QStringList getOptions(QLibrary* lib);
    static QStringList getDependencies(QLibrary* lib);
    static bool contains(const QStringList& n1, const QStringList& n2);

    QMap<QString, QWidget*> getWidgets();
    QStringList getNames();
};


#endif //CARPI_QT_MODULELOADER_H
