/*
 * $Id$
 */

/* -------------------------------------------------------------------- */
/* WARNING: Automatically generated source file. DO NOT EDIT!           */
/*          Instead, edit corresponding .qth file,                      */
/*          or the generator tool itself, and run regenarate.           */
/* -------------------------------------------------------------------- */

/*
 * Harbour Project source code:
 * QT wrapper main header
 *
 * Copyright 2009 Pritpal Bedi <pritpal@vouchcac.com>
 *
 * Copyright 2009 Marcos Antonio Gambeta <marcosgambeta at gmail dot com>
 * www - http://www.harbour-project.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA (or visit the web site http://www.gnu.org/).
 *
 * As a special exception, the Harbour Project gives permission for
 * additional uses of the text contained in its release of Harbour.
 *
 * The exception is that, if you link the Harbour libraries with other
 * files to produce an executable, this does not by itself cause the
 * resulting executable to be covered by the GNU General Public License.
 * Your use of that executable is in no way restricted on account of
 * linking the Harbour library code into it.
 *
 * This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU General Public License.
 *
 * This exception applies only to the code released by the Harbour
 * Project under the name Harbour.  If you copy code from other
 * Harbour Project or Free Software Foundation releases into a copy of
 * Harbour, as the General Public License permits, the exception does
 * not apply to the code that you add in this way.  To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for Harbour, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 *
 */
/*----------------------------------------------------------------------*/

#include "hbapi.h"
#include "../hbqt.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/

/*
 *  enum HorizontalHeaderFormat { SingleLetterDayNames, ShortDayNames, LongDayNames, NoHorizontalHeader }
 *  enum SelectionMode { NoSelection, SingleSelection }
 *  enum VerticalHeaderFormat { ISOWeekNumbers, NoVerticalHeader }
 */

/*
 *  Constructed[ 38/39 [ 97.44% ] ]
 *
 *  *** Unconvered Prototypes ***
 *  -----------------------------
 *
 *  QMap<QDate, QTextCharFormat> dateTextFormat () const
 */

#include <QtCore/QPointer>

#include <QtGui/QCalendarWidget>
#include <QtGui/QTextCharFormat>

/*
 * QCalendarWidget ( QWidget * parent = 0 )
 * ~QCalendarWidget ()
 */

typedef struct
{
  void * ph;
  QT_G_FUNC_PTR func;
  QPointer< QCalendarWidget > pq;
} QGC_POINTER_QCalendarWidget;

QT_G_FUNC( release_QCalendarWidget )
{
   QGC_POINTER_QCalendarWidget * p = ( QGC_POINTER_QCalendarWidget * ) Cargo;

   HB_TRACE( HB_TR_DEBUG, ( "release_QCalendarWidget              p=%p", p));
   HB_TRACE( HB_TR_DEBUG, ( "release_QCalendarWidget             ph=%p pq=%p", p->ph, (void *)(p->pq)));

   if( p && p->ph && p->pq )
   {
      const QMetaObject * m = ( ( QObject * ) p->ph )->metaObject();
      if( ( QString ) m->className() != ( QString ) "QObject" )
      {
         switch( hbqt_get_object_release_method() )
         {
         case HBQT_RELEASE_WITH_DELETE:
            delete ( ( QCalendarWidget * ) p->ph );
            break;
         case HBQT_RELEASE_WITH_DESTRUTOR:
            ( ( QCalendarWidget * ) p->ph )->~QCalendarWidget();
            break;
         case HBQT_RELEASE_WITH_DELETE_LATER:
            ( ( QCalendarWidget * ) p->ph )->deleteLater();
            break;
         }
         p->ph = NULL;
         HB_TRACE( HB_TR_DEBUG, ( "release_QCalendarWidget             Object deleted!" ) );
         #if defined( __HB_DEBUG__ )
            hbqt_debug( "  YES release_QCalendarWidget             %i B %i KB", ( int ) hb_xquery( 1001 ), hbqt_getmemused() );
         #endif
      }
      else
      {
         HB_TRACE( HB_TR_DEBUG, ( "release_QCalendarWidget             Object Name Missing!" ) );
         #if defined( __HB_DEBUG__ )
            hbqt_debug( "  NO  release_QCalendarWidget" );
         #endif
      }
   }
   else
   {
      HB_TRACE( HB_TR_DEBUG, ( "release_QCalendarWidget             Object Allready deleted!" ) );
      #if defined( __HB_DEBUG__ )
         hbqt_debug( "  DEL release_QCalendarWidget" );
      #endif
   }
}

