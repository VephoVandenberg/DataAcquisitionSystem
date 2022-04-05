#include "agent.h"

Agent::Agent(QTextEdit *out, QProgressBar *bar, QWidget *parent) :
    QWidget(parent), m_counter(0), m_output(out), m_progress(bar)
{
    m_analyzerProcess = new QProcess(this);
}

Agent::~Agent()
{
    
}

void Agent::start()
{

}

void Agent::parserSendData()
{

}



