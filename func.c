#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "func.h"
#include "field.h"

// #define DEBUG //デバッグ情報出力

//--カーソル座標入力
void cursorInput(int *x, int *y){
    char str[6] = {0};
    int tx = 0, ty = 0;
    printf("where?(x,y) >");
    fgets(str, 5, stdin);
    tx = str[0];
    ty = str[2];
    if(tx >= '0' && tx <= SIZE + 47 && ty >= '0' && tx <= SIZE + 47){
        *x = tx - 48;
        *y = ty - 48;
    }else{
        *x = 0;
        *y = 0;
    }
}
//--先攻/後攻の決定
void setTurn(int *turn){
    char label[][20]={"プレイヤー","CPU"};

    srand((unsigned)time(NULL));
    *turn = random() % 2;
    
    printf("%sが先攻です。\n", label[*turn]);
}

//--マップ座標値の取得
unsigned int getmap(int x, int y){
    return y * WIDTH + x;
}
//--マップ値を座標値に変換
void getpos(int map, int *x, int *y){
    *y = (int) (map / WIDTH);
    *x = (int) (map % WIDTH);
}

//--フィールド描画
void drawField(unsigned char *map){
    for(int y = 0; y < WIDTH; y++){
        for(int x = 0; x < WIDTH; x++){
            char cell=' ';
            switch (map[getmap(x, y)]){
                case 1:
                    cell='O';
                    break;

                case 2:
                    cell='*';
                    break;

                default:
                    cell=' ';
                    break;
            }
            printf("%c ", cell);
        }
        printf("\n");
    }
}

//--石を置けるか
int placeable(unsigned char *map, int x, int y){
    return (x >= 0 && y >= 0 && x < WIDTH && y < WIDTH && map[getmap(x,y)]==0);
}

//--フィールド判定
int checkField(unsigned char map[]){
    int result = 0; //チェック結果
    int rcidx[] = {-1, -1, -1, -1}; //リーチインデックス

    int checkList[WIDTH * 2 - 1]; //チェックリスト
    int dirList[WIDTH * 2 - 1]; //チェック方向リスト(左斜め、右斜め、縦、横)

    //--チェックリスト、方向リストを作成
    for(int i = 0; i < WIDTH; i++){
        checkList[i] = i;
        if(i == 0) dirList[i] = 0b1011;
        else if(i == WIDTH - 1) dirList[i] = 0b0110;
        else dirList[i] = 0b0010;
    }
    for(int i = 1; i < WIDTH; i++){
        checkList[i + WIDTH - 1] = i * WIDTH;
        dirList[i + WIDTH - 1] = 0b0001;
    }

    //--チェックリスト、方向リストに従ってチェックする
    for(int type = 1; type <= 2; type++){
        for(int i = 0; i < (WIDTH * 2 - 1); i++){
            result |= check(map, checkList[i], dirList[i], type, rcidx);
        }
        if(result) return type;
    }

    //--もしかして全部埋まってる?
    int tmp = 1;
    for(int i = 0; i < SIZE; i++){
        tmp *= map[i];
    }
    return -((tmp != 0) + 1); //tmpがゼロになった->まだ埋まっていない(return -1)
}

//--マス判定(調査index, 方向(縦、横、斜め), チェックする駒の種類, リーチの場合のindex)
int check(unsigned char map[], int idx, int dir, int type, int *rcidx){
    //--
    int result = 0; //結果
    int rcnt = 0, ridx = 0; //リーチカウンタ/index

    //--indexが指す座標を取得
    int ix = 0, iy = 0;
    getpos(idx, &ix, &iy);

    //--左斜め
    if(dir & 0b0100){
        result = 1;
        rcnt = 0;
        //--右上から左下に向かって舐める
        for(int j = WIDTH - 1; j >= 0; j--){
            if(map[getmap(j, WIDTH - 1 - j)] != type){
                result = 0;
                ridx = getmap(j, WIDTH - 1 - j);
                rcnt++;
            }
        }
        if(rcnt == 1) rcidx[3] = ridx;

        //--埋まった時点でreturn
        if(result == 1){
            #ifdef DEBUG
                printf("インデックス %d で左斜め方向に揃いました。チェックを中止します。\n", idx);
            #endif
            return 1;
        }
    }

    //--右斜め
    if(dir & 0b1000){
        result = 1;
        rcnt = 0;
        //--左上から右下に向かって舐める
        for(int i = 0; i < WIDTH; i++){
            if(map[getmap(i, i)] != type){
                result = 0;
                ridx = getmap(i, i);
                rcnt++;
            }
        }
        if(rcnt == 1) rcidx[2] = ridx;

        //--埋まった時点でreturn
        if(result == 1){
            #ifdef DEBUG
                printf("インデックス %d で右斜め方向に揃いました。チェックを中止します。\n", idx);
            #endif
            return 1;
        }
    }

    //--縦
    if(dir & 0b0010){
        result = 1;
        rcnt = 0;
        //--indexの座標からx軸を固定して舐める
        for(int y = 0; y < WIDTH; y++){
            if(map[getmap(ix, y)] != type){
                result = 0;
                ridx = getmap(ix, y);
                rcnt++;
            }
        }
        if(rcnt == 1) rcidx[1] = ridx;

        //--埋まった時点でreturn
        if(result == 1){
            #ifdef DEBUG
                printf("インデックス %d で縦方向に揃いました。チェックを中止します。\n", idx);
            #endif
            return 1;
        }
    }

    //--横
    if(dir & 0b0001){
        result = 1;
        rcnt = 0;
        //--indexの座標からy軸を固定して舐める
        for(int x = 0; x < WIDTH; x++){
            if(map[getmap(x, iy)] != type){
                result = 0;
                ridx = getmap(x, iy);
                rcnt++;
            }
        }
        if(rcnt == 1) rcidx[0] = ridx;

        //--埋まった時点でreturn
        if(result == 1){
            #ifdef DEBUG
                printf("インデックス %d で横方向に揃いました。チェックを中止します。\n", idx);
            #endif
            return 1;
        }
    }

    #ifdef DEBUG
        printf("インデックス %d は揃っていません。\n", idx);
    #endif
    return 0;
}