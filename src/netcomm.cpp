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
  data = "";
  requestTimer.setSingleShot(true);
  requestTimer.setInterval(30000);
  connect(&requestTimer, &QTimer::timeout, this, &NetComm::requestTimout);
}

NetComm::~NetComm()
{
}

void NetComm::request(QString query, QString postData)
{
  QUrl url(query);
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36");

  if(postData.isEmpty()) {
    get(request);
  } else {
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    post(request, postData.toUtf8());
  }
  requestTimer.start();
}

void NetComm::replyFinished(QNetworkReply *reply)
{
  requestTimer.stop();
  /*
  QUrl url = reply->url();
  if(reply->error()) {
    qDebug("Download of %s failed: %s\n",
	   url.toEncoded().constData(),
	   qPrintable(reply->errorString()));
  }
  QList<QNetworkReply::RawHeaderPair> headerList = reply->rawHeaderPairs();
  foreach(QNetworkReply::RawHeaderPair header, headerList) {
    qDebug("RAW HEADER: '%s', '%s'\n", header.first.data(), header.second.data());
  }
  */
  // If we got redirected, we need to know where to proceed from
  contentType = reply->rawHeader("Content-Type");

  if(reply->rawHeader("Location").left(4) == "http") {
    redirUrl = reply->rawHeader("Location");
  } else {
    redirUrl = "";
  }
  data = reply->readAll();
  reply->deleteLater();
  emit dataReady();
}

QByteArray NetComm::getData()
{
  return data;
}

QByteArray NetComm::getRedirUrl()
{
  return redirUrl;
}

QByteArray NetComm::getContentType()
{
  return contentType;
}

void NetComm::requestTimout()
{
  data = "";
  redirUrl = "";
  contentType = "";
  emit dataReady();
}
