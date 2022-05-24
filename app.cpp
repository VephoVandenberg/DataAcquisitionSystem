
#include <QApplication>
#include <QPushButton>
#include <QLayout>
#include <iostream>

#include "src/GUI/HDDWidget.h"
#include "src/GUI/RAMWidget.h"
#include "src/GUI/NETWidget.h"
#include "src/AGENTS/agent.h"

const int width = 1000;
const int height = 400;

int main(int argc, char **argv)
{
    // Window initialization
    
    QApplication app(argc, argv);
    QWidget *appWindow = new QWidget;

    QVBoxLayout layout;
    QTabWidget tabs;
    
    HDDWidget *HDDTab = new HDDWidget("/home/", "*",
				      "HDD_result",
				      &tabs);
    

    RAMWidget *RAMTab = new RAMWidget("RAM_result", &tabs);

    NETWidget *NETTab = new NETWidget("NET_results", &tabs);
    
    tabs.setFocus();
    tabs.addTab(HDDTab, "&HDD");
    tabs.addTab(RAMTab, "&RAM");
    tabs.addTab(NETTab, "&NET");
    layout.addWidget(&tabs);
    
    appWindow->setLayout(&layout);
    appWindow->resize(width, height);
    appWindow->show();

    app.exec();

    delete HDDTab;
    delete RAMTab;
    delete NETTab;
    
    return 0;
}
