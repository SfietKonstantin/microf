#ifndef SOCIALCONTENTITEM_H
#define SOCIALCONTENTITEM_H

#include <QtCore/QObject>
#include <QtQml/QQmlParserStatus>

class SocialNetwork;
class SocialObject;
class SocialRequest;
class SocialContentItemPrivate;
class SocialContentBuilder;
class SocialContentItem: public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(SocialNetwork * socialNetwork READ socialNetwork WRITE setSocialNetwork
               NOTIFY socialNetworkChanged)
    Q_PROPERTY(SocialRequest * request READ request WRITE setRequest NOTIFY requestChanged)
    Q_PROPERTY(SocialContentBuilder * builder READ builder WRITE setBuilder NOTIFY builderChanged)
    Q_PROPERTY(SocialObject * object READ object CONSTANT)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(ErrorType error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_ENUMS(Status)
    Q_ENUMS(ErrorType)
public:
    enum Status {
        Null,
        Busy,
        Ready,
        Error
    };
    enum ErrorType {
        NoError = 0,
        NetworkError = 1, // Network error
        DataError = 2, // Failed to process data coming from the social network
        SocialNetworkError = 3, // The social network emitted an error
        InternalError = 4 // Error in microf
    };
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
    Status status() const;
    ErrorType error() const;
    QString errorString() const;
public Q_SLOTS:
    bool load();
Q_SIGNALS:
    void socialNetworkChanged();
    void requestChanged();
    void builderChanged();
    void objectChanged();
    void statusChanged();
    void errorChanged();
    void errorStringChanged();
protected:
    QScopedPointer<SocialContentItemPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(SocialContentItem)
};

#endif // SOCIALCONTENTITEM_H
