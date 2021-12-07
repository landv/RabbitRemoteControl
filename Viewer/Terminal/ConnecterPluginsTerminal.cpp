#include "ConnecterPluginsTerminal.h"
#include "Connect.h"
#include "ConnectThreadTerminal.h"

#include <QDialog>
#include <QApplication>
#include <QDebug>
#include <QTextCodec>
#include <QSettings>

#ifdef BUILD_QUIWidget
    #include "QUIWidget/QUIWidget.h"
#endif

#include "RabbitCommonLog.h"

CConnecterPluginsTerminal::CConnecterPluginsTerminal(CPluginViewer *parent)
    : CConnecter(parent),
      m_pConsole(nullptr),
      m_pThread(nullptr),
      m_bThread(false),
      m_bExit(false)
{
    m_pConsole = new CFrmTermWidget();
    m_pConsole->setAutoClose(true);
    
    bool check = false;
    check = connect(m_pConsole, SIGNAL(titleChanged()),
                    this, SLOT(slotTerminalTitleChanged()));
    Q_ASSERT(check);
    check = connect(m_pConsole, SIGNAL(finished()),
                    this, SIGNAL(sigDisconnected()));
    Q_ASSERT(check);
    check = connect(m_pConsole, SIGNAL(sigZoomReset()),
                    this, SLOT(slotZoomReset()));
    Q_ASSERT(check);
}

CConnecterPluginsTerminal::~CConnecterPluginsTerminal()
{
    qDebug() << "CConnecterPluginsTerminal::~CConnecterPluginsTerminal()";
    
    if(m_pConsole)
    {
        m_pConsole->deleteLater();
        m_pConsole = nullptr;
    }
}

QWidget* CConnecterPluginsTerminal::GetViewer()
{
    return m_pConsole;
}

qint16 CConnecterPluginsTerminal::Version()
{
    return 0;
}

int CConnecterPluginsTerminal::Load(QSettings &set)
{
    int nRet = 0;
    CParameterTerminal* pPara = GetPara();
    Q_ASSERT(pPara);
    if(!pPara) return -1;

    qint16 version = 0;
    pPara->OnLoad(set);

    nRet = OnLoad(set);
    return nRet;
}

int CConnecterPluginsTerminal::Save(QSettings &set)
{
    int nRet = 0;
    
    CParameterTerminal* pPara = GetPara();
    Q_ASSERT(pPara);
    if(!pPara) return -1;
    
    pPara->OnSave(set);
    nRet = OnSave(set);
    return nRet;
}

int CConnecterPluginsTerminal::Connect()
{
    int nRet = 0;

    nRet = SetParamter();

    if(m_bThread)
    {
        m_pThread = new CConnectThreadTerminal(this);
        bool check = connect(m_pThread, SIGNAL(finished()),
                        m_pThread, SLOT(deleteLater()));
        Q_ASSERT(check);
        if(m_pThread)
            m_pThread->start();
        return 0;
    }
    
    nRet = OnConnect();
    if(nRet < 0)
        emit sigDisconnected();
    else if(0 == nRet)
        emit sigConnected();
    
    if(m_pConsole)
        emit sigUpdateName(ServerName());
    
    return nRet;
}

int CConnecterPluginsTerminal::DisConnect()
{
    int nRet = 0;

    if(m_bThread && m_pThread)
    {
        m_pThread->quit();
    } else
        nRet = OnDisConnect();
    emit sigDisconnected();
    return nRet;
}

int CConnecterPluginsTerminal::SetParamter()
{
    int nRet = 0;

    CParameterTerminal* pPara = GetPara();
    Q_ASSERT(pPara);
    if(!pPara) return -1;
#if QTERMWIDGET_VERSION >= QT_VERSION_CHECK(0, 9, 0)
    m_pConsole->setTerminalSizeHint(pPara->GetSizeHint());
    m_pConsole->setBidiEnabled(pPara->GetDirectional());
    m_pConsole->disableBracketedPasteMode(pPara->GetDisableBracketedPasteMode());
#endif
    m_pConsole->setTerminalFont(pPara->GetFont());
    m_pConsole->setKeyboardCursorShape(pPara->GetCursorShape());
    m_pConsole->setColorScheme(pPara->GetColorScheme());
    m_pConsole->setScrollBarPosition(pPara->GetScrollBarPosition());
    m_pConsole->setFlowControlEnabled(pPara->GetFlowControl());
    m_pConsole->setFlowControlWarningEnabled(pPara->GetFlowControlWarning());
    m_pConsole->setMotionAfterPasting(pPara->GetMotionAfterPasting());
    m_pConsole->setTerminalOpacity(1.0 - pPara->GetTransparency() / 100.0);
    m_pConsole->setTerminalBackgroundImage(pPara->GetBackgroupImage());
        
    m_pConsole->setKeyBindings(pPara->GetKeyBindings());
    m_pConsole->setTextCodec(QTextCodec::codecForName(pPara->GetTextCodec().toStdString().c_str()));
    m_pConsole->setHistorySize(pPara->GetHistorySize());
    
//    m_pConsole->setMonitorActivity(false);
//    m_pConsole->setMonitorSilence(false);
//    m_pConsole->setBlinkingCursor(true);
    

    return nRet;
}

void CConnecterPluginsTerminal::slotTerminalTitleChanged()
{
    m_pConsole->setWindowTitle(m_pConsole->title());
    slotSetServerName(m_pConsole->title());
}

void CConnecterPluginsTerminal::slotZoomReset()
{
    if(m_pConsole)
        m_pConsole->setTerminalFont(GetPara()->GetFont());
}

int CConnecterPluginsTerminal::OnConnect()
{
    return 0;
}

int CConnecterPluginsTerminal::OnDisConnect()
{
    return 0;
}

int CConnecterPluginsTerminal::OnLoad(QSettings &set)
{
    return 0;
}

int CConnecterPluginsTerminal::OnSave(QSettings &set)
{
    return 0;
}

QString CConnecterPluginsTerminal::ServerName()
{
    CParameterTerminal* pPara = GetPara();
    if(CConnecter::ServerName().isEmpty())
    {
        if(pPara && !pPara->GetHost().isEmpty())
            return pPara->GetHost() + ":" + QString::number(pPara->GetPort());
        else
            return CConnecter::Name();
    }
    return CConnecter::ServerName();
}

CConnect* CConnecterPluginsTerminal::InstanceConnect()
{
    return nullptr;
}
