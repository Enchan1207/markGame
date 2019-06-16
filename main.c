#include <stdio.h>
#include "func.h"
#include "Betaengine.h"
#include "field.h"

/*---
 * 
 * 4目並べ
 * 
---*/

//--
unsigned char map[SIZE]={0}; //フィールド
int cx = 0, cy = 0; //カーソル位置
int cpx = 0, cpy = 0; //CPUが石を置く位置 
int result = -1; //試合結果
int turn = 0, tcnt = 0; //ターン(0:プレイヤー 1:CPU)、ターンカウンタ

//--
int main(){
    //--先攻、後攻を決める
    setTurn(&turn);

    //--メインループ
    while(result == -1){
        drawField(map); //フィールド描画

        switch(turn){
            //--プレイヤー
            case 0:
                do{
                    cursorInput(&cx, &cy);
                    if(!placeable(map, cx, cy)) printf("その場所には置けません。\n");
                }while (!placeable(map, cx, cy));
                map[getmap(cx, cy)] = 1;
                printf("(%d,%d)に石を置きました。\n", cx, cy);
            break;

            //--CPU
            case 1:
                setCPUpos(map, &cpx, &cpy);
                map[getmap(cpx, cpy)] = 2;
                printf("CPUが(%d,%d)に石を置きました。\n", cpx, cpy);
            break;
        }

        result = checkField(map); //勝敗判定
        tcnt++;
        turn = (tcnt+1) %2; //ターンを回す
    }

    //--試合終了
    printf("試合終了!");
    char label[][20]={"プレイヤー", "CPU"};
    switch(result){
        case 1:
        case 2:
            printf("%sの勝利\n", label[result - 1]);
        break;

        case -2:
            printf("引き分け\n");
        break;
    }
    drawField(map);
    return 0;
}