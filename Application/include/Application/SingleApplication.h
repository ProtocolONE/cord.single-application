#pragma once
#include <Application/ArgumentParser.h>

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtWidgets/QApplication>

#include <Windows.h>


namespace P1 {
  namespace Application {

    class SingleApplicationEventFilter;

    class SingleApplication : public QApplication
    {
      Q_OBJECT
    public:
      SingleApplication(int &argc, char *argv[], const QString& uniqueApplicationName);
      ~SingleApplication();

      bool isAlreadyRunning() const;

      void startListen();
      void sendArguments();
      void sendArguments(const QStringList& additionalArguments);
      void sendMessage(const QString& message);

      void setIpcPortPath(const QString& ipcPortPath);
      const QString& ipcPortPath();

      bool containsCommand(const QString& name);
      QStringList getCommandArguments(const QString& name);

      virtual bool notify(QObject* receiver, QEvent* event);

    public slots:
      void initializeFinished();
      void allowSecondInstance();

    signals:
      void messageRecived(QString message);
      void commandRecieved(QString name, QStringList arguments);
      void sendMessageFinished();
      void forceQuit();
      void taskBarButtonCreated();

    private slots:
      void clientConnected();
      void clientReadyRead();
      void clientDisconnected();

    private:
      QString _ipcPortPath;
      bool _isAlreadyRunning;
      HANDLE _mutex;
      QTcpServer *_server;
      ArgumentParser _argumentParser;
      SingleApplicationEventFilter *_eventFilter;
    };
  }
}
