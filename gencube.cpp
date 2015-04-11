#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<vector>
#include"gencube.h"
using namespace std;
void pipesInit(int (*pipes)[7]){
    pipes[nY][pX]=pipes[nX][pY]=1;
    pipes[nY][pZ]=pipes[nZ][pY]=2;
    pipes[nY][nX]=pipes[pX][pY]=3;
    pipes[nY][nZ]=pipes[pZ][pY]=4;

    pipes[nZ][pX]=pipes[nX][pZ]=5;
    pipes[nZ][nX]=pipes[pX][pZ]=6;
    pipes[pZ][nX]=pipes[pX][nZ]=7;
    pipes[pZ][pX]=pipes[nX][nZ]=8;

    pipes[pY][pX]=pipes[nX][nY]=9;
    pipes[pY][pZ]=pipes[nZ][nY]=10;
    pipes[pY][nX]=pipes[pX][nY]=11;
    pipes[pY][nZ]=pipes[pZ][nY]=12;

    pipes[pX][pX]=pipes[nX][nX]=13;
    pipes[pY][pY]=pipes[nY][nY]=14;
    pipes[pZ][pZ]=pipes[nZ][nZ]=15;
}
vector<pair<int,vector<int>>> genCube(){
    const int K=11;
    int tubes[K][K][K]={{{0}}};

    int x=rand()%K;
    int y=rand()%K;
    int z=rand()%K;

    int pipes[7][7]={0};
    pipesInit(pipes);

    int direct=randDirect();
    vector<pair<int,vector<int>>> list;
    for(int step=rand()%(K*K)+K*K/2;step>0;step--){
        int tried=2<<invert(direct);
        while(true){
            if(isChock(tried,direct)){step=-1;break;}
            int dir=randDirect();
            if(isInvert(dir,direct)){continue;}

            if(dir==pX){
                tried|=(2<<pX);
                if(x+1==K){continue;}
                if(tubes[x+1][y][z]){continue;}
                //x++;
            }
            else if(dir==pY){
                tried|=(2<<pY);
                if(y+1==K){continue;}
                if(tubes[x][y+1][z]){continue;}
                //y++;
            }
            else if(dir==pZ){
                tried|=(2<<pZ);
                if(z+1==K){continue;}
                if(tubes[x][y][z+1]){continue;}
                //z++;
            }
            else if(dir==nX){
                tried|=(2<<nX);
                if(x==0){continue;}
                if(tubes[x-1][y][z]){continue;}
                //x--;
            }
            else if(dir==nY){
                tried|=(2<<nY);
                if(y==0){continue;}
                if(tubes[x][y-1][z]){continue;}
                //y--;
            }
            else if(dir==nZ){
                tried|=(2<<nZ);
                if(z==0){continue;}
                if(tubes[x][y][z-1]){continue;}
                //z--;
            }
            tubes[x][y][z]=pipes[direct][dir];

            pair<int,vector<int>> pack;
            pack.first=tubes[x][y][z];
            pack.second=vector<int>(3);
            pack.second[0]=x-K/2;
            pack.second[1]=y-K/2;
            pack.second[2]=z-K/2;
            list.push_back(pack);

            switch(dir){
            case pX:
                x++;break;
            case pY:
                y++;break;
            case pZ:
                z++;break;
            case nX:
                x--;break;
            case nY:
                y--;break;
            case nZ:
                z--;break;
            }

            direct=dir;
            break;
        }
    }
    return list;
}
void printList(std::vector<std::pair<int,std::vector<int>>> list){
    for(int i=0;i<list.size();i++){
        printf("%d [%d][%d][%d]\n",
            list[i].first,
            list[i].second[0],
            list[i].second[1],
            list[i].second[2]);
    }
}
