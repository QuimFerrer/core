/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Source file for the Xbp*Classes
 *
 * Copyright 2009 Pritpal Bedi <pritpal@vouchcac.com>
 * http://www.harbour-project.org
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
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*
 *                               EkOnkar
 *                         ( The LORD is ONE )
 *
 *                Xbase++ Compatible xbpFontDialog Class
 *
 *                  Pritpal Bedi <pritpal@vouchcac.com>
 *                              02Jul2009
 */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#include "hbclass.ch"
#include "common.ch"

#include "xbp.ch"
#include "appevent.ch"
#include "hbqt.ch"

/*----------------------------------------------------------------------*/

CLASS XbpFontDialog INHERIT XbpWindow

   /* Appearance */
   DATA     title                                 INIT   ""
   DATA     buttonApply                           INIT   .F.
   DATA     buttonCancel                          INIT   .T.
   DATA     buttonHelp                            INIT   .F.
   DATA     buttonOk                              INIT   .T.
   DATA     buttonReset                           INIT   .F.
   DATA     strikeOut                             INIT   .T.
   DATA     underscore                            INIT   .T.

   DATA     name                                  INIT   .T.
   DATA     style                                 INIT   .T.
   DATA     size                                  INIT   .T.

   DATA     displayFilter                         INIT   .T.
   DATA     printerFilter                         INIT   .T.

   DATA     familyName                            INIT   " "
   DATA     nominalPointSize                      INIT   0

   DATA     bitmapOnly                            INIT   .F.
   DATA     fixedOnly                             INIT   .F.
   DATA     proportionalOnly                      INIT   .T.


   DATA     outLine                               INIT   .T.
   DATA     previewBGClr                          INIT   GraMakeRGBColor( {255,255,255} )
   DATA     previewFGClr                          INIT   GraMakeRGBColor( {0,0,0} )
   DATA     previewString                         INIT   " "
   DATA     printerPS                             INIT   NIL
   DATA     screenPS                              INIT   NIL

   DATA     synthesizeFonts                       INIT   .T.

   DATA     vectorOnly                            INIT   .F.
   DATA     vectorSizes                           INIT   {}

   DATA     viewPrinterFonts                      INIT   .F.
   DATA     viewScreenFonts                       INIT   .T.

   METHOD   new()
   METHOD   create()
   METHOD   destroy()
   METHOD   display( nMode )
   METHOD   exeBlock()

   DATA     sl_activateApply
   ACCESS   activateApply                         INLINE ::sl_activateApply
   ASSIGN   activateApply( bBlock )               INLINE ::sl_activateApply := bBlock

   DATA     sl_activateCancel
   ACCESS   activateCancel                        INLINE ::sl_activateCancel
   ASSIGN   activateCancel( bBlock )              INLINE ::sl_activateCancel := bBlock

   DATA     sl_activateOk
   ACCESS   activateOk                            INLINE ::sl_activateOk
   ASSIGN   activateOk( bBlock )                  INLINE ::sl_activateOk := bBlock

   DATA     sl_activateReset
   ACCESS   activateReset                         INLINE ::sl_activateReset
   ASSIGN   activateReset( bBlock )               INLINE ::sl_activateReset := bBlock

   DATA     oScreenPS
   DATA     oPrinterPS
   DATA     aPos                                  INIT   { 0, 0 }
   DATA     ok                                    INIT   .f.

   METHOD   GetXbpFont()                          PROTECTED

   DATA     oOptions

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD XbpFontDialog:new( oParent, oOwner, oScreenPS, oPrinterPS, aPos )

   DEFAULT oParent    TO ::oParent
   DEFAULT oOwner     TO ::oOwner
   DEFAULT oScreenPS  TO ::oScreenPS
   DEFAULT oPrinterPS TO ::oPrinterPS
   DEFAULT aPos       TO ::aPos

   ::oParent    := oParent
   ::oOwner     := oOwner
   ::oScreenPS  := oScreenPS
   ::oPrinterPS := oPrinterPS
   ::aPos       := aPos

   ::xbpWindow:init( oParent, oOwner )

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpFontDialog:create( oParent, oOwner, oScreenPS, oPrinterPS, aPos )

   DEFAULT oParent    TO ::oParent
   DEFAULT oOwner     TO ::oOwner
   DEFAULT oScreenPS  TO ::oScreenPS
   DEFAULT oPrinterPS TO ::oPrinterPS
   DEFAULT aPos       TO ::aPos

   ::oParent    := oParent
   ::oOwner     := oOwner
   ::oScreenPS  := oScreenPS
   ::oPrinterPS := oPrinterPS
   ::aPos       := aPos

   IF ::viewPrinterFonts .and. ::oPrinterPS == NIL
      ::viewPrinterFonts := .f.
   ENDIF
   IF ( ! ::viewScreenFonts .and. ! ::viewPrinterFonts )
      ::viewScreenFonts := .t.
   ENDIF

   ::xbpWindow:create( oParent, oOwner )

   ::oWidget := QFontDialog():new()

   ::connect( ::pwidget, "accepted()"               , {|o,p| ::exeBlock( 1, p, o ) } )
   ::connect( ::pwidget, "finished(int)"            , {|o,p| ::exeBlock( 2, p, o ) } )
   ::connect( ::pwidget, "rejected()"               , {|o,p| ::exeBlock( 3, p, o ) } )
   ::connect( ::pwidget, "currentFontChanged(QFont)", {|o,p| ::exeBlock( 4, p, o ) } )
   ::connect( ::pwidget, "fontSelected(QFont)"      , {|o,p| ::exeBlock( 5, p, o ) } )

   IF ::aPos[ 1 ] + ::aPos[ 2 ] != 0
      ::setPos()
   ENDIF
   ::oParent:addChild( Self )
   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpFontDialog:exeBlock( nEvent, p1 )
   LOCAL nRet := XBP_ALLOW

   HB_SYMBOL_UNUSED( p1 )

   DO CASE
   CASE nEvent == 3
      IF hb_isBlock( ::sl_quit )
         nRet := eval( ::sl_quit, 0, 0, Self )
      ENDIF
      IF nRet == XBP_REJECT
         ::oWidget:reject()
      ELSE
         ::oWidget:accept()
      ENDIF
   ENDCASE

   RETURN nRet

