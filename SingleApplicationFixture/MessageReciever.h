#pragma once

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
