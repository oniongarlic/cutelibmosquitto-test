#include "sopomygga.h"
#include <QDebug>

SopoMygga::SopoMygga(QObject *parent) :
    QObject(parent),
    m_hostname("localhost"),
    m_cleanSession(true),
    m_clientId(),
    mosquittopp(NULL, true)
{

}

int SopoMygga::connect()
{
    int r, s;

    r=mosquittopp::reinitialise(m_clientId.isEmpty() ? NULL : m_clientId.toLocal8Bit().data(), m_cleanSession);
    if (r!=MOSQ_ERR_SUCCESS) {
        qWarning() << "Failed to set client id";
        return r;
    }

    r=mosquittopp::username_pw_set(m_username.isEmpty() ? NULL : m_username.toLocal8Bit().data(), m_password.isEmpty() ? NULL : m_password.toLocal8Bit().data());
    if (r!=MOSQ_ERR_SUCCESS) {
        qWarning() << "Failed to set username/password";
        return r;
    }

    r=mosquittopp::connect_async(m_hostname.toLocal8Bit().data());
    if (r!=MOSQ_ERR_SUCCESS) {
        qWarning() << "Connection failure";
        return r;
    }

    s=socket();

    m_notifier_read = new QSocketNotifier(s, QSocketNotifier::Read, this);
    QObject::connect(m_notifier_read, SIGNAL(activated(int)), this, SLOT(loopRead()));

    m_notifier_write = new QSocketNotifier(s, QSocketNotifier::Write, this);
    QObject::connect(m_notifier_write, SIGNAL(activated(int)), this, SLOT(loopWrite()));


    emit connecting();

    return r;
}

void SopoMygga::loopRead() {
    loop_read();
}

void SopoMygga::loopWrite() {
    loop_misc();
    loop_write();
}

int SopoMygga::disconnect()
{
    int r=mosquittopp::disconnect();
    delete m_notifier_read;
    delete m_notifier_write;
    return r;
}

int SopoMygga::reconnect()
{
    return reconnect_async();
}

int SopoMygga::subscribe(QString topic, int qos)
{
    int r=mosquittopp::subscribe(&m_smid, topic.toLocal8Bit().data(), qos);

    return r;
}

int SopoMygga::unsubscribe(QString topic)
{
    int r=mosquittopp::unsubscribe(&m_mid, topic.toLocal8Bit().data());

    return r;
}

int SopoMygga::publish(QString topic, QString data, int qos, bool retain)
{
    int r=mosquittopp::publish(&m_pmid, topic.toLocal8Bit().data(), data.size(), data.toLocal8Bit().data(), qos, retain);

    return r;
}

int SopoMygga::setWill(QString topic, QString data, int qos, bool retain)
{
    int r=will_set(topic.toLocal8Bit().data(), data.size(), data.toLocal8Bit().data(), qos, retain);

    return r;
}

void SopoMygga::clearWill()
{
    will_clear();
}

void SopoMygga::on_message(const mosquitto_message *message)
{
    QString topic(message->topic);
    QString data=QString::fromLocal8Bit((char *)message->payload, message->payloadlen);

    emit msg(topic, data);
}

void SopoMygga::on_error()
{
    emit error();
}

void SopoMygga::on_log(int level, const char *str)
{
    qDebug() << level << str;
}

void SopoMygga::setClientId(QString clientId)
{
    if (m_clientId == clientId)
        return;

    m_clientId = clientId;
    emit clientIdChanged(clientId);
}

void SopoMygga::setCleanSession(bool cleanSession)
{
    if (m_cleanSession == cleanSession)
        return;

    m_cleanSession = cleanSession;
    emit cleanSessionChanged(cleanSession);
}