/*----------------------------------------------------------------------*/

METHOD XbpFontDialog:display( nMode )
   LOCAL aInfo := nMode
   LOCAL nResult

   //::setPosAndSize()

   IF nMode == 0                                   // Parent and modal
      nResult := ::oWidget:exec()
      nMode   := nResult
   ELSE                                            // Non-modal
      ::oWidget:show()
   ENDIF

   RETURN ::GetXbpFont( aInfo )

/*----------------------------------------------------------------------*/

#if 0
METHOD XbpFontDialog:wndProc( hWnd, nMessage, nwParam, nlParam )
   LOCAL aRect, nL, nH

   HB_SYMBOL_UNUSED( nlParam )

   DO CASE

   CASE nMessage == WM_INITDIALOG
      ::hWnd := hWnd

      IF !empty( ::title )
         Win_setWindowText( ::hWnd, ::title )
      ENDIF
      IF !( ::buttonCancel )
         Win_EnableWindow( Win_GetDlgItem( ::hWnd,IDCANCEL ), .f. )
      ENDIF
      IF !( ::buttonApply )
         Win_EnableWindow( Win_GetDlgItem( ::hWnd,1026 ), .f. )
      ENDIF
      IF !( ::buttonHelp )
         Win_EnableWindow( Win_GetDlgItem( ::hWnd,1038 ), .f. )
      ENDIF
      IF !( ::strikeOut )
         Win_EnableWindow( Win_GetDlgItem( ::hWnd,1040 ), .f. )
      ENDIF
      IF !( ::underscore )
         Win_EnableWindow( Win_GetDlgItem( ::hWnd,1041 ), .f. )
      ENDIF
      IF !( ::name )
         Win_EnableWindow( Win_GetDlgItem( ::hWnd,1136 ), .f. )
      ENDIF
      IF !( ::style )
         Win_EnableWindow( Win_GetDlgItem( ::hWnd,1137 ), .f. )
      ENDIF
      IF !( ::size )
         Win_EnableWindow( Win_GetDlgItem( ::hWnd,1138 ), .f. )
      ENDIF

      IF ::aPos[ 1 ] > 0 .OR. ::aPos[ 2 ] > 0
         aRect := Win_GetWindowRect( ::hWnd )
         Win_MoveWindow( ::hWnd, ::aPos[ 1 ], ::aPos[ 2 ], aRect[3]-aRect[1], aRect[4]-aRect[2], .f. )
      ENDIF

      RETURN 1

   CASE nMessage == WM_COMMAND
      nL := Win_LoWord( nwParam )
      nH := Win_HiWord( nwParam )

      HB_SYMBOL_UNUSED( nH )

      DO CASE

      CASE nL == IDOK
         ::ok := .t.
         IF hb_isBlock( ::sl_activateOk )
            eval( ::sl_activateOk, ::GetxbpFont(), NIL, Self )
         ENDIF

      CASE nL == IDCANCEL
         IF hb_isBlock( ::sl_activateCancel )
            eval( ::sl_activateCancel, NIL, NIL, Self )
         ENDIF

      CASE nL == 1026
         IF hb_isBlock( ::sl_activateApply )
            eval( ::sl_activateApply, ::GetxbpFont(), NIL, Self )
         ENDIF

      CASE nL == 1038  /* Help */

      ENDCASE

   ENDCASE

   RETURN 0
#endif
/*----------------------------------------------------------------------*/

METHOD XbpFontDialog:destroy()

   ::xbpWindow:destroy()

   RETURN Self

