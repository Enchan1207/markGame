#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "Betaengine.h"
#include "func.h"
#include "field.h"

#define VER 0.6
// #define DEBUG //デバッグ情報出力

//
//  CPU思考エンジン
//

//--prototype
int getReachstat(unsigned char map[], int type, int *rclist, int rllength);

int setCPUpos(unsigned char *map, int *cpx, int *cpy){
    /*-----
     * - 思考プロセスの順番
     * ・自分がリーチしていればそこに石を置く
     * ・プレイヤーのリーチは潰す
     * ・双方ともに何も置かれていなければ角を優先的に置く
     * ・プレイヤーが先に置いていれば至近距離の置けそうな場所に置き、リーチの流れを阻止する
    -----*/

    int tcx = 0, tcy = 0; //置く座標の一時変数(tmp cpu x,y)
    int rclist[WIDTH * 2]={0}; //ここのリーチ数はフィールドサイズによって変える(4x4なら7以上同時にリーチになることはないはず)

    //--自分とプレイヤーのリーチ状態を取得し、置けるところがあればそのまま座標を返す
    for(int type = 1; type < 3; type++){
        for(int i = 0; i<(sizeof(rclist) / sizeof(int)); i++)
            rclist[i] = -1;
        getReachstat(map, type, rclist, sizeof(rclist) / sizeof(int));

        for(int i = 0; i < (sizeof(rclist) / sizeof(int)); i++){
            getpos(rclist[i], &tcx, &tcy);
            if(placeable(map, tcx, tcy)){
                *cpx = tcx;
                *cpy = tcy;
                return 0;
            }
        }
    }

    //--それ以外はランダムで
    srand((unsigned)time(NULL));
    int cnt = 0;
    do{
        tcx = random() % WIDTH;
        tcy = random() % WIDTH;

        cnt++;
    }while(!placeable(map, tcx, tcy) && cnt<1024);

    if(cnt < 1023){
        *cpx = tcx;
        *cpy = tcy;
        return 0;
    }else{
        *cpx = -1;
        *cpy = -1;
        return 1;
    }

    return 0;
}

//--指定駒のリーチ状態を取得
int getReachstat(unsigned char map[], int type, int *rclist, int rllength){
    int _rcidx[] = {-1, -1, -1, -1}; //リーチインデックス
    int checkList[WIDTH * 2 - 1]; //チェックリスト
    int dirList[WIDTH * 2 - 1]; //チェック方向リスト(左斜め、右斜め、縦、横)
    int rlcnt = 0; //インデックスリストのカウンタ

    //--チェックリスト、方向リストを作成
    for(int i = 0; i < WIDTH; i++){
        checkList[i] = i;
        if(i == 0) dirList[i] = 0b1011;
        else if(i == WIDTH - 1) dirList[i] = 0b0110;
        else dirList[i] = 0b0010;
    }
    for(int i = 1; i < WIDTH; i++){
        checkList[i + WIDTH - 1] = i;
        dirList[i + WIDTH -1 ] = 0b0001;
    }

    //--各インデックスごとにリーチをチェック
    for(int i = 0; i < WIDTH * 2 - 1; i++){
        check(map, checkList[i], dirList[i], type, _rcidx);

        for(int j = 0; j < 4; j++){
            //--リーチしていればインデックスリストに追加
            if(_rcidx[j] != -1){
                //--インデックスが重複していなければ追加
                bool exist = false;
                for(int k = 0; k < rllength; k++)
                    if(rclist[k] == _rcidx[j]) exist = true;

                if(!exist){
                    rclist[rlcnt] = _rcidx[j];
                    rlcnt++;

                    #ifdef DEBUG
                        printf("インデックス%dをリストに追加しました。\n", _rcidx[j]);
                    #endif
                }
            }
        }
    }
    return -1;
}