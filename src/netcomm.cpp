/***************************************************************************
 *            netcomm.cpp
 *
 *  Wed Jun 7 12:00:00 CEST 2017
 *  Copyright 2017 Lars Muldjord
 *  muldjordlars@gmail.com
 ****************************************************************************/
/*
 *  This file is part of skyscraper.
 *
 *  skyscraper is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  skyscraper is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with skyscraper; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#include "netcomm.h"

#include <QUrl>
#include <QNetworkRequest>

constexpr int MAXSIZE = 100*1024*1024;

NetComm::NetComm(QSharedPointer<NetManager> manager)
  : manager(manager)
{
  requestTimer.setSingleShot(true);
  requestTimer.setInterval(30000);
  connect(&requestTimer, &QTimer::timeout, this, &NetComm::requestTimeout);
}

void NetComm::request(QString query, QString postData, QList<QPair<QString, QString> > headers)
{
  QUrl url(query);
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:74.0) Gecko/20100101 Firefox/74.0");
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

  if(!headers.isEmpty()) {
    for(const auto &header: headers) {
      request.setRawHeader(header.first.toUtf8(), header.second.toUtf8());
    }
  }

  if(postData.isNull()) {
    reply = manager->getRequest(request);
  } else {
    reply = manager->postRequest(request, postData.toUtf8());
  }
  connect(reply, &QNetworkReply::finished, this, &NetComm::replyReady);
  connect(reply, &QNetworkReply::downloadProgress, this, &NetComm::dataDownloaded);
  requestTimer.start();
}

void NetComm::replyReady()
{
  requestTimer.stop();
  data = reply->readAll();
  error = reply->error();
  contentType = reply->rawHeader("Content-Type");;
  redirUrl = reply->rawHeader("Location");
  reply->deleteLater();
  emit dataReady();
}

QByteArray NetComm::getData()
{
  return data;
}

QNetworkReply::NetworkError NetComm::getError(const int &verbosity)
{
  if(error != QNetworkReply::NoError && verbosity >= 1) {
    switch(error) {
    case QNetworkReply::RemoteHostClosedError:
      // 'screenscraper' will often give this error when it's overloaded.
      // But since we retry a couple of times, it's rarely a problem.
      printf("\033[1;31mNetwork error: 'QNetworkReply::RemoteHostClosedError', scraping module service might be overloaded.\033[0m\n");
      break;
    case QNetworkReply::TimeoutError:
      printf("\033[1;31mNetwork error: 'QNetworkReply::TimeoutError'\033[0m\n");
      break;
    case QNetworkReply::NetworkSessionFailedError:
      printf("\033[1;31mNetwork error: 'QNetworkReply::NetworkSessionFailedError'\033[0m\n");
      break;
    case QNetworkReply::ContentNotFoundError:
      // Don't show an error on these. For some modules I am guessing for urls and
      // sometimes they simply don't exist. It's not an error in those cases.
      //printf("\033[1;31mNetwork error: 'QNetworkReply::ContentNotFoundError'\033[0m\n");
      break;
    case QNetworkReply::ContentReSendError:
      printf("\033[1;31mNetwork error: 'QNetworkReply::ContentReSendError'\033[0m\n");
      break;
    case QNetworkReply::ContentGoneError:
      printf("\033[1;31mNetwork error: 'QNetworkReply::ContentGoneError'\033[0m\n");
      break;
    case QNetworkReply::InternalServerError:
      printf("\033[1;31mNetwork error: 'QNetworkReply::InternalServerError'\033[0m\n");
      break;
    case QNetworkReply::UnknownNetworkError:
      printf("\033[1;31mNetwork error: 'QNetworkReply::UnknownNetworkError'\033[0m\n");
      break;
    case QNetworkReply::UnknownContentError:
      printf("\033[1;31mNetwork error: 'QNetworkReply::UnknownContentError'\033[0m\n");
      break;
    case QNetworkReply::UnknownServerError:
      printf("\033[1;31mNetwork error: 'QNetworkReply::UnknownServerError'\033[0m\n");
      break;
    default:
      printf("\033[1;31mNetwork error: '%d'\033[0m\n", error);
      break;
    }
  }
  return error;
}

QByteArray NetComm::getContentType()
{
  return contentType;
}

QByteArray NetComm::getRedirUrl()
{
  return redirUrl;
}

void NetComm::dataDownloaded(qint64 bytesReceived, qint64)
{
  if(bytesReceived > MAXSIZE) {
    printf("Retrieved data size exceeded maximum of 100 MB, cancelling network request...\n");
    reply->abort();
  }
}

void NetComm::requestTimeout()
{ 
  printf("\033[1;33mRequest timed out, server might be busy / overloaded...\033[0m\n");
  reply->abort();
}
