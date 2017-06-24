#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char s[] = "1225225 75243  9  21";
    char temp[5];
    int inf[7];//id, p1x, p2x, bx, by, s1,s2;
    int head=1, tail=3,i;
    strncpy (temp, s, 1);
    temp[1] = '\0';
    inf[0] = atoi(temp);
    printf("%d\n", inf[0]);

    for (i=1;i<7;i++){
        strncpy (temp, s+head, 3);
        temp[4] = '\0';
        inf[i] = atoi(temp);
        printf("%d\n", inf[i]);
        head += 3;
        tail+=3;
    }
}
