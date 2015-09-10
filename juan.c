#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include  <sys/types.h>
int main()
{
   while(1)
   {
   char* nombre= (char*)malloc(100);
   char *token;
   int i=0;
   int tam=1; //cantidad palabras que parte por defecto que fue ingresado una palabra por defecto
   int largo=0;
   int j=0;
   
 
   /**
   Saludo inicial terminal
   */
  
    fgets(nombre,100,stdin);
    largo=strlen(nombre);
    nombre[largo-1]=0;
    /**
    Buscar en nombre cuantos espacios hay para saber cuantas palabras hay y crear la variable parse que guardara cada palabra por separado
    */
    while(j<largo)
       {
         if(nombre[j]==' ')
           tam++;
         j++;
       }
   printf("cantidad de palabras %i\n",tam);
   char *parse[tam];
   /* get the first token */
   token= strtok(nombre," ");
   parse[i] = token;
   i++;
   /* walk through other tokens */
   
   while( token != NULL ) 
   {  
      token= strtok(NULL," ");
      parse[i] = token;
   i++;
   }
  j=0;
   while(j<tam)
   {
      printf("en la palabra %s \n",parse[j] );
   j++;
   }
   j=0;
   while(j<tam)
   {
 
  
   execvp(parse[0],parse);
   
  }
}
}