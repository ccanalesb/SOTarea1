#include <stdio.h>
#include <string.h>

int main ()
{
  char str[25];
  scanf("%s", str);
  char *p;
  
  printf ("Split \"%s\" in tokens:\n", str);
  
  p = strtok (str," ");
  printf ("%s\n", p[0]);
  // printf ("%s\n", p);
  // while (p != NULL)
  // {
  //   printf ("%s\n", p);
  //   p = strtok (NULL, " ,");
  // }
  return 0;
}
