/*
 * $Id$
 */

/*
 * Author....: Ted Means
 * CIS ID....: 73067,3332
 *
 * This is an original work by Ted Means and is placed in the
 * public domain.
 *
 * Modification history:
 * ---------------------
 *
 *    Rev 1.5   16 Oct 1992 00:20:28   GLENN
 * Cleaned up documentation header.
 *
 *    Rev 1.4   16 Oct 1992 00:08:44   GLENN
 * Just making sure we had Ted's latest revision.
 *
 *    Rev 1.3   13 Oct 1992 20:45:46   GLENN
 * Complete rewrite by Ted Means, dumping assembler version for a
 * Clipper version.
 *
 *    Rev 1.2   15 Aug 1991 23:03:54   GLENN
 * Forest Belt proofread/edited/cleaned up doc
 *
 *    Rev 1.1   14 Jun 1991 19:52:16   GLENN
 * Minor edit to file header
 *
 *    Rev 1.0   01 Apr 1991 01:01:42   GLENN
 * Nanforum Toolkit
 *
 */

#include "setcurs.ch"
#include "inkey.ch"

#xtranslate EnhColor( <colorspec> ) => ;
      SubStr( <colorspec>, At( ",", <colorspec> ) + 1 )

#xtranslate isOkay( <exp> ) => ;
      ( <exp> \ > 0 .AND. <exp> \ <= nCount )

#xtranslate isBetween( <val>, <lower>, <upper> ) => ;
      ( <val> \ >= <lower> .AND. <val> \ <= <upper> )

#define nTriggerInkey hb_keyCode( Upper( SubStr( cPrompt, nTrigger, 1 ) ) )
#define cTrigger SubStr( cPrompt, nTrigger, 1 )
#define nCurrent nMenu,nActive
#define nLast nMenu,nPrev

// These arrays hold information about each menu item

THREAD STATIC t_aRow          := { {} }
THREAD STATIC t_aCol          := { {} }
THREAD STATIC t_aPrompt       := { {} }
THREAD STATIC t_aColor        := { {} }
THREAD STATIC t_aMsgRow       := { {} }
THREAD STATIC t_aMsgCol       := { {} }
THREAD STATIC t_aMessage      := { {} }
THREAD STATIC t_aMsgColor     := { {} }
THREAD STATIC t_aTrigger      := { {} }
THREAD STATIC t_aTriggerInkey := { {} }
THREAD STATIC t_aTriggerColor := { {} }
THREAD STATIC t_aHome         := { {} }
THREAD STATIC t_aEnd          := { {} }
THREAD STATIC t_aUp           := { {} }
THREAD STATIC t_aDown         := { {} }
THREAD STATIC t_aLeft         := { {} }
THREAD STATIC t_aRight        := { {} }
THREAD STATIC t_aExecute      := { {} }
THREAD STATIC t_nLevel        := 1

