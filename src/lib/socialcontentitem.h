#ifndef SOCIALCONTENTITEM_H
#define SOCIALCONTENTITEM_H

#include <QtCore/QObject>
#include <QtQml/QQmlParserStatus>
#include "isocialcontent.h"

class SocialNetwork;
class SocialObject;
class SocialRequest;
class SocialContentItemBuilder;
class SocialContentItemPrivate;
class SocialContentItem: public QObject, public ISocialContent, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(SocialNetwork * socialNetwork READ socialNetwork WRITE setSocialNetwork
               NOTIFY socialNetworkChanged)
    Q_PROPERTY(SocialRequest * request READ request WRITE setRequest NOTIFY requestChanged)
    Q_PROPERTY(SocialContentItemBuilder * builder READ builder WRITE setBuilder NOTIFY builderChanged)
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
    SocialContentItemBuilder * builder() const;
    void setBuilder(SocialContentItemBuilder * builder);
    SocialObject * object() const;
    SocialNetworkStatus::type status() const override;
    SocialNetworkError::type error() const override;
    QString errorString() const override;
public Q_SLOTS:
    bool load();
Q_SIGNALS:
    void socialNetworkChanged();
    void requestChanged();
    void builderChanged();
    void objectChanged();
    void statusChanged() override;
    void errorChanged() override;
    void errorStringChanged() override;
    void finished(bool ok) override;
protected:
    QScopedPointer<SocialContentItemPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(SocialContentItem)
};

#endif // SOCIALCONTENTITEM_H
