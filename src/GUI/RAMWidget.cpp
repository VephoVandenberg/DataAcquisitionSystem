#ifdef WIN32
#include <unistd.h>
#else
#include <unistd.h>
#endif
#include <iostream>

#include "RAMWidget.h"

RAMWidget::RAMWidget(QString currentFName, QWidget *parent) :
    QWidget(parent), m_label1("Result file"), m_label2("System profile"),
    m_start("Start"), m_getOnlyDump("Collect only memory dump"), m_saveRawDump("Save dump after RAM data gathering"), m_saveTempFiles("Save temporary files, gathered while collecting the data"), m_dumper(this)
{
    m_timer = new QTimer(this);
    
    m_progress.setValue(0);
    m_output.setReadOnly(true);

    m_grdLayout.setMargin(5);
    m_grdLayout.setSpacing(15);

    m_grdLayout.addWidget(&m_label1, 0, 0, 1, 1);
    m_grdLayout.addWidget(&m_lFName, 0, 1, 1, 2);
    m_grdLayout.addWidget(&m_start, 0, 3, 1, 1);
    m_grdLayout.addWidget(&m_label2, 1, 0, 1, 1);
    m_grdLayout.addWidget(&m_profileComboBox, 1, 1, 1, 3);
    m_grdLayout.addWidget(&m_getOnlyDump, 2, 0, 1, 4);
    m_grdLayout.addWidget(&m_saveRawDump, 3, 0, 1, 4);
    m_grdLayout.addWidget(&m_saveTempFiles, 4, 0, 1, 4);
    m_grdLayout.addWidget(&m_output, 5, 0, 1, 4);
    m_grdLayout.addWidget(&m_progress, 6, 0, 1, 4);

    this->setLayout(&m_grdLayout);
    m_start.setEnabled(false);

    m_profileComboBox.addItems(getProfiles());

    m_lFName.setText(currentFName);

    connect(&m_start, SIGNAL(clicked(bool)), SLOT(startBtnClicked()));
    connect(&m_profileComboBox, SIGNAL(currentTextChanged(QString)), SLOT(changedProfiles(QString)));
    connect(&m_getOnlyDump, SIGNAL(clicked(bool)), SLOT(dumpChkClicked()));
    connect(&m_dumper, SIGNAL(errorOccurred(QProcess::ProcessError)), SLOT(dumperProcessError(QProcess::ProcessError)));
    connect(&m_dumper, SIGNAL(readyReadStandardOutput()), SLOT(parserErrorOccured(QProcess::ProcessError)));
}

RAMWidget::~RAMWidget()
{
    delete m_timer;
}

void RAMWidget::compliteProgress(int end)
{
    for (int iterator = m_progress.value(); iterator <= end; iterator++)
    {
	#ifdef WIN32
	_sleep((unsigned)100);
	#else
	sleep((unsigned)100);
	#endif
	m_progress.setValue(iterator);
    }
}

QStringList RAMWidget::getProfiles()
{

    m_profiles.insert("", "");
    QFile f("build/profiles");
    
    if (!f.open(QFile::ReadOnly))
    {
	QMessageBox::critical(this, "Error", "Cannot load the list of profiles from file (" + f.fileName() + ").\n"
			      "Try to reinstall FDAS and run srcipt mem/getProfiles.py\n"
			      "Module of memory scanning shall not work");

	return m_profiles.keys();
    }
    else
    {
	while (!f.atEnd())
	{
	    QString buf = f.readLine();
	    buf.chop(1);
	    m_profiles.insert(buf.split("- A Profile for")[1], buf.split("- A Profile for")[0]);
	}
	f.close();
	return m_profiles.keys();
    }
    
}

void RAMWidget::cleanUp(QString dir)
{
    
    QDir dirHandler(dir);
    QStringList list = dirHandler.entryList(QDir::Files);

    // Delete files

    foreach(QString fileName, list)
    {
        if(m_saveRawDump.isChecked())
	{
	    if(fileName.endsWith(".img"))
	    {
		continue;
	    }
	}
	
        if(m_saveTempFiles.isChecked())
	{
	    if(fileName.endsWith(".tmp"))
	    {
		continue;
	    }
	}

	
        if(fileName.endsWith(".csv"))
	{
	    continue;
	}
        dirHandler.remove(fileName);
        m_output.append("\tDeleted: " + dir + "/" + fileName);
    }

    // To the depth

    list = dirHandler.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QString newDirectory, list)
    {
        cleanUp(dir + "/" + newDirectory);
        dirHandler.rmdir(newDirectory);
    }
}

void RAMWidget::changedProfiles(QString profile)
{
    if (!m_getOnlyDump.isChecked())
    {
	m_start.setEnabled(!profile.isEmpty());
	std::cout << !profile.isEmpty() << std::endl;
    }
}