FUNCTION FT_Prompt( nRow, nCol, cPrompt, cColor, ;
      nMsgRow, nMsgCol, cMessage, cMsgColor, ;
      nTrigger, cTriggerColor, nHome, nEnd, ;
      nUp, nDown, nLeft, nRight, bExecute )

   // If the prompt color setting is not specified, use default
   IF cColor  == NIL
      cColor  := SetColor()
   ENDIF

   // If no message is supplied, set message values to NIL

   IF cMessage == NIL

      nMsgRow := nMsgCol := cMsgColor := NIL

   ELSE

      // If message row not supplied, use the default
      IF nMsgRow == NIL
         nMsgRow := Set( _SET_MESSAGE )
      ENDIF

      // If message column not supplied, use the default
      IF nMsgCol == NIL
         IF Set( _SET_MCENTER )
            nMsgCol := Int( ( MaxCol() + 1 - Len( cPrompt ) ) / 2 )
         ELSE
            nMsgCol := 0
         ENDIF
      ENDIF

      // If message color not specified, use the default
      IF cMsgColor == NIL
         cMsgColor := cColor
      ENDIF
   ENDIF

   // If trigger values not specifed, set the defaults
   IF nTrigger       == NIL
      nTrigger      := 1
   ENDIF
   IF cTriggerColor  == NIL
      cTriggerColor := cColor
   ENDIF

   // Now add elements to the static arrays -- t_nLevel indicates the recursion
   // level, which allows for nested menus.
   AAdd(          t_aRow[ t_nLevel ], nRow          )
   AAdd(          t_aCol[ t_nLevel ], nCol          )
   AAdd(       t_aPrompt[ t_nLevel ], cPrompt       )
   AAdd(        t_aColor[ t_nLevel ], cColor        )
   AAdd(       t_aMsgRow[ t_nLevel ], nMsgRow       )
   AAdd(       t_aMsgCol[ t_nLevel ], nMsgCol       )
   AAdd(      t_aMessage[ t_nLevel ], cMessage      )
   AAdd(     t_aMsgColor[ t_nLevel ], cMsgColor     )
   AAdd(      t_aTrigger[ t_nLevel ], nTrigger      )
   AAdd( t_aTriggerInkey[ t_nLevel ], nTriggerInkey )
   AAdd( t_aTriggerColor[ t_nLevel ], cTriggerColor )
   AAdd(         t_aHome[ t_nLevel ], nHome         )
   AAdd(          t_aEnd[ t_nLevel ], nEnd          )
   AAdd(           t_aUp[ t_nLevel ], nUp           )
   AAdd(         t_aDown[ t_nLevel ], nDown         )
   AAdd(         t_aLeft[ t_nLevel ], nLeft         )
   AAdd(        t_aRight[ t_nLevel ], nRight        )
   AAdd(      t_aExecute[ t_nLevel ], bExecute      )

   // Now display the prompt for the sake of compatibility
   DispBegin()
   hb_DispOutAt( nRow, nCol, cPrompt, cColor )
   hb_DispOutAt( nRow, nCol - 1 + nTrigger, cTrigger, cTriggerColor )
   DispEnd()

   RETURN NIL

