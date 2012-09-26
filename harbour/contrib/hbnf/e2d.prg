/*
 * $Id$
 */

/*
 * File......: e2d.prg
 * Author....: Gary Baren
 * CIS ID....: 75470,1027
 *
 * This is an original work by Gary Baren and is hereby placed in the
 * public domain.
 *
 * Modification history:
 * ---------------------
 *
 *    Rev 1.2   15 Aug 1991 23:03:28   GLENN
 * Forest Belt proofread/edited/cleaned up doc
 *
 *    Rev 1.1   14 Jun 1991 19:51:40   GLENN
 * Minor edit to file header
 *
 *    Rev 1.0   07 Jun 1991 23:03:32   GLENN
 * Initial revision.
 *
 */

#ifdef FT_TEST

PROCEDURE Main( sNumE )

   QOut( FT_E2D( sNumE ) )

   RETURN

#endif

FUNCTION ft_e2d( sNumE )

   LOCAL nMant, nExp

   nMant := Val( Left( sNumE, At( 'E', sNumE ) - 1 ) )
   nExp  := Val( SubStr( sNumE,                    ;
      At( 'E', sNumE ) + 1,           ;
      Len( sNumE ) - At( 'E', sNumE ) ;
      )                           ;
      )

   RETURN nMant * 10 ^ nExp
