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
 * Copyright 2009-2010 Pritpal Bedi <bedipritpal@hotmail.com>
 * www - http://harbour-project.org
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
/*                            C R E D I T S                             */
/*----------------------------------------------------------------------*/
/*
 * Marcos Antonio Gambeta
 *    for providing first ever prototype parsing methods. Though the current
 *    implementation is diametrically different then what he proposed, still
 *    current code shaped on those footsteps.
 *
 * Viktor Szakats
 *    for directing the project with futuristic vision;
 *    for designing and maintaining a complex build system for hbQT, hbIDE;
 *    for introducing many constructs on PRG and C++ levels;
 *    for streamlining signal/slots and events management classes;
 *
 * Istvan Bisz
 *    for introducing QPointer<> concept in the generator;
 *    for testing the library on numerous accounts;
 *    for showing a way how a GC pointer can be detached;
 *
 * Francesco Perillo
 *    for taking keen interest in hbQT development and peeking the code;
 *    for providing tips here and there to improve the code quality;
 *    for hitting bulls eye to describe why few objects need GC detachment;
 *
 * Carlos Bacco
 *    for implementing HBQT_TYPE_Q*Class enums;
 *    for peeking into the code and suggesting optimization points;
 *
 * Przemyslaw Czerpak
 *    for providing tips and trick to manipulate HVM internals to the best
 *    of its use and always showing a path when we get stuck;
 *    A true tradition of a MASTER...
*/
/*----------------------------------------------------------------------*/


#include "hbclass.ch"


FUNCTION QApplication( ... )
   RETURN HB_QApplication():new( ... )


CREATE CLASS QApplication INHERIT HbQtObjectHandler, HB_QCoreApplication FUNCTION HB_QApplication

   METHOD  new( ... )

   METHOD  commitData( pManager )
   METHOD  isSessionRestored()
   METHOD  saveState( pManager )
   METHOD  sessionId()
   METHOD  sessionKey()
   METHOD  styleSheet()
   METHOD  activeModalWidget()
   METHOD  activePopupWidget()
   METHOD  activeWindow()
   METHOD  alert( pWidget, nMsec )
   METHOD  beep()
   METHOD  changeOverrideCursor( pCursor )
   METHOD  clipboard()
   METHOD  colorSpec()
   METHOD  cursorFlashTime()
   METHOD  desktop()
   METHOD  desktopSettingsAware()
   METHOD  doubleClickInterval()
   METHOD  exec()
   METHOD  focusWidget()
   METHOD  font( ... )
   METHOD  fontMetrics()
   METHOD  globalStrut()
   METHOD  isEffectEnabled( nEffect )
   METHOD  isLeftToRight()
   METHOD  isRightToLeft()
   METHOD  keyboardInputDirection()
   METHOD  keyboardInputInterval()
   METHOD  keyboardInputLocale()
   METHOD  keyboardModifiers()
   METHOD  layoutDirection()
   METHOD  mouseButtons()
   METHOD  overrideCursor()
   METHOD  palette( ... )
   METHOD  quitOnLastWindowClosed()
   METHOD  restoreOverrideCursor()
   METHOD  setActiveWindow( pActive )
   METHOD  setColorSpec( nSpec )
   METHOD  setCursorFlashTime( nInt )
   METHOD  setDesktopSettingsAware( lOn )
   METHOD  setDoubleClickInterval( nInt )
   METHOD  setEffectEnabled( nEffect, lEnable )
   METHOD  setFont( pFont, pClassName )
   METHOD  setGlobalStrut( pQSize )
   METHOD  setGraphicsSystem( cSystem )
   METHOD  setKeyboardInputInterval( nInt )
   METHOD  setLayoutDirection( nDirection )
   METHOD  setOverrideCursor( pCursor )
   METHOD  setPalette( pPalette, pClassName )
   METHOD  setQuitOnLastWindowClosed( lQuit )
   METHOD  setStartDragDistance( nL )
   METHOD  setStartDragTime( nMs )
   METHOD  setStyle( ... )
   METHOD  setWheelScrollLines( nInt )
   METHOD  setWindowIcon( pIcon )
   METHOD  startDragDistance()
   METHOD  startDragTime()
   METHOD  style()
   METHOD  syncX()
   METHOD  topLevelAt( ... )
   METHOD  type()
   METHOD  wheelScrollLines()
   METHOD  widgetAt( ... )
   METHOD  windowIcon()
   METHOD  aboutQt()
   METHOD  closeAllWindows()
   METHOD  setStyleSheet( cSheet )

   ENDCLASS


METHOD QApplication:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QApplication( ... )
   RETURN Self


METHOD QApplication:commitData( pManager )
   RETURN Qt_QApplication_commitData( ::pPtr, hbqt_ptr( pManager ) )


METHOD QApplication:isSessionRestored()
   RETURN Qt_QApplication_isSessionRestored( ::pPtr )


METHOD QApplication:saveState( pManager )
   RETURN Qt_QApplication_saveState( ::pPtr, hbqt_ptr( pManager ) )


