#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//配列のインデックスとして使用. X+Y = Z+W = 3 となっている.
#define X 0
#define Z 1
#define W 2
#define Y 3
//黒と白に使用. それぞれの色の駒の最初の番号を表す.
#define BL 0
#define WH 3

typedef signed char int8;
typedef unsigned long uint32;

//杞憂だと思うが探索の際にメモリが不足しないように小さいサイズにしておいた.
//int8[6][4]の 1次元目が駒, 2次元目が座標(X, Y, Z, W)
typedef struct {
	int8 stone[6][4];
	int8 nextBW;
} board_t;

//インストラクション(移動の指示を文字列からxy座標に変換したもの)
typedef struct {
	int8 yfrom, xfrom, yto, xto;
} inst_t;

//自分の白黒(シェル引数で指定(0:黒, 1:白).無い場合は黒)
int8 myBW = BL;

int Min(int a, int b)
{
	return a < b ? a : b;
}
int Max(int a, int b)
{
	return a < b ? b : a;
}

void BubbleStep(int* a, int* b)
{
	if (*a > *b)
	{
		int tmp = *a;
		*a = *b;
		*b = tmp;
	}
}

//3つの整数が連続していたら1を返し, それ以外は0を返す.
int IsSuccessive(int a, int b, int c, int interval)
{
	BubbleStep(&a, &b);
	BubbleStep(&b, &c);
	BubbleStep(&a, &b);

	if (b == a + interval && c == b + interval)
		return 1;
	else
		return 0;
}

//盤面を表現する整数(32bit)に変換する
uint32 Board2ID(board_t* b)
{
	uint32 res = 0;
	int idx[6];
	for (int i = 0; i < 6; i++)
		idx[i] = b->stone[i][Y] * 5 + b->stone[i][X];

	//それぞれの色の石をソート
	for (int i = 0; i < 2; i++)
	{
		BubbleStep(idx + 3*i    , idx + 3*i + 1);
		BubbleStep(idx + 3*i + 1, idx + 3*i + 2);
		BubbleStep(idx + 3*i    , idx + 3*i + 1);
	}

	//インデックスは0~24なので5bitで表現でき、それを並べる
	for (int i = 0; i < 6; i++)
		res |= idx[i] << (5 * i);
	
	res |= b->nextBW << 30;

	return res;
}


//1:黒の勝ち -1:白の勝ち 0:勝ち負けなし
int WinJudge(board_t* b)
{
	for (int i = 0; i < 4; i++)
	{
		if (b->stone[0][i] == b->stone[1][i] && b->stone[1][i] == b->stone[2][i])
		{
			if (IsSuccessive(b->stone[0][3 - i], b->stone[1][3 - i], b->stone[2][3 - i], (i == X || i == Y ? 1 : 2)))	//3-iで X<->Y, Z<->W
				return 1;
		}
		if (b->stone[3][i] == b->stone[4][i] && b->stone[4][i] == b->stone[5][i])
		{
			if (IsSuccessive(b->stone[3][3 - i], b->stone[4][3 - i], b->stone[5][3 - i], (i == X || i == Y ? 1 : 2)))
				return -1;
		}
	}
	return 0;
}

void PrintBoard(board_t* b)
{
	char strings[5][5];
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			strings[i][j] = ' ';

	for (int i = 0; i < 6; i++)
		strings[b->stone[i][Y]][b->stone[i][X]] = (i < 3 ? 'x' : 'o');

	printf("\n  A B C D E \n");
	for (int i = 0; i < 5; i++)
	{
		printf("%d ", 5 - i);
		for (int j = 0; j < 5; j++)
		{
			printf("%c ", strings[i][j]);
		}
		printf("\n");
	}
}

//コマンド(文字列)をインストラクションに変換して返す.
//文字列のチェックはしていない.
inst_t Cmd2Inst(char* cmd)
{
	inst_t res;
	res.yfrom = '5' - cmd[0];
	res.xfrom = cmd[1] - 'A';
	res.yto = '5' - cmd[2];
	res.xto = cmd[3] - 'A';

	return res;
}

void PrintInst(inst_t inst)
{
	char cmd[5];
	cmd[0] = '5' - inst.yfrom;
	cmd[1] = 'A' + inst.xfrom;
	cmd[2] = '5' - inst.yto;
	cmd[3] = 'A' + inst.xto;
	cmd[4] = '\0';

	printf("%s\n",cmd);
}

