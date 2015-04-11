#ifndef DEF_GENCUBE
#define DEF_GENCUBE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<vector>
enum directEnum{
    pX=1,pY=2,pZ=3,nX=6,nY=5,nZ=4
};
inline int invert(int dir){
    return 7-dir;
}
inline bool isInvert(int a,int b){
    return a+b==7;
}
inline bool isChock(int tried,int direct){
    return tried==(
        (2<<pX)|(2<<pY)|(2<<pZ)|
        (2<<nX)|(2<<nY)|(2<<nZ));
}
inline int randDirect(){
    return (rand()%2)?(rand()%3+1):(rand()%3+4);
}
void pipesInit(int (*pipes)[7]);
std::vector<std::pair<int,std::vector<int>>> genCube();
void printList(std::vector<std::pair<int,std::vector<int>>> list);
#endif // DEF_GENCUBE
