  #include "MessageReciever.h"

#include <Application/SingleApplication.h>
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QTimer>

int main(int argc, char *argv[])
{
    P1::Application::SingleApplication a(argc, argv, "SingleApplicationTest_{9B7BB371-B180-4BAA-959D-D46C0B155C8B}");

    a.setIpcPortPath("HKEY_CURRENT_USER\\Software\\PONE\\TESTSINGLEAPPLICATIONE");

    MessageReciever reciever;
    if (a.arguments().size() > 1) {
      //QStringList args = a.arguments();
      //args.removeFirst();
      //QString tmp = args.join("|");

      if (a.isAlreadyRunning()) {
        QStringList args;
        args << "-activate";
        a.sendArguments(args);
        //a.sendMessage(tmp);
      } else {
        QObject::connect(&a, SIGNAL(messageRecived(QString)), &reciever, SLOT(messageRecieved(QString)));
        a.startListen();
      }

    } else {
      qDebug() << a.isAlreadyRunning();
    }

    return a.exec();
}