//インストラクションを入力
//現在の盤面のポインタを渡し,それに変更を加える
//反則判定はあとでやる
int Move(board_t* b, inst_t inst)
{
	for (int i = 0; i < 6; i++)
	{
		if (b->stone[i][X] == inst.xfrom && b->stone[i][Y] == inst.yfrom)
		{
			b->stone[i][X] = inst.xto;
			b->stone[i][Y] = inst.yto;
			b->stone[i][Z] = b->stone[i][X] + b->stone[i][Y];
			b->stone[i][W] = b->stone[i][X] - b->stone[i][Y];

			b->nextBW = WH - b->nextBW;
			return 0;
		}
	}
	return 1;
}

//現在の盤面と結果用の配列を渡す
//可能手の数を返す
int SearchNext(board_t* b, board_t resBoards[24], inst_t resInsts[24])
{
	int count = 0;


	for (int i = b->nextBW; i < 3 + b->nextBW; i++)
	{
		int8 ends[8];	//X+, X-, Z+, Z-, W+, W-, Y+, Y-方向に進んだ時の到着座標
		//他の駒にぶつからない場合の到着座標. Z, Wは非自明な計算が必要.
		ends[0] = 4;
		ends[1] = 0;
		ends[2] = (b->stone[i][W] >= 0 ? 8 - b->stone[i][W] : 8 + b->stone[i][W]);
		ends[3] = (b->stone[i][W] >= 0 ? b->stone[i][W] : -b->stone[i][W]);
		ends[4] = (b->stone[i][Z] <= 4 ? b->stone[i][Z] : 8 - b->stone[i][Z]);
		ends[5] = (b->stone[i][Z] <= 4 ? -b->stone[i][Z] : b->stone[i][Z] - 8);
		ends[6] = 4;
		ends[7] = 0;

		//他の駒による制限
		for (int j = 0; j < 6; j++)
		{
			if (j == i)
				continue;

			//動かしたい駒といずれかの座標が等しいとき制限
			for (int k = 0; k < 4; k++)
			{
				if (b->stone[j][k] == b->stone[i][k])
				{
					if (b->stone[j][3 - k] > b->stone[i][3 - k])	//直交+方向を制限
						ends[2 * (3 - k)] = Min(ends[2 * (3 - k)], b->stone[j][3 - k] - (k == X || k == Y ? 1 : 2));	//X, Yは隣が+-1, Z, Wは+-2

					else
						ends[2 * (3 - k) + 1] = Max(ends[2 * (3 - k) + 1], b->stone[j][3 - k] + (k == X || k == Y ? 1 : 2));
				}
			}
		}

		//移動可能なら結果に追加
		for (int j = 0; j < 8; j++)
		{
			if (ends[j] == b->stone[i][j / 2])	//到着先が元座標と同じなら移動不可
				continue;

			resInsts[count].yfrom = b->stone[i][Y];
			resInsts[count].xfrom = b->stone[i][X];

			switch (j)
			{
				//X方向移動
			case 0:
			case 1:
				resInsts[count].yto = resInsts[count].yfrom;
				resInsts[count].xto = ends[j];
				break;

				//Z
			case 2:
			case 3:
				resInsts[count].yto = (ends[j] - b->stone[i][W]) / 2;
				resInsts[count].xto = (ends[j] + b->stone[i][W]) / 2;
				break;

				//W
			case 4:
			case 5:
				resInsts[count].yto = (b->stone[i][Z] - ends[j]) / 2;
				resInsts[count].xto = (b->stone[i][Z] + ends[j]) / 2;
				break;

				//Y
			case 6:
			case 7:
				resInsts[count].yto = ends[j];
				resInsts[count].xto = resInsts[count].xfrom;
				break;
			}

			resBoards[count] = *b;
			Move(&resBoards[count], resInsts[count]);
			count++;
		}
	}

	return count;
}

//depth分先まで全探索して勝ち負けを見る
//2:その手で勝ち、1:自分の勝ち確、-1:自分の負け確、0:分からない
int Anticipate(board_t* b, int depth, int maxDepth)
{
	if (depth == 0)
	{
		return 0;
	}
	int judge = WinJudge(b);

	if (judge != 0)
	{
		return depth == maxDepth ? 2 : 1;
	}
	else
	{
		board_t nexts[24];
		inst_t insts[24];
		int cnt = SearchNext(b, nexts, insts);
		int winBoardCnt = 0;//相手が打った後で自分の勝ち確になっている数
		for (int i = 0; i < cnt; i++)
		{
			int result = Anticipate(&nexts[i], depth - 1, maxDepth) * (-1);
			if (result == -1)//次の手で相手が勝つ可能性があるなら自分は負け
				return -1;
			else
				winBoardCnt += result;
		}
		if (winBoardCnt == cnt)//次の手で相手がどう打っても自分の勝ちならその手は勝ち確
			return 1;
		else
			return 0;
	}
}

