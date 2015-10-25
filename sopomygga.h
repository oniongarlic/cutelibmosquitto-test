#ifndef SOPOMYGGA_H
#define SOPOMYGGA_H

#include <QObject>
#include <QSocketNotifier>
#include <mosquittopp.h>

using namespace mosqpp;

class SopoMygga : public QObject, public mosquittopp
{
    Q_OBJECT
public:
    explicit SopoMygga(QObject *parent = 0);
    ~SopoMygga() {}

    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedeChanged)
    Q_PROPERTY(QString clientId READ clientId WRITE setClientId NOTIFY clientIdChanged)
    Q_PROPERTY(bool cleanSession READ cleanSession WRITE setCleanSession NOTIFY cleanSessionChanged)

    Q_INVOKABLE int connect();
    Q_INVOKABLE int disconnect();
    Q_INVOKABLE int reconnect();

    Q_INVOKABLE int subscribe(QString topic, int qos=0);
    Q_INVOKABLE int unsubscribe(QString topic);

    Q_INVOKABLE int publish(QString topic, QString data, int qos=0, bool retain=false);

    Q_INVOKABLE int setWill(QString topic, QString data, int qos=0, bool retain=false);
    Q_INVOKABLE void clearWill();

    void on_connect(int rc) {
        emit connected();
    }

    void on_disconnect(int rc) {
        emit disconnected();
    }

    void on_message(const struct mosquitto_message *message);

    void on_error();
    void on_log(int level, const char *str);

    Q_ENUMS(mosq_err_t)

    QString clientId() const
    {
        return m_clientId;
    }

    bool cleanSession() const
    {
        return m_cleanSession;
    }

    bool isConnected() const
    {
        return m_isConnected;
    }

signals:
    void connecting();
    void connected();
    void disconnected();
    void msg(QString topic, QString data);
    void error();

    void isConnectedeChanged(bool connected);
    void clientIdChanged(QString clientId);
    void cleanSessionChanged(bool cleanSession);

public slots:

    void setClientId(QString clientId);
    void setCleanSession(bool cleanSession);

private slots:
    void loopRead();
    void loopWrite();

private:
    QSocketNotifier *m_notifier_read;
    QSocketNotifier *m_notifier_write;

    QString m_hostname;
    int m_port;
    int m_keepalive;

    QString m_username;
    QString m_password;

    int m_mid;
    int m_smid;
    int m_pmid;
    QString m_clientId;
    bool m_cleanSession;
    bool m_isConnected;
};

#endif // SOPOMYGGA_H
