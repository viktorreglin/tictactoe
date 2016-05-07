#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> //for sleep( )


// TICTACTOE (das Spiel)

//Funktionsdefinition gegen Warnmeldung - Deklaration folgt weiter unten
int emptyField( int board[3][3], int number );


typedef struct {
    int row;
    int col;
} fieldrc;


//wandelt 1 und -1 in x und o um
char numberToSymbol( int number ){
    char sym;
    
    switch(number){
     
        case 1:
            sym = 'x';
            break;
            
        case -1:
            sym = 'o';
            break;
            
        case 0:
            sym = '-';
            break;
        
    }
    
    return sym;    
}


//zeigt das aktuelle Spielfeld
void printBoard( int board[3][3] ){
    
    printf( "Spielfeld %17s\n", "Feldnummern" );
    printf( "%2c %2c %2c %10d %2d %2d\n", numberToSymbol( board[0][0] ), numberToSymbol( board[0][1] ), numberToSymbol( board[0][2] ), 1, 2, 3 );
    printf( "%2c %2c %2c %10d %2d %2d\n", numberToSymbol( board[1][0] ), numberToSymbol( board[1][1] ), numberToSymbol( board[1][2] ), 4, 5, 6 );
    printf( "%2c %2c %2c %10d %2d %2d\n", numberToSymbol( board[2][0] ), numberToSymbol( board[2][1] ), numberToSymbol( board[2][2] ), 7, 8, 9 );
}


//verwandelt Feldnummer (1 bis 9) in Zeile 0..2 und Spalte 0..2
fieldrc convertToRC( int field ){
    fieldrc rc;
    
    rc.row = (field - 1) / 3;
    rc.col = (field - 1) % 3;
    
    return rc;
}

//verwandelt Zeilte 0..2 und Spalte 0..2 in Feldnummer (1..9)
int convertToFieldnumber( fieldrc rc ){
    int fieldNumber;
    
    fieldNumber = rc.row * 3 + rc.col + 1;
    
    return fieldNumber;
}


//gibt 1 züruck wenn das aktuelle Feld frei ist, 0 falls bestzt
int legalMove( int board[3][3], int fieldNumber ){
    int legal, row, col;
    
    row = (convertToRC( fieldNumber )).row;
    col = (convertToRC( fieldNumber )).col;
    
    legal = board[row][col] == 0 ? 1 : 0;
    return legal;
}


//bestimmt den nächsten Zug der KI
int aiMove( int board[3][3], int player ){
    int order[9] = { 1, 3, 9, 7, 5, 2, 6, 8, 4 };
    int i;
    
    if( emptyField( board, player ) ) { //falls eigener Sieg möglich
        return emptyField( board, player );
    } else if( emptyField( board, player * -1 ) ) { //falls Sieg verhindern nötig
        return emptyField( board, player * -1 );
    } else {
        for( i = 0; i < 9; i++ ){
            if( legalMove( board, order[i] ) ) return order[i];
        }
    }
    
    return 0;
}


//gibt das leere Feld der ersten Zeile/Spalte/Diagonalen mit Summe = 2 * number zurück
int emptyField( int board[3][3], int number ){
    int i, j, sum;
    fieldrc field;
    
    for( i = 0; i < 3; i++ ){
        sum = board[i][0] + board[i][1] + board[i][2]; //Zeilensummen
        if( sum == 2 * number ){
            for( j = 0; j < 3; j++ ){
                if( board[i][j] == 0 ){
                    field.row = i;
                    field.col = j;
                    return convertToFieldnumber( field );
                }
            }
        }
    }
    
    for( i = 0; i < 3; i++ ){
        sum = board[0][i] + board[1][i] + board[2][i]; //Spaltensummen
        if( sum == 2 * number){
            for( j = 0; j < 3; j++ ){
                if( board[j][i] == 0 ){
                    field.row = j;
                    field.col = i;
                    return convertToFieldnumber( field );
                }
            }
        }
    }
    
    sum = board[0][0] + board[1][1] + board[2][2]; //diagonale Summe
    if( sum == 2 * number ){
        if( board[0][0] == 0 ){
            field.row = 0;
            field.col = 0;
            return convertToFieldnumber( field );
        } else if( board[1][1] == 0 ) {
            field.row = 1;
            field.col = 1;
            return convertToFieldnumber( field );
        } else {
            field.row = 2;
            field.col = 2;
            return convertToFieldnumber( field );
        }
    }
    
    sum = board[0][2] + board[1][1] + board[2][0]; //andere diagonale Summe
    if( sum == 2 * number ){
        if( board[0][2] == 0 ){
            field.row = 0;
            field.col = 2;
            return convertToFieldnumber( field );
        } else if( board[1][1] == 0 ) {
            field.row = 1;
            field.col = 1;
            return convertToFieldnumber( field );
        } else {
            field.row = 2;
            field.col = 0;
            return convertToFieldnumber( field );
        }
    }
    
    return 0;
}




