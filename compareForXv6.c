
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

/*  Xv6 version
 *  [Name] Wenqiang Bao
 *  [Contact] wb854@nyu.edu
 *  
 *  What is this program? 
 *  --Compare two files to check if two files are the same. If not same, output the different lines.
 *  --So you can check if student answer file is correct.
 *  
 *  How to run? 
 *  --compareForXv6 file1 file2
 */

#define LINE_BUFFER_MAX 1024 // max number of characters for a line

char line1[LINE_BUFFER_MAX];
char line2[LINE_BUFFER_MAX];

// read a line from file each time
int readline(int file, char* buffer, const int LIMIT){
    char c;
    int idx = 0;
    while(read(file,&c,1) != 0 && idx < LIMIT){
        buffer[idx] = c;
        if(c == '\n'){
            buffer[idx] = '\0';
            return 1;
        }
        idx++;
    }
    if( idx >= LIMIT ){
        printf(1, "Exceeds: %d characters limit for a line.\n", LIMIT);
        return -1;
    }
    buffer[idx] = '\0';
    return 0;
}

// compare two lines
int comparelines(char* line1, char* line2, const int LIMIT){
    int idx = 0;
    char c1;
    char c2;
    while( idx < LIMIT ){
        c1 = line1[idx];
        c2 = line2[idx];
        if(c1 == '\0' || c2 == '\0')
            break;
        if( c1 != c2 )
            return -1;
        idx++ ;
    }
    if( idx >= LIMIT ){
        printf(1,"Exceeds: %d characters limit for a line.\n", LIMIT);
        return -2; 
    }
    if( c1 == '\0' && c2 == '\0')
        return 1;
    return -1;
}

int main(int argn, char* argv[]){
    char* filename1;
    char* filename2;
    int file1;
    int file2;
    int lineidx = 0;
    int dffs = 0;
    if(argn != 3){
        printf(1,"Format: compare file1 file2\n");
        exit();
    }
    filename1 = argv[1];
    filename2 = argv[2];
    
    if((file1 = open(filename1,O_RDONLY)) < 0 ||
    (file2 = open(filename2,O_RDONLY)) < 0){
        printf(1,"Error: Can not open file.\n");
        exit();
    }
    
    while(1){
        int readres_1;
        int readres_2;
        int compres;
        readres_1 = readline(file1,line1,LINE_BUFFER_MAX);
        readres_2 = readline(file2,line2,LINE_BUFFER_MAX);
        if( readres_1 == -1 || readres_2 == -1 ) // error
            break;
        compres = comparelines(line1, line2, LINE_BUFFER_MAX);
        if( compres == -2) // error
            break;
        if( compres == -1){ // different line
            printf(1,"[line %d]\n",lineidx);
            printf(1,"%s\t\"%s\"\n",filename1,line1);
            printf(1,"%s\t\"%s\"\n\n",filename2,line2);
            dffs++;
        }
        lineidx++;
        if(readres_1 == 0 && readres_2 == 0) // no remaining line
            break;
        if(readres_1 == 0){ // file2 has remaining line
            while(readres_2 != 0){
                readres_2 = readline(file2,line2,LINE_BUFFER_MAX);
                if(line2[0] == '\0'){
                    lineidx++;
                    continue;
                }
                printf(1,"[line %d]\n",lineidx);
                printf(1,"%s\t\"%s\"\n", filename1, "");
                printf(1,"%s\t\"%s\"\n\n", filename2, line2);
                lineidx++;
                dffs++;
            }
            break;
        }
        if(readres_2 == 0){ // file1 has remaining line
            while(readres_1 != 0){
                readres_1 = readline(file1,line1,LINE_BUFFER_MAX);
                if(line1[0] == '\0'){
                    lineidx++;
                    continue;
                }
                printf(1,"[line %d]\n",lineidx);
                printf(1,"%s\t\"%s\"\n", filename1, line1);
                printf(1,"%s\t\"%s\"\n\n", filename2, "");
                lineidx++;
                dffs++;
            }
            break;
        }
    }
    if(dffs == 0)
        printf(1,"[Passed]\nTwo files perfectly matched.\n");
    else
        printf(1,"\n%d different lines occured.\n", dffs);
    close(file1);
    close(file2);
    exit();
}
