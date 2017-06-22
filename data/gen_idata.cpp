#include<bits/stdc++.h>
using namespace std;
int main(int argc,char *argv[]){
    FILE *fout;
    int S = atoi(argv[1]);
    fout = fopen(argv[2],"w");
    for(int i=0;i<S;i++){
        for(int j=0;j<S;j++){
            int tmp = rand()%256;
            fprintf(fout,"%d ",tmp);
        }
        fprintf(fout,"\n");
    }
    return 0;
}
