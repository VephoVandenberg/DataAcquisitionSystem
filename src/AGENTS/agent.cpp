#include "agent.h"

Agent::Agent(QTextEdit *out, QProgressBar *bar, QWidget *parent) :
    QWidget(parent), m_counter(0), m_output(out), m_progress(bar)
{
    m_analyzerProcess = new QProcess(this);
    connect(m_analyzerProcess, SIGNAL(readyReadStandardOutput()), SLOT(parserSendData()));
    connect(m_analyzerProcess, SIGNAL(errorOccured(QProcess::ProcessError)), SLOT(parserErrorOccured(QProcess::ProcessError)));
}

Agent::~Agent()
{
    
}

void Agent::start()
{
    QFile file(QDir::currentPath() + "//flist");
    file.open(QFile::WriteOnly);
    QStringList list;
    m_output->append("Counting files");
    countAndGetFiles(m_root, list);
    int len = list.count();
    m_progress->setRange(0, len);

    foreach(QString str, list)
    {
	file.write((str+'\n').toUtf8());
    }
    file.close();

    m_output->append("List of files has been recieved, files to process:" + QString::number(len));
    m_resultFName = "Analysis_results//hdd//" + m_resultFName;
    
}

void Agent::parserSendData()
{

}

void Agent::countAndGetFiles(QDir dir, QStringList &list)
{
    QApplication::processEvents();
    QDir::Filters filter = QDir::Files|QDir::System;

    if (m_checkHiddenFiles)
    {
	filter |= QDir::Hidden;
    }

    list += dir.entryList(m_masks.split(" "), filter).replaceInStrings(QRegExp("^"), (dir.absolutePath()+'/'));
    m_counter = list.size();

    filter = QDir::Dirs | QDir::NoDotAndDotDot;
    if (m_checkHiddenDirs)
    {
	filter |= QDir::Hidden;
    }

    QStringList subdirs = dir.entryList(filter);
    foreach(QString subdir, subdirs)
    {
	countAndGetFiles(QDir(dir.absoluteFilePath(subdir)), list);
    }
}


