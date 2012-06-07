#ifndef _GGS_APPLICATION_SINGLEAPPICATIONFIXTURE_H_
#define _GGS_APPLICATION_SINGLEAPPICATIONFIXTURE_H_

#include <QtCore/QObject>
#include <QtCore/QDebug>
class MessageReciever : public QObject
{
  Q_OBJECT
public:
  MessageReciever(QObject *parent = 0);
  ~MessageReciever();

public slots:
  void messageRecieved(QString message);
};

#endif // _GGS_APPLICATION_SINGLEAPPICATIONFIXTURE_H_