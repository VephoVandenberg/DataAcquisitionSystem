#include <iostream>
#include <unistd.h>
#include <QTextStream>
#include "NETWidget.h"


NETWidget::NETWidget(QString currentFName, QWidget *parent) :
    QWidget(parent), m_label1("Report file"),
    m_start("Start"), m_tcpCons("TCP connections"), m_udpCons("UDP connections"), m_unixCons("UNIX connections"), m_all("All ports"), m_listening("All listening ports"), m_statistic("Summary statistic"), m_saveTemp("Save temp files"),  m_dumper(this)
{
    
    m_progress.setValue(0);
    m_output.setReadOnly(true);

    m_grid.setMargin(m_margin);
    m_grid.setSpacing(m_spacing);
    m_grid.addWidget(&m_label1, 0, 0);
    m_grid.addWidget(&m_lFName, 0, 1, 1, 2);
    m_grid.addWidget(&m_start, 3, 1, 1, 2);
    m_grid.addWidget(&m_tcpCons, 1, 0);
    m_grid.addWidget(&m_udpCons, 1, 1);
    m_grid.addWidget(&m_unixCons, 1, 2);
    m_grid.addWidget(&m_all, 2, 0);
    m_grid.addWidget(&m_listening, 2, 1);
    m_grid.addWidget(&m_statistic, 2, 2);
    m_grid.addWidget(&m_saveTemp, 3, 0);
    m_grid.addWidget(&m_output, 0, 3, 5, 3);
    m_grid.addWidget(&m_progress, 4, 0, 1, 3);
    
    m_start.setEnabled(false);
    
    this->setLayout(&m_grid);
       
    m_lFName.setText(currentFName);
    
    connect(&m_start, SIGNAL(clicked(bool)), SLOT(startBtnClicked()));
    
    connect(&m_all, SIGNAL(clicked(bool)), SLOT(radioChkClicked()));
    connect(&m_listening, SIGNAL(clicked(bool)), SLOT(radioChkClicked()));
    connect(&m_statistic, SIGNAL(clicked(bool)), SLOT(radioChkClicked()));
    
    connect(&m_dumper, SIGNAL(errorOccurred(QProcess::ProcessError)), SLOT(dumperProcessError(QProcess::ProcessError)));
    connect(&m_dumper, SIGNAL(readyReadStandardOutput()), SLOT(parserSendData()));

}

NETWidget::~NETWidget()
{
}

void NETWidget::compliteProgress(int end)
{
    for (int iterator = m_progress.value(); iterator <= end; iterator++)
    {
	m_progress.setValue(iterator);
    }
}


void NETWidget::cleanUp()
{
    m_dumper.start("sudo rm build/analysisResults/NET/*temp.csv");
}

void NETWidget::changedProfiles(QString profile)
{
    m_dumper.start("sudo rm build/analysisResults/NET*temp.csv");
}

void NETWidget::startBtnClicked()
{
    m_output.clear();
    
    m_start.setEnabled(false);
    m_tcpCons.setEnabled(false);
    m_udpCons.setEnabled(false);
    m_unixCons.setEnabled(false);
    m_lFName.setEnabled(false);

    m_progress.setRange(0, 100);
    
    m_output.clear();

    QString resultFile = QDir::currentPath() + "/build/analysisResults/NET/" + m_lFName.text() + QDateTime::currentDateTime().toString("hh.mm.ss_dd.MM.yyyy") + ".csv";

    m_resultFile = resultFile;
    
    QString tcpF = m_tcpCons.isChecked() ? "t" : "";
    QString udpF = m_udpCons.isChecked() ? "u" : "";
    QString unixF = m_unixCons.isChecked() ? "x" : "";
    
    QString format;

    if (m_all.isChecked())
    {
	format = "-a";
    }
    else if (m_listening.isChecked())
    {
	format = "-l";
    }
    else
    {
	format = "-s";
    }

    m_tempFile = resultFile.remove(".csv") + "temp.csv";
    m_dumper.setStandardOutputFile(m_tempFile);
    QString command = "sudo netstat " + format + tcpF + udpF + unixF;
    std::cout << command.toStdString() << std::endl;
    m_output.append("Data gathering has been started");
    m_dumper.start(command);
    m_dumper.waitForStarted(5000);
    m_dumper.waitForFinished(5000);

    m_dumper.closeReadChannel(QProcess::StandardOutput);

    m_currentEdge = 50;
    compliteProgress(m_currentEdge);
    //
    sleep(2);
    
    m_output.append("CSV file is being generated");
    
    if (m_all.isChecked())
    {
	QString protocolCSV = resultFile.remove(".csv") + "prtocols.csv";
	QString unixCSV = resultFile.remove(".csv") + "unix.csv";
	command = "sudo python3 src/PARSERS/NET_parser.py " + m_tempFile +  " " + protocolCSV + " " + unixCSV + " all" + tcpF + udpF + unixF;

    }
    else if (m_listening.isChecked())
    {
	QString protocolCSV = resultFile.remove(".csv") + "prtocols.csv";
	QString unixCSV = resultFile.remove(".csv") + "unix.csv";
	command = "sudo python3 src/PARSERS/NET_parser.py " + m_tempFile +  " " + protocolCSV + " " + unixCSV + " listen"  + tcpF + udpF + unixF;

    }
    else
    {
	resultFile = resultFile.remove(".csv") + ".csv";
	command = "sudo python3 src/PARSERS/NET_parser.py " + m_tempFile + " " + resultFile + " statistic"  + tcpF + udpF + unixF;

    }

    QProcess pythonProcess;
    pythonProcess.start(command);
    pythonProcess.waitForStarted();
    pythonProcess.waitForFinished();

    m_currentEdge = 100;
    compliteProgress(m_currentEdge);

    parserFinished();


}


void NETWidget::radioChkClicked()
{
    if (m_all.isChecked())
    {
	m_start.setEnabled(true);
	m_unixCons.setEnabled(true);
	m_udpCons.setEnabled(true);
    }
    else if (m_statistic.isChecked())
    {
	m_start.setEnabled(true);
	m_unixCons.setEnabled(false);
	m_udpCons.setEnabled(true);
    }
    else
    {
	m_start.setEnabled(true);
	m_udpCons.setEnabled(false);
	m_unixCons.setEnabled(true);
    }
}

void NETWidget::parserFinished()
{
    m_output.append("Parserma successfully finished its job!");

    if (!m_saveTemp.isChecked())
    {
	cleanUp();
    }
    
    m_progress.setValue(0);
    m_start.setEnabled(true);
    m_tcpCons.setEnabled(true);
    m_udpCons.setEnabled(true);
    m_lFName.setEnabled(true);

}

void NETWidget::parserSendData()
{
    QString result = QString(m_dumper.readAll());

    foreach (QString str, result.split('\n'))
    {
	std::cout << str.toStdString() << std::endl;
    }
}

void NETWidget::dumperProcessError(QProcess::ProcessError error)
{
    switch(error)
    {
    case QProcess::Crashed:
    {
	QMessageBox::information(this, "Error",  "Crashed");
    }break;

    case QProcess::FailedToStart:
    {
	QMessageBox::information(this, "Error",  "Failed to start");
    }break;

    case QProcess::UnknownError:
    {
	QMessageBox::information(this, "Error",  "Unknown Error");
    }break;
    }
}
