#include "HDDWidget.h"

HDDWidget::HDDWidget(QString path, QString mask, QString frame, QWidget *parent = 0) :
    QWidget(parent)
{
    watcher = new QFileSystemWatcher(this);
    watcher->addPath(path);
    watcher->setObjectName("Watcher");

    
}

HDDWidget::~HDDWidget()
{
    
}
