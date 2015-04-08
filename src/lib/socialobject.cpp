#include "socialobject.h"
#include "socialobject_p.h"
#include <QtCore/private/qobject_p.h>
#include <QtCore/private/qmetaobjectbuilder_p.h>

class SocialObjectMetaObject: public QAbstractDynamicMetaObject
{
public:
    explicit SocialObjectMetaObject(QObject *object);
    ~SocialObjectMetaObject();
    int metaCall(QMetaObject::Call c, int id, void **a) override;
    int createProperty(const char *name, const char *) override;
    void clear();
    void setProperty(const char *name, const QVariant &value);
private:
    void buildMeta();
    void setProperty(int id, const QVariant &value);
    QObject *m_object;
    QMap<int, QVariant> m_properties;
    QScopedPointer<QMetaObjectBuilder> m_builder;
    QMetaObject *m_metaObject;
    int m_propertyOffset;
    int m_signalOffset;
};

SocialObjectMetaObject::SocialObjectMetaObject(QObject *object)
    : m_object(object), m_builder(new QMetaObjectBuilder(object->metaObject()))
    , m_metaObject(0), m_propertyOffset(0), m_signalOffset(0)
{
    m_builder->setClassName(object->metaObject()->className());
    m_builder->setFlags(QMetaObjectBuilder::DynamicMetaObject);
    buildMeta();
    m_propertyOffset = m_metaObject->propertyOffset();
    m_signalOffset = m_metaObject->methodOffset();
}

SocialObjectMetaObject::~SocialObjectMetaObject()
{
    free(m_metaObject);
}

int SocialObjectMetaObject::metaCall(QMetaObject::Call c, int id, void **a)
{
    if (c == QMetaObject::ReadProperty && id >= m_propertyOffset) {
        int propertyId = id - m_propertyOffset;
        if (m_properties.contains(propertyId)) {
            *reinterpret_cast<QVariant *>(a[0]) = m_properties.value(propertyId);
        }
        return -1;
    }
    return m_object->qt_metacall(c, id, a);
}

int SocialObjectMetaObject::createProperty(const char *name, const char *)
{
    int signalId = m_builder->methodCount();
    int id = m_builder->propertyCount();
    m_builder->addSignal("__" + QByteArray::number(id) + "()");
    QMetaPropertyBuilder propertyBuilder = m_builder->addProperty(name, "QVariant", signalId);
    propertyBuilder.setWritable(false);
    buildMeta();
    m_properties.insert(id, QVariant::fromValue<QObject *>(0));
    return id + m_propertyOffset;
}

void SocialObjectMetaObject::clear()
{
    for (int i = 0; i < m_builder->propertyCount(); ++i) {
        QVariant value = m_properties.value(i);
        if (value.canConvert<QObject *>()) {
            QObject *valueObject = value.value<QObject *>();
            if (valueObject) {
                valueObject->deleteLater();
            }
        }
        setProperty(i, QVariant::fromValue<QObject *>(0));
    }
}

void SocialObjectMetaObject::setProperty(const char *name, const QVariant &value)
{
    int id = indexOfProperty(name);
    if (id == -1) {
        createProperty(name, 0);
    }
    setProperty(id - m_propertyOffset, value);
}

void SocialObjectMetaObject::buildMeta()
{
    if (m_metaObject) {
        free(m_metaObject);
    }
    m_metaObject = m_builder->toMetaObject();
    *static_cast<QMetaObject *>(this) = *m_metaObject;
    QObjectPrivate::get(m_object)->metaObject = this;
}

void SocialObjectMetaObject::setProperty(int id, const QVariant &value)
{
    if (m_properties.contains(id)) {
        const QVariant &oldValue = m_properties.value(id);
        if (oldValue != value) {
            m_properties.insert(id, value);
            activate(m_object, m_signalOffset + id, 0);
        }
    }
}

SocialObjectPrivate::SocialObjectPrivate(SocialObject *q)
    : q_ptr(q)
{
}

SocialObjectPrivate::~SocialObjectPrivate()
{
}

void SocialObjectPrivate::setProperty(SocialObject *object, const char *name, const QVariant &value)
{
    object->d_func()->setProperty(name, value);
}

void SocialObjectPrivate::clear(SocialObject *object)
{
    object->d_func()->clear();
}

void SocialObjectPrivate::setProperty(const char *name, const QVariant &value)
{
    m_meta->setProperty(name, value);
}

void SocialObjectPrivate::clear()
{
    m_meta->clear();
}

SocialObject::SocialObject(QObject *parent)
    : QObject(parent), d_ptr(new SocialObjectPrivate(this))
{
    Q_D(SocialObject);
    d->m_meta = new SocialObjectMetaObject(this);
}

SocialObject::~SocialObject()
{
}

