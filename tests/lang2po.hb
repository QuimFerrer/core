/*
 * Converts lang modules to standard .po files
 *
 * Copyright 2013 Viktor Szakats (harbour syenar.net)
 * www - http://harbour-project.org
 *
 */

#pragma -w3
#pragma -km+
#pragma -ko+

#include "hblang.ch"

#define LEFTEQUAL( l, r )       ( Left( l, Len( r ) ) == r )

PROCEDURE Main()

   LOCAL tmp, tmp1
   LOCAL nCount
   LOCAL cName
   LOCAL cPO

   nCount := __dynsCount()
   FOR tmp := 1 TO nCount
      cName := __dynsGetName( tmp )
      IF LEFTEQUAL( cName, "HB_LANG_" )
         cName := SubStr( cName, Len( "HB_LANG_" ) + 1 )
         IF Len( cName ) != 5 .AND. ;
            ! "|" + cName + "|" $ "|RUKOI8|UAKOI8|ZHB5|ZHGB|"
            cPO := Item( "", Meta( cName ) )
            /* TODO: do something with the metadata (position 0 to 5) */
            FOR tmp1 := HB_LANG_ITEM_BASE_MONTH TO HB_LANG_ITEM_MAX_ - 1
               cPO += Item( hb_langMessage( tmp1, "en" ), ;
                  iif( hb_langMessage( tmp1, "en" ) == hb_langMessage( tmp1, cName ), "", hb_langMessage( tmp1, cName ) ) )
            NEXT
            hb_MemoWrit( Lower( hb_FNameName( cName ) ) + ".po", hb_StrShrink( cPO, Len( hb_eol() ) ) )
         ENDIF
      ENDIF
   NEXT

   RETURN

STATIC FUNCTION Meta( cName )

   LOCAL cISO_TimeStamp := ISO_TimeStamp()

   LOCAL hMeta
   LOCAL cMeta

   LOCAL meta
   LOCAL tmp

   /* NOTE: workaround for Harbour not retaining definition order of hash literals */
   hMeta := { => }
   hb_HKeepOrder( hMeta, .T. )
   hMeta[ "Project-Id-Version:"        ] := "harbour"
   hMeta[ "Report-Msgid-Bugs-To:"      ] := "https://groups.google.com/group/harbour-devel/"
   hMeta[ "POT-Creation-Date:"         ] := cISO_TimeStamp
   hMeta[ "PO-Revision-Date:"          ] := cISO_TimeStamp
   hMeta[ "Last-Translator:"           ] := "a b <a.b@c.d>"
   hMeta[ "Language-Team:"             ] := "https://www.transifex.com/projects/p/harbour/"
   hMeta[ "MIME-Version:"              ] := "1.0"
   hMeta[ "Content-Type:"              ] := "text/plain; charset=UTF-8"
   hMeta[ "Content-Transfer-Encoding:" ] := "8bit"

   FOR tmp := 0 TO 5
      hMeta[ hb_StrFormat( "Harbour-Meta-%1$d:", tmp ) ] := hb_langMessage( tmp, cName )
   NEXT

   cMeta := '"' + e"\n"
   FOR EACH meta IN hMeta
      cMeta += ;
         '"' + ;
         meta:__enumKey() + ;
         " " + ;
         meta + ;
         "\n" + ;
         iif( meta:__enumIsLast(), "", '"' + e"\n" )
   NEXT

   RETURN cMeta

STATIC FUNCTION ISO_TimeStamp()

   LOCAL nOffset := hb_UTCOffset()

   RETURN hb_StrFormat( "%1$s%2$s%3$02d%4$02d", ;
      hb_TToC( hb_DateTime(), "YYYY-MM-DD", "HH:MM" ), ;
      iif( nOffset < 0, "-", "+" ), ;
      Int( nOffset / 3600 ), ;
      Int( ( ( nOffset / 3600 ) - Int( nOffset / 3600 ) ) * 60 ) )

STATIC FUNCTION Item( cEN, cTrs )
   RETURN hb_StrFormat( ;
      "#, c-format" + hb_eol() + ;
      'msgid "%1$s"' + hb_eol() + ;
      'msgstr "%2$s"' + hb_eol() + ;
      hb_eol(), cEN, cTrs )
