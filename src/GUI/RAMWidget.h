#pragma once

#include <QWidget>
#include <QTimer>
#include <QtWidgets>



class RAMWidget : public QWidget
{
    Q_OBJECT

public:
    RAMWidget(QString currentFName, QWidget *parent);
    ~RAMWidget();
    
    void compliteProgress( int fin );
    QStringList getProfiles();
    void cleanUp( QString dir);

public slots:
    void changedProfiles(QString profile);
    void startBtnClicked();
    void dumpComplited(int exCode, QProcess::ExitStatus exStatus);
    void moveProgress();
    void dumpChkClicked();
    void parserFinished(int exCode, QProcess::ExitStatus exStatus);
    void parserSendData();


private:
    int m_currentEdge;
    QString m_dumpDirectory, m_tempFiles;
    QTimer *m_timer;
    QProcess m_dumper;
    QMap<QString, QString> m_profiles;

    QGridLayout m_grdLayout;
    QPushButton m_start;
    QComboBox m_profileComboBox;
    QCheckBox m_getOnlyDump, m_saveRawDump, m_saveTempFiles;
    QProgressBar m_progress;
    QLabel m_label1, m_label2;
    QLineEdit m_lFName;
    QTextEdit m_output;

private:
    int getLineAmount();
};
