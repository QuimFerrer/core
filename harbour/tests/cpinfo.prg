/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *    Simple program to generate information for Harbour CP module definition.
 *    Compile it with Clipper and link with given national sorting module
 *    (usually ntx*.obj) and then execute to generate letters strings for
 *    given national sorting module. Then use this string to define Harbour
 *    CP module in source/codepage/ directory.
 *
 * Copyright 2009 Przemyslaw Czerpak <druzus / at / priv.onet.pl>
 * www - http://www.harbour-project.org
 *
 */


proc main()
   local cUp, cLo, cOrd, cOrd2, c, i, a, lWarn

   set alternate to cpinfo.txt additive
   set alternate on

#ifdef __HARBOUR__
   /* for test */
   REQUEST HB_CODEPAGE_PLMAZ
   set( _SET_CODEPAGE, "PLMAZ" )
#endif

   a := array( 256 )
   for i := 1 to len( a )
      a[ i ] := i - 1
   next
   asort( a,,, { |x,y| chr( x ) + chr( 0 ) < chr( y ) + chr( 0 ) } )

   ? date(), time(), os(), version()
#ifdef __HARBOUR__
   ? "Character encoding: " + Set( _SET_CODEPAGE )
#else
   ? "Character encoding: " + _natSortVersion()
#endif
   ? repl( "=", 50 )
   lWarn := .t.
   for i := 1 to len( a ) - 1
      if a[ i ] > a[ i + 1 ]
         lWarn := .f.
         exit
      endif
   next
   if lWarn
      ? "simple byte sorting !!!"
      lWarn := .f.
   endif
   cUp := cLo := cOrd := ""
   for i := 1 to len( a )
      if i < len(a) .and. a[i] > a[ i + 1 ] .and. !isalpha( chr( a[ i ] ) )
         ? "non alpha character " + charval( chr( a[ i ] ) ) + ;
           " sorted in non ASCII order !!!"
         lWarn := .t.
      endif
      c := chr( a[ i ] )
      cOrd += c
      if isdigit( c )
         if asc( c ) < asc( "0" ) .or. asc( c ) > asc( "9" )
            ? "character " + charis( c ) + " defined as digit"
            lWarn := .t.
         endif
      elseif asc( c ) >= asc( "0" ) .and. asc( c ) <= asc( "9" )
         ? "character " + charis( c ) + " is not defined as digit"
         lWarn := .t.
      endif
      if isalpha( c )
         if isupper( c )
            cUp += c
            if islower( c )
               ? "character " + charis( c ) + " defined as upper and lower"
               lWarn := .t.
            endif
            if lower( c ) == c
               ? "character " + charis( c ) + ;
                 " is the same as upper and lower"
               lWarn := .t.
            endif
         elseif islower( c )
            cLo += c
            if isupper( c )
               ? "character " + charis( c ) + " defined as upper and lower"
               lWarn := .t.
            endif
            if upper( c ) == c
               ? "character " + charis( c ) + ;
                 " is the same as upper and lower"
               lWarn := .t.
            endif
         else
            ? "character " + charis( c ) + " not defined as upper or lower"
            lWarn := .t.
         endif
      elseif islower( c ) .or. isupper( c )
         ? "wrongly defined character " + ;
           charval( c ) + ":" + charinfo( c )
         lWarn := .t.
      endif
   next
   cOrd2 := ""
   for i := 0 to 255
      if i == asc( cUp )
         cOrd2 += cUp
      elseif i == asc( cLo )
         cOrd2 += cLo
      endif
      c := chr( i )
      if ! c $ cUp + cLo
         cOrd2 += chr( i )
      endif
   next
   if ! len( cUp ) == len( cLo )
      ? "number of upper and lower characters is different"
      lWarn := .t.
   endif
   if ! cOrd == cOrd2
      ? "letters are not sorted continuously"
      lWarn := .t.
   endif
   if lWarn
      ? "Warning: irregular CP which needs special definition in Harbour"
   endif
   ? 'upper: "' + cUp + '"'
   ? 'lower: "' + cLo + '"'
   ? repl( "=", 50 )
   ?
return

static function charval( c )
return "'" + c + "' (" + ltrim( str( asc( c ) ) ) + ")"

static function charis( c )
return "'" + c + "' (" + ltrim( str( asc( c ) ) ) + ":" + ;
       iif( isalpha( c ), "A", " " ) + ;
       iif( isupper( c ), "U", " " ) + ;
       iif( islower( c ), "L", " " ) + ;
       iif( isdigit( c ), "D", " " ) + ")"

static function charinfo( c )
   local cInfo
   cInfo :=   "ISALPHA->" + iif( isalpha( c ), "Y", "N" )
   cInfo += ", ISUPPER->" + iif( isupper( c ), "Y", "N" )
   cInfo += ", ISLOWER->" + iif( islower( c ), "Y", "N" )
   cInfo += ", ISDIGIT->" + iif( isdigit( c ), "Y", "N" )
return cInfo
