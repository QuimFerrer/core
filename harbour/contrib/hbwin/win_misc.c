/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Misc Windows API functions
 *
 * Copyright 2008-2009 Viktor Szakats (harbour.01 syenar.hu)
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

/*
 * The following parts are Copyright of the individual authors.
 * www - http://www.harbour-project.org
 *
 * Copyright 2009 Francesco Saverio Giudice <info / at / fsgiudice.com>
 *    WIN_SYSREFRESH()
 *
 * See COPYING for licensing terms.
 *
 */

#define HB_OS_WIN_USED

#include "hbapi.h"
#include "hbwinuni.h"

#ifndef QS_ALLPOSTMESSAGE
#define QS_ALLPOSTMESSAGE   0x0100
#endif

HB_FUNC( WIN_RUNDETACHED )
{
   void * hCommandName;
   void * hCommandLine;

#if ! defined( HB_OS_WIN_CE )
   STARTUPINFO si;
   PROCESS_INFORMATION pi;

   ZeroMemory( &si, sizeof( si ) );
   si.cb = sizeof( si );
   ZeroMemory( &pi, sizeof( pi ) );
#endif

   if( CreateProcess(
         ( LPCTSTR ) HB_PARSTR( 1, &hCommandName, NULL ),      /* Command name */
         ( LPTSTR ) HB_PARSTR( 2, &hCommandLine, NULL ),       /* Command line (Unicode version needs an non-const buffer) */
         NULL,                                                 /* Process handle not inheritable */
         NULL,                                                 /* Thread handle not inheritable */
         FALSE,                                                /* Set handle inheritance to FALSE */
#if ! defined( HB_OS_WIN_CE )
         hb_parl( 4 ) ? CREATE_NO_WINDOW : CREATE_NEW_CONSOLE, /* Creation flags */
#else
         CREATE_NEW_CONSOLE,                                   /* Creation flags */
#endif
         NULL,                                                 /* Use parent's environment block */
         NULL,                                                 /* Use parent's starting directory */
#if ! defined( HB_OS_WIN_CE )
         &si,                                                  /* Pointer to STARTUPINFO structure */
         &pi )                                                 /* Pointer to PROCESS_INFORMATION structure */
#else
         NULL,                                                 /* Pointer to STARTUPINFO structure */
         NULL )                                                /* Pointer to PROCESS_INFORMATION structure */
#endif
      )
   {
      hb_retl( HB_TRUE );

#if ! defined( HB_OS_WIN_CE )
      hb_stornl( pi.dwProcessId, 3 );

      /* Close process and thread handles. */
      CloseHandle( pi.hProcess );
      CloseHandle( pi.hThread );
#endif
   }
   else
   {
      hb_stornl( -1, 3 );
      hb_retl( HB_FALSE );
   }

   hb_strfree( hCommandName );
   hb_strfree( hCommandLine );
}

HB_FUNC( WIN_LOADRESOURCE )
{
   HANDLE hInstance = NULL;

   /* Set default return value */
   hb_retc_null();

   if( hb_winmainArgGet( &hInstance, NULL, NULL ) )
   {
      void * hName;
      void * hType;

      HRSRC hRes = FindResource( ( HMODULE ) hInstance, 
                                 ( LPCTSTR ) HB_PARSTRDEF( 1, &hName, NULL ),
                                 ( LPCTSTR ) HB_PARSTRDEF( 2, &hType, NULL ) );

      if( hRes )
      {
         HGLOBAL hMem = LoadResource( NULL, hRes );

         if( hMem )
         {
            void * pMem = LockResource( hMem );

            if( pMem )
               hb_retclen( ( char * ) pMem, SizeofResource( NULL, hRes ) );
         }
      }

      hb_strfree( hName );
      hb_strfree( hType );
   }
}

