#ifndef SOCIALCONTENTITEM_H
#define SOCIALCONTENTITEM_H

#include <QtCore/QObject>
#include <QtQml/QQmlParserStatus>
#include "socialnetworkstatus.h"
#include "socialnetworkerror.h"

class SocialNetwork;
class SocialObject;
class SocialRequest;
class SocialContentBuilder;
class SocialContentItemPrivate;
class SocialContentItem: public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(SocialNetwork * socialNetwork READ socialNetwork WRITE setSocialNetwork
               NOTIFY socialNetworkChanged)
    Q_PROPERTY(SocialRequest * request READ request WRITE setRequest NOTIFY requestChanged)
    Q_PROPERTY(SocialContentBuilder * builder READ builder WRITE setBuilder NOTIFY builderChanged)
    Q_PROPERTY(SocialObject * object READ object CONSTANT)
    Q_PROPERTY(SocialNetworkStatus::type status READ status NOTIFY statusChanged)
    Q_PROPERTY(SocialNetworkError::type error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_ENUMS(Status)
    Q_ENUMS(ErrorType)
public:
    explicit SocialContentItem(QObject *parent = 0);
    virtual ~SocialContentItem();
    void classBegin() override;
    void componentComplete() override;
    SocialNetwork * socialNetwork() const;
    void setSocialNetwork(SocialNetwork *socialNetwork);
    SocialRequest * request() const;
    void setRequest(SocialRequest *request);
    SocialContentBuilder * builder() const;
    void setBuilder(SocialContentBuilder * builder);
    SocialObject * object() const;
    SocialNetworkStatus::type status() const;
    SocialNetworkError::type error() const;
    QString errorString() const;
public Q_SLOTS:
    bool load();
Q_SIGNALS:
    void socialNetworkChanged();
    void requestChanged();
    void builderChanged();
    void objectChanged();
    void statusChanged();
    void finished(bool ok);
    void errorChanged();
    void errorStringChanged();
protected:
    QScopedPointer<SocialContentItemPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(SocialContentItem)
};

#endif // SOCIALCONTENTITEM_H
