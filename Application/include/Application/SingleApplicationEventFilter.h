#pragma once

#include <QtCore/QObject>
#include <QtCore/QAbstractNativeEventFilter>

namespace P1 {
  namespace Application {
    class SingleApplicationEventFilter: public QObject, public QAbstractNativeEventFilter
    {
      Q_OBJECT

    public:
      SingleApplicationEventFilter(QObject *parent = 0);
      virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;

    signals:
      void forceQuit();
    };
  }
}
