#include "MessageReciever.h"

#include <Application/SingleApplication.h>
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>

int main(int argc, char *argv[])
{
    GGS::Application::SingleApplication a(argc, argv, "SingleApplicationTest_{9B7BB371-B180-4BAA-959D-D46C0B155C8B}");
    a.setIpcPortPath("HKEY_CURRENT_USER\\Software\\GGS\\TESTSINGLEAPPLICATIONE");

    MessageReciever reciever;
    if (a.arguments().size() > 1) {
      QStringList args = a.arguments();
      args.removeFirst();
      QString tmp = args.join("|");

      if (a.isAlreadyRunning()) {
        a.sendMessage(tmp);
      } else {
        QObject::connect(&a, SIGNAL(messageRecived(QString)), &reciever, SLOT(messageRecieved(QString)));
        a.startListen();
      }

    } else {
      qDebug() << a.isAlreadyRunning();
    }

    return a.exec();
}