//bildet Summen um herauszufinden ob der aktive Spieler gewonnen hat
int checkWin( int board[3][3], int player ){
    int i, sum;
        
    for( i = 0; i < 3; i++ ){
        sum = board[i][0] + board[i][1] + board[i][2]; //Zeilensummen
        if( sum == 3 * player ){
            printf( "Zeile %d\n", i );
            return 1;
        }
    }
    
    for( i = 0; i < 3; i++ ){
        sum = board[0][i] + board[1][i] + board[2][i]; //Spaltensummen
        if( sum == 3 * player ){
            printf( "Spalte %d\n", i );
            return 1;
        }
    }
    
    sum = board[0][0] + board[1][1] + board[2][2]; //diagonale Summe
    if( sum == 3 * player ){
        printf( "Diagonale von links oben nach rechts unten.\n" );
        return 1;
    }
    
    sum = board[0][2] + board[1][1] + board[2][0]; //andere diagonale Summe
    if( sum == 3 * player ){
        printf( "Diagonale von links unten nach rechts oben.\n" );
        return 1;
    }
    
    return 0;
}

//gibt Erklärungen aus falls der Programmaufruf inkorrekt war
void usage(  )
{
   printf("\nUSAGE: tictactoe MODE PLAYER\n\n");
   printf("                   MODE is 10 for user vs user, 20 for user vs ai, 30 for ai vs ai \n");
   printf("                        PLAYER indicates who goes first, needs to be x or o \n\n");
   exit( 1 );
}


int main( int argc, char * argv[] ){
    char wiederholen[80] = "j", playerSymbol;
    int mode, player, startingPlayer;

    if( argc != 3 )
        usage();

    mode = atoi( argv[1] ); //erster Parameter für Modus
    if( mode != 10 && mode !=20 && mode !=30 )
        usage();
    
    playerSymbol = *argv[2]; //zweiter Parameter für anfangenden Spieler
    
    if( playerSymbol != 'x' && playerSymbol != 'o' )
        usage();
    
    player = playerSymbol == 'x' ? 1 : -1;
    startingPlayer = player;
    
    do{
        player = startingPlayer;
        player *= -1; //wird am Anfang des Spiels wieder umgedreht
        
        printf( "\n***************\n" );
        printf( "** tictactoe **\n" );
        printf( "***************\n\n" );
        
        switch( mode ){
            case 10:
                printf( "Modus %d --> Spieler vs Spieler\n", mode );
                break;
                
            case 20:
                printf( "Modus %d --> Spieler vs KI\n\n", mode );
                printf( "Spieler beginnt\n" );
                break;
                
            case 30:
                printf( "Modus %d --> KI vs KI\n", mode );
                break;
                
            default:
                printf( "Keinen Modus erkannt. Beende.\n" );
                exit (1);
        }
        
        int board[3][3], row, col, fieldNumber;
        int i, end = 0, legal = 0, moveCounter = 0; //initiieren der Startwerte bei jedem Spiel
        
        for( row = 0; row < 3; row++ ){ //alle Felder werden auf 0 gesetzt
            for( col = 0; col < 3; col++){
                board[row][col] = 0;
            }
        }
        
        printf( "\n" );
        printBoard( board );
        printf( "\n" );
        
        do{
            player *= -1; //Spielerwechsel
            
            do{
                sleep( 1 );
                printf( "\n%c ist dran.\n", numberToSymbol( player ) );
                
                switch( mode ){
                    case 10:
                        printf( "Feldnummer eingeben (1 bis 9): " );
                        scanf( "%d", &fieldNumber ); //Zeile und Spalte einlesen, jeweils von 0 bis 2
                        break;
                        
                    case 20:
                        if( player == startingPlayer ){
                            printf( "Feldnummer eingeben (1 bis 9): " );
                            scanf( "%d", &fieldNumber ); //Zeile und Spalte einlesen, jeweils von 0 bis 2
                        } else fieldNumber = aiMove( board, player );
                        break;
                        
                    case 30:
                        fieldNumber = aiMove( board, player );
                        break;
                }
                
                printf( "\n" );
                                
                sleep( 1 );
                if( legalMove( board, fieldNumber ) ){ //Feld nur belegen falls noch 0
                    legal = 1;
                    board[(convertToRC( fieldNumber )).row][(convertToRC( fieldNumber )).col] = player;
                    moveCounter++;
                } else{
                    printf( "Feld schon belegt.\n" );
                    legal = 0;
                }
            } while( !legal ); //so lange nach Zeile und Spalte fragen bis ein unbesetztes Feld angegeben wurde
            
            printBoard( board );
            printf( "\n" );
            
            if( checkWin( board, player ) ){
                printf( "%c hat gewonnen.\n", numberToSymbol( player ) );
                end = 1;
            } else if( moveCounter == 9 ){ //moveCounter == 9 heißt alle Felder besetzt
                printf( "Unentschieden.\n" );
                end = 1;
            }
        } while( !end ); //so lange spielen bis einer gewonnen hat oder ein Unentschieden vorliegt
        
        printf( "\nNoch einmal? (j/n): " );
        scanf( "%s", &wiederholen[0] );
    } while( *wiederholen == 'j' ); //neue Spiele starten bis Aufrufer keine Lust mehr hat
    
    return 0;
}