#include "HDDWidget.h"

HDDWidget::HDDWidget(QString path, QString mask, QString frame, QWidget *parent) :
    QWidget(parent), m_watcher(this),
    m_label1("Chose directory"), m_label2("Enter mask"), m_label3("Report name"),
    m_confirmDir("Browse"), m_start("Start"), m_lDir(path), m_lMask(mask), m_lFName(frame)
{
    m_watcher.addPath(path);
    m_watcher.setObjectName("HDDWatcher");

    m_list.setReadOnly(true);

    m_grid.setMargin(m_margin);
    m_grid.setSpacing(m_spacing);
    m_grid.addWidget(&m_label1, 0, 0);
    m_grid.addWidget(&m_label2, 1, 0);
    m_grid.addWidget(&m_label3, 2, 0);
    m_grid.addWidget(&m_lDir, 0, 1, 1, 2);
    m_grid.addWidget(&m_lMask, 1, 1, 1, 2);
    m_grid.addWidget(&m_lFName, 2, 1, 1, 2);
    m_grid.addWidget(&m_confirmDir, 0, 3);
    m_grid.addWidget(&m_start, 1, 3);
    m_grid.addWidget(&m_progress, 5, 0, 1, 4);
    m_grid.addWidget(&m_list, 4, 0, 1, 4);

    this->setLayout(&m_grid);
    m_start.setFocus();

    m_lDir.setObjectName("lDirEdit");

    connect(&m_confirmDir, SIGNAL(clicked()), SLOT(choseDirBtnClicked()));
    connect(&m_start, SIGNAL(clicked()), SLOT(startBtnClicked()));
    connect(&m_lDir, SIGNAL(textEdited()), SLOT(dirChanged()()));
    connect(&m_watcher, SIGNAL(dirChanged(QString)), SLOT(outputChkBoxClicked()()));

    // Text coloring
    m_editText.setColor(QPalette::Text, Qt::green);
    m_lDir.setPalette(m_editText);
    m_lMask.setPalette(m_editText);
    m_lFName.setPalette(m_editText);
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
}

void HDDWidget::startBtnClicked()
{
    m_start.setEnabled(false);
    m_list.clear();
}

void HDDWidget::dirChanged()
{
    QDir buf(QDir(m_lDir.text()));
    bool isValid = buf.text();

    if (isValid)
    {
	m_watcher.addPath(m_lDir.text());
	m_lDir.setPalette(&m_editText);
	m_start.setEnabled(true);
	return;
    }

    m_start.setEnabled(false);
}

void HDDWidget::outputChkBoxClicked()
{

}