#include "socialcontentitem.h"
#include "socialcontentitem_p.h"
#include <QtCore/QDebug>
#include "socialnetwork_p.h"
#include "socialobject.h"
#include "socialobject_p.h"
#include "socialcontentbuilder.h"
#include "socialcontentbuilder_p.h"

SocialContentItemPrivate::SocialContentItemPrivate(SocialContentItem *q)
    : q_ptr(q), m_socialNetwork(0), m_request(0), m_builder(0), m_object(0)
    , m_status(SocialContentItem::Null) , m_error(SocialContentItem::NoError)
{
}

void SocialContentItemPrivate::handleNetworkReply(SocialContentItem &contentItem,
                                                  QNetworkReply::NetworkError error,
                                                  const QString &errorString,
                                                  const QByteArray &data)
{
    contentItem.d_func()->handleNetworkReply(error, errorString, data);
}

void SocialContentItemPrivate::setContentItemObject(SocialContentItem &contentItem,
                                                    const QJsonObject &json,
                                                    IPropertiesAdaptor::Ptr adaptor)
{
    contentItem.d_func()->setContentItemObject(json, adaptor);
}

void SocialContentItemPrivate::setContentItemError(SocialContentItem &contentItem,
                                                   SocialContentItem::ErrorType error,
                                                   const QString &errorString)
{
    contentItem.d_func()->setError(error, errorString);
}

void SocialContentItemPrivate::setContentItemObject(const QJsonObject &json,
                                                    IPropertiesAdaptor::Ptr adaptor)
{
    Q_Q(SocialContentItem);
    for (const QString &key : json.keys()) {
        QVariant value = adaptor->adaptProperty(key, json.value(key));
        SocialObjectPrivate::setProperty(m_object, key.toLocal8Bit(), value);
    }
    setStatus(SocialContentItem::Ready);
}


void SocialContentItemPrivate::setStatus(SocialContentItem::Status status)
{
    Q_Q(SocialContentItem);
    if (m_status != status) {
        m_status = status;
        emit q->statusChanged();
    }
}

void SocialContentItemPrivate::setError(SocialContentItem::ErrorType error,
                                        const QString &errorString)
{
    Q_Q(SocialContentItem);
    setStatus(SocialContentItem::Error);
    if (m_error != error) {
        m_error = error;
        emit q->errorChanged();
    }
    if (m_errorString != errorString) {
        m_errorString = errorString;
        emit q->errorStringChanged();
    }
}

void SocialContentItemPrivate::handleNetworkReply(QNetworkReply::NetworkError error,
                                                  const QString &errorString, const QByteArray &data)
{
    Q_Q(SocialContentItem);
    if (!m_builder) {
        qWarning() << "SocialContentItemPrivate::setData() called without replyParser";
        setError(SocialContentItem::InternalError, "Internal error");
        return;
    }

    if (error == QNetworkReply::NoError) {
        SocialContentBuilderPrivate::build(*m_builder, *q, data);
    } else {
        SocialContentBuilderPrivate::buildError(*m_builder, *q, error, errorString, data);
    }

    if (m_status == SocialContentItem::Busy) {
        qWarning() << "SocialContentItemPrivate::setData() builder did not perform an action";
        setError(SocialContentItem::InternalError, "Builder did not perform an action");
    }
}

SocialContentItem::SocialContentItem(QObject *parent)
    : QObject(parent), d_ptr(new SocialContentItemPrivate(this))
{
    Q_D(SocialContentItem);
    d->m_object = new SocialObject(this);
}

SocialContentItem::~SocialContentItem()
{
}

void SocialContentItem::classBegin()
{
}

void SocialContentItem::componentComplete()
{
}

SocialNetwork *SocialContentItem::socialNetwork() const
{
    Q_D(const SocialContentItem);
    return d->m_socialNetwork;
}

void SocialContentItem::setSocialNetwork(SocialNetwork *socialNetwork)
{
    Q_D(SocialContentItem);
    if (d->m_socialNetwork != socialNetwork) {
        d->m_socialNetwork = socialNetwork;
        emit socialNetworkChanged();
    }
}

SocialRequest * SocialContentItem::request() const
{
    Q_D(const SocialContentItem);
    return d->m_request;
}

void SocialContentItem::setRequest(SocialRequest *request)
{
    Q_D(SocialContentItem);
    if (d->m_request != request) {
        d->m_request = request;
        emit requestChanged();
    }
}

SocialContentBuilder * SocialContentItem::builder() const
{
    Q_D(const SocialContentItem);
    return d->m_builder;
}

void SocialContentItem::setBuilder(SocialContentBuilder *replyParser)
{
    Q_D(SocialContentItem);
    if (d->m_builder != replyParser) {
        d->m_builder = replyParser;
        emit builderChanged();
    }
}

SocialObject *SocialContentItem::object() const
{
    Q_D(const SocialContentItem);
    return d->m_object;
}

SocialContentItem::Status SocialContentItem::status() const
{
    Q_D(const SocialContentItem);
    return d->m_status;
}

SocialContentItem::ErrorType SocialContentItem::error() const
{
    Q_D(const SocialContentItem);
    return d->m_error;
}

QString SocialContentItem::errorString() const
{
    Q_D(const SocialContentItem);
    return d->m_errorString;
}

bool SocialContentItem::load()
{
    Q_D(SocialContentItem);
    if (d->m_status == Busy) {
        qWarning() << "SocialContentItem::load() called while status is Busy";
        return false;
    }

    if (!d->m_socialNetwork) {
        qWarning() << "SocialContentItem::load() called without socialNetwork";
        return false;
    }

    if (!d->m_request) {
        qWarning() << "SocialContentItem::load() called without request";
        return false;
    }

    if (!d->m_builder) {
        qWarning() << "SocialContentItem::load() called without builder";
        return false;
    }

    bool ok = SocialNetworkPrivate::contentItemLoad(*d->m_socialNetwork, *this, *d->m_request);
    if (ok) {
        d->setStatus(Busy);
    }
    return ok;
}
