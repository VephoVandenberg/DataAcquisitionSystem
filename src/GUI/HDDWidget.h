#pragma once

#include <QWidget>
#include <QtWidgets>
#include <QPushButton>

class HDDWidget : public QWidget
{
    Q_OBJECT
    
public:
    HDDWidget(QString path, QString mask, QString frame, QWidget *parent = 0);
    ~HDDWidget() {};
    
private:
    QFileSystemWatcher watcher;
    QGridLayout grid;

    QPushButton confirmDir;
    QPushButton start;

    QProgressBar progress;

    QTextEdit list;

    QLabel label1;
    QLabel label2;
};
