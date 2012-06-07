#include "MessageReciever.h"


MessageReciever::MessageReciever(QObject *parent)
  : QObject(parent)
{
}


MessageReciever::~MessageReciever()
{
}

void MessageReciever::messageRecieved(QString message)
{
  qDebug() << message;
}
