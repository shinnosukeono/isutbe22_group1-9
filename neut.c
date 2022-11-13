#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//�z��̃C���f�b�N�X�Ƃ��Ďg�p. 3����������Ƃ� X+Y = Z+W = 3 �ƂȂ��Ă���.
#define X 0
#define Z 1
#define W 2
#define Y 3
//���Ɣ��Ɏg�p. ���ꂼ��̐F�̋�̍ŏ��̔ԍ���\��.
#define BL 0
#define WH 3

typedef signed char int8;

//�X�J���Ǝv�����T���̍ۂɃ��������s�����Ȃ��悤�ɏ������T�C�Y�ɂ��Ă�����.
//int8[6][4]�� 1�����ڂ���, 2�����ڂ����W(X, Y, Z, W)
typedef struct {
	int8 stone[6][4];
	int8 nextBW;
} board_t;

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

	if(b == a + interval && c == a + 2 * interval)
		return 1;
	else
		return 0;
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

//"4C4E"�̂悤�ȕ���������
//�V�����Ֆʂ��쐬���ă|�C���^��Ԃ�. �G���[�̂Ƃ���NULL��Ԃ�.
//��̈ړ��\����͍��̂Ƃ��낵�Ă��Ȃ�
board_t* Move(board_t* b, char* cmd)
{
	if (strlen(cmd) != 4) return NULL;

	if (!('1' <= cmd[0] && cmd[0] <= '5')) return NULL;
	int yfrom = 4 - (cmd[0] - '1');
	
	if (!('A' <= cmd[1] && cmd[1] <= 'E')) return NULL;
	int xfrom = cmd[1] - 'A';
	
	if (!('1' <= cmd[2] && cmd[2] <= '5')) return NULL;
	int yto = 4 - (cmd[2] - '1');
	
	if (!('A' <= cmd[3] && cmd[3] <= 'E')) return NULL;
	int xto = cmd[3] - 'A';

	board_t* nextBoard = (board_t*) malloc(sizeof(board_t));
	*nextBoard = *b;

	for (int i = 0; i < 6; i++)
	{
		if (nextBoard->stone[i][X] == xfrom && nextBoard->stone[i][Y] == yfrom)
		{
			nextBoard->stone[i][X] = xto;
			nextBoard->stone[i][Y] = yto;
			nextBoard->stone[i][Z] = nextBoard->stone[i][X] + nextBoard->stone[i][Y];
			nextBoard->stone[i][W] = nextBoard->stone[i][X] - nextBoard->stone[i][Y];

			nextBoard->nextBW = WH - b->nextBW;
			return nextBoard;
  		}
	}
	return NULL;
}

//���݂̔Ֆʂƌ��ʗp�̔z���n��
//�\��̐���Ԃ�
int SearchNext(board_t* b, board_t* resBoards[], char resCmds[][5])
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

			resCmds[count][0] = '5' - b->stone[i][Y];
			resCmds[count][1] = 'A' + b->stone[i][X];
			
			switch (j)
			{
				//X�����ړ�
			case 0:
			case 1:
				resCmds[count][2] = resCmds[count][0];
				resCmds[count][3] = 'A' + ends[j];
				break;

				//Z
			case 2:
			case 3:
				resCmds[count][2] = '5' - (ends[j] - b->stone[i][W]) / 2;
				resCmds[count][3] = 'A' + (ends[j] + b->stone[i][W]) / 2;
				break;

				//W
			case 4:
			case 5:
				resCmds[count][2] = '5' - (b->stone[i][Z] - ends[j]) / 2;
				resCmds[count][3] = 'A' + (b->stone[i][Z] + ends[j]) / 2;
				break;

				//Y
			case 6:
			case 7:
				resCmds[count][2] = '5' - ends[j];
				resCmds[count][3] = resCmds[count][1];
				break;
			}

			resCmds[count][4] = '\0';
			resBoards[count] = Move(b, resCmds[count]);
			count++;
		}
	}

	return count;
}

int main(int argc, char* args[])
{
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
	board_t* nexts[24];
	char cmds[24][5];
	int cnt;

	char input[5];
	while (WinJudge(&board) == 0)
	{
		PrintBoard(&board);
		cnt = SearchNext(&board, nexts, cmds);
		for (int i = 0; i < cnt; i++)
		{
			printf("%s\n", cmds[i]);
		}

		scanf("%4s", input);
		board = *Move(&board, input);
	}

	PrintBoard(&board);
	
}