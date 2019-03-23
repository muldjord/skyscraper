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

#define MAXSIZE 42000000

NetComm::NetComm()
{
  requestTimer.setSingleShot(true);
  requestTimer.setInterval(60000);
  connect(&requestTimer, &QTimer::timeout, this, &NetComm::requestTimeout);
}

void NetComm::request(QString query, QString postData, QString headerKey, QString headerValue)
{
  QUrl url(query);
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36");
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
  if(!headerKey.isEmpty() && !headerValue.isEmpty()) {
    request.setRawHeader(headerKey.toUtf8(), headerValue.toUtf8());
  }
  
  if(postData.isEmpty()) {
    reply = get(request);
  } else {
    reply = post(request, postData.toUtf8());
  }
  connect(reply, &QNetworkReply::finished, this, &NetComm::replyReady);
  connect(reply, &QNetworkReply::downloadProgress, this, &NetComm::dataDownloaded);
  requestTimer.start();
}

void NetComm::replyReady()
{
  requestTimer.stop();
  data = reply->readAll();
  contentType = reply->rawHeader("Content-Type");;
  redirUrl = reply->rawHeader("Location");
  reply->deleteLater();
  emit dataReady();
}

QByteArray NetComm::getData()
{
  return data;
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
    printf("Too much data! API is buggy, cancelling network request...\n");
    reply->abort();
  }
}

void NetComm::requestTimeout()
{ 
  printf("Request timed out, aborting request...\n");
  reply->abort();
}
