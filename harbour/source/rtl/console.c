/*
 * $Id$
 */

#ifdef WINDOWS
   #include <windows.h>
#endif

#include <hbsetup.h>
#include <extend.h>
#include <ctoharb.h>
#include <dates.h>
#include <set.h>
#if defined(__GNUC__)
  #include <unistd.h>
  #if defined(__DJGPP__)
    #include <io.h>
  #endif
#else
   #include <io.h>
#endif
#ifdef USE_GTAPI
   #include <gtapi.h>
#endif

#define ACCEPT_BUFFER_LEN 256 /*length of input buffer for ACCEPT command */

#if defined(OS_UNIX_COMPATIBLE)
#define CRLF_BUFFER_LEN 2     /*length of buffer for CR/LF characters */
#else
#define CRLF_BUFFER_LEN 3     /*length of buffer for CR/LF characters */
#endif

HARBOUR HB___ACCEPT(void);
HARBOUR HB_COL( void );
HARBOUR HB_DEVOUT( void );
HARBOUR HB_DEVPOS( void );
HARBOUR HB___EJECT( void );
HARBOUR HB_MAXCOL( void );
HARBOUR HB_MAXROW( void );
HARBOUR HB_OUTSTD( void );
HARBOUR HB_OUTERR( void );
HARBOUR HB_PCOL( void );
HARBOUR HB_PROW( void );
HARBOUR HB_ROW( void );
HARBOUR HB_SCROLL( void );
HARBOUR HB_SETPOS( void );
HARBOUR HB_SETPRC( void );
HARBOUR HB_QOUT( void );
HARBOUR HB_QQOUT( void );
HARBOUR HB_DISPBOX( void );
HARBOUR HB_DISPBEGIN( void );
HARBOUR HB_DISPEND( void );
HARBOUR HB_DISPCOUNT( void );
HARBOUR HB_ISCOLOR( void );
HARBOUR HB_NOSNOW( void );

static SYMBOL symbols[] = {
{ "__ACCEPT" , FS_PUBLIC, HB___ACCEPT , 0 },
{ "__EJECT"  , FS_PUBLIC, HB___EJECT  , 0 },
{ "COL"      , FS_PUBLIC, HB_COL      , 0 },
{ "DEVOUT"   , FS_PUBLIC, HB_DEVOUT   , 0 },
{ "DEVPOS"   , FS_PUBLIC, HB_DEVPOS   , 0 },
{ "MAXCOL"   , FS_PUBLIC, HB_MAXCOL   , 0 },
{ "MAXROW"   , FS_PUBLIC, HB_MAXROW   , 0 },
{ "OUTERR"   , FS_PUBLIC, HB_OUTERR   , 0 },
{ "OUTSTD"   , FS_PUBLIC, HB_OUTSTD   , 0 },
{ "PCOL"     , FS_PUBLIC, HB_PCOL     , 0 },
{ "PROW"     , FS_PUBLIC, HB_PROW     , 0 },
{ "ROW"      , FS_PUBLIC, HB_ROW      , 0 },
{ "SCROLL"   , FS_PUBLIC, HB_SCROLL   , 0 },
{ "SETPOS"   , FS_PUBLIC, HB_SETPOS   , 0 },
{ "SETPRC"   , FS_PUBLIC, HB_SETPRC   , 0 },
{ "QOUT"     , FS_PUBLIC, HB_QOUT     , 0 },
{ "QQOUT"    , FS_PUBLIC, HB_QQOUT    , 0 },
{ "DISPBOX"  , FS_PUBLIC, HB_DISPBOX  , 0 },
{ "DISPBEGIN", FS_PUBLIC, HB_DISPBEGIN, 0 },
{ "DISPEND"  , FS_PUBLIC, HB_DISPEND  , 0 },
{ "DISPCOUNT", FS_PUBLIC, HB_DISPCOUNT, 0 },
{ "ISCOLOR"  , FS_PUBLIC, HB_ISCOLOR  , 0 },
{ "NOSNOW"   , FS_PUBLIC, HB_NOSNOW   , 0 }
};

void Console__InitSymbols( void )
{
   ProcessSymbols( symbols, sizeof(symbols)/sizeof( SYMBOL ) );
}

