#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

// Global parameters
struct setup {
    int excel ;
    int digit ;
    int base ;
    int ratio ;
    int title ;
    int start ;
    int end ;
    int nDigits ;
    int ** sums ;
    int ** digits ;
} SU = {
    .excel = 0,
    .digit = 9,
    .base = 10,
    .title = 0,
    .ratio = 0,
    .start=321,
    .end=321,
    };
    

void Add( void ) {
    // Add incr in sum
    int d ;
    for ( d=1 ; d<SU.base ; ++d ) {
        int i;
        int carry = 0;
        for (i=0;i<SU.nDigits;++i) {
            SU.sums[d][i] += SU.digits[d][i] + carry ;
            if ( SU.sums[d][i] >= SU.base  ) {
                SU.sums[d][i] -= SU.base ;
                carry = 1 ;
            } else {
                carry = 0 ;
            }
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

void Digit( int place ) {
    // Put digit in next place
    int d ;
    for ( d=1 ; d<SU.base ; ++d ) {
        SU.digits[d][place] = d ;
    }
}

long int SumDigits( int digit ) {
    // Add the digits
    int i ;
    long int sum = 0 ;
    for ( i=0 ; i < SU.nDigits ; ++i ) {
        sum += SU.sums[digit][i] ;
    }
    return sum ;
}


void Repeat( void ) {
    int d ;
    for ( d=1 ; d<SU.base ; ++d ) {
        int zeroes ;
        int repeats = 0 ;
        int i ;
        int j ;
        int * Sum = SU.sums[d] ;
        int sod = 0 ;
        
        for ( zeroes=SU.nDigits-1 ; zeroes>1 ; --zeroes ) {
            if ( Sum[zeroes] != 0 ) {
                break ;
            }
        }
        if ( zeroes < SU.base*3 ) {
            printf("Sums too small for this base\n") ;
            break ;
        }
        j = zeroes - SU.base + 1 ;
        while ( j > SU.base - 1 ) {
            for ( i = 0 ; i < SU.base - 1  ; ++i ) {
                if ( Sum[zeroes-i] != Sum[j] ) {
                    j=0 ; 
                    break ;
                }
                --j ;
            }
            ++ repeats ;
        }
        printf( "Digit=%d, Base=%d, Repeats=%d\t\t" , d, SU.base, repeats ) ;
        for ( i=0 ; i<SU.base-1 ; ++i ) {
            sod += Sum[zeroes-i] ;
            printf("%d ",Sum[zeroes-i]) ;
        }
        printf("\t\tAvg=%g\n",(0.0+sod)/(SU.base-1.0)) ;
    }
    printf("\n");
}
     

void Makespace( void ) {
    int d ;
    for ( d=1 ; d<SU.base ; ++d ) {
        SU.digits[d] = calloc( SU.nDigits , sizeof(int) ) ;
        if ( SU.digits[d] == NULL ) {
            fprintf(stderr,"Memory exhausted\n");
            exit(1) ;
        }
        Zero( SU.digits[d] ) ;
        
        SU.sums[d] = calloc( SU.nDigits , sizeof(int) ) ;
        if ( SU.sums[d] == NULL ) {
            fprintf(stderr,"Memory exhausted\n");
            exit(1) ;
        }
        Zero( SU.sums[d] ) ;
    }
}

void Freespace( void ) {
    int d ;
    for ( d=1 ; d<SU.base ; ++d ) {
         free(SU.sums[d]) ;
         free(SU.digits[d]) ;
    }
}

void SumOfDigits( void ) {
    int place;     

    SU.nDigits = SU.end + 2 ;
    
    Makespace() ;

    for ( place = 1 ; place <= SU.end ; ++place ) {
        Digit(place);
        Add() ;
    }
    Repeat() ;

    Freespace() ;
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
    printf("-r\tratios of SOD/length\n");
    printf("-b10\tnumber base to use -- default 10\n");
    printf("-t\tAdd column titles\n");
    printf("321\tMaximum length of repeating digits string\n");
    printf("1-321\tShow a range of maximum digit strings\n");
}
 
int main( int argc, char **argv ) {
    int c;
    int opterr = 0 ;

    while ( (c = getopt( argc, argv, "hrb::tx" )) != -1 ) {
        switch ( c ) {
        case 'h':
            help() ;
            exit(0);
            break ;
        case 'r':
            SU.ratio = 1 ;
            break ;
        case 't':
            SU.title = 1;
            break ;
        case 'x':
            SU.excel = 1 ;
            break ;
        case 'b':
            SU.base = (int)strtol(optarg,NULL,0) ;
            if ( SU.base < 1 ) {
                fprintf( stderr, "bad base\n" ) ;
                exit(1) ;
            }
            break ;
        }
    }
    
    // allocate array pointers (not actual arrays)
    SU.sums = calloc( SU.base, sizeof(int*) ) ;
    SU.digits = calloc( SU.base, sizeof(int*) ) ;
    
    if  ( SU.sums == NULL || SU.digits == NULL ) {
        fprintf(stderr, "Memory exhausted\n");
        exit(1);
    }
    
    if ( SU.title ) {
        if ( SU.excel ) {
            int d ;
            printf("length");
            for (d=0 ; d<SU.base ; ++d) {
                printf(",SOD_%d",d);
            }
            printf("\n");
        } else {
            printf("Repeating digit strings base %d and sum of digits of sum\n",SU.base);
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
    
    free( SU.digits);
    free( SU.sums) ;
    
    return 0 ;
 }
 
            
        