void * gcAllocate_QCalendarWidget( void * pObj )
{
   QGC_POINTER_QCalendarWidget * p = ( QGC_POINTER_QCalendarWidget * ) hb_gcAllocate( sizeof( QGC_POINTER_QCalendarWidget ), gcFuncs() );

   p->ph = pObj;
   p->func = release_QCalendarWidget;
   new( & p->pq ) QPointer< QCalendarWidget >( ( QCalendarWidget * ) pObj );
   #if defined( __HB_DEBUG__ )
      hbqt_debug( "          new_QCalendarWidget             %i B %i KB", ( int ) hb_xquery( 1001 ), hbqt_getmemused() );
   #endif
   return( p );
}

HB_FUNC( QT_QCALENDARWIDGET )
{
   void * pObj = NULL;

   pObj = ( QCalendarWidget* ) new QCalendarWidget( hbqt_par_QWidget( 1 ) ) ;

   hb_retptrGC( gcAllocate_QCalendarWidget( pObj ) );
}
/*
 * int dateEditAcceptDelay () const
 */
HB_FUNC( QT_QCALENDARWIDGET_DATEEDITACCEPTDELAY )
{
   hb_retni( hbqt_par_QCalendarWidget( 1 )->dateEditAcceptDelay() );
}

/*
 * QTextCharFormat dateTextFormat ( const QDate & date ) const
 */
HB_FUNC( QT_QCALENDARWIDGET_DATETEXTFORMAT )
{
   hb_retptrGC( gcAllocate_QTextCharFormat( new QTextCharFormat( hbqt_par_QCalendarWidget( 1 )->dateTextFormat( *hbqt_par_QDate( 2 ) ) ) ) );
}

/*
 * Qt::DayOfWeek firstDayOfWeek () const
 */
HB_FUNC( QT_QCALENDARWIDGET_FIRSTDAYOFWEEK )
{
   hb_retni( ( Qt::DayOfWeek ) hbqt_par_QCalendarWidget( 1 )->firstDayOfWeek() );
}

/*
 * QTextCharFormat headerTextFormat () const
 */
HB_FUNC( QT_QCALENDARWIDGET_HEADERTEXTFORMAT )
{
   hb_retptrGC( gcAllocate_QTextCharFormat( new QTextCharFormat( hbqt_par_QCalendarWidget( 1 )->headerTextFormat() ) ) );
}

/*
 * HorizontalHeaderFormat horizontalHeaderFormat () const
 */
HB_FUNC( QT_QCALENDARWIDGET_HORIZONTALHEADERFORMAT )
{
   hb_retni( ( QCalendarWidget::HorizontalHeaderFormat ) hbqt_par_QCalendarWidget( 1 )->horizontalHeaderFormat() );
}

/*
 * bool isDateEditEnabled () const
 */
HB_FUNC( QT_QCALENDARWIDGET_ISDATEEDITENABLED )
{
   hb_retl( hbqt_par_QCalendarWidget( 1 )->isDateEditEnabled() );
}

/*
 * bool isGridVisible () const
 */
HB_FUNC( QT_QCALENDARWIDGET_ISGRIDVISIBLE )
{
   hb_retl( hbqt_par_QCalendarWidget( 1 )->isGridVisible() );
}

/*
 * bool isNavigationBarVisible () const
 */
HB_FUNC( QT_QCALENDARWIDGET_ISNAVIGATIONBARVISIBLE )
{
   hb_retl( hbqt_par_QCalendarWidget( 1 )->isNavigationBarVisible() );
}

/*
 * QDate maximumDate () const
 */
HB_FUNC( QT_QCALENDARWIDGET_MAXIMUMDATE )
{
   hb_retptrGC( gcAllocate_QDate( new QDate( hbqt_par_QCalendarWidget( 1 )->maximumDate() ) ) );
}

/*
 * QDate minimumDate () const
 */
HB_FUNC( QT_QCALENDARWIDGET_MINIMUMDATE )
{
   hb_retptrGC( gcAllocate_QDate( new QDate( hbqt_par_QCalendarWidget( 1 )->minimumDate() ) ) );
}