static unsigned short dev_row, dev_col, p_row, p_col;
static char CrLf [ CRLF_BUFFER_LEN ];

void InitializeConsole( void )
{
#if defined(OS_DOS_COMPATIBLE)
   CrLf [0] = 13;
   CrLf [1] = 10;
   CrLf [2] = 0;
#else
   CrLf [0] = 10;
   CrLf [1] = 0;
#endif

#ifdef USE_GTAPI
   dev_row = gtWhereY();
   dev_col = gtWhereX();
   hb_gtSetPos( dev_row, dev_col );
#else
   dev_row = 0;
   dev_col = 0;
#endif
   p_row = p_col = 0;
}

USHORT hb_max_row( void )
{
#ifdef USE_GTAPI
   return hb_gtMaxRow ();
#else
   return 23; /* QUESTION: Shouldn't this be 24 ? info@szelvesz.hu */
#endif
}

USHORT hb_max_col( void )
{
#ifdef USE_GTAPI
   return hb_gtMaxCol ();
#else
   return 79;
#endif
}

#ifndef USE_GTAPI
static void adjust_pos( char * fpStr, WORD uiLen, USHORT * row, USHORT * col, USHORT max_row, USHORT max_col )
{
   WORD uiCount;

   for( uiCount = 0; uiCount < uiLen; uiCount++ )
   {
      switch( fpStr[ uiCount ]  )
      {
         case 7:
            break;
         case 8:
            if( *col ) (*col)--;
            else
            {
               *col = max_col;
               if( *row ) (*row)--;
            }
            break;
         case 10:
            if( *row < max_row ) (*row)++;
            break;
         case 13:
            *col = 0;
            break;
         default:
            if( *col < max_col ) (*col)++;
            else
            {
               *col = 0;
               if( *row < max_row ) (*row)++;
            }
      }
   }
}
#endif

HARBOUR HB___ACCEPT( void ) /* Internal Clipper function used in ACCEPT command  */
                         /* Basically the simplest Clipper function to        */
                         /* receive data. Parameter : cPrompt. Returns : cRet */
{
   char *szResult = ( char * ) hb_xgrab(ACCEPT_BUFFER_LEN); /* Return parameter. */
   char *szPrompt = hb_parc(1);    /* Pass prompt                            */
   long lLen      = hb_parclen(1); /* Please change to long later on         */

   if( hb_pcount() == 1 )          /* cPrompt passed                         */
   {
      PushSymbol( GetDynSym( "QOUT" )->pSymbol );  /* push the symbol pointer to the Harbour stack */
      PushNil();                 /* places nil at self, as we are not sending a msg */
      PushString( szPrompt, lLen ); /* places parameters on to the stack */
      Do( 1 );                   /* 1 parameter supplied. Invoke the virtual machine */
   }

#ifdef OS_UNIX_COMPATIBLE
   fgets( szResult, ACCEPT_BUFFER_LEN, stdin );             /* Read the data. Using fgets()            */
#else
/*TODO: check if it can be replaced with fgets() function
*/
   gets( szResult ); /* Read the data. using gets(). Note; it doesn't check for buffer overflow */
#endif
   hb_retc( szResult );
   hb_xfree( szResult );
}

typedef void void_func_int (char *, WORD);

/* Format items for output, then call specified output function */
static void hb_out( WORD wParam, void_func_int * hb_out_func )
{
   char * szText;
   PHB_ITEM pItem = hb_param( wParam, IT_ANY );
   char szBuffer [11];

   switch( hb_parinfo( wParam ) )
   {
      case IT_DATE:
           szText = hb_dtoc( hb_pards( wParam ), szBuffer );
           if( szText )
                 hb_out_func( szText, strlen( szText ) );
           break;

      case IT_DOUBLE:
      case IT_INTEGER:
      case IT_LONG:
           szText = hb_str( pItem, 0, 0 ); /* Let hb_str() do the hard work */
           if( szText )
           {
              hb_out_func( szText, strlen( szText ) );
              hb_xfree( szText );
           }
           break;

      case IT_NIL:
           hb_out_func( "NIL", 3 );
           break;

      case IT_LOGICAL:
           if( hb_parl( wParam ) )
              hb_out_func( ".T.", 3 );
           else
              hb_out_func( ".F.", 3 );
           break;

      case IT_STRING:
           hb_out_func( hb_parc( wParam ), hb_parclen( wParam ) );
           break;

      default:
           break;
   }
}

