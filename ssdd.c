#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

// Global parameters
struct setup {
    int excel ;
    int digit ;
    int title ;
    int start ;
    int end ;
    int nDigits ;
} SU = {
    .excel = 0,
    .digit = 9,
    .title=0,
    .start=321,
    .end=321,
    };
    

void Add( int* incr, int* sum ) {
    // Add incr in sum
    int i;
    int carry = 0;
    for (i=0;i<SU.nDigits;++i) {
        sum[i] += incr[i] + carry ;
        if ( sum[i] > 9 ) {
            sum[i] -= 10 ;
            carry = 1 ;
        } else {
            carry = 0 ;
        }
    }
}

void Zero( int* num ) {
    // zero the number
    int i ;
    for ( i=0 ; i < SU.nDigits ; ++i ) {
        num[i] = 0 ;
    }
}

void Nine( int nines, int * num ) {
    // Put 9 in first nines place
    int i ;
    for ( i=0 ; i < nines ; ++i ) {
        num[i] = SU.digit ;
    }
}

long int SumDigits( int * num ) {
    // Add the digits
    int i ;
    long int sum = 0 ;
    for ( i=0 ; i < SU.nDigits ; ++i ) {
        sum += num[i] ;
    }
    return sum ;
}

void SumOfDigits( void ) {
    // Allocate and add all the nines up to the max
    int n;     
    int * sum ;
    int * nine ;

    SU.nDigits = SU.end + 2 ;

    sum = calloc( SU.nDigits, sizeof(int) ) ;
    nine = calloc( SU.nDigits, sizeof(int) ) ;

    if ( sum == NULL || nine == NULL ) {
        exit(1) ;
    }

    Zero( sum ) ;
    for ( n = 1 ; n <= SU.end ; ++n ) {
        Zero( nine ) ;
        Nine( n, nine ) ;
        Add( nine, sum ) ;
        if ( n >= SU.start ) {
            if ( SU.excel ) {
                printf("%d,%ld\n",n , SumDigits( sum ) ) ;
            } else {
                printf( "digits=%d, sum=%ld\n", n , SumDigits( sum ) ) ;
            }
        }
    }

    free( nine ) ;
    free( sum ) ;
}

void Limits( char * str ) {
    int start ;
    int end ;
    
    switch ( sscanf( str, "%d-%d", &start, &end ) ) {
    case 1:
        if ( start < 0 ) {
            fprintf(stderr, "Invalid value %d\nNote that range cannot have spaces e.g. 10-100\n",start);
            exit(1);
        }
        SU.start = start ;
        SU.end = start ;
        SumOfDigits() ;
        break ;
    case 2:
        if ( start < 0 ) {
            fprintf(stderr, "Invalid value %d\n",start);
            exit(1);
        } else if ( start > end ) {
            fprintf(stderr,"Invalid range start %d end %d\n",start,end);
            exit(1) ;
        }
        SU.start = start ;
        SU.end = end ;
        SumOfDigits() ;
        break ;
    default:
        break ;
    }
}
 
void help( void ) {
    printf("Add Digits -- by Paul H Alfille based on concept by Daniel Saddawi-Konefka\n");
    printf("\tShows sum of digits for a sum of numbers:\n");
    printf("\t9+99+999+9999+99999+...\n");
    printf("-h\tthis help message\n");
    printf("-x\tExcel (i.e. csv) format\n");
    printf("-d9\tdigit to use -- default 9\n");
    printf("-t\tAdd column titles\n");
    printf("321\tMaximum length of repeating digits string\n");
    printf("1-321\tShow a range of maximum digit strings\n");
}
 
int main( int argc, char **argv ) {
    int c;
    int opterr = 0 ;

    while ( (c = getopt( argc, argv, "hd::tx" )) != -1 ) {
        switch ( c ) {
        case 'h':
            help() ;
            exit(0);
            break ;
        case 't':
            SU.title = 1;
            break ;
        case 'x':
            SU.excel = 1 ;
            break ;
        case 'd':
            SU.digit = (int)strtol(optarg,NULL,0) ;
            if ( SU.digit < 1 || SU.digit > 14 ) {
                fprintf( stderr, "bad digit\n" ) ;
                exit(1) ;
            }
            break ;
        }
    }
    
    if ( SU.title ) {
        if ( SU.excel ) {
            printf("length,SOD_%d\n",SU.digit);
        } else {
            printf("Repeating digit strings (%d) and sum of digits of sum\n",SU.digit);
        }
    }
    
    if ( optind == argc ) {
        SumOfDigits() ;
    } else {
        int index ;
        for ( index = optind ; index < argc ; ++index ) {
            char * token ;
            char * str = argv[index] ;
            while( (token=strsep(&str,",")) != NULL ) {
                Limits(token) ;
            }
        }
    }
     return 0 ;
 }
 
            
        


