/*
 * $Id$
 */

/*
 * Copyright 2009 Viktor Szakats (harbour.01 syenar.hu)
 * See COPYING for licensing terms.
 */

/* TOFIX: Ugly hack to avoid #include "directry.ch" */
#define F_NAME          1       /* File name */
#define F_ATTR          5       /* File attribute */

#define _PS_            hb_osPathSeparator()

PROCEDURE Main()
   LOCAL nErrorLevel := 0
   LOCAL cFile
   LOCAL aFile

   LOCAL aArray
   LOCAL tmp
   LOCAL cOptions
   LOCAL cOldDir

   IF Empty( GetEnv( "HB_PLATFORM" ) ) .OR. ;
      Empty( GetEnv( "HB_COMPILER" ) ) .OR. ;
      Empty( GetEnv( "HB_BIN_INSTALL" ) ) .OR. ;
      Empty( GetEnv( "HB_LIB_INSTALL" ) ) .OR. ;
      Empty( GetEnv( "HB_INC_INSTALL" ) )

      OutStd( "! Error: This program has to be called from the GNU Make process." + hb_osNewLine() )
      ErrorLevel( 1 )
      RETURN
   ENDIF

   /* Creating hbmk.cfg */

   OutStd( "! Making " + GetEnv( "HB_BIN_INSTALL" ) + _PS_ + "hbmk.cfg..." + hb_osNewLine() )

   cFile := ""
   cFile += "# hbmk2 configuration" + hb_osNewLine()
   cFile += "# Generated by Harbour build process" + hb_osNewLine()
   cFile += hb_osNewLine()
   cFile += "libpaths=../contrib/%{hb_name}" + hb_osNewLine()
   cFile += "libpaths=../addons/%{hb_name}" + hb_osNewLine()
   cFile += "libpaths=../examples/%{hb_name}" + hb_osNewLine()

   IF GetEnv( "HB_PLATFORM" ) == "dos" .AND. ;
      ! Empty( GetEnv( "HB_HAS_WATT" ) )
      cFile += hb_osNewLine()
      cFile += "{dos&djgpp}syslibs=watt" + hb_osNewLine()
      cFile += "{dos&watcom}syslibs=wattcpwf" + hb_osNewLine()
      cFile += "{dos}libpaths=${WATT_ROOT}/lib" + hb_osNewLine()
   ENDIF

   hb_MemoWrit( GetEnv( "HB_BIN_INSTALL" ) + _PS_ + "hbmk.cfg", cFile )

   /* Installing some misc files */

   IF GetEnv( "HB_PLATFORM" ) $ "win|wce|os2|dos" .AND. ;
      ! Empty( GetEnv( "HB_INSTALL_PREFIX" ) )

      FOR EACH aFile IN Directory( "Change*" )
         hb_FCopy( aFile[ F_NAME ], GetEnv( "HB_INSTALL_PREFIX" ) + _PS_ + iif( GetEnv( "HB_PLATFORM" ) == "dos", "CHANGES", aFile[ F_NAME ] ) )
      NEXT

      hb_FCopy( "COPYING", GetEnv( "HB_INSTALL_PREFIX" ) + _PS_ + "COPYING" )
      hb_FCopy( "INSTALL", GetEnv( "HB_INSTALL_PREFIX" ) + _PS_ + "INSTALL" )
      hb_FCopy( "TODO"   , GetEnv( "HB_INSTALL_PREFIX" ) + _PS_ + "TODO" )
   ENDIF

   /* Import library generation */

   IF GetEnv( "HB_PLATFORM" ) $ "win|wce|os2" .AND. ;
      GetEnv( "HB_BUILD_IMPLIB" ) == "yes" .AND. ;
      ! Empty( GetEnv( "HB_HOST_BIN_DIR" ) )

      aArray := {;
         { "HB_WITH_ADS"       , "Redistribute\ace32.dll"   , .F., "" },;
         { "HB_WITH_ADS"       , "ace32.dll"                , .F., "" },;
         { "HB_WITH_ADS"       , "32bit\ace32.dll"          , .F., "" },;
         { "HB_WITH_ALLEGRO"   , "..\bin\alleg42.dll"       , .T., "alleg" },;
         { "HB_WITH_BLAT"      , "..\blat.dll"              , .T., "" },;
         { "HB_WITH_CAIRO"     , "..\..\bin\libcairo-2.dll" , .T., "cairo" },;
         { "HB_WITH_CURL"      , "..\libcurl.dll"           , .T., "" },;
         { "HB_WITH_CURL"      , "..\bin\libcurl.dll"       , .T., "" },;
         { "HB_WITH_FIREBIRD"  , "..\bin\fbclient.dll"      , .F., "" },; /* Doesn't work with mingw*/cygwin, because .lib has another name in another directory */
         { "HB_WITH_FREEIMAGE" , "..\Dist\FreeImage.dll"    , .F., "" },;
         { "HB_WITH_GD"        , "..\bin\bgd.dll"           , .F., "" },;
         { "HB_WITH_LIBHARU"   , "..\libhpdf.dll"           , .F., "" },;
         { "HB_WITH_LIBHARU"   , "..\lib_dll\libhpdf.dll"   , .F., "" },;
         { "HB_WITH_MYSQL"     , "..\lib\opt\libmySQL.dll"  , .F., "" },;
         { "HB_WITH_OCILIB"    , "..\lib32\ociliba.dll"     , .F., "" },;
         { "HB_WITH_OCILIB"    , "..\lib32\ocilibm.dll"     , .F., "" },;
         { "HB_WITH_OCILIB"    , "..\lib32\ocilibw.dll"     , .F., "" },;
         { "HB_WITH_OPENSSL"   , "..\out32dll\libeay32.dll" , .T., "" },;
         { "HB_WITH_OPENSSL"   , "..\out32dll\ssleay32.dll" , .T., "" },;
         { "HB_WITH_OPENSSL"   , "..\dll\libeay32.dll"      , .T., "" },;
         { "HB_WITH_OPENSSL"   , "..\dll\ssleay32.dll"      , .T., "" },;
         { "HB_WITH_OPENSSL"   , "..\libeay32.dll"          , .T., "" },;
         { "HB_WITH_OPENSSL"   , "..\ssleay32.dll"          , .T., "" },;
         { "HB_WITH_PGSQL"     , "..\lib\libpq.dll"         , .T., "" }}

      FOR EACH tmp IN aArray
         IF ! Empty( GetEnv( tmp[ 1 ] ) )
            hb_processRun( GetEnv( "HB_HOST_BIN_DIR" ) + _PS_ + "hbmk2" +;
                           " " + Chr( 34 ) + "-mkimplib=" + GetEnv( tmp[ 1 ] ) + _PS_ + StrTran( tmp[ 2 ], "\", _PS_ ) + Chr( 34 ) +;
                           " " + Chr( 34 ) + GetEnv( "HB_LIB_INSTALL" ) + _PS_ + tmp[ 4 ] + Chr( 34 ) +;
                           iif( tmp[ 3 ], " -mkimplibms", "" ) )
         ENDIF
      NEXT

      /* HACK: Automatic implib generation doesn't work in case of FireBird, so we manually create it. [vszakats] */
      IF GetEnv( "HB_COMPILER" ) $ "mingw|mingw64|cygwin"
         hb_FCopy( GetEnv( "HB_WITH_FIREBIRD" ) + _PS_ + StrTran( "..\lib\fbclient_ms.lib", "\", _PS_ ), GetEnv( "HB_LIB_INSTALL" ) + _PS_ + "libfbclient.a" )
      ENDIF

      /* Exception: We use static libs with mingw */
      IF GetEnv( "HB_COMPILER" ) == "mingw" .AND. ;
         ! Empty( GetEnv( "HB_WITH_OCILIB" ) )
         hb_FCopy( GetEnv( "HB_WITH_OCILIB" ) + _PS_ + StrTran( "..\lib32\libociliba.a", "\", _PS_ ), GetEnv( "HB_LIB_INSTALL" ) + _PS_ + "libociliba.a" )
         hb_FCopy( GetEnv( "HB_WITH_OCILIB" ) + _PS_ + StrTran( "..\lib32\libocilibm.a", "\", _PS_ ), GetEnv( "HB_LIB_INSTALL" ) + _PS_ + "libocilibm.a" )
         hb_FCopy( GetEnv( "HB_WITH_OCILIB" ) + _PS_ + StrTran( "..\lib32\libocilibw.a", "\", _PS_ ), GetEnv( "HB_LIB_INSTALL" ) + _PS_ + "libocilibw.a" )
      ENDIF
   ENDIF

   /* Creating shared version of Harbour binaries */

   IF !( GetEnv( "HB_PLATFORM" ) $ "dos|linux" ) .AND. ;
      !( GetEnv( "HB_BUILD_DLL" ) == "no" ) .AND. ;
      !( GetEnv( "HB_BUILD_SHARED" ) == "yes" )

      cOptions := ""
      IF GetEnv( "HB_BUILD_MODE" ) == "cpp"
         cOptions += " -cpp=yes"
      ELSEIF GetEnv( "HB_BUILD_MODE" ) == "c"
         cOptions += " -cpp=no"
      ENDIF
      IF GetEnv( "HB_BUILD_DEBUG" ) == "yes"
         cOptions += " -debug"
      ENDIF

      OutStd( "! Making shared version of Harbour binaries..." + hb_osNewLine() )

      FOR EACH tmp IN Directory( "utils\*", "D" )
         IF "D" $ tmp[ F_ATTR ] .AND. ;
            !( tmp[ F_NAME ] == "." ) .AND. ;
            !( tmp[ F_NAME ] == ".." ) .AND. ;
            hb_FileExists( "utils\" + tmp[ F_NAME ] + "\" + tmp[ F_NAME ] + ".hbp" )

            hb_processRun( GetEnv( "HB_HOST_BIN_DIR" ) + _PS_ + "hbmk2" +;
                           " -quiet -q0 -lang=en -shared" + cOptions +;
                           " " + Chr( 34 ) + "-o" + GetEnv( "HB_BIN_INSTALL" ) + _PS_ + tmp[ F_NAME ] + "-dll" + Chr( 34 ) +;
                           " " + Chr( 34 ) + StrTran( "utils\" + tmp[ F_NAME ] + "\" + tmp[ F_NAME ] + ".hbp", "\", _PS_ ) + Chr( 34 ) )
         ENDIF
      NEXT

   ENDIF

   /* Creating install packages */

   IF GetEnv( "HB_PLATFORM" ) $ "win|wce|os2|dos" .AND. ;
      GetEnv( "HB_BUILD_PKG" ) == "yes" .AND. ;
      ! Empty( GetEnv( "HB_TOP" ) )

      tmp := GetEnv( "HB_TOP" ) + _PS_ + GetEnv( "HB_PKGNAME" ) + ".zip"

      OutStd( "! Making Harbour .zip install package: '" + tmp + "'" + hb_osNewLine() )

      FErase( tmp )

      /* NOTE: Believe it or not this is the official method to zip a different dir with subdirs
               without including the whole root path in filenames; you have to 'cd' into it.
               Even with zip 3.0. For this reason we need absolute path in HB_TOP. There is also
               no zip 2.x compatible way to force creation of a new .zip, so we have to delete it
               first to avoid mixing in an existing .zip file. [vszakats] */

      cOldDir := _PS_ + CurDir()
      DirChange( GetEnv( "HB_INSTALL_PREFIX" ) + _PS_ + ".." )

      hb_processRun( GetEnv( "HB_DIR_ZIP" ) + "zip" +;
                     " -q -9 -X -r -o" +;
                     " " + FN_Escape( tmp ) +;
                     " . -i " + FN_Escape( GetEnv( "HB_PKGNAME" ) + _PS_ + "*" ) +;
                     " -x *.tds -x *.exp" )

      DirChange( cOldDir )

      IF GetEnv( "HB_PLATFORM" ) $ "win|wce"

         tmp := GetEnv( "HB_TOP" ) + _PS_ + GetEnv( "HB_PKGNAME" ) + ".exe"

         OutStd( "! Making Harbour .exe install package: '" + tmp + "'" + hb_osNewLine() )

         hb_processRun( GetEnv( "HB_DIR_NSIS" ) + "makensis.exe" +;
                        " -V2" +;
                        " " + FN_Escape( "package\mpkg_win.nsi" ) )
      ENDIF
   ENDIF

   ErrorLevel( nErrorLevel )

   RETURN

STATIC FUNCTION FN_Escape( cFN )
   RETURN Chr( 34 ) + cFN + Chr( 34 )