/* Output an item to STDOUT */
static void hb_outstd( char * fpStr, WORD uiLen )
{
   WORD uiCount = uiLen;
   char * fpPtr = fpStr;

   while( uiCount-- )
      printf( "%c", *fpPtr++ );
   fflush( stdout );
#ifdef USE_GTAPI
   if( isatty( fileno( stdout ) ) )
   {
      dev_row = gtWhereY();
      dev_col = gtWhereX();
      hb_gtSetPos( dev_row, dev_col );
   }
#else
   adjust_pos( fpStr, uiLen, &dev_row, &dev_col, hb_max_row(), hb_max_col() );
#endif
}

/* Output an item to STDERR */
static void hb_outerr( char * fpStr, WORD uiLen )
{
   WORD uiCount = uiLen;
   char * fpPtr = fpStr;
   while( uiCount-- )
      fprintf( stderr, "%c", *fpPtr++ );
   fflush( stderr );
#ifdef USE_GTAPI
   if( isatty( fileno( stdout ) ) )
   {
      dev_row = gtWhereY();
      dev_col = gtWhereX();
      hb_gtSetPos( dev_row, dev_col );
   }
#else
   adjust_pos( fpStr, uiLen, &dev_row, &dev_col, hb_max_row(), hb_max_col() );
#endif
}

/* Output an item to the screen and/or printer and/or alternate */
static void hb_altout( char * fpStr, WORD uiLen )
{
   if( hb_set.HB_SET_CONSOLE )
   {
   #ifdef USE_GTAPI
      hb_gtWriteCon( fpStr, uiLen );
      hb_gtGetPos( &dev_row, &dev_col );
   #else
      WORD uiCount;
     for( uiCount = 0; uiCount < uiLen; uiCount++ )
         printf( "%c", fpStr[ uiCount ] );
      adjust_pos( fpStr, uiLen, &dev_row, &dev_col, hb_max_row(), hb_max_col() );
   #endif
   }
   if( hb_set.HB_SET_ALTERNATE && hb_set_althan >= 0 )
   {
      /* Print to alternate file if SET ALTERNATE ON and valid alternate file */
      write( hb_set_althan, fpStr, uiLen );
   }
   if( hb_set.HB_SET_PRINTER && hb_set_printhan >= 0 )
   {
      /* Print to printer if SET PRINTER ON and valid printer file */
      write( hb_set_printhan, fpStr, uiLen );
      p_col += uiLen;
   }
}

/* Output an item to the screen and/or printer */
static void hb_devout( char * fpStr, WORD uiLen )
{
   if( hb_stricmp( hb_set.HB_SET_DEVICE, "PRINTER" ) == 0 && hb_set_printhan >= 0 )
   {
      /* Display to printer if SET DEVICE TO PRINTER and valid printer file */
      write( hb_set_printhan, fpStr, uiLen );
      p_col += uiLen;
   }
   else
   {
   #ifdef USE_GTAPI
      /* Otherwise, display to console */
      hb_gtWrite( fpStr, uiLen );
      hb_gtGetPos( &dev_row, &dev_col );
   #else
      WORD uiCount;
      for( uiCount = 0; uiCount < uiLen; uiCount++ )
         printf( "%c", fpStr[ uiCount ] );
      adjust_pos( fpStr, uiLen, &dev_row, &dev_col, hb_max_row(), hb_max_col() );
   #endif
   }
}

void hb_setpos( USHORT row, USHORT col )
{
   USHORT count;
   #ifdef USE_GTAPI
      hb_gtSetPos( row, col );
   #else
      for( count = dev_row; count < row; count++ ) printf("\n");
      if( row > dev_row ) dev_col = 0;
      for( count = dev_col; count < col; count++ ) printf(" ");
   #endif
      dev_row = row;
      dev_col = col;
}

