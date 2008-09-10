/*
 * $Id$
 */

/*
 * File......: getenvrn.c
 * Author....: Rick Whitt
 * CIS ID....: 70672,605
 *
 * This is an original work by Rick Whitt and is placed in the
 * public domain.
 *
 * Modification history:
 * ---------------------
 *    Rev 1.2a  09 Sep 1996            JO
 * Added underscore prefix to environ() calls for MSC 8.0
 * Note: 5.2e version will work if linked with MSC OldNames.lib
 *
 *    Rev 1.2   01 Jan 1996 03:01:00   TED
 * Added prototypes to kill compiler warning.
 *
 *    Rev 1.1   15 Aug 1991 23:08:42   GLENN
 * Forest Belt proofread/edited/cleaned up doc
 *
 *    Rev 1.0   17 Jul 1991 22:08:12   GLENN
 * Initial revision.
 *
 */


/*  $DOC$
 *  $FUNCNAME$
 *     FT_GETE()
 *  $CATEGORY$
 *     Environment
 *  $ONELINER$
 *     Return the entire current environment
 *  $SYNTAX$
 *     FT_GETE( [ @<xReceiveVar> ] ) -> nNumStrings
 *  $ARGUMENTS$
 *     <xReceiveVar> is the variable to receive the environment data.
 *
 *     <xReceiveVar> can be a character type variable, in which case
 *     the function will place all environment strings in the variable
 *     separated by carriage return/line feeds (chr 13 + chr(10)).
 *
 *     <xReceiveVar> can be an array type, in which case the function
 *     will place each string in an array element.  The array MUST be
 *     declared with the proper number of elements prior to passing it
 *     to the function.  This can be done by calling FT_GETE() without
 *     parameters first to get the number of strings in the environment.
 *
 *     Note that the argument MUST be passed by reference. Since arrays
 *     are by nature passed by reference, the "@" symbol is optional when
 *     passing an array.
 *
 *     If no argument is passed, FT_GETE() merely returns the number
 *     of strings in the environment.
 *  $RETURNS$
 *     FT_GETE() returns the total number of strings found in the
 *     current program's environment.
 *  $DESCRIPTION$
 *     This function stores ALL of the current program's environment
 *     variables in either a block of text lines or in an array.  It is
 *     useful for looking at the entire environment at once, or recording
 *     a snapshot of it to a file for later inspection, such as when a
 *     program error occurs.  If the value of ONE SPECIFIC variable is
 *     desired, use Clipper's built-in GETE() function.
 *
 *     This function uses the undocumented internal variable "_environ",
 *     as well as the functions _strcpy(), _strcat(), and _strlen() from
 *     CLIPPER.LIB
 *  $EXAMPLES$
 *     Get the environment in text form and browse it:
 *
 *        cEnvBlock   := ""
 *        nNumStrings := FT_GETE(@cEnvBlock)
 *        @  0, 0 to MAXROW() - 1, MAXCOL()
 *        @  MAXROW(), 0 say 'Browse strings, press ESC to exit...'
 *        MEMOWRIT(cEnvBlock, 1, 1, MAXROW() - 2,MAXCOL() - 1, .F.)
 *
 *     Get the environment in text form and write it to a file:
 *
 *        cEnvBlock := ""
 *        FT_GETE(@cEnvBlock)
 *        MEMOWRIT("environ.txt", cEnvBlock)
 *
 *     Get the environment in Array form:
 *
 *        aEnvArray := ARRAY(FT_GETE())
 *        FT_GETE(aEnvArray)
 *        ? aEnvArray[1]       // "COMSPEC=C:\command.com"
 *        ? aEnvArray[2]       // "PATH=C:\;C:\windows;C:\util;C:\harbour"
 *          ... etc ...
 *  $END$
 */

#define HB_OS_WIN_32_USED
#include "hbapi.h"

#if defined(HB_OS_UNIX_COMPATIBLE)
#   include <unistd.h>
#   if defined( HB_OS_DARWIN )
#      include <crt_externs.h>
#      define environ (*_NSGetEnviron())
#   elif !defined( __WATCOMC__ )
extern char **environ;
#   endif
#elif defined(HB_OS_DOS)
#   define environ _environ
extern char **_environ;
#elif defined(HB_OS_WIN_32)
#   include <windows.h>
#endif

