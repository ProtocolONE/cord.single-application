/****************************************************************************
** This file is a part of Syncopate Limited GameNet Application or it parts.
**
** Copyright (©) 2011 - 2012, Syncopate Limited and/or affiliates. 
** All rights reserved.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
****************************************************************************/

#include <Application/ArgumentParser.h>
#include <QtCore/QMutexLocker>
#include <QtCore/QUrl>

namespace GGS {
  namespace Application {

    ArgumentParser::ArgumentParser(QObject *parent)
      : QObject(parent)
      , _isInitFinished(false)
      , _uriPrefix("gamenet")
    {
    }


    ArgumentParser::~ArgumentParser()
    {
    }

    void ArgumentParser::parse(QStringList arguments)
    {
      if (arguments.count() <= 0)
        return;

      QHash<QString, QStringList> result;

      QString name;
      QStringList commandArguments;
      bool commandStarted = false;

      Q_FOREACH(QString arg, arguments)
      {
        if (commandStarted) {
          if (!this->isCommand(arg)) {
            commandArguments.append(arg);
            continue;
          }

          this->preparseCommandAndPutToResult(name, commandArguments, result);
          commandStarted=false;
        }

        if (!this->tryGetCommandName(arg, name)) {
          continue;
        }

        commandArguments.clear();
        commandStarted = true;
      }

      if (commandStarted) {
        this->preparseCommandAndPutToResult(name, commandArguments, result);
      }

      if (result.size() <= 0)
        return;

      QMutexLocker locker(&this->_mutex);
      if (this->_isInitFinished) {
        this->emitCommands(result);
        return;
      }

      Q_FOREACH(QString key, result.keys()) {
        this->_commandCache[key] = result[key];
      }
    }

    void ArgumentParser::initFinished()
    {
      QMutexLocker locker(&this->_mutex);
      this->_isInitFinished = true;
      this->emitCommands(this->_commandCache);
      this->_commandCache.clear();
    }

    bool ArgumentParser::tryGetCommandName(const QString& argument, QString& commandName)
    {
      QString tmp = argument.trimmed();
      if (tmp.size() <= 1)
        return false;

      if (tmp[0] == '-') {
        if (tmp[1] == '-') {
          if (tmp.size() <= 2)
            return false;

          commandName = tmp.right(tmp.size() - 2);
          return true;
        }

        commandName = tmp.right(tmp.size() - 1);
        return true;
      }

      if (tmp[0] == '/') {
        commandName = tmp.right(tmp.size() - 1);
        return true;
      }

      return false;
    }

    bool ArgumentParser::isCommand(const QString& argument)
    {
      if (argument.size() <= 1)
        return false;

      return argument[0] == '/' || argument[0] == '-';
    }

    void ArgumentParser::emitCommands(const QHash<QString, QStringList>& commands)
    {
      Q_FOREACH(QString key, commands.keys()) {
        emit this->commandRecieved(key, commands[key]);
      }
    }

    void ArgumentParser::uriParse(const QStringList& commandArguments, QHash<QString, QStringList>& result)
    {
      if (commandArguments.size() < 1)
        return;

      QString uriArg;
      
      uriArg = commandArguments.at(0);
      QUrl url(uriArg);
      if (url.scheme() != this->_uriPrefix)
        return;

      QString commandName = url.host();
      QString argsString = url.path();
      QStringList args = argsString.split('/', QString::SkipEmptyParts);
      result[commandName] = args;
    }

    void ArgumentParser::preparseCommandAndPutToResult(QString &name, QStringList commandArguments, QHash<QString, QStringList> &result)
    {
      if (name == "uri") {
        this->uriParse(commandArguments, result);
      } else if (name.startsWith("uri:") && name.length() > 4) {
        QStringList tmp;
        tmp << name.right(name.length() - 4);
        this->uriParse(tmp, result);
      } else {
        result[name] = commandArguments;
      }
    }

  }
}