HB_FUNC( WIN_GETCOMMANDLINEPARAM )
{
   char * buffer = HB_TCHAR_CONVFROM( GetCommandLine() );
   long pos;

   /* Skip application path */
   pos = 0;
   if( buffer[ pos ] == '"' )
   {
      /* If it contains spaces, it will be enclosed in quote chars,
         skip this to get to the command line. */
      pos++;
      while( buffer[ pos ] && buffer[ pos ] != '"' )
         pos++;

      if( buffer[ pos ] == '"' )
         pos++;
   }
   else
   {
      /* If not, look for the next space for the beginning of the
         command line. */
      while( buffer[ pos ] && buffer[ pos ] != ' ' )
         pos++;
   }

   if( buffer[ pos ] == ' ' )
      pos++;

   {
      /* Convert from OS codepage */
      char * pszFree = NULL;
      const char * pszResult = hb_osDecodeCP( buffer + pos, &pszFree, NULL );

      if( pszFree )
         hb_retc_buffer( pszFree );
      else
         hb_retc( pszResult );
   }

   HB_TCHAR_FREE( buffer );
}

HB_FUNC( WIN_ANSITOWIDE )
{
   HB_SIZE nLen = hb_parclen( 1 );
   BSTR wString = hb_mbntowc( hb_parcx( 1 ), nLen );

   hb_retclen_buffer( ( char * ) wString, nLen * sizeof( wchar_t ) );
}

HB_FUNC( WIN_WIDETOANSI )
{
   HB_SIZE nLen = hb_parclen( 1 );
   char * cString = hb_wcntomb( ( wchar_t * ) hb_parcx( 1 ), nLen );

   hb_retclen_buffer( cString, nLen / sizeof( wchar_t ) );
}

HB_FUNC( WIN_N2P )
{
   hb_retptr( ( void * ) ( HB_PTRDIFF ) hb_parnint( 1 ) );
}

HB_FUNC( WIN_P2N )
{
   hb_retnint( ( HB_PTRDIFF ) hb_parptr( 1 ) );
}

HB_FUNC( WIN_HINSTANCE )
{
   HANDLE hInstance;

   hb_winmainArgGet( &hInstance, NULL, NULL );

   hb_retptr( hInstance );
}

HB_FUNC( WIN_HPREVINSTANCE )
{
   HANDLE hPrevInstance;

   hb_winmainArgGet( NULL, &hPrevInstance, NULL );

   hb_retptr( hPrevInstance );
}

HB_FUNC( WIN_NCMDSHOW )
{
   int nCmdShow;

   hb_winmainArgGet( NULL, NULL, &nCmdShow );

   hb_retni( nCmdShow );
}

HB_FUNC( WIN_LOWORD )
{
   hb_retni( ( int ) LOWORD( ( DWORD ) hb_parnl( 1 ) ) );
}

HB_FUNC( WIN_HIWORD )
{
   hb_retni( ( int ) HIWORD( ( DWORD ) hb_parnl( 1 ) ) );
}

HB_FUNC( WIN_SYSREFRESH )
{
   DWORD dwMsec = ( DWORD ) hb_parnl( 1 );

   HANDLE hDummyEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

   /* Begin the operation and continue until it is complete
      or until the user clicks the mouse or presses a key. */

   if( MsgWaitForMultipleObjects( 1, &hDummyEvent, FALSE, ( dwMsec == 0 ? INFINITE : dwMsec ), QS_ALLINPUT | QS_ALLPOSTMESSAGE ) == WAIT_OBJECT_0 + 1 )
   {
      MSG msg;

      while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
      {
         switch( msg.message )
         {
            case WM_CLOSE:
            {
               CloseHandle( hDummyEvent );
               hb_retni( 1 );
               return;
            }
            case WM_QUIT:
            {
               CloseHandle( hDummyEvent );
               hb_retnint( msg.wParam );
               return;
            }
#if 0
            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_KEYDOWN:
            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            case WM_KEYUP:
               /* Perform any required cleanup. */
               break;
               /* exit; */

#endif
            default:
               TranslateMessage( &msg );
               DispatchMessage( &msg );
         }
      }
   }

   CloseHandle( hDummyEvent );
   hb_retni( 0 );
}
