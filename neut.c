#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//�z��̃C���f�b�N�X�Ƃ��Ďg�p. X+Y = Z+W = 3 �ƂȂ��Ă���.
#define X 0
#define Z 1
#define W 2
#define Y 3
//���Ɣ��Ɏg�p. ���ꂼ��̐F�̋�̍ŏ��̔ԍ���\��.
#define BL 0
#define WH 3

typedef signed char int8;
typedef unsigned long uint32;

//�X�J���Ǝv�����T���̍ۂɃ��������s�����Ȃ��悤�ɏ������T�C�Y�ɂ��Ă�����.
//int8[6][4]�� 1�����ڂ���, 2�����ڂ����W(X, Y, Z, W)
typedef struct {
	int8 stone[6][4];
	int8 nextBW;
} board_t;

//�C���X�g���N�V����(�ړ��̎w���𕶎��񂩂�xy���W�ɕϊ���������)
typedef struct {
	int8 yfrom, xfrom, yto, xto;
} inst_t;

//�����̔���(�V�F�������Ŏw��(0:��, 1:��).�����ꍇ�͍�)
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

//3�̐������A�����Ă�����1��Ԃ�, ����ȊO��0��Ԃ�.
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

//�Ֆʂ�\�����鐮��(32bit)�ɕϊ�����
uint32 Board2ID(board_t* b)
{
	uint32 res = 0;
	int idx[6];
	for (int i = 0; i < 6; i++)
		idx[i] = b->stone[i][Y] * 5 + b->stone[i][X];

	//���ꂼ��̐F�̐΂��\�[�g
	for (int i = 0; i < 2; i++)
	{
		BubbleStep(idx + 3*i    , idx + 3*i + 1);
		BubbleStep(idx + 3*i + 1, idx + 3*i + 2);
		BubbleStep(idx + 3*i    , idx + 3*i + 1);
	}

	//�C���f�b�N�X��0~24�Ȃ̂�5bit�ŕ\���ł��A�������ׂ�
	for (int i = 0; i < 6; i++)
		res |= idx[i] << (5 * i);
	
	res |= b->nextBW << 30;

	return res;
}


//1:���̏��� -1:���̏��� 0:���������Ȃ�
int WinJudge(board_t* b)
{
	for (int i = 0; i < 4; i++)
	{
		if (b->stone[0][i] == b->stone[1][i] && b->stone[1][i] == b->stone[2][i])
		{
			if (IsSuccessive(b->stone[0][3 - i], b->stone[1][3 - i], b->stone[2][3 - i], (i == X || i == Y ? 1 : 2)))	//3-i�� X<->Y, Z<->W
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

//�R�}���h(������)���C���X�g���N�V�����ɕϊ����ĕԂ�.
//������̃`�F�b�N�͂��Ă��Ȃ�.
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

	printf("%s",cmd);
}

//�C���X�g���N�V���������
//���݂̔Ֆʂ̃|�C���^��n��,����ɕύX��������
//��������͂��Ƃł��
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

//���݂̔Ֆʂƌ��ʗp�̔z���n��
//�\��̐���Ԃ�
int SearchNext(board_t* b, board_t resBoards[24], inst_t resInsts[24])
{
	int count = 0;


	for (int i = b->nextBW; i < 3 + b->nextBW; i++)
	{
		int8 ends[8];	//X+, X-, Z+, Z-, W+, W-, Y+, Y-�����ɐi�񂾎��̓������W
		//���̋�ɂԂ���Ȃ��ꍇ�̓������W. Z, W�͔񎩖��Ȍv�Z���K�v.
		ends[0] = 4;
		ends[1] = 0;
		ends[2] = (b->stone[i][W] >= 0 ? 8 - b->stone[i][W] : 8 + b->stone[i][W]);
		ends[3] = (b->stone[i][W] >= 0 ? b->stone[i][W] : -b->stone[i][W]);
		ends[4] = (b->stone[i][Z] <= 4 ? b->stone[i][Z] : 8 - b->stone[i][Z]);
		ends[5] = (b->stone[i][Z] <= 4 ? -b->stone[i][Z] : b->stone[i][Z] - 8);
		ends[6] = 4;
		ends[7] = 0;

		//���̋�ɂ�鐧��
		for (int j = 0; j < 6; j++)
		{
			if (j == i)
				continue;

			//������������Ƃ����ꂩ�̍��W���������Ƃ�����
			for (int k = 0; k < 4; k++)
			{
				if (b->stone[j][k] == b->stone[i][k])
				{
					if (b->stone[j][3 - k] > b->stone[i][3 - k])	//����+�����𐧌�
						ends[2 * (3 - k)] = Min(ends[2 * (3 - k)], b->stone[j][3 - k] - (k == X || k == Y ? 1 : 2));	//X, Y�ׂ͗�+-1, Z, W��+-2

					else
						ends[2 * (3 - k) + 1] = Max(ends[2 * (3 - k) + 1], b->stone[j][3 - k] + (k == X || k == Y ? 1 : 2));
				}
			}
		}

		//�ړ��\�Ȃ猋�ʂɒǉ�
		for (int j = 0; j < 8; j++)
		{
			if (ends[j] == b->stone[i][j / 2])	//�����悪�����W�Ɠ����Ȃ�ړ��s��
				continue;

			resInsts[count].yfrom = b->stone[i][Y];
			resInsts[count].xfrom = b->stone[i][X];

			switch (j)
			{
				//X�����ړ�
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

//depth����܂őS�T�����ď�������������
//2:���̎�ŏ����A1:�����̏����m�A-1:�����̕����m�A0:������Ȃ�
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
		int winBoardCnt = 0;//���肪�ł�����Ŏ����̏����m�ɂȂ��Ă��鐔
		for (int i = 0; i < cnt; i++)
		{
			int result = Anticipate(&nexts[i], depth - 1, maxDepth) * (-1);
			if (result == -1)//���̎�ő��肪���\��������Ȃ玩���͕���
				return -1;
			else
				winBoardCnt += result;
		}
		if (winBoardCnt == cnt)//���̎�ő��肪�ǂ��ł��Ă������̏����Ȃ炻�̎�͏����m
			return 1;
		else
			return 0;
	}
}

int main(int argc, char* args[])
{
	//�V�F�������Ŕ����ݒ�
	if (argc == 2)
		myBW = args[1][0] == '0' ? BL : WH;

	//��̔z�u=�Ֆ�
	//0-2:��, 3-5:��
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

	//�����������܂�
	board_t nexts[24];
	inst_t insts[24];
	int cnt;

	char input[5];
	while (WinJudge(&board) == 0)
	{
		PrintBoard(&board);
		printf("%8X\n", Board2ID(&board));
		if (board.nextBW == myBW)
			printf("My Turn\n");
		else
			printf("Your Turn\n");

		cnt = SearchNext(&board, nexts, insts);
		for (int i = 0; i < cnt; i++)
		{
			PrintInst(insts[i]);
			switch (Anticipate(&nexts[i], 6, 6))
			{
			case 0:
				printf("������Ȃ�\n");
				break;
			case 1:
				printf("�����m\n");
				break;
			case 2:
				printf("����\n");
				break;
			case -1:
				printf("�����m\n");
				break;
			}
			
		}

		scanf("%4s", input);
		Move(&board, Cmd2Inst(input));
	}

	PrintBoard(&board);

}