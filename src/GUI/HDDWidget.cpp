#include "HDDWidget.h"

HDDWidget::HDDWidget(QString path, QString mask, QString frame, QWidget *parent) :
    QWidget(parent), m_watcher(this), m_label1("Chose directory"), m_label2("Enter mask"),
    m_confirmDir("Browse"), m_lDir(path), m_lMask(mask), m_lFName(frame)
{
    m_watcher.addPath(path);
    m_watcher.setObjectName("HDDWatcher");

    m_list.setReadOnly(true);

    m_grid.setMargin(5);
    m_grid.setSpacing(15);
    m_grid.addWidget(&m_label1, 0, 0);
    m_grid.addWidget(&m_label2, 1, 0);
    m_grid.addWidget(&m_lDir, 0, 1, 1, 2);
    m_grid.addWidget(&m_lMask, 1, 1, 1, 2);
    m_grid.addWidget(&m_lFName, 2, 1, 1, 2);
    m_grid.addWidget(&m_confirmDir, 0, 3);
    m_grid.addWidget(&m_start, 1, 3);
    m_grid.addWidget(&m_progress, 5, 0, 1, 4);
    m_grid.addWidget(&m_list, 4, 0, 1, 4);

    this->setLayout(&m_grid);
    m_start.setFocus();

    m_lDir.setObjectName("lDirEdit");
}

HDDWidget::~HDDWidget()
{
    
}
