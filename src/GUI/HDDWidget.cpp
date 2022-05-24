
#include "HDDWidget.h"

HDDWidget::HDDWidget(QString path, QString mask, QString frame, QWidget *parent) :
    QWidget(parent), m_watcher(this),
    m_label1("Choose directory"), m_label2("Enter mask"), m_label3("Report name"),
    m_confirmDir("Browse directory"), m_start("Start"),
    m_lDir(path), m_lMask(mask), m_lFName(frame),
    m_needHiddenDirs("Analyze files in hidden folders"),
    m_needHiddenFiles("Analyze hidden files"),
    m_needOutput("Full Output")
{
    m_watcher.addPath(path);
    m_watcher.setObjectName("HDDWatcher");

    // Analyser initialization
    m_agent = new Agent(&m_list, &m_progress, this);
    m_agent->setOutputFlags(m_needOutput.isChecked());

    m_list.setReadOnly(true);

    m_grid.setMargin(m_margin);
    m_grid.setSpacing(m_spacing);
    m_grid.addWidget(&m_label1, 0, 0);
    m_grid.addWidget(&m_label2, 1, 0);
    m_grid.addWidget(&m_label3, 2, 0);
    m_grid.addWidget(&m_lDir, 0, 1, 1, 2);
    m_grid.addWidget(&m_lMask, 1, 1, 1, 2);
    m_grid.addWidget(&m_lFName, 2, 1, 1, 2);
    m_grid.addWidget(&m_needOutput, 3, 0);
    m_grid.addWidget(&m_needHiddenDirs, 3, 1);
    m_grid.addWidget(&m_needHiddenFiles, 3, 2);
    m_grid.addWidget(&m_confirmDir, 4, 0);
    m_grid.addWidget(&m_start, 4, 2);
    m_grid.addWidget(&m_progress, 5, 0, 1, 3);
    m_grid.addWidget(&m_list, 0, 4, 6, 3);

    m_label1.setBuddy(&m_lDir);
    m_label2.setBuddy(&m_lMask);
    m_label3.setBuddy(&m_lFName);

    this->setLayout(&m_grid);
    m_start.setFocus();

    m_lDir.setObjectName("lDirEdit");

    connect(&m_confirmDir, SIGNAL(clicked()), SLOT(choseDirBtnClicked()));
    connect(&m_start, SIGNAL(clicked()), SLOT(startBtnClicked()));
    connect(&m_lDir, SIGNAL(textEdited(QString)), SLOT(dirChanged()));
    connect(&m_watcher, SIGNAL(directoryChanged(QString)), SLOT(dirChanged()));
    connect(&m_needOutput, SIGNAL(clicked(bool)), SLOT(outputChkBoxClicked()));
    connect(m_agent, SIGNAL(pfinished(QString)), SLOT(dataCollectionFinished(QString)));

}

HDDWidget::~HDDWidget()
{
    
}

void HDDWidget::choseDirBtnClicked()
{
    QString str = QFileDialog::getExistingDirectory(0,
						    "Choose directory to analyze",
						    m_lDir.text());
    if (!str.isEmpty())
    {
	m_lDir.setText(str);
    }
}

void HDDWidget::dataCollectionFinished(QString status)
{
    m_start.setEnabled(true);
    m_needOutput.setEnabled(true);
    QMessageBox::information(this, "Hard drive parser", "Finished " + status);
    m_lFName.setText("HDD_results_" + QDateTime::currentDateTime().toString("hh.mm.ss_dd.MM.yyyy") + ".csv");
}

void HDDWidget::startBtnClicked()
{
    m_start.setEnabled(false);
    m_list.clear();

    m_agent->setRootDirectory(QDir(m_lDir.text()));
    m_agent->setAnalysisFlags(m_lMask.text(),
			      m_needHiddenDirs.isChecked(),
			      m_needHiddenFiles.isChecked());
    m_agent->setFName(m_lFName.text() + (m_lFName.text().endsWith(".csv") ? "" : ".csv"));
    m_agent->start();
}

void HDDWidget::dirChanged()
{
    QDir buf(QDir(m_lDir.text()));
    bool isValid = buf.exists();

    if (isValid)
    {
	m_watcher.addPath(m_lDir.text());
	m_lDir.setPalette(m_editText);
	m_start.setEnabled(true);
	return;
    }

    m_lDir.setPalette(m_editText);
    m_start.setEnabled(false);
}

void HDDWidget::outputChkBoxClicked()
{
    m_agent->setOutputFlags(m_needOutput.isChecked());
}