FUNCTION FT_MenuTo( bGetSet, cReadVar, lCold )

   LOCAL nMenu   := t_nLevel++
   LOCAL nActive
   LOCAL nCount  := Len( t_aRow[ nMenu ] )
   LOCAL lChoice := .F.
   LOCAL nCursor := Set( _SET_CURSOR, SC_NONE )
   LOCAL nKey, bKey, nScan, lWrap, cScreen, nPrev

   IF ! HB_ISLOGICAL( lCold )
      lCold := .F.
   ENDIF

   // Validate the incoming parameters and assign some reasonable defaults
   // to prevent a crash later.

   cReadVar := iif( cReadVar == NIL, "", Upper( cReadVar ) )

   IF bGetSet == NIL
      bGetSet := {|| 1 }
   ENDIF

   // Eval the incoming getset block to initialize nActive, which indicates
   // the menu prompt which is to be active when the menu is first displayed.
   // If nActive is outside the appropriate limits, a value of 1 is assigned.

   nActive := Eval( bGetSet )

   IF ( nActive < 1 .OR. nActive > nCount )
      nActive := 1
   ENDIF

   // Increment the recursion level in case a hotkey procedure
   // calls FT_Prompt().  This will cause a new set of prompts
   // to be created without disturbing the current set.

   AAdd(          t_aRow, {} )
   AAdd(          t_aCol, {} )
   AAdd(       t_aPrompt, {} )
   AAdd(        t_aColor, {} )
   AAdd(       t_aMsgRow, {} )
   AAdd(       t_aMsgCol, {} )
   AAdd(      t_aMessage, {} )
   AAdd(     t_aMsgColor, {} )
   AAdd(      t_aTrigger, {} )
   AAdd( t_aTriggerInkey, {} )
   AAdd( t_aTriggerColor, {} )
   AAdd(           t_aUp, {} )
   AAdd(         t_aDown, {} )
   AAdd(         t_aLeft, {} )
   AAdd(        t_aRight, {} )
   AAdd(      t_aExecute, {} )

   // Loop until Enter or Esc is pressed

   WHILE ! lChoice

      // Evaluate the getset block to update the target memory variable
      // in case it needs to be examined by a hotkey procedure.

      Eval( bGetSet, nActive )

      // Get the current setting of SET WRAP so that the desired menu behavior
      // can be implemented.

      lWrap := Set( _SET_WRAP )

      // If a message is to be displayed, save the current screen contents
      // and then display the message, otherwise set the screen buffer to NIL.

      DispBegin()

      IF t_aMessage[ nCurrent ] != NIL
         cScreen := SaveScreen( t_aMsgRow[ nCurrent ], t_aMsgCol[ nCurrent ], ;
            t_aMsgRow[ nCurrent ], t_aMsgCol[ nCurrent ] + ;
            Len( t_aMessage[ nCurrent ] ) - 1 )

         hb_DispOutAt( t_aMsgRow[ nCurrent ], t_aMsgCol[ nCurrent ], ;
            t_aMessage[ nCurrent ], t_aMsgColor[ nCurrent ]  )

      ELSE
         cScreen := NIL
      ENDIF

      // Display the prompt using the designated colors for the prompt and
      // the trigger character.

      hb_DispOutAt( t_aRow[ nCurrent ], t_aCol[ nCurrent ], ;
         t_aPrompt[ nCurrent ], EnhColor( t_aColor[ nCurrent ] ) )

      hb_DispOutAt( t_aRow[ nCurrent ], ;
         t_aCol[ nCurrent ] - 1 + t_aTrigger[ nCurrent ], ;
         SubStr( t_aPrompt[ nCurrent ], t_aTrigger[ nCurrent ], 1 ), ;
         EnhColor( t_aTriggerColor[ nCurrent ] ) )

      DispEnd()

      // Wait for a keystroke
      nKey := Inkey( 0 )

      // If the key was an alphabetic char, convert to uppercase
      IF isBetween( nKey, 97, 122 )
         nKey -= 32
      ENDIF

      // Set nPrev to the currently active menu item
      nPrev := nActive

      DO CASE

         // Check for a hotkey, and evaluate the associated block if present.

      CASE ( bKey := SetKey( nKey ) ) != NIL
         Eval( bKey, ProcName( 1 ), ProcLine( 1 ), cReadVar )

         // If Enter was pressed, either exit the menu or evaluate the
         // associated code block.

      CASE nKey == K_ENTER
         IF t_aExecute[ nCurrent ] != NIL
            Eval( t_aExecute[ nCurrent ] )
         ELSE
            lChoice := .T.
         ENDIF

         // If ESC was pressed, set the selected item to zero and exit.

      CASE nKey == K_ESC
         lChoice := .T.
         nActive := 0

         // If Home was pressed, go to the designated menu item.

      CASE nKey == K_HOME
         nActive := iif( t_aHome[ nCurrent ] == NIL, 1, t_aHome[ nCurrent ] )

         // If End was pressed, go to the designated menu item.

      CASE nKey == K_END
         nActive := iif( t_aEnd[ nCurrent ] == NIL, nCount, t_aEnd[ nCurrent ] )

         // If Up Arrow was pressed, go to the designated menu item.

      CASE nKey == K_UP
         IF t_aUp[ nCurrent ] == NIL
            if --nActive < 1
               nActive := iif( lWrap, nCount, 1 )
            ENDIF
         ELSE
            IF isOkay( t_aUp[ nCurrent ] )
               nActive := t_aUp[ nCurrent ]
            ENDIF
         ENDIF

         // If Down Arrow was pressed, go to the designated menu item.

      CASE nKey == K_DOWN
         IF t_aDown[ nCurrent ] == NIL
            if ++nActive > nCount
               nActive := iif( lWrap, 1, nCount )
            ENDIF
         ELSE
            IF isOkay( t_aDown[ nCurrent ] )
               nActive := t_aDown[ nCurrent ]
            ENDIF
         ENDIF

         // If Left Arrow was pressed, go to the designated menu item.

      CASE nKey == K_LEFT
         IF t_aLeft[ nCurrent ] == NIL
            if --nActive < 1
               nActive := iif( lWrap, nCount, 1 )
            ENDIF
         ELSE
            IF isOkay( t_aLeft[ nCurrent ] )
               nActive := t_aLeft[ nCurrent ]
            ENDIF
         ENDIF

         // If Right Arrow was pressed, go to the designated menu item.

      CASE nKey == K_RIGHT
         IF t_aRight[ nCurrent ] == NIL
            if ++nActive > nCount
               nActive := iif( lWrap, 1, nCount )
            ENDIF
         ELSE
            IF isOkay( t_aRight[ nCurrent ] )
               nActive := t_aRight[ nCurrent ]
            ENDIF
         ENDIF

         // If a trigger letter was pressed, handle it based on the COLD
         // parameter.

      CASE ( nScan := AScan( t_aTriggerInkey[ nMenu ], nKey ) ) > 0
         nActive := nScan
         IF ! lCold
            hb_keyPut( K_ENTER )
         ENDIF
      ENDCASE

      // Erase the highlight bar in preparation for the next iteration

      IF ! lChoice
         DispBegin()
         hb_DispOutAt( t_aRow[ nLast ], t_aCol[ nLast ], ;
            t_aPrompt[ nLast ], t_aColor[ nLast ] )

         hb_DispOutAt( t_aRow[ nLast ], t_aCol[ nLast ] - 1 + t_aTrigger[ nLast ], ;
            SubStr( t_aPrompt[ nLast ], t_aTrigger[ nLast ], 1 ), ;
            t_aTriggerColor[ nLast ] )

         IF cScreen != NIL
            RestScreen( t_aMsgRow[ nLast ], ;
               t_aMsgCol[ nLast ], ;
               t_aMsgRow[ nLast ], ;
               t_aMsgCol[ nLast ]  ;
               + Len( t_aMessage[ nLast ] ) - 1, ;
               cScreen )
         ENDIF
         DispEnd()
      ENDIF
   ENDDO

   // Now that we're exiting, decrement the recursion level and erase all
   // the prompt information for the current invocation.

   t_nLevel--

   ASize(          t_aRow, t_nLevel )
   ASize(          t_aCol, t_nLevel )
   ASize(       t_aPrompt, t_nLevel )
   ASize(        t_aColor, t_nLevel )
   ASize(       t_aMsgRow, t_nLevel )
   ASize(       t_aMsgCol, t_nLevel )
   ASize(      t_aMessage, t_nLevel )
   ASize(     t_aMsgColor, t_nLevel )
   ASize(      t_aTrigger, t_nLevel )
   ASize( t_aTriggerInkey, t_nLevel )
   ASize( t_aTriggerColor, t_nLevel )
   ASize(           t_aUp, t_nLevel )
   ASize(         t_aDown, t_nLevel )
   ASize(         t_aLeft, t_nLevel )
   ASize(        t_aRight, t_nLevel )
   ASize(      t_aExecute, t_nLevel )

   t_aRow[ t_nLevel ] := {}
   t_aCol[ t_nLevel ] := {}
   t_aPrompt[ t_nLevel ] := {}
   t_aColor[ t_nLevel ] := {}
   t_aMsgRow[ t_nLevel ] := {}
   t_aMsgCol[ t_nLevel ] := {}
   t_aMessage[ t_nLevel ] := {}
   t_aMsgColor[ t_nLevel ] := {}
   t_aTrigger[ t_nLevel ] := {}
   t_aTriggerInkey[ t_nLevel ] := {}
   t_aTriggerColor[ t_nLevel ] := {}
   t_aUp[ t_nLevel ] := {}
   t_aDown[ t_nLevel ] := {}
   t_aLeft[ t_nLevel ] := {}
   t_aRight[ t_nLevel ] := {}
   t_aExecute[ t_nLevel ] := {}

   Set( _SET_CURSOR, nCursor )

   Eval( bGetSet, nActive )

   RETURN nActive
