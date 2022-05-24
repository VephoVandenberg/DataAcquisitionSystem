#ifdef WIN32
#include <unistd.h>
#else
#include <unistd.h>
#endif
#include <iostream>

#include "RAMWidget.h"

RAMWidget::RAMWidget(QString currentFName, QWidget *parent) :
    QWidget(parent), m_label1("Report file"),m_label2("Choose memory dump"), m_label3("System profile"),
    m_start("Start"), m_browse("Browse dump"), m_dumpFromCurrMachine("My computer dump"), m_saveRawDump("Save dump"), m_saveTempFiles("Save temporary files"), m_psscan("Scan process objects"), m_pslist("All running processes"), m_sockscan("Scan sockets"),  m_dumper(this)
{
    m_timer = new QTimer(this);
    
    m_progress.setValue(0);
    m_output.setReadOnly(true);

    m_grid.setMargin(m_margin);
    m_grid.setSpacing(m_spacing);
    m_grid.addWidget(&m_label1, 0, 0);
    m_grid.addWidget(&m_label2, 1, 0);
    m_grid.addWidget(&m_label3, 2, 0);
    m_grid.addWidget(&m_lFName, 0, 1, 1, 2);
    m_grid.addWidget(&m_lDir, 1, 1, 1, 2);
    m_grid.addWidget(&m_start, 5, 2, 1, 1);
    m_grid.addWidget(&m_browse, 5, 0, 1, 1);
    m_grid.addWidget(&m_profileComboBox, 2, 1, 1, 2);
    m_grid.addWidget(&m_dumpFromCurrMachine, 3, 0);
    m_grid.addWidget(&m_saveRawDump, 3, 1);
    m_grid.addWidget(&m_saveTempFiles, 3, 2);
    m_grid.addWidget(&m_psscan, 4, 0);
    m_grid.addWidget(&m_pslist, 4, 1);
    m_grid.addWidget(&m_sockscan, 4, 2);
    m_grid.addWidget(&m_output, 0, 3, 7, 3);
    m_grid.addWidget(&m_progress, 6, 0, 1, 3);
    
    m_saveRawDump.setEnabled(false);
    m_start.setEnabled(false);
    m_lDir.setReadOnly(true);
    
    this->setLayout(&m_grid);
    
    m_profileComboBox.addItems(getProfiles());
   
    m_lFName.setText(currentFName);
    
    connect(&m_start, SIGNAL(clicked(bool)), SLOT(startBtnClicked()));
    connect(&m_browse, SIGNAL(clicked()), SLOT(choseDirBtnClicked()));
    
    connect(&m_profileComboBox, SIGNAL(currentTextChanged(QString)), SLOT(changedProfiles(QString)));
    connect(&m_dumpFromCurrMachine, SIGNAL(clicked(bool)), SLOT(dumpChkClicked()));
    connect(&m_lDir, SIGNAL(textChanged(const QString)), SLOT(changedProfiles(QString)));
    connect(&m_dumper, SIGNAL(errorOccurred(QProcess::ProcessError)), SLOT(dumperProcessError(QProcess::ProcessError)));
    connect(&m_dumper, SIGNAL(readyReadStandardOutput()), SLOT(parserSendData()));

}

RAMWidget::~RAMWidget()
{
    delete m_timer;
}