/*----------------------------------------------------------------------*/
/*
 * Only callable from ::activateOK and ::activateApply
 */
METHOD XbpFontDialog:GetXbpFont( aFont )
   LOCAL oXbpFont := 0

   HB_SYMBOL_UNUSED( aFont )
   #if 0
   DEFAULT aFont TO Wvg_ChooseFont_GetLogFont( ::hWnd )

   oWvgFont := XbpFont():new()

   oWvgFont:familyName       := aFont[ 1 ]
   oWvgFont:height           := aFont[ 2 ]
   oWvgFont:nominalPointSize := Wvg_HeightToPointSize( /* hdc */, oWvgFont:height )
   oWvgFont:width            := aFont[ 3 ]
   oWvgFont:bold             := aFont[ 4 ] > 400
   oWvgFont:italic           := aFont[ 5 ]
   oWvgFont:underscore       := aFont[ 6 ]
   oWvgFont:strikeOut        := aFont[ 7 ]
   oWvgFont:codePage         := aFont[ 8 ]
   oWvgFont:setCompoundName( trim( aFont[ 1 ] +" "+ IF( oWvgFont:bold, "Bold ", "" ) + ;
                                                    IF( oWvgFont:italic, "Italic", "" ) ) )
   oWvgFont:create()
   #endif
   RETURN oXbpFont

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
//
//                          Class XbpFont()
//
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

CLASS XbpFont

   DATA     hFont
   DATA     oPS
   DATA     hdc

   DATA     familyName                            INIT   ""
   DATA     height                                INIT   0
   DATA     nominalPointSize                      INIT   0

   DATA     width                                 INIT   0
   DATA     widthClass                            INIT   .F.

   DATA     bold                                  INIT   .F.
   DATA     weightClass                           INIT   0 //FW_DONTCARE

   DATA     italic                                INIT   .F.
   DATA     strikeout                             INIT   .F.
   DATA     underscore                            INIT   .F.
   DATA     codePage                              INIT   0 //DEFAULT_CHARSET

   DATA     fixed                                 INIT   .F.
   DATA     antiAliased                           INIT   .F.

   DATA     compoundName                          INIT   ""
   METHOD   setCompoundName( cName )              INLINE ::compoundName := cName

   DATA     generic                               INIT   .T.

   DATA     baseLine                              INIT   0                READONLY
   DATA     dbcs                                  INIT   .F.
   DATA     kerning                               INIT   .F.
   DATA     mbcs                                  INIT   .F.
   DATA     vector                                INIT   .F.
   DATA     outlined                              INIT   .F.

   DATA     aFontInfo                             INIT   {}

   METHOD   new( oPS )
   METHOD   create( cFontName )
   METHOD   configure( cFontName )
   METHOD   list()
   METHOD   createFont()

   DESTRUCTOR destroy()

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD XbpFont:new( oPS )

   DEFAULT oPS TO ::oPS

   ::oPS := oPS

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpFont:create( cFontName )

   DEFAULT cFontName TO ::familyName

   ::familyName := cFontName

   ::createFont()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpFont:configure( cFontName )

   DEFAULT cFontName TO ::familyName

   ::familyName := cFontName

   ::createFont()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpFont:destroy()

   ::xbpWindow:destroy()

   RETURN Self

/*----------------------------------------------------------------------*/

METHOD XbpFont:list()
   LOCAL aList := {}

   RETURN aList

/*----------------------------------------------------------------------*/

METHOD XbpFont:createFont()
   LOCAL aFont := {}

   IF ::hFont <> NIL
      // Win_DeleteObject( ::hFont )
      ::hFont := NIL
   ENDIF

   IF ::oPS <> NIL
      //::height := xbp_PointSizeToHeight( ::oPS:hdc, ::nominalPointSize )
   ENDIF

   ::aFontInfo := array( 15 )

   ::aFontInfo[  1 ] := ::familyName
   ::aFontInfo[  2 ] := ::height
   ::aFontInfo[  3 ] := ::width
   ::aFontInfo[  4 ] := IF( ::bold, 0, 0 )
   ::aFontInfo[  5 ] := ::italic
   ::aFontInfo[  6 ] := ::underscore
   ::aFontInfo[  7 ] := ::strikeout
   ::aFontInfo[  8 ] := ::codePage
   ::aFontInfo[  9 ] := 0
   ::aFontInfo[ 10 ] := 0
   ::aFontInfo[ 11 ] := 0
   ::aFontInfo[ 12 ] := 0
   ::aFontInfo[ 13 ] := 0 //DEFAULT_QUALITY
   ::aFontInfo[ 14 ] := NIL

   //aFont := Xbp_FontCreate( ::aFontInfo )

   IF empty( aFont[ 1 ] )
      RETURN nil
   ENDIF

   ::hFont     := aFont[ 15 ]
   ::aFontInfo := aFont

   RETURN ::hFont

/*----------------------------------------------------------------------*/
