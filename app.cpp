#include <QApplication>
#include <QPushButton>
#include <QLayout>
#include <iostream>

#include "src/GUI/HDDWidget.h"
#include "src/GUI/RAMWidget.h"
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
				      "HDD_results",
				      &tabs);
    

    RAMWidget *RAMTab = new RAMWidget("RAM_result.csv", &tabs);
    
    tabs.setFocus();
    tabs.addTab(HDDTab, "&HDD");
    tabs.addTab(RAMTab, "&RAM");
    layout.addWidget(&tabs);
    
    appWindow->setLayout(&layout);
    appWindow->resize(width, height);
    appWindow->show();

    app.exec();

    delete HDDTab;
    delete RAMTab;
    
    return 0;
}
