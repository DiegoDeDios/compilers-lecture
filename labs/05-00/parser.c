#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

const char* ID = "id";
FILE * fp;

char E(char** ts);
char Eprime(char** ts);
char T(char** ts);
char Tprime(char** ts);
char F(char** ts);
char parse(char** ts);

int openFile(char* filename){
    int fd = open(filename, O_RDONLY);
    return fd;
}

off_t getSize(int fd){
    off_t curOffset = lseek(fd, (size_t)0, SEEK_CUR);
    off_t size = lseek(fd, (size_t)0, SEEK_END); 
    lseek(fd, curOffset, SEEK_SET);
    return size;
}



/* From book;
peek, which examines the next input token without advancing the input
advance, which advances the input by one token

procedure match(ts, token)
if ts.peek( ) = token
then call ts.advance( )
else call error( Expected token)
end
*/

void nextToken(char** ts) {
   while (*ts[0] == ' ' || *ts[0] == '\n' || *ts[0] == '\t')
   *ts = *ts + 1;
 }

char match(char** ts, const char* token){
    if (strncmp(*ts, token, strlen(token)) == 0) {
         *ts = *ts + strlen(token);
         return 1;
      }
      return 0;
}


char E(char** ts){
   char *original = *ts;
   nextToken(ts);
   //printf("E\n");
   if(!T(ts)){
       *ts = original;
       return 0;
   }
     if(!Eprime(ts)){
       *ts = original;
       return 0;
   }
    fprintf(fp,"E -- T;\n");
    fprintf(fp,"E -- Eprime;\n");
   return 1;
}
char Eprime(char** ts){
   char *original = *ts;
   nextToken(ts);
   //printf("Eprime\n");
   if(match(ts, "+")){
       //printf("+\n");
        if(!T(ts)){
            *ts = original;
       return 0;
   }
        if(!Eprime(ts)){
            *ts = original;
            return 0;
        }
    fprintf(fp,"Eprime -- \"+\";\n");
    fprintf(fp,"Eprime -- T;\n");
    fprintf(fp,"Eprime -- Eprime;\n");
    return 1;
   }
   fprintf(fp,"Eprime -- e;\n");
   return 1;

}
char T(char** ts){
   char *original = *ts;
   nextToken(ts);
   //printf("T\n");
   if(!F(ts)){
       *ts = original;
       return 0;
   }
   if(!Tprime(ts)){
       *ts = original;
       return 0;
   }
   fprintf(fp,"T -- F;\n");
   fprintf(fp,"T -- Tprime;\n");
   return 1;
}
char Tprime(char** ts){
   char *original = *ts;
   nextToken(ts);
   //printf("Tprime\n");
   if(match(ts, "*")){
       //printf("*\n");
      if(!F(ts)){
       *ts = original;
       return 0;
    }
    if(!Tprime(ts)){
       *ts = original;
       return 0;
        }
        fprintf(fp,"Tprime -- \"*\";\n");
        fprintf(fp,"Tprime -- F;\n");
        fprintf(fp,"Tprime -- Tprime;\n");
        return 1;
    
   }
   fprintf(fp,"Tprime -- e;\n");
   return 1;
}
char F(char** ts){
   char *original = *ts;
   nextToken(ts);
   //printf("F\n");
   if(match(ts, ID)){
       fprintf(fp,"F -- id;\n");
       //printf("id\n");
       return 1;
   }
   
    if(!match(ts, "(")){
    *ts = original;
    return 0;
   }
   if(!E(ts)){
    *ts = original;
    return 0;
   }
   if(!match(ts,")")){
    *ts = original;
    return 0;
   }
   fprintf(fp,"F -- (;\n");
   fprintf(fp,"F -- E;\n");
   fprintf(fp,"F -- );\n");
   return 1;
}
char parse(char** ts){
   char *original = *ts;
   if(E(ts)){
       nextToken(ts);
       if(*ts[0]=='\0'){
           return 1;
       }
       else{
        *ts = original;
        return 0;
       }
   }
   return 0; 
}

/*Remember that you're using file descriptors! Close them at the end and free the space on the heap
Shut up
No, you shut up
*/



int main(char argc, char** argv){
    int fd = openFile(argv[1]);
    if(fd == -1){
        printf("Invalid file\n");
        return -1;
    }
    off_t size = getSize(fd);
    char* fileBuffer = (char*)malloc(sizeof(char)*size);
    char* copy = fileBuffer;
    read(fd,fileBuffer,size);
    fp = fopen ("Graph.txt","w");
    fprintf(fp,"graph G {\nnodesep=0.8;\nranksep=0.5;\n");
    if(parse(&copy)){
        printf("Cadena aceptada\n");
        fprintf(fp,"}");
        fclose(fp);
        free(fileBuffer);
        close(fd);
    }
    return 0;
}