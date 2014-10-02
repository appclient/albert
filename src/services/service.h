// albert - a simple application launcher for linux
// Copyright (C) 2014 Manuel Schneider
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QVector>
#include <QIcon>

/**************************************************************************/
struct Service
{
	class Item;
	struct CaseInsensitiveCompare;
	struct CaseInsensitiveComparePrefix;

	Service(){}
	virtual ~Service(){}
	virtual void query(const QString&, QVector<Item*>*) const noexcept = 0;
	virtual void save (const QString&) const = 0;
	virtual void load (const QString&) = 0;
};

/**************************************************************************/
struct Service::Item
{
	Item(){}
	virtual ~Item(){}

	virtual QString   title()      const = 0;
	virtual QIcon     icon()       const = 0;
	virtual QString   infoText()   const = 0;
	virtual QString   complete()   const = 0;
	virtual void      action()           = 0;
	virtual QString   actionText() const = 0;
	qint64            lastAccess() const {return _lastAccess;}

protected:
	qint64 _lastAccess;
};



/**************************************************************************/
struct Service::CaseInsensitiveCompare
{
	inline bool operator()( Service::Item const* lhs, Service::Item const* rhs )       const {return (*this)( lhs->title(), rhs->title());}
	inline bool operator()( QString const& lhs, Service::Item const* rhs )    const {return (*this)( lhs, rhs->title() );}
	inline bool operator()( Service::Item const* lhs, QString const& rhs )    const {return (*this)( lhs->title(), rhs );}
	inline bool operator()( QString const& lhs, QString const& rhs ) const {return lhs.compare(rhs, Qt::CaseInsensitive)<0;}
};



/**************************************************************************/
struct Service::CaseInsensitiveComparePrefix
{
	inline bool operator()( Service::Item const* pre, Service::Item const* rhs ) const {return (*this)( pre->title(), rhs->title() );}
	inline bool operator()( QString const& pre, Service::Item const* rhs ) const {return (*this)( pre, rhs->title() );}
	inline bool operator()( Service::Item const* pre, QString const& rhs ) const {return (*this)( pre->title(), rhs );}
	inline bool operator()( QString const& pre, QString const& rhs ) const	{
		QString::const_iterator a,b;
		a = pre.cbegin();
		b = rhs.cbegin();
		QChar ca,cb;
		while (a != pre.cend() && b != rhs.end()){
			ca = a++->toLower();
			cb = b++->toLower();
			if (ca < cb)
				return true;
			if (ca > cb)
				return false;
		}
		return false;
	}
};

#endif // SERVICE_H