void RAMWidget::compliteProgress(int end)
{
    for (int iterator = m_progress.value(); iterator <= end; iterator++)
    {
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

void RAMWidget::cleanUp()
{
    if (m_dumpFromCurrMachine.isChecked() && !m_saveRawDump.isChecked())
    {
	m_dumper.start("sudo rm build/memory/*.mem");
    }
    else
    {
	if (!m_saveTempFiles.isChecked())
	{
	    m_dumper.start("sudo rm build/analysisResults/RAM/*temp.csv");
	}
    }
	
}

void RAMWidget::changedProfiles(QString profile)
{
    if (!m_profileComboBox.currentText().isEmpty() &&
	!m_lDir.displayText().isEmpty())
    {
	m_profileComboBox.setEnabled(true);
	m_saveRawDump.setEnabled(false);
	m_start.setEnabled(true);
    }
    else
    {
	m_start.setEnabled(false);
    }
}

void RAMWidget::startBtnClicked()
{
    m_output.clear();
    
    m_start.setEnabled(false);
    m_dumpFromCurrMachine.setEnabled(false);
    m_saveRawDump.setEnabled(false);
    m_saveTempFiles.setEnabled(false);
    m_profileComboBox.setEnabled(false);
    m_lFName.setEnabled(false);

    m_progress.setRange(0, 100);
    
    m_output.clear();


    QString resultFile = QDir::currentPath() + "/build/analysisResults/RAM/" + m_lFName.text() + QDateTime::currentDateTime().toString("hh.mm.ss_dd.MM.yyyy") + ".csv";
    std::cout << resultFile.toStdString() << std::endl;
    if (m_dumpFromCurrMachine.isChecked())
    {
	m_output.append("Dump collection has been started. Please wait, dump collection may take some time...");

	QStringList args;
	QString dumpPath = QDir::currentPath() + "/build/memory/";
	std::cout << dumpPath.toStdString() << std::endl;
	args << dumpPath;
	
	m_dumper.start("sudo sh build/memory/getMemoryDump.sh " + dumpPath);
	m_dumper.waitForStarted();
	m_dumper.waitForFinished(180000);
	m_output.append("Memory dump has been collected");
	m_currentEdge = 35;
	compliteProgress(m_currentEdge);

// Get your kernels profile
	m_output.append("Determine profile");
	m_dumper.start("sudo python build/memory/buileKernelProfile.sh");
	m_dumper.waitForStarted(18000);
	m_dumper.waitForFinished();
	m_output.append("Determine profile");
	m_dumper.start("sudo python /opt/volatility/vol.py --info");
	m_dumper.waitForStarted(18000);
	m_dumper.waitForFinished();
	m_currentEdge = 50;
	compliteProgress(m_currentEdge);
// Now Voladility should be started

	QString plugin;
	
	if (m_pslist.isChecked())
	{
	    plugin = "linux_pslist";
	}
	else if (m_sockscan.isChecked())
	{
	    plugin = "linux_sockscan";
	}
	else
	{
	    plugin = "linux_psscan";
	}

	QString tempCSVFile = resultFile.remove(".csv") + "temp.csv";
	QString dumpItself = dumpPath + "myMemory.mem";
// Collectd data from csv file
	QString command = "sudo python /opt/volatility/vol.py --file=" + dumpItself + " --profile=" + m_currentLinuxProfile + " --output-file=" + tempCSVFile + " " + plugin;
	std::cout << command.toStdString() << std::endl;
	m_dumper.start(command);
	m_dumper.waitForStarted();
	m_dumper.waitForFinished();

	m_currentEdge = 69;
	compliteProgress(m_currentEdge);

// Parse csv file
	command = "sudo python3 src/PARSERS/RAM_parser.py " + tempCSVFile + " " + resultFile + ".csv " + plugin;
	std::cout << command.toStdString() << std::endl;
	m_dumper.start(command);
	m_dumper.waitForFinished();
	m_currentEdge = 100;
	compliteProgress(100);
    }
    else
    {
	QString memoryDump = m_lDir.displayText();
	std::cout << memoryDump.toStdString() << std::endl;

	m_output.append("Volatility analysis has been started");
	m_dumper.start("sudo python /opt/volatility/vol.py --file=" + memoryDump  + " imageinfo");
	m_dumper.waitForFinished();
	
	m_currentEdge = 15;
	compliteProgress(m_currentEdge);
	
	QString profile = m_profiles[m_profileComboBox.currentText()];
	QString tempResultFile = resultFile;
	QString tempCSVFile = tempResultFile.remove(".csv") + "temp.csv";
	QString plugin;

	if (m_pslist.isChecked())
	{
	    plugin = "pslist";
	}
	else if (m_sockscan.isChecked())
	{
	    plugin = "sockscan";
	}
	else
	{
	    plugin = "psscan";
	}

// Collectd data from csv file
	QString command = "sudo python /opt/volatility/vol.py --file=" + memoryDump + " --profile=" + profile + " --output-file=" + tempCSVFile + " " + plugin;
	m_dumper.start(command);
	m_dumper.waitForStarted();
	m_dumper.waitForFinished();

	m_currentEdge = 69;
	compliteProgress(m_currentEdge);

// Parse csv file
	command = "sudo python3 src/PARSERS/RAM_parser.py " + tempCSVFile + " " + resultFile + " " + plugin;
	std::cout << command.toStdString() << std::endl;
	m_dumper.start(command);
	m_dumper.waitForFinished();
	m_currentEdge = 100;
	compliteProgress(100);

    }

    parserFinished();
}



void RAMWidget::dumpChkClicked()
{
    if (m_dumpFromCurrMachine.isChecked())
    {	
	m_browse.setEnabled(false);
	m_profileComboBox.setEnabled(false);
	m_browse.setEnabled(false);
	m_lDir.setEnabled(false);
	m_start.setEnabled(true);
	m_saveRawDump.setEnabled(true);
	m_saveTempFiles.setEnabled(false);
    }
    else
    {
	m_profileComboBox.setEnabled(true);
	m_lDir.setEnabled(true);
	m_browse.setEnabled(true);
	m_start.setEnabled(false);
	m_saveRawDump.setEnabled(false);
	m_saveTempFiles.setEnabled(true);

    }
}

void RAMWidget::parserFinished()
{
    cleanUp();
    m_output.append("Parser successfully finished its job!");
  

    m_progress.setValue(0);
    m_start.setEnabled(true);
    m_dumpFromCurrMachine.setEnabled(true);
    m_saveRawDump.setEnabled(false);
    m_saveTempFiles.setEnabled(true);
    m_profileComboBox.setEnabled(true);
    m_lFName.setEnabled(true);

}

void RAMWidget::parserSendData()
{
    QString result = QString::fromLocal8Bit(m_dumper.readAll());

    foreach (QString str, result.split('\n'))
    {
	std::cout << str.toStdString() << std::endl;
        if (str.contains("Suggested Profile(s)"))
	{
	    str.remove('\t');
	    m_output.append(str);
	    
	    QString value = m_profiles[m_profileComboBox.currentText()].remove(' ');
	    if (!str.contains(value))
	    {
		QMessageBox::information(this, "Error",  "In order to get a proper work of system choose on of the suggested profiles - " + str);
		m_progress.setValue(0);

		m_start.setEnabled(true);
		m_profileComboBox.setEnabled(true);
		m_dumpFromCurrMachine.setEnabled(true);
	    }
	}

	if (str.contains("Current"))
	{
	    m_currentLinuxProfile = str.split(" - A Profile for Linux Current")[0];
	    m_output.append("Profile for this System: " + m_currentLinuxProfile);
	}
    }
}

int RAMWidget::getLineAmount()
{
    int lA = 4 * 60;
    if (m_dumpFromCurrMachine.isChecked())
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

void RAMWidget::choseDirBtnClicked()
{
    QString str = QFileDialog::getOpenFileName(0,
					       "Choose memory dump to analyze",
					       m_lDir.text(),
					       tr("Memory dumps (*.mem *.lime *.img *.raw *.vmem *.dmp *mddramimage)"));
    if (!str.isEmpty())
    {
	m_lDir.setText(str);
    }
}
