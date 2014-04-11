/****************************************************************************
** This file is a part of Syncopate Limited GameNet Application or it parts.
**
** Copyright (©) 2011 - 2014, Syncopate Limited and/or affiliates. 
** All rights reserved.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
****************************************************************************/

#include <Application/SingleApplicationEventFilter.h>

#include <QtCore/QDebug>

#include <Windows.h>

namespace GGS {
  namespace Application {
      SingleApplicationEventFilter::SingleApplicationEventFilter(QObject *parent)
        : QObject(parent)
      {
      }

      bool SingleApplicationEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
      {
        MSG* msg = reinterpret_cast<MSG*>(message);
        if (WM_QUERYENDSESSION == msg->message) {
          qDebug() << "WM_QUERYENDSESSION" << " lparam " << msg->lParam << "wparam " << msg->wParam;
          *result = 1;
          return true;
        }

        if (WM_ENDSESSION == msg->message) {
          qDebug() << "WM_ENDSESSION" << " lparam " << msg->lParam << "wparam " << msg->wParam;
          *result = 0;
          emit this->forceQuit();
          return true;
        }
        return false;
      }
  }
}