#define NORETURN   0
#define CHARTYPE   1
#define ARRAYTYPE  2
#define CRLF       "\x0D\x0A"
/*
unsigned int strlen( char * );
char * strcpy( char *, char * );
char * strcat( char *, char * );
*/
HB_FUNC( FT_GETE )
{
   /* INTERNALS WARNING: All references to 'environ', strlen(), ;
      strcpy(), and strcat() are undocumented Clipper 5.0 internals.
    */
#if defined(HB_OS_DOS) || defined(HB_OS_UNIX_COMPATIBLE)
   {

      char *buffer = NULL;
      int x;
      int buffsize = 0;
      int rettype = NORETURN;

      if( ISCHAR( 1 ) )
         rettype = CHARTYPE;
      if( ISARRAY( 1 ) )
         rettype = ARRAYTYPE;

      /* scan strings first and add up total size */
      if( rettype == CHARTYPE )
      {
         for( x = 0; environ[x]; x++ )
         {
            /* add length of this string plus 2 for the crlf */
            buffsize += ( strlen( environ[x] ) + 2 );
         }
         /* add 1 more byte for final nul character */
         buffsize++;
         /* now allocate that much memory and make sure 1st byte is a nul */
         buffer = ( char * ) hb_xgrab( buffsize + 1 );
         buffer[0] = '\0';
      }

      for( x = 0; environ[x]; x++ )
      {
         if( !environ[x] )
            /* null string, we're done */
            break;

         if( rettype == CHARTYPE )
         {
            /* tack string onto end of buffer */
            hb_strncat( buffer, environ[x], buffsize );
            /* add crlf at end of each string */
            hb_strncat( buffer, CRLF, buffsize );
         }
         else if( rettype == ARRAYTYPE )
            /* store string to next array element */
            hb_storc( environ[x], 1, x + 1 );
      }

      if( rettype == CHARTYPE )
      {
         /* return buffer to app and free memory */
         hb_storc( buffer, 1 );
         hb_xfree( buffer );
      }

      /* return number of strings found */
      hb_retni( x );
   }
#elif defined(HB_OS_WIN_32)
   {
      char *buffer = NULL;
      LPVOID lpEnviron = GetEnvironmentStrings();
      char *sCurEnv;
      int x;
      int buffsize = 0;
      int rettype = NORETURN;

      char * szEnviron = HB_TCHAR_CONVFROM( lpEnviron );

      if( ISCHAR( 1 ) )
         rettype = CHARTYPE;
      if( ISARRAY( 1 ) )
         rettype = ARRAYTYPE;

      if( rettype == CHARTYPE )
         /* scan strings first and add up total size */
      {
         for( sCurEnv = szEnviron; *sCurEnv; sCurEnv++ )
         {
            if( !*sCurEnv )
               /* null string, we're done */
               break;

            /* add length of this string plus 2 for the crlf */
            buffsize += ( strlen( ( char * ) sCurEnv ) + 2 );

            while( *sCurEnv )
               sCurEnv++;
         }
         /* add 1 more byte for final nul character */
         buffsize++;

         /* now allocate that much memory and make sure 1st byte is a nul */
         buffer = ( char * ) hb_xgrab( buffsize + 1 );
         buffer[0] = '\0';
      }
      x = 0;
      for( sCurEnv = szEnviron; *sCurEnv; sCurEnv++ )
      {
         if( !*sCurEnv )
            /* null string, we're done */
            break;

         if( rettype == CHARTYPE )
         {
            /* tack string onto end of buffer */
            hb_strncat( buffer, ( char * ) sCurEnv, buffsize );
            /* add crlf at end of each string */
            hb_strncat( buffer, CRLF, buffsize );
         }

         if( rettype == ARRAYTYPE )
            /* store string to next array element */
            hb_storc( ( char * ) sCurEnv, 1, x + 1 );
         x++;
         while( *sCurEnv )
            sCurEnv++;
      }

      if( rettype == CHARTYPE )
      {
         /* return buffer to app and free memory */
         hb_storc( buffer, 1 );
         hb_xfree( buffer );
      }

      /* return number of strings found */
      hb_retni( x );

      HB_TCHAR_FREE( szEnviron );
      FreeEnvironmentStrings( ( LPTSTR ) lpEnviron );
   }

#endif
}
