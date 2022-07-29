// Author: Kang Lin <kl222@126.com>

#include "PluginClient.h"
#include "RabbitCommonDir.h"
#include "RabbitCommonLog.h"
#include <QLocale>
#include <QDebug>
#include <QApplication>
#include <QDir>

CPluginClient::CPluginClient(QObject *parent) : QObject(parent)
{
}

CPluginClient::~CPluginClient()
{
    qApp->removeTranslator(&m_Translator);
}

int CPluginClient::InitTranslator()
{
    QString szTranslatorFile = RabbitCommon::CDir::Instance()->GetDirPluginsTranslation("plugins/Client")
            + QDir::separator() + Name() + "_" + QLocale::system().name() + ".qm";
    if(!m_Translator.load(szTranslatorFile))
    {
        LOG_MODEL_ERROR("CPluginClient", "Open translator file fail:",
                        szTranslatorFile.toStdString().c_str());
        return -1;
    }
    qApp->installTranslator(&m_Translator);
    return 0;
}

const QString CPluginClient::Id() const
{
    return Protol() + ":" + Name();
}

const QString CPluginClient::DisplayName() const
{
    return Name();
}

const QIcon CPluginClient::Icon() const
{
    return QIcon(":/image/Connect");
}