void hb_devpos( USHORT row, USHORT col )
{
   USHORT count;
   /* Position printer if SET DEVICE TO PRINTER and valid printer file
      otherwise position console */
   if( hb_stricmp( hb_set.HB_SET_DEVICE, "PRINTER" ) == 0 && hb_set_printhan >= 0 )
   {
      if( row < p_row )
      {
         write( hb_set_printhan, "\x0C", 1 );
         p_row = p_col = 0;
      }
      for( count = p_row; count < row; count++ ) write( hb_set_printhan, CrLf, CRLF_BUFFER_LEN-1 );
      if( row > p_row ) p_col = 0;
      col += hb_set.HB_SET_MARGIN;
      for( count = p_col; count < col; count++ ) write( hb_set_printhan, " ", 1 );
      p_row = row;
      p_col = col;
   }
   else hb_setpos( row, col );
}

HARBOUR HB_OUTSTD( void ) /* writes a list of values to the standard output device */
{
   WORD w;

   for( w = 0; w < hb_pcount(); w++ )
   {
      hb_out( w + 1, hb_outstd );
      if( w < hb_pcount() - 1) hb_outstd( " ", 1 );
   }
}

HARBOUR HB_OUTERR( void ) /* writes a list of values to the standard error device */
{
   WORD w;

   for( w = 0; w < hb_pcount(); w++ )
   {
      hb_out( w + 1, hb_outerr );
      if( w < hb_pcount() - 1) hb_outerr( " ", 1 );
   }
}

HARBOUR HB_QQOUT( void ) /* writes a list of values to the current device (screen or printer) and is affected by SET ALTERNATE */
{
   WORD w;

   for( w = 0; w < hb_pcount(); w++ )
   {
      hb_out( w + 1, hb_altout );
      if( w < hb_pcount() - 1) hb_altout( " ", 1 );
   }
}

HARBOUR HB_QOUT( void )
{
   #ifdef WINDOWS
      MessageBox( 0, hb_parc( 1 ), "Harbour", 0 );
   #else
      int count;
      hb_altout( CrLf, CRLF_BUFFER_LEN-1 );
      if( hb_set.HB_SET_PRINTER && hb_set_printhan >= 0 )
      {
         p_row++;
         p_col = hb_set.HB_SET_MARGIN;
         count = p_col;
         while( count-- > 0 )
            write( hb_set_printhan, " ", 1 );
      }
      HB_QQOUT();
   #endif
}

HARBOUR HB_SETPOS( void ) /* Sets the screen position */
{
   PHB_ITEM pRow, pCol;
   if( hb_pcount() > 1 )
   {
      pRow = hb_param( 1, IT_NUMERIC );
      pCol = hb_param( 2, IT_NUMERIC );
      if( pRow && pCol )
         hb_setpos( hb_parni( 1 ), hb_parni( 2 ) );
   }
}

HARBOUR HB_DEVPOS( void ) /* Sets the screen and/or printer position */
{
   PHB_ITEM pRow, pCol;
   if( hb_pcount() > 1 )
   {
      pRow = hb_param( 1, IT_NUMERIC );
      pCol = hb_param( 2, IT_NUMERIC );
      if( pRow && pCol )
         hb_devpos( hb_parni( 1 ), hb_parni( 2 ) );
   }
}

HARBOUR HB_DEVOUT( void ) /* writes a single values to the current device (screen or printer), but is not affected by SET ALTERNATE */
{
   if( hb_pcount() > 0 )
   {
#ifdef USE_GTAPI
      char fpOldColor[ CLR_STRLEN ];

      if( ISCHAR(2) )
      {
         hb_gtGetColorStr( fpOldColor );
         hb_gtSetColorStr( _parc(2) );
      }
#endif

      hb_out( 1, hb_devout );

#ifdef USE_GTAPI
      if( ISCHAR(2) )
      {
         hb_gtSetColorStr( fpOldColor );
      }
#endif
   }
}

HARBOUR HB___EJECT( void ) /* Ejects the current page from the printer */
{
   if( hb_stricmp( hb_set.HB_SET_DEVICE, "PRINTER" ) == 0 && hb_set_printhan >= 0 )
   {
      write( hb_set_printhan, "\x0C\x0D", 2 );
      p_row = p_col = 0;
   }
}

HARBOUR HB_PROW( void ) /* Returns the current printer row position */
{
   hb_retni( p_row );
}

HARBOUR HB_PCOL( void ) /* Returns the current printer row position */
{
   hb_retni( p_col );
}

