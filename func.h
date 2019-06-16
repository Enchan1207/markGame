#ifndef FUNC_H
#define FUNC_H

//--prototype
void drawField(unsigned char *map); //フィールド描画

unsigned int getmap(int x, int y); //座標値取得
void getpos(int map, int *x, int *y); //マップ値を座標値に変換

void setTurn(int *turn); //先攻/後攻を決める
void cursorInput(int *x, int *y); //カーソル座標入力

int placeable(unsigned char *map, int x, int y); //その場所に置けるか?
int checkField(unsigned char map[]); //フィールド判定
int check(unsigned char map[], int idx, int dir, int type, int *rcidx); //マス判定

#endif //FUNC_H