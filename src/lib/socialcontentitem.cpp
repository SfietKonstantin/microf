#include "socialcontentitem.h"
#include "socialcontentitem_p.h"
#include <QtCore/QDebug>
#include "socialnetwork_p.h"
#include "socialcontentitembuilder_p.h"

SocialContentItemPrivate::SocialContentItemPrivate(SocialContentItem *q)
    : AbstractSocialContentPrivate(q), m_socialNetwork(0), m_request(0), m_builder(0)
{
}

void SocialContentItemPrivate::setContentItemObject(SocialContentItem &contentItem,
                                                    const QVariantMap &properties,
                                                    const QVariantMap &metadata)
{
    contentItem.d_func()->setContentItemObject(properties, metadata);
}

void SocialContentItemPrivate::setContentItemError(SocialContentItem &contentItem,
                                                   SocialNetworkError::type error,
                                                   const QString &errorMessage,
                                                   const QString &errorCode)
{
    contentItem.d_func()->setError(error, errorMessage, errorCode);
}

bool SocialContentItemPrivate::build(QNetworkReply::NetworkError error, const QString &errorMessage,
                                     const QByteArray &data)
{
    Q_Q(SocialContentItem);
    if (!m_builder) {
        return false;
    }
    SocialContentItemBuilderPrivate::build(*m_builder, *q, error, errorMessage, data, metadata());
    return true;
}

void SocialContentItemPrivate::setContentItemObject(const QVariantMap &object,
                                                    const QVariantMap &metadata)
{
    Q_Q(SocialContentItem);
    setObject(object);
    setMetadata(metadata);
    setStatus(SocialNetworkStatus::Ready);
    emit q->finished(true);
}

void SocialContentItemPrivate::setObject(QVariantMap object)
{
    Q_Q(SocialContentItem);
    if (m_object != object) {
        bool isEmpty = m_object.isEmpty();
        m_object = object;
        emit q->objectChanged();
        if (isEmpty != object.isEmpty()) {
            emit q->emptyChanged();
        }
    }
}

SocialContentItem::SocialContentItem(QObject *parent)
    : QObject(parent), d_ptr(new SocialContentItemPrivate(this))
{
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

SocialContentItemBuilder * SocialContentItem::builder() const
{
    Q_D(const SocialContentItem);
    return d->m_builder;
}

void SocialContentItem::setBuilder(SocialContentItemBuilder *replyParser)
{
    Q_D(SocialContentItem);
    if (d->m_builder != replyParser) {
        d->m_builder = replyParser;
        emit builderChanged();
    }
}

const QVariantMap & SocialContentItem::object() const
{
    Q_D(const SocialContentItem);
    return d->m_object;
}

bool SocialContentItem::isEmpty() const
{
    Q_D(const SocialContentItem);
    return d->m_object.isEmpty();
}

SocialNetworkStatus::type SocialContentItem::status() const
{
    Q_D(const SocialContentItem);
    return d->status;
}

SocialNetworkError::type SocialContentItem::error() const
{
    Q_D(const SocialContentItem);
    return d->error;
}

QString SocialContentItem::errorMessage() const
{
    Q_D(const SocialContentItem);
    return d->errorMessage;
}

QString SocialContentItem::errorCode() const
{
    Q_D(const SocialContentItem);
    return d->errorCode;
}

bool SocialContentItem::load()
{
    Q_D(SocialContentItem);
    if (d->status == SocialNetworkStatus::Busy) {
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

    bool ok = SocialNetworkPrivate::socialContentLoad(*d->m_socialNetwork, *d, *d->m_request,
                                                      SocialRequest::Load);
    if (ok) {
        d->setStatus(SocialNetworkStatus::Busy);
    }
    return ok;
}
