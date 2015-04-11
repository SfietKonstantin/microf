#include "socialcontentitem.h"
#include "socialcontentitem_p.h"
#include <QtCore/QDebug>
#include "socialnetwork_p.h"
#include "socialobject.h"
#include "socialobject_p.h"
#include "socialcontentitembuilder.h"
#include "socialcontentitembuilder_p.h"

SocialContentItemPrivate::SocialContentItemPrivate(SocialContentItem *q)
    : AbstractSocialContentPrivate(q), q_ptr(q), m_socialNetwork(0), m_request(0), m_builder(0)
    , m_object(0)
{
}

void SocialContentItemPrivate::setContentItemObject(SocialContentItem &contentItem,
                                                    const QVariantMap &properties)
{
    contentItem.d_func()->setContentItemObject(properties);
}

void SocialContentItemPrivate::setContentItemError(SocialContentItem &contentItem,
                                                   SocialNetworkError::type error,
                                                   const QString &errorString)
{
    contentItem.d_func()->setError(error, errorString);
}

bool SocialContentItemPrivate::build(QNetworkReply::NetworkError error, const QString &errorString,
                                     const QByteArray &data)
{
    Q_Q(SocialContentItem);
    if (!m_builder) {
        return false;
    }
    SocialContentItemBuilderPrivate::build(*m_builder, *q, error, errorString, data);
    return true;
}

void SocialContentItemPrivate::setContentItemObject(const QVariantMap &properties)
{
    Q_Q(SocialContentItem);
    for (const QString &key : properties.keys()) {
        const QVariant &value = properties.value(key);
        SocialObjectPrivate::setProperty(m_object, key.toLocal8Bit(), value);
    }
    setStatus(SocialNetworkStatus::Ready);
    emit q->finished(true);
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

SocialObject *SocialContentItem::object() const
{
    Q_D(const SocialContentItem);
    return d->m_object;
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

QString SocialContentItem::errorString() const
{
    Q_D(const SocialContentItem);
    return d->errorString;
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

    bool ok = SocialNetworkPrivate::socialContentLoad(*d->m_socialNetwork, *d, *d->m_request);
    if (ok) {
        d->setStatus(SocialNetworkStatus::Busy);
    }
    return ok;
}
