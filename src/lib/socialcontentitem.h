#ifndef SOCIALCONTENTITEM_H
#define SOCIALCONTENTITEM_H

#include <QtCore/QObject>
#include <QtCore/QVariantMap>
#include <QtQml/QQmlParserStatus>
#include "isocialcontent.h"

class SocialNetwork;
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
    Q_PROPERTY(bool empty READ isEmpty NOTIFY emptyChanged)
    Q_PROPERTY(QVariantMap object READ object NOTIFY objectChanged)
    Q_PROPERTY(SocialNetworkStatus::type status READ status NOTIFY statusChanged)
    Q_PROPERTY(SocialNetworkError::type error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(QString errorCode READ errorCode NOTIFY errorCodeChanged)
    Q_ENUMS(Status)
    Q_ENUMS(ErrorType)
public:
    explicit SocialContentItem(QObject *parent = 0);
    virtual ~SocialContentItem();
    void classBegin() Q_DECL_OVERRIDE;
    void componentComplete() Q_DECL_OVERRIDE;
    SocialNetwork * socialNetwork() const;
    void setSocialNetwork(SocialNetwork *socialNetwork);
    SocialRequest * request() const;
    void setRequest(SocialRequest *request);
    SocialContentItemBuilder * builder() const;
    void setBuilder(SocialContentItemBuilder * builder);
    const QVariantMap & object() const;
    bool isEmpty() const;
    SocialNetworkStatus::type status() const Q_DECL_OVERRIDE;
    SocialNetworkError::type error() const Q_DECL_OVERRIDE;
    QString errorMessage() const Q_DECL_OVERRIDE;
    QString errorCode() const Q_DECL_OVERRIDE;
public Q_SLOTS:
    bool load();
Q_SIGNALS:
    void socialNetworkChanged();
    void requestChanged();
    void builderChanged();
    void objectChanged();
    void emptyChanged();
    void statusChanged() Q_DECL_OVERRIDE;
    void errorChanged() Q_DECL_OVERRIDE;
    void errorMessageChanged() Q_DECL_OVERRIDE;
    void errorCodeChanged() Q_DECL_OVERRIDE;
    void finished(bool ok) Q_DECL_OVERRIDE;
protected:
    QScopedPointer<SocialContentItemPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(SocialContentItem)
};

#endif // SOCIALCONTENTITEM_H
