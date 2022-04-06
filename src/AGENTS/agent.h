#pragma once

#include <QWidget>
#include <QtWidgets>
#include <QDir>
#include <QFile>
#include <QProcess>

class Agent : public QWidget
{
    Q_OBJECT
    
public:
    Agent(QTextEdit *out, QProgressBar *bar, QWidget *parent = 0);
    ~Agent();

    void start();
    void setFname(QString fName);
    void setOutputFlag(bool needOut);
	    
public slots:
    void parserSendData();
    
private:
    void countAndGetFiles(QDir dir, QStringList &list);
    
private:
    QDir m_root;
    QProcess *m_analyzerProcess;
    QTextEdit *m_output;
    QProgressBar *m_progress;

    bool m_checkHiddenDirs, m_checkHiddenFiles, m_outputFlag;
    unsigned int m_counter;
    QString m_masks, m_resultFName;
};