void RAMWidget::startBtnClicked()
{
    m_start.setEnabled(false);
    m_getOnlyDump.setEnabled(false);
    m_saveRawDump.setEnabled(false);
    m_saveTempFiles.setEnabled(false);
    m_profileComboBox.setEnabled(false);
    m_lFName.setEnabled(false);

    m_output.clear();
    m_output.append("Dump collection has been started");
    m_output.repaint();
    m_progress.setRange(0, 100);

    QString text = m_lFName.text().replace(' ', '_');
    m_lFName.setText(text + (text.endsWith(".csv") ? "" : ".csv"));
    m_dumpDirectory = "build/analysisResults/RAM" + m_lFName.text().remove(".csv");

    QDir dirHandler(m_dumpDirectory);
    if (!dirHandler.exists())
    {
	dirHandler.mkdir(".");
    }

    // Memorizing dumping4 start
    // connect(&m_dumper, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(dumpComplited(int, QProcess::ExitStatus)));
    ////////////////////////////////////////////////
    m_dumper.start("sh build/memory/getMemoryDump.sh");
    m_dumper.waitForFinished();

    m_timer->setInterval(getLineAmount() * 15);
    if (m_getOnlyDump.isChecked())
    {
	m_currentEdge = 100;
    }
    else
    {
	m_currentEdge = 34;
    }
    connect(m_timer, SIGNAL(timeout()), SLOT(moveProgress()));
    // m_timer->start();
}

void RAMWidget::dumpComplited(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus != QProcess::NormalExit)
    {
	m_output.append("Memorize has finished its task with eror (" + m_dumper.errorString() + "). Error code:" + exitCode);
    }
    else if (m_getOnlyDump.isChecked())
    {
	m_output.append("Memorize successfully finished its job. Dump is saved with adress:" + m_dumpDirectory);
	m_timer->stop();
    }
    else
    {
	compliteProgress(m_currentEdge);
	m_timer->setInterval(getLineAmount() * 10);
	disconnect(&m_dumper, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(dumpComplited(int, QProcess::ExitStatus)));
	connect(&m_dumper, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(parserFinished(int, QProcess::ExitStatus)));
	connect(&m_dumper, SIGNAL(readyRead()), SLOT(parserSendData()));

	// Getting dump's path 

	QStringList list;
        
	QDir dirHandler(m_dumpDirectory);
	QString dumpFName = dirHandler.entryList(QStringList("*.img"), QDir::Files)[0];

	m_output.append("Memorize successfully finished its job. Analysis of temporary files has been started");
	QString params;
	params = "\"../" + m_dumpDirectory + "/" + dumpFName + "\"";
	params += "\"" + m_profiles[m_profileComboBox.currentText()].trimmed() + "\"";
	params += "\"../" + (m_tempFiles = "Analysis_results/ram/" + m_lFName.text().remove(".csv")) +  "/\"";
	params += "\"" + m_lFName.text() + "\"";

	std::cout << m_profiles[m_profileComboBox.currentText()].trimmed().toStdString() << std::endl;
	// m_dumper.start("cmd /C ..\\py\\py.exe MemParser.py" + parameters);
    }
}

void RAMWidget::moveProgress()
{
    int value = m_progress.value();
    if(value != m_currentEdge - 1)
    {
	if(value == m_currentEdge - 15 || value == m_currentEdge - 10)
	{
	    m_timer->setInterval(m_timer->interval() * 1.5 );
	}
	m_progress.setValue(value++);
    }
}

void RAMWidget::dumpChkClicked()
{
    if (m_getOnlyDump.isChecked())
    {
	m_start.setEnabled(false);
	std::cout << "Here" << std::endl;
    }
    else
    {
	changedProfiles(m_profileComboBox.currentText());
	std::cout << "Now here" << std::endl;
    }
}

void RAMWidget::parserFinished(int exCode, QProcess::ExitStatus exStatus)
{
    m_output.append("Deletion of temporary files in " + m_tempFiles + "...");
    cleanUp(m_tempFiles);
    m_output.append("Parser successfully finished its job!");
    QApplication::alert(this);
    // Some job with sende should be performed
    
    QMessageBox::information(this,"RAM_Parser","Analysis has been finished " +
                             (exStatus == QProcess::NormalExit? "with success!" : ("with error:" + m_dumper.errorString())));
    disconnect(&m_dumper, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(parserFinished(int, QProcess::ExitStatus)));
    disconnect(&m_dumper, SIGNAL(readyRead()), this, SLOT(parserSendData()));

    m_lFName.setText(QDateTime::currentDateTime().toString("HH.mm.ss_dd.MM.yyyy") + ".csv"),
    m_progress.setValue(0);
    m_timer->stop();
    m_start.setEnabled(true);
    m_getOnlyDump.setEnabled(true);
    m_saveRawDump.setEnabled(true);
    m_saveTempFiles.setEnabled(true);
    m_profileComboBox.setEnabled(true);
    m_lFName.setEnabled(true);

    std::cout << "Clicked" << std::endl;
}

void RAMWidget::parserSendData()
{
    QString result = QString::fromLocal8Bit(m_dumper.readAll());

    foreach (QString str, result.split('\n'))
    {
	if (str.startsWith("#"))
	{
	    m_output.append(str.mid(1, str.size() - 2));
	    compliteProgress(m_currentEdge);
	    m_currentEdge += 6;
	}
	else
	{
	    m_output.append(str);
	    m_output.repaint();
	}
    }
}

int RAMWidget::getLineAmount()
{
    int lA = 4 * 60;
    if (m_getOnlyDump.isChecked())
    {
	return lA;
    }
    lA += 10 * (5 * 60);
    return lA + 5 * 60;
}

void RAMWidget::dumperProcessError(QProcess::ProcessError error)
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
