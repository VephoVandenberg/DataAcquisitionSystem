#ifdef WIN32
#include <unistd.h>
#else
#include <unistd.h>
#endif

#include "RAMWidget.h"

RAMWidget::RAMWidget(QString currentFName, QWidget *parent) :
    QWidget(parent), m_label1("Result file"), m_label2("System profile"),
    m_start("Start"), m_getOnlyDump("Collect only memory dump"), m_saveRawDump("Save dump after RAM data gathering"), m_saveTempFiles("Save temporary files, gathered while collecting the data") 
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
}

void RAMWidget::cleanUp(QString dir)
{
}

void RAMWidget::changedProfiles(QString profile)
{
}

void RAMWidget::startBtnClicked()
{
}

void RAMWidget::dumpComplited(int exCode, QProcess::ExitStatus exStatus)
{
}

void RAMWidget::moveProgress()
{
}

void RAMWidget::dumpChkClicked()
{
}

void RAMWidget::parserFinished(int exCode, QProcess::ExitStatus exStatus)
{
}

void RAMWidget::parserSendData()
{
}
