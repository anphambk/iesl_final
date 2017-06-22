#include <bits/stdc++.h>
using namespace std;
const int FSIZE = 3;
const int ISIZE = 5;
const int OSIZE = ISIZE - FSIZE + 1;
double filter[FSIZE][FSIZE];
double image[ISIZE][ISIZE];
double opsum[OSIZE][OSIZE];
int main(int argc,char *argv[]){
    FILE *fin1,*fin2,*fout;
    fin1 = fopen(argv[1],"r");
    fin2 = fopen(argv[2],"r");
    fout = fopen(argv[3],"w");
    for(int i=0;i<ISIZE;i++)
        for(int j=0;j<ISIZE;j++)fscanf(fin1,"%lf",&image[i][j]);
    
    for(int i=0;i<FSIZE;i++)
        for(int j=0;j<FSIZE;j++)fscanf(fin2,"%lf",&filter[i][j]);
    
    for(int i=0;i<OSIZE;i++)
        for(int j=0;j<OSIZE;j++)
            for(int k=0;k<FSIZE;k++)
                for(int l=0;l<FSIZE;l++)
                    opsum[i][j] += image[i+k][j+l] * filter[k][l];

    for(int i=0;i<OSIZE;i++){
        for(int j=0;j<OSIZE;j++){
            fprintf(fout,"%f ",opsum[i][j]);
        }
        fprintf(fout,"\n");
    }
    return 0;
}
