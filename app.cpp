#include <QApplication>
#include <QPushButton>
#include <QLayout>
#include <iostream>

#include "src/GUI/HDDWidget.h"
#include "src/GUI/RAMWidget.h"

int main(int argc, char **argv)
{
    // Window initialization
    
    QApplication app(argc, argv);
    QWidget *appWindow = new QWidget;

    QPushButton *button = new QPushButton("Let us see this", appWindow);
    button->setGeometry(20, 10, 30, 30);


    QVBoxLayout *layout = new QVBoxLayout;

    appWindow->setLayout(layout);
    appWindow->resize(800, 600);
    appWindow->show();

    return app.exec();
}
