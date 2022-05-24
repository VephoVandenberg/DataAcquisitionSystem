#pragma once

#include <QWidget>
#include <QTimer>
#include <QtWidgets>
#include <QProcess>


class NETWidget : public QWidget
{
    Q_OBJECT

public:
    NETWidget(QString currentFName, QWidget *parent);
    ~NETWidget();
    
    void compliteProgress(int end);
    void cleanUp();
    void parserFinished();

public slots:
    void changedProfiles(QString profile);
    void startBtnClicked();
    void radioChkClicked();
    void parserSendData();
    void dumperProcessError(QProcess::ProcessError error);

private:
    int m_currentEdge;
    QString m_resultFile, m_tempFile;
    QProcess m_dumper;
    QMap<QString, QString> m_profiles; 

    QGridLayout m_grid;
    QPushButton m_start, m_browse;
    QCheckBox m_tcpCons, m_udpCons, m_unixCons, m_saveTemp;
    QRadioButton m_all, m_listening, m_statistic;
    QProgressBar m_progress;
    QLabel m_label1;
    QLineEdit m_lFName;
    QTextEdit m_output;

    QString m_currentLinuxProfile;

    int m_margin = 10;
    int m_spacing = 15;
};