HARBOUR HB_SETPRC( void ) /* Sets the current printer row and column positions */
{
   if( hb_pcount() > 1 )
   {
      PHB_ITEM pRow = hb_param( 1, IT_NUMERIC );
      PHB_ITEM pCol = hb_param( 1, IT_NUMERIC );
      if( pRow && pCol )
      {
         p_row = hb_parni( 1 );
         p_col = hb_parni( 2 );
      }
   }
}

HARBOUR HB_SCROLL( void ) /* Scrolls a screen region (requires the GT API) */
{
   int top = 0, left = 0, bottom = hb_max_row(), right = hb_max_col(),
       v_scroll = 0, h_scroll = 0;

   if( hb_pcount() > 0 && hb_param( 1, IT_NUMERIC ) )
      top = hb_parni( 1 );
   if( hb_pcount() > 1 && hb_param( 2, IT_NUMERIC ) )
      left = hb_parni( 2 );
   if( hb_pcount() > 2 && hb_param( 3, IT_NUMERIC ) )
      bottom = hb_parni( 3 );
   if( hb_pcount() > 3 && hb_param( 4, IT_NUMERIC ) )
      right = hb_parni( 4 );
   if( hb_pcount() > 4 && hb_param( 5, IT_NUMERIC ) )
      v_scroll = hb_parni( 5 );
   if( hb_pcount() > 5 && hb_param( 6, IT_NUMERIC ) )
      h_scroll = hb_parni( 6 );

#ifdef USE_GTAPI
   hb_gtScroll( top, left, bottom, right, v_scroll, h_scroll );
#else
   if( top == 0 && bottom == hb_max_row()
   && left == 0 && right == hb_max_col()
   && v_scroll == 0 && h_scroll == 0 )
   {
      int count;
      dev_row = hb_max_row();
      for( count = 0; count < dev_row ; count++ ) printf( "\n" );
      dev_row = dev_col = 0;
   }
#endif
}

HARBOUR HB_MAXROW( void ) /* Return the maximum screen row number (zero origin) */
{
   hb_retni( hb_max_row () );
}

HARBOUR HB_MAXCOL( void ) /* Return the maximum screen column number (zero origin) */
{
   hb_retni( hb_max_col () );
}

HARBOUR HB_ROW( void ) /* Return the current screen row position (zero origin) */
{
   hb_retni( dev_row );
}

HARBOUR HB_COL( void ) /* Return the current screen column position (zero origin) */
{
   hb_retni( dev_col );
}

HARBOUR HB_DISPBOX (void)
{
#ifdef USE_GTAPI
   if (ISNUM(1) && ISNUM(2) && ISNUM(3) && ISNUM(4))
   {
      char szOldColor [CLR_STRLEN];

      if (ISCHAR(6))
      {
         _gtGetColorStr(szOldColor);
         _gtSetColorStr(_parc(6));
      }

      if (ISCHAR(1))
      {
         _gtBox(_parni(1), _parni(2), _parni(3), _parni(4), _parc(5));
      }
      else if (ISNUM(5) && _parni(5) == 2)
      {
         _gtBoxD(_parni(1), _parni(2), _parni(3), _parni(4));
      }
      else
      {
         _gtBoxS(_parni(1), _parni(2), _parni(3), _parni(4));
      }

      if (ISCHAR(6))
      {
         _gtSetColorStr(szOldColor);
      }
   }
#endif
}

HARBOUR HB_DISPBEGIN (void)
{
#ifdef USE_GTAPI
   _gtDispBegin();
#endif
}

HARBOUR HB_DISPEND (void)
{
#ifdef USE_GTAPI
   _gtDispBegin();
#endif
}

HARBOUR HB_DISPCOUNT (void)
{
#ifdef USE_GTAPI
   _retni(_gtDispCount());
#else
   _retni(0);
#endif
}

HARBOUR HB_ISCOLOR (void)
{
#ifdef USE_GTAPI
   _retl(_gtIsColor());
#else
   _retl(FALSE);
#endif
}

HARBOUR HB_NOSNOW (void)
{
#ifdef USE_GTAPI
   if (ISLOG(1))
   {
      _gtSetSnowFlag(_parl(1));
   }
#endif
}