METHOD QApplication:sessionId()
   RETURN Qt_QApplication_sessionId( ::pPtr )


METHOD QApplication:sessionKey()
   RETURN Qt_QApplication_sessionKey( ::pPtr )


METHOD QApplication:styleSheet()
   RETURN Qt_QApplication_styleSheet( ::pPtr )


METHOD QApplication:activeModalWidget()
   RETURN HB_QWidget():from( Qt_QApplication_activeModalWidget( ::pPtr ) )


METHOD QApplication:activePopupWidget()
   RETURN HB_QWidget():from( Qt_QApplication_activePopupWidget( ::pPtr ) )


METHOD QApplication:activeWindow()
   RETURN HB_QWidget():from( Qt_QApplication_activeWindow( ::pPtr ) )


METHOD QApplication:alert( pWidget, nMsec )
   RETURN Qt_QApplication_alert( ::pPtr, hbqt_ptr( pWidget ), nMsec )


METHOD QApplication:beep()
   RETURN Qt_QApplication_beep( ::pPtr )


METHOD QApplication:changeOverrideCursor( pCursor )
   RETURN Qt_QApplication_changeOverrideCursor( ::pPtr, hbqt_ptr( pCursor ) )


METHOD QApplication:clipboard()
   RETURN HB_QClipboard():from( Qt_QApplication_clipboard( ::pPtr ) )


METHOD QApplication:colorSpec()
   RETURN Qt_QApplication_colorSpec( ::pPtr )


METHOD QApplication:cursorFlashTime()
   RETURN Qt_QApplication_cursorFlashTime( ::pPtr )


METHOD QApplication:desktop()
   RETURN HB_QDesktopWidget():from( Qt_QApplication_desktop( ::pPtr ) )


METHOD QApplication:desktopSettingsAware()
   RETURN Qt_QApplication_desktopSettingsAware( ::pPtr )


METHOD QApplication:doubleClickInterval()
   RETURN Qt_QApplication_doubleClickInterval( ::pPtr )


METHOD QApplication:exec()
   RETURN Qt_QApplication_exec( ::pPtr )


METHOD QApplication:focusWidget()
   RETURN HB_QWidget():from( Qt_QApplication_focusWidget( ::pPtr ) )