/*
 * int monthShown () const
 */
HB_FUNC( QT_QCALENDARWIDGET_MONTHSHOWN )
{
   hb_retni( hbqt_par_QCalendarWidget( 1 )->monthShown() );
}

/*
 * QDate selectedDate () const
 */
HB_FUNC( QT_QCALENDARWIDGET_SELECTEDDATE )
{
   hb_retptrGC( gcAllocate_QDate( new QDate( hbqt_par_QCalendarWidget( 1 )->selectedDate() ) ) );
}

/*
 * SelectionMode selectionMode () const
 */
HB_FUNC( QT_QCALENDARWIDGET_SELECTIONMODE )
{
   hb_retni( ( QCalendarWidget::SelectionMode ) hbqt_par_QCalendarWidget( 1 )->selectionMode() );
}

/*
 * void setDateEditAcceptDelay ( int delay )
 */
HB_FUNC( QT_QCALENDARWIDGET_SETDATEEDITACCEPTDELAY )
{
   hbqt_par_QCalendarWidget( 1 )->setDateEditAcceptDelay( hb_parni( 2 ) );
}

/*
 * void setDateEditEnabled ( bool enable )
 */
HB_FUNC( QT_QCALENDARWIDGET_SETDATEEDITENABLED )
{
   hbqt_par_QCalendarWidget( 1 )->setDateEditEnabled( hb_parl( 2 ) );
}

/*
 * void setDateTextFormat ( const QDate & date, const QTextCharFormat & format )
 */
HB_FUNC( QT_QCALENDARWIDGET_SETDATETEXTFORMAT )
{
   hbqt_par_QCalendarWidget( 1 )->setDateTextFormat( *hbqt_par_QDate( 2 ), *hbqt_par_QTextCharFormat( 3 ) );
}

/*
 * void setFirstDayOfWeek ( Qt::DayOfWeek dayOfWeek )
 */
HB_FUNC( QT_QCALENDARWIDGET_SETFIRSTDAYOFWEEK )
{
   hbqt_par_QCalendarWidget( 1 )->setFirstDayOfWeek( ( Qt::DayOfWeek ) hb_parni( 2 ) );
}

/*
 * void setHeaderTextFormat ( const QTextCharFormat & format )
 */
HB_FUNC( QT_QCALENDARWIDGET_SETHEADERTEXTFORMAT )
{
   hbqt_par_QCalendarWidget( 1 )->setHeaderTextFormat( *hbqt_par_QTextCharFormat( 2 ) );
}

/*
 * void setHorizontalHeaderFormat ( HorizontalHeaderFormat format )
 */
HB_FUNC( QT_QCALENDARWIDGET_SETHORIZONTALHEADERFORMAT )
{
   hbqt_par_QCalendarWidget( 1 )->setHorizontalHeaderFormat( ( QCalendarWidget::HorizontalHeaderFormat ) hb_parni( 2 ) );
}

/*
 * void setMaximumDate ( const QDate & date )
 */
HB_FUNC( QT_QCALENDARWIDGET_SETMAXIMUMDATE )
{
   hbqt_par_QCalendarWidget( 1 )->setMaximumDate( *hbqt_par_QDate( 2 ) );
}

/*
 * void setMinimumDate ( const QDate & date )
 */
HB_FUNC( QT_QCALENDARWIDGET_SETMINIMUMDATE )
{
   hbqt_par_QCalendarWidget( 1 )->setMinimumDate( *hbqt_par_QDate( 2 ) );
}

/*
 * void setSelectionMode ( SelectionMode mode )
 */
HB_FUNC( QT_QCALENDARWIDGET_SETSELECTIONMODE )
{
   hbqt_par_QCalendarWidget( 1 )->setSelectionMode( ( QCalendarWidget::SelectionMode ) hb_parni( 2 ) );
}

/*
 * void setVerticalHeaderFormat ( VerticalHeaderFormat format )
 */
HB_FUNC( QT_QCALENDARWIDGET_SETVERTICALHEADERFORMAT )
{
   hbqt_par_QCalendarWidget( 1 )->setVerticalHeaderFormat( ( QCalendarWidget::VerticalHeaderFormat ) hb_parni( 2 ) );
}

/*
 * void setWeekdayTextFormat ( Qt::DayOfWeek dayOfWeek, const QTextCharFormat & format )
 */
