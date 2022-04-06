#pragma once

#include <QWidget>
#include <QtWidgets>
#include <QPushButton>
#include <QGridLayout>

class HDDWidget : public QWidget
{
    Q_OBJECT
    
public:
    HDDWidget(QString path, QString mask, QString frame, QWidget *parent = 0);
    ~HDDWidget();

public slots:
    void startBtnClicked();
    void choseDirBtnClicked();
    void dirChanged();
    void outputChkBoxClicked();
    void dataCollectionFinished(QString status);
    
private:
    QFileSystemWatcher m_watcher;
    QGridLayout m_grid;
    QPushButton m_confirmDir;
    QPushButton m_start;
    QProgressBar m_progress;
    QTextEdit m_list;
    QLabel m_label1, m_label2, m_label3;
    QLineEdit m_lDir, m_lMask, m_lFName;
    QPalette m_editText;
    QCheckBox m_needOutput, m_needHiddenFiles, m_needHiddenDirs;
    
    int m_margin = 5;
    int m_spacing = 10;
};
