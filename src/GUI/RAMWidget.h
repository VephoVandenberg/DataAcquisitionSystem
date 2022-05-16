#pragma once

#include <QWidget>
#include <QTimer>
#include <QtWidgets>
#include <QProcess>


class RAMWidget : public QWidget
{
    Q_OBJECT

public:
    RAMWidget(QString currentFName, QWidget *parent);
    ~RAMWidget();
    
    void compliteProgress(int end);
    QStringList getProfiles();
    void cleanUp();
    void parserFinished();

public slots:
    void changedProfiles(QString profile);
    void startBtnClicked();
    void dumpChkClicked();
    void parserSendData();
    void dumperProcessError(QProcess::ProcessError error);
    void choseDirBtnClicked();

private:
    int m_currentEdge;
    QString m_resultDirectory, m_tempFiles;
    QTimer *m_timer;
    QProcess m_dumper;
    QMap<QString, QString> m_profiles; 

    QGridLayout m_grid;
    QPushButton m_start, m_browse;
    QComboBox m_profileComboBox;
    QCheckBox m_dumpFromCurrMachine, m_saveRawDump, m_saveTempFiles;
    QRadioButton m_psscan, m_pslist, m_sockscan;
    QProgressBar m_progress;
    QLabel m_label1, m_label2, m_label3;
    QLineEdit m_lFName, m_lDir;
    QTextEdit m_output;

    QString m_currentLinuxProfile;

    int m_margin = 5;
    int m_spacing = 10;

private:
    int getLineAmount();
};