HB_FUNC( QT_QCALENDARWIDGET_SETWEEKDAYTEXTFORMAT )
{
   hbqt_par_QCalendarWidget( 1 )->setWeekdayTextFormat( ( Qt::DayOfWeek ) hb_parni( 2 ), *hbqt_par_QTextCharFormat( 3 ) );
}

/*
 * VerticalHeaderFormat verticalHeaderFormat () const
 */
HB_FUNC( QT_QCALENDARWIDGET_VERTICALHEADERFORMAT )
{
   hb_retni( ( QCalendarWidget::VerticalHeaderFormat ) hbqt_par_QCalendarWidget( 1 )->verticalHeaderFormat() );
}

/*
 * QTextCharFormat weekdayTextFormat ( Qt::DayOfWeek dayOfWeek ) const
 */
HB_FUNC( QT_QCALENDARWIDGET_WEEKDAYTEXTFORMAT )
{
   hb_retptrGC( gcAllocate_QTextCharFormat( new QTextCharFormat( hbqt_par_QCalendarWidget( 1 )->weekdayTextFormat( ( Qt::DayOfWeek ) hb_parni( 2 ) ) ) ) );
}

/*
 * int yearShown () const
 */
HB_FUNC( QT_QCALENDARWIDGET_YEARSHOWN )
{
   hb_retni( hbqt_par_QCalendarWidget( 1 )->yearShown() );
}

/*
 * void setCurrentPage ( int year, int month )
 */
HB_FUNC( QT_QCALENDARWIDGET_SETCURRENTPAGE )
{
   hbqt_par_QCalendarWidget( 1 )->setCurrentPage( hb_parni( 2 ), hb_parni( 3 ) );
}

/*
 * void setDateRange ( const QDate & min, const QDate & max )
 */
HB_FUNC( QT_QCALENDARWIDGET_SETDATERANGE )
{
   hbqt_par_QCalendarWidget( 1 )->setDateRange( *hbqt_par_QDate( 2 ), *hbqt_par_QDate( 3 ) );
}

/*
 * void setGridVisible ( bool show )
 */
HB_FUNC( QT_QCALENDARWIDGET_SETGRIDVISIBLE )
{
   hbqt_par_QCalendarWidget( 1 )->setGridVisible( hb_parl( 2 ) );
}

/*
 * void setNavigationBarVisible ( bool visible )
 */
HB_FUNC( QT_QCALENDARWIDGET_SETNAVIGATIONBARVISIBLE )
{
   hbqt_par_QCalendarWidget( 1 )->setNavigationBarVisible( hb_parl( 2 ) );
}

/*
 * void setSelectedDate ( const QDate & date )
 */
HB_FUNC( QT_QCALENDARWIDGET_SETSELECTEDDATE )
{
   hbqt_par_QCalendarWidget( 1 )->setSelectedDate( *hbqt_par_QDate( 2 ) );
}

/*
 * void showNextMonth ()
 */
HB_FUNC( QT_QCALENDARWIDGET_SHOWNEXTMONTH )
{
   hbqt_par_QCalendarWidget( 1 )->showNextMonth();
}

/*
 * void showNextYear ()
 */
HB_FUNC( QT_QCALENDARWIDGET_SHOWNEXTYEAR )
{
   hbqt_par_QCalendarWidget( 1 )->showNextYear();
}

/*
 * void showPreviousMonth ()
 */
HB_FUNC( QT_QCALENDARWIDGET_SHOWPREVIOUSMONTH )
{
   hbqt_par_QCalendarWidget( 1 )->showPreviousMonth();
}

/*
 * void showPreviousYear ()
 */
HB_FUNC( QT_QCALENDARWIDGET_SHOWPREVIOUSYEAR )
{
   hbqt_par_QCalendarWidget( 1 )->showPreviousYear();
}

/*
 * void showSelectedDate ()
 */
HB_FUNC( QT_QCALENDARWIDGET_SHOWSELECTEDDATE )
{
   hbqt_par_QCalendarWidget( 1 )->showSelectedDate();
}

/*
 * void showToday ()
 */
HB_FUNC( QT_QCALENDARWIDGET_SHOWTODAY )
{
   hbqt_par_QCalendarWidget( 1 )->showToday();
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/
