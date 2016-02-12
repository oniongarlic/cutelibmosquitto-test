#ifndef SOPOMYGGA_H
#define SOPOMYGGA_H

#include <QObject>
#include <QSocketNotifier>
#include <QTimer>
#include <QMap>
#include <mosquittopp.h>

using namespace mosqpp;

class SopoMygga : public QObject, public mosquittopp
{
    Q_OBJECT
public:
    explicit SopoMygga(QObject *parent = 0);
    ~SopoMygga();

    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedeChanged)
    Q_PROPERTY(QString clientId READ clientId WRITE setClientId NOTIFY clientIdChanged)
    Q_PROPERTY(bool cleanSession READ cleanSession WRITE setCleanSession NOTIFY cleanSessionChanged)
    Q_PROPERTY(QString hostname READ hostname WRITE setHostname NOTIFY hostnameChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)

    Q_PROPERTY(int keepalive READ keepalive WRITE setKeepalive NOTIFY keepaliveChanged)

    Q_PROPERTY(bool tlsEnabled READ tlsEnabled WRITE setTls_enabled NOTIFY tlsEnabledChanged)

    Q_INVOKABLE int connectToHost();
    Q_INVOKABLE int disconnectFromHost();
    Q_INVOKABLE int reconnectToHost();

    Q_INVOKABLE int subscribe(const QString topic, int qos=0);
    Q_INVOKABLE int unsubscribe(const QString topic);

    Q_INVOKABLE int publish(const QString topic, QString data, int qos=0, bool retain=false);
    Q_INVOKABLE int publish(const QString topic, QByteArray data, int qos=0, bool retain=false);

    Q_INVOKABLE int setWill(const QString topic, QString data, int qos=0, bool retain=false);
    Q_INVOKABLE void clearWill();

    void addTopicMatch(const QString topic, int topic_d);
    int removeTopicMatch(const QString topic);

    void on_connect(int rc);
    void on_disconnect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_error();
    void on_log(int level, const char *str);

    void timerEvent(QTimerEvent *event);

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

    int keepalive() const
    {
        return m_keepalive;
    }

    QString hostname() const
    {
        return m_hostname;
    }

    int port() const
    {
        return m_port;
    }

    bool tlsEnabled() const
    {
        return m_tlsEnabled;
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

    void keepaliveChanged(int keepalive);

    void topicMatch(int topic_id);

    void hostnameChanged(QString hostname);

    void portChanged(int port);

    void tlsEnabledChanged(bool tlsEnabled);

public slots:

    void setClientId(QString clientId);
    void setCleanSession(bool cleanSession);

    void setKeepalive(int keepalive)
    {
        if (m_keepalive == keepalive)
            return;

        m_keepalive = keepalive;
        emit keepaliveChanged(keepalive);
    }

    void setHostname(QString hostname)
    {
        if (m_hostname == hostname)
            return;

        m_hostname = hostname;
        emit hostnameChanged(hostname);
    }

    void setPort(int port)
    {
        if (m_port == port)
            return;

        m_port = port;
        emit portChanged(port);
    }

    void setTls_enabled(bool tlsEnabled)
    {
        if (m_tlsEnabled == tlsEnabled)
            return;

        m_tlsEnabled = tlsEnabled;
        emit tlsEnabledChanged(tlsEnabled);
    }

private slots:
    void loopRead();
    void loopWrite();

private:
    void shutdown();

    QSocketNotifier *m_notifier_read;
    QSocketNotifier *m_notifier_write;

    int m_timer;

    // Peer
    QString m_hostname;
    int m_port;
    int m_keepalive;

    // User
    QString m_username;
    QString m_password;

    // TLS
    bool m_tlsEnabled;
    bool m_tls_insecure;
    QString m_tls_ca;
    QString m_tls_capath;
    QString m_tls_cert;
    QString m_tls_key;

    int m_mid;
    int m_smid;
    int m_pmid;
    QString m_clientId;
    bool m_cleanSession;
    bool m_isConnected;

    QMap<QString, int> m_topics;
};

#endif // SOPOMYGGA_H
