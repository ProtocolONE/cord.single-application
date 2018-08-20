#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QMutex>

namespace P1 {
  namespace Application {
    class ArgumentParser : public QObject
    {
      Q_OBJECT
    public:
      explicit ArgumentParser(QObject *parent = 0);
      virtual ~ArgumentParser();

      const QString& uriPrefix() const;
      void setUriPrefix(const QString& value);

      void parse(QStringList arguments);
      void clear();

      QStringList cachedCommands();
      bool contains(const QString& name);
      QStringList commandArguments(const QString& name);

    public slots:
      void initFinished();

    signals:
      void commandRecieved(QString name, QStringList arguments);

    private:
      bool tryGetCommandName(const QString& argument, QString& commandName);
      bool isCommand(const QString& argument);
      void emitCommands(const QHash<QString, QStringList>& commands);

      void preparseCommandAndPutToResult(QString &name, QStringList commandArguments, QHash<QString, QStringList> &result);
      void uriParse(const QStringList& commandArguments, QHash<QString, QStringList>& result);

      QString _uriPrefix;
      QHash<QString, QStringList> _commandCache;
      QMutex _mutex;
      bool _isInitFinished;
    };

  }
}