METHOD QApplication:font( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         SWITCH __objGetClsName( hb_pvalue( 1 ) )
         CASE "QWIDGET"
            RETURN HB_QFont():from( Qt_QApplication_font_1( ::pPtr, ... ) )
         // RETURN HB_QFont():from( Qt_QApplication_font_2( ::pPtr, ... ) )
         ENDSWITCH
      ENDCASE
      EXIT
   CASE 0
      RETURN HB_QFont():from( Qt_QApplication_font( ::pPtr, ... ) )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QApplication:fontMetrics()
   RETURN HB_QFontMetrics():from( Qt_QApplication_fontMetrics( ::pPtr ) )


METHOD QApplication:globalStrut()
   RETURN HB_QSize():from( Qt_QApplication_globalStrut( ::pPtr ) )


METHOD QApplication:isEffectEnabled( nEffect )
   RETURN Qt_QApplication_isEffectEnabled( ::pPtr, nEffect )


METHOD QApplication:isLeftToRight()
   RETURN Qt_QApplication_isLeftToRight( ::pPtr )


METHOD QApplication:isRightToLeft()
   RETURN Qt_QApplication_isRightToLeft( ::pPtr )


METHOD QApplication:keyboardInputDirection()
   RETURN Qt_QApplication_keyboardInputDirection( ::pPtr )


METHOD QApplication:keyboardInputInterval()
   RETURN Qt_QApplication_keyboardInputInterval( ::pPtr )


METHOD QApplication:keyboardInputLocale()
   RETURN HB_QLocale():from( Qt_QApplication_keyboardInputLocale( ::pPtr ) )


METHOD QApplication:keyboardModifiers()
   RETURN Qt_QApplication_keyboardModifiers( ::pPtr )


METHOD QApplication:layoutDirection()
   RETURN Qt_QApplication_layoutDirection( ::pPtr )


METHOD QApplication:mouseButtons()
   RETURN Qt_QApplication_mouseButtons( ::pPtr )


METHOD QApplication:overrideCursor()
   RETURN HB_QCursor():from( Qt_QApplication_overrideCursor( ::pPtr ) )


METHOD QApplication:palette( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         SWITCH __objGetClsName( hb_pvalue( 1 ) )
         CASE "QWIDGET"
            RETURN HB_QPalette():from( Qt_QApplication_palette_1( ::pPtr, ... ) )
         // RETURN HB_QPalette():from( Qt_QApplication_palette_2( ::pPtr, ... ) )
         ENDSWITCH
      ENDCASE
      EXIT
   CASE 0
      RETURN HB_QPalette():from( Qt_QApplication_palette( ::pPtr, ... ) )
   ENDSWITCH
   RETURN hbqt_error()


METHOD QApplication:quitOnLastWindowClosed()
   RETURN Qt_QApplication_quitOnLastWindowClosed( ::pPtr )


METHOD QApplication:restoreOverrideCursor()
   RETURN Qt_QApplication_restoreOverrideCursor( ::pPtr )


METHOD QApplication:setActiveWindow( pActive )
   RETURN Qt_QApplication_setActiveWindow( ::pPtr, hbqt_ptr( pActive ) )


METHOD QApplication:setColorSpec( nSpec )
   RETURN Qt_QApplication_setColorSpec( ::pPtr, nSpec )


METHOD QApplication:setCursorFlashTime( nInt )
   RETURN Qt_QApplication_setCursorFlashTime( ::pPtr, nInt )


METHOD QApplication:setDesktopSettingsAware( lOn )
   RETURN Qt_QApplication_setDesktopSettingsAware( ::pPtr, lOn )


METHOD QApplication:setDoubleClickInterval( nInt )
   RETURN Qt_QApplication_setDoubleClickInterval( ::pPtr, nInt )


METHOD QApplication:setEffectEnabled( nEffect, lEnable )
   RETURN Qt_QApplication_setEffectEnabled( ::pPtr, nEffect, lEnable )


METHOD QApplication:setFont( pFont, pClassName )
   RETURN Qt_QApplication_setFont( ::pPtr, hbqt_ptr( pFont ), hbqt_ptr( pClassName ) )


METHOD QApplication:setGlobalStrut( pQSize )
   RETURN Qt_QApplication_setGlobalStrut( ::pPtr, hbqt_ptr( pQSize ) )


METHOD QApplication:setGraphicsSystem( cSystem )
   RETURN Qt_QApplication_setGraphicsSystem( ::pPtr, cSystem )


METHOD QApplication:setKeyboardInputInterval( nInt )
   RETURN Qt_QApplication_setKeyboardInputInterval( ::pPtr, nInt )


METHOD QApplication:setLayoutDirection( nDirection )
   RETURN Qt_QApplication_setLayoutDirection( ::pPtr, nDirection )


METHOD QApplication:setOverrideCursor( pCursor )
   RETURN Qt_QApplication_setOverrideCursor( ::pPtr, hbqt_ptr( pCursor ) )


METHOD QApplication:setPalette( pPalette, pClassName )
   RETURN Qt_QApplication_setPalette( ::pPtr, hbqt_ptr( pPalette ), hbqt_ptr( pClassName ) )


METHOD QApplication:setQuitOnLastWindowClosed( lQuit )
   RETURN Qt_QApplication_setQuitOnLastWindowClosed( ::pPtr, lQuit )


METHOD QApplication:setStartDragDistance( nL )
   RETURN Qt_QApplication_setStartDragDistance( ::pPtr, nL )


METHOD QApplication:setStartDragTime( nMs )
   RETURN Qt_QApplication_setStartDragTime( ::pPtr, nMs )


METHOD QApplication:setStyle( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) )
         RETURN HB_QStyle():from( Qt_QApplication_setStyle_1( ::pPtr, ... ) )
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QApplication_setStyle( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QApplication:setWheelScrollLines( nInt )
   RETURN Qt_QApplication_setWheelScrollLines( ::pPtr, nInt )


METHOD QApplication:setWindowIcon( pIcon )
   RETURN Qt_QApplication_setWindowIcon( ::pPtr, hbqt_ptr( pIcon ) )


METHOD QApplication:startDragDistance()
   RETURN Qt_QApplication_startDragDistance( ::pPtr )


METHOD QApplication:startDragTime()
   RETURN Qt_QApplication_startDragTime( ::pPtr )


METHOD QApplication:style()
   RETURN HB_QStyle():from( Qt_QApplication_style( ::pPtr ) )


METHOD QApplication:syncX()
   RETURN Qt_QApplication_syncX( ::pPtr )


METHOD QApplication:topLevelAt( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN HB_QWidget():from( Qt_QApplication_topLevelAt_1( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN HB_QWidget():from( Qt_QApplication_topLevelAt( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QApplication:type()
   RETURN Qt_QApplication_type( ::pPtr )


METHOD QApplication:wheelScrollLines()
   RETURN Qt_QApplication_wheelScrollLines( ::pPtr )


METHOD QApplication:widgetAt( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN HB_QWidget():from( Qt_QApplication_widgetAt_1( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN HB_QWidget():from( Qt_QApplication_widgetAt( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN hbqt_error()


METHOD QApplication:windowIcon()
   RETURN HB_QIcon():from( Qt_QApplication_windowIcon( ::pPtr ) )


METHOD QApplication:aboutQt()
   RETURN Qt_QApplication_aboutQt( ::pPtr )


METHOD QApplication:closeAllWindows()
   RETURN Qt_QApplication_closeAllWindows( ::pPtr )


METHOD QApplication:setStyleSheet( cSheet )
   RETURN Qt_QApplication_setStyleSheet( ::pPtr, cSheet )

