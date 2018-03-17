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

NetComm::NetComm()
{
  connect(this, &NetComm::finished, this, &NetComm::replyFinished);
  requestTimer.setSingleShot(true);
  requestTimer.setInterval(30000);
  connect(&requestTimer, &QTimer::timeout, this, &NetComm::cancelRequest);
}

void NetComm::request(QString query, QString postData)
{
  clearAll();
  QUrl url(query);
  if(url.isValid()) {
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    
    if(postData.isEmpty()) {
      get(request);
    } else {
      post(request, postData.toUtf8());
    }
  }
  requestTimer.start();
}

void NetComm::replyFinished(QNetworkReply *reply)
{
  requestTimer.stop();

  contentType = reply->rawHeader("Content-Type");

  data = reply->readAll();

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

void NetComm::cancelRequest()
{
  clearAll();
  emit dataReady();
}

void NetComm::clearAll()
{
  clearAccessCache();
  contentType.clear();
  data.clear();
}