//盤面を与えると一つの数値を返す評価関数
//とりあえず三つの駒のばらつきを返してます
int EvaluationFanction(board_t* b)
{
	int checkBW = 3 - b->nextBW;
	int stone1x = b->stone[checkBW][X];
	int stone1y = b->stone[checkBW][Y];
	int stone2x = b->stone[checkBW + 1][X];
	int stone2y = b->stone[checkBW + 1][Y];
	int stone3x = b->stone[checkBW + 2][X];
	int stone3y = b->stone[checkBW + 2][Y];

	return stone1x*stone1x + stone2x*stone2x + stone3x*stone3x - stone1x*stone2x - stone2x*stone3x - stone1x*stone3x + stone1y*stone1y + stone2y*stone2y + stone3y*stone3y - stone1y*stone2y - stone2y*stone3y - stone1y*stone3y;
}

//次の手を一つ決めて返す関数
inst_t DecideNext(board_t* b)
{
	board_t nexts[24];
	inst_t insts[24];
	int cnt = SearchNext(b, nexts, insts);
	inst_t retInst = insts[0];//全部負け確だった時用に適当な値入れとく
	int minValue = 999; //とりあえずでかい値で初期化
	for (int i = 0; i < cnt; i++)
	{
		switch (Anticipate(&nexts[i], 6, 6))
		{
		case 0://分からない時は評価関数で評価した値が小さいものを選ぶ
			if(EvaluationFanction(&nexts[i]) < minValue)
			{
				retInst = insts[i];
				minValue = EvaluationFanction(&nexts[i]);
			}
			break;
		case 1://勝ち確の時はそれを返す。ただしこのターンで自分が勝てる可能性があるので探索はやめない
			retInst = insts[i];
			minValue = -1;
			break;
		case 2://この一手で勝てる時は探索もやめてその手を返す
			return insts[i];
			break;
		case -1://負け確は選ばない
			break;
		}
	}

	return retInst;
}

//反則手の判定
//現在の盤面から打てる手を全て調べてそのどれかと一致しているか調べる
int FoulJudge(board_t* b, inst_t inst)
{
	board_t nexts[24];
	inst_t insts[24];
	int cnt = SearchNext(b, nexts, insts);
	for (int i = 0; i < cnt; i++)
	{
		if((insts[i].xto == inst.xto) && (insts[i].yto == inst.yto) && (insts[i].xfrom == inst.xfrom) && (insts[i].yfrom == inst.yfrom))
		{
			return 0;
		}
	}
	return 1;
}

int main(int argc, char* args[])
{
	//シェル引数で白黒設定
	if (argc == 2)
		myBW = args[1][0] == '0' ? BL : WH;

	//駒の配置=盤面
	//0-2:黒, 3-5:白
	board_t board;

	int8 initial[6][4] = {
		{2, 3, 1, 1},
		{1, 5, -3, 4},
		{3, 7, -1, 4},
		{1, 1, 1, 0},
		{3, 3, 3, 0},
		{2, 5, -1, 3}
	};

	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 4; j++)
			board.stone[i][j] = initial[i][j];

	board.nextBW = BL;

	//初期化ここまで

	char input[5];
	int foul = 0; //反則手の判定、1:反則手
	while ((WinJudge(&board) == 0) && (foul == 0))
	{
		//PrintBoard(&board);
		//printf("%8lX\n", Board2ID(&board));
		if (board.nextBW == myBW)
		{
			//printf("My Turn\n");
			inst_t inst = DecideNext(&board);
			PrintInst(inst);
			Move(&board, inst);
		}
		else{
			//printf("Your Turn\n");
			scanf("%4s", input);
			inst_t inst = Cmd2Inst(input);
			foul = FoulJudge(&board, inst);
			Move(&board, inst);
		}
	}

	//PrintBoard(&board);
	if(foul == 1)
	{
		printf("You Lose\n");
	}
	else if(((WinJudge(&board) == 1) && (myBW == BL)) || ((WinJudge(&board) == -1) && (myBW == WH)))
	{
		printf("You Lose\n");
	}
	else
	{
		printf("You Win\n");
	}

}