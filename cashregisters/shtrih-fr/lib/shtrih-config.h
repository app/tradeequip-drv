/****************************************************************************
** $Id: shtrih-config.h,v 1.3 2006/04/28 12:23:51 red75 Exp $
**
** Trade equipment common portable library project
**
** Copyright (C) 1999-2006 Leader InfoTech.  All rights reserved.
**
** This file is part of the Library of the Ananas
** automation accounting system.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.leaderit.ru/ or email sales@leaderit.ru
**
** Contact org@leaderit.ru if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef CONFIGSHTRIHFR_H

#define CONFIGSHTRIHFR_H
#include <teglobal.h> // Config from tradeequip
#ifdef SHTRIH_BUILD
#  define SHTRIH_EXPORT LIB_DLLEXPORT
#  define SHTRIH_TEMPLATE_EXTERN
#else
#  define SHTRIH_EXPORT LIB_DLLIMPORT
#  define SHTRIH_TEMPLATE_EXTERN TEMPLATE_EXTERN
#endif

#endif
