#include <QApplication>
#include <QPushButton>
#include <QLayout>
#include <iostream>

#include "src/GUI/HDDWidget.h"
#include "src/GUI/RAMWidget.h"
#include "src/AGENTS/agent.h"

const int width = 800;
const int height = 300;

int main(int argc, char **argv)
{
    // Window initialization
    
    QApplication app(argc, argv);
    QWidget *appWindow = new QWidget;

    QVBoxLayout layout;
    QTabWidget tabs;
    
    HDDWidget *HDDTab = new HDDWidget("/home/", "*",
				      "HDD_results",
				      &tabs);
    tabs.setFocus();
    tabs.addTab(HDDTab, "&HDD");

    layout.addWidget(&tabs);
    
    appWindow->setLayout(&layout);
    appWindow->resize(width, height);
    appWindow->show();

    app.exec();

    return 0;
}
