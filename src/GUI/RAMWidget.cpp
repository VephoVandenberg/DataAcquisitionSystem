#ifdef WIN32
#include <unistd.h>
#else
#include <unistd.h>
#endif

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

    connect(&m_start, SIGNAL(clicked(bool)), SLOT(dumpChkClicked));
    connect(&m_profileComboBox, SIGNAL(currentTextChanged(QString)), SLOT(changedProfiles(QString)));
    connect(&m_getOnlyDump, SIGNAL(clicked(bool)), SLOT(dumpChkClicked()));
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

    // Memorizing dumpint start
    m_dumper.setWorkingDirectory("build/memory");
    connect(&m_dumper, SIGNAL(finished(int, QProcess:ExitStatus)), SLOT(dumpComplited(int exCode, QProcess::ExitStatus)));
    ////////////////////////////////////////////////
    // investigate what is written int the bat file
    #ifdef WIN32
    #else
    #endif
    ////////////////////////////////////////////////
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
    }
    else
    {
	changedProfiles(m_profileComboBox.currentText());
    }
}

void RAMWidget::parserFinished(int exCode, QProcess::ExitStatus exStatus)
{
    
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
