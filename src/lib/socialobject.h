#ifndef SOCIALOBJECT_H
#define SOCIALOBJECT_H

#include <QtCore/QObject>
#include <QtCore/QJsonObject>
#include "ipropertiesadaptor.h"

class SocialObjectPrivate;
class SocialObject: public QObject
{
    Q_OBJECT
public:
    explicit SocialObject(QObject *parent = 0);
    virtual ~SocialObject();
protected:
    QScopedPointer<SocialObjectPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(SocialObject)
};

#endif // SOCIALOBJECT_H
