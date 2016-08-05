/****************************************************************************
** This file is a part of Syncopate Limited GameNet Application or it parts.
**
** Copyright (©) 2011 - 2012, Syncopate Limited and/or affiliates. 
** All rights reserved.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
****************************************************************************/
#ifndef _GGS_APPLICATION_ARGUMENTPARSER_H_
#define _GGS_APPLICATION_ARGUMENTPARSER_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QMutex>

namespace GGS {
  namespace Application {
    class ArgumentParser : public QObject
    {
      Q_OBJECT
    public:
      ArgumentParser(QObject *parent = 0);
      ~ArgumentParser();

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

#endif // _GGS_APPLICATION_ARGUMENTPARSER_H_