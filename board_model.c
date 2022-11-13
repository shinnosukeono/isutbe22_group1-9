#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
  /* ゲーム続行条件 */
  int situation = 1;
  
  /* 盤面 */
  int piece[6] = {1, 3, 17, 7, 21, 23};
  int piece_temp[6] = {1, 3, 17, 7, 21, 23};

  while (situation){

    /* 入力 */
    char input[5];
    scanf("%s", input);
    int conv;
    if (input[0] == 65)
      conv = 0;
    else if (input[0] == 66)
      conv = 1;
    else if (input[0] == 67)
      conv = 2;
    else if (input[0] == 68)
      conv = 3;
    else
      conv = 4;

    int input_pos = (input[1] - 49) * 5 + conv;

    int conv2;
    if (input[2] == 65)
      conv2 = 0;
    else if (input[2] == 66)
      conv2 = 1;
    else if (input[2] == 67)
      conv2 = 2;
    else if (input[2] == 68)
      conv2 = 3;
    else
      conv2 = 4;

    int	input_mov = (input[3] - 49) * 5 + conv2;

    /* 有効な指手か検証 */
    int i;
    int existence = 0;
    for (i = 3; i < 6; i++){
      if (piece[i] == input_pos){
	piece[i] = input_mov;
	existence = 1;
	break;
      }
    }
    if (existence == 0){
      situation = 0;
      printf("You Lose\n");
      break;
    }

    if (input_pos == input_mov){
      printf("You Lose\n");
      break;
    }

    int mov_ex = 0;
    int j;
    int path;
    for (j = 0; j < 8; j++){
      path = input_pos;
      int cont = 1;
      if (j == 0){
	while (cont){
	  int next_pos = path - 5;
	  if (next_pos < 0)
	    cont = 0;
	  else{
	    int count = 0;
	    for (i = 0; i < 6; i++){
	      if (piece_temp[i] == next_pos)
		count++;
	    }
	    if (count == 1)
	      cont = 0;
	  }
	  path -=  5;
	}
	if (path + 5 == input_mov){
	  mov_ex = 1;
	  break;
	}
      }
      else if (j == 1){
	while (cont){
	  int next_pos = path - 4;
	  if (next_pos < 0)
	    cont = 0;
	  else if (next_pos % 5 == 0)
	    cont = 0;
	  else{
	    int count = 0;
	    for (i = 0; i < 6; i++){
	      if (piece_temp[i] == next_pos)
		count++;
	    }
	    if (count == 1)
	      cont = 0;
	  }
	  if (cont == 0)
	    break;
	  path = path - 4;
	}
	if (path == input_mov){
	  mov_ex = 1;
	  break;
	}
      }
      else if (j == 2){
	while (cont){
	  int next_pos = path + 1;
	  if (next_pos % 5 == 0)
	    cont = 0;
	  else if (next_pos > 24)
	    cont = 0;
	  else{
	    int count = 0;
	    for (i = 0; i < 6; i++){
	      if (piece_temp[i] == next_pos)
		count++;
	    }
	    if (count == 1)
	      cont = 0;
	  }
	  if (cont == 0)
	    break;
	  path = path + 1;
	}
	if (path == input_mov){
	  mov_ex = 1;
	  break;
	}
      }
      else if (j == 3){
	while (cont){
	  int next_pos = path + 6;
	  if (next_pos > 24)
	    cont = 0;
	  else if (next_pos % 5 == 0)
	    cont = 0;
	  else{
	    int count = 0;
	    for (i = 0; i < 6; i++){
	      if (piece_temp[i] == next_pos)
		count++;
	    }
	    if (count == 1)
	      cont = 0;
	  }
	  if (cont == 0)
	    break;
	  path = path + 6;
	}
	if (path == input_mov){
	  mov_ex = 1;
	  break;
	}
      }
      else if (j == 4){
	while (cont){
	  int next_pos = path + 5;
	  if (next_pos > 24)
	    cont = 0;
	  else{
	    int count = 0;
	    for (i = 0; i < 6; i++){
	      if (piece_temp[i] == next_pos)
		count++;
	    }
	    if (count == 1)
	      cont = 0;
	  }
	  if (cont == 0)
	    break;
	  path = path + 5;
	}
	if (path == input_mov){
	  mov_ex = 1;
	  break;
	}
      }
      else if (j == 5){
	while (cont){
	  int next_pos = path + 4;
	  if (next_pos > 24)
	    cont = 0;
	  else if (next_pos % 5 == 4)
	    cont = 0;
	  else{
	    int count = 0;
	    for (i = 0; i < 6; i++){
	      if (piece_temp[i] == next_pos)
		count++;
	    }
	    if (count == 1)
	      cont = 0;
	  }
	  if (cont == 0)
	    break;
	  path = path + 4;
	}
	if (path == input_mov){
	  mov_ex = 1;
	  break;
	}
      }
      else if (j == 6){
	while (cont){
	  int next_pos = path - 1;
	  if (next_pos % 5 == 4)
	    cont = 0;
	  else if (next_pos < 0)
	    cont = 0;
	  else{
	    int count = 0;
	    for (i = 0; i < 6; i++){
	      if (piece_temp[i] == next_pos)
		count++;
	    }
	    if (count == 1)
	      cont = 0;
	  }
	  if (cont == 0)
	    break;
	  path = path - 1;
	}
	if (path == input_mov){
	  mov_ex = 1;
	  break;
	}
      }
      else{
	while (cont){
	  int next_pos = path - 6;
	  if (next_pos < 0)
	    cont = 0;
	  else if (next_pos % 5 == 4)
	    cont = 0;
	  else{
	    int count = 0;
	    for (i = 0; i < 6; i++){
	      if (piece_temp[i] == next_pos)
		count++;
	    }
	    if (count == 1)
	      cont = 0;
	  }
	  if (cont == 0)
	    break;
	  path = path - 6;
	}
	if (path == input_mov){
	  mov_ex = 1;
	  break;
	}
      }
    }

    for (i = 0; i < 6; i++)
      piece_temp[i] = piece[i];
    
    if (mov_ex == 0){
      printf("You Lose\n");
      situation = 0;
      break;
    }
    
    /* 終了条件 */
    for (i = 0; i < 3; i++){
      int sub_sum = abs(piece[0] - piece[1]) + abs(piece[1] - piece[2]) + abs(piece[2] - piece[0]);
      int middle_3 = (piece[0] + piece[1] + piece[2]) / 3;
      if (middle_3 * 3 != (piece[0]+piece[1]+piece[2]))
        break;
      int mmod5 = middle_3 % 5;
      if (piece[i] == (piece[0] + piece[1] + piece[2]) / 3  &&
          (sub_sum == 16 || sub_sum == 24)){
        if ((mmod5 == 1|| mmod5 == 2 || mmod5 == 3) && (6 <= middle_3 && middle_3 <= 18)){
          printf("You Lose\n");
          situation = 0;
          break;
	}
      }
      else if (piece[i] == (piece[0] + piece[1] + piece[2]) / 3  &&
               (sub_sum == 20)){
        if (5 <= middle_3 && middle_3 <= 19){
          printf("You Lose\n");
            situation = 0;
            break;
        }
      }
      else if (piece[i] == (piece[0] + piece[1] + piece[2]) / 3  &&
               (sub_sum == 4)){
	if (mmod5 == 1|| mmod5 == 2 || mmod5 == 3){
          printf("You Lose\n");
            situation = 0;
            break;
        }
      }
    }
    
    for (i = 3; i < 6; i++){
      int sub_sum = abs(piece[3] - piece[4]) + abs(piece[4] - piece[5]) + abs(piece[5] - piece[3]);
      int middle_3 = (piece[3] + piece[4] + piece[5]) / 3;
      if (middle_3 * 3 != (piece[3]+piece[4]+piece[5]))
        break;
      int mmod5 = middle_3 % 5;
      if (piece[i] == (piece[3] + piece[4] + piece[5]) / 3  &&
          (sub_sum == 16 || sub_sum == 24)){
	if ((mmod5 == 1|| mmod5 == 2 || mmod5 == 3) && (6 <= middle_3 && middle_3 <= 18)){ 
	  printf("You Win\n");
          situation = 0;
	  break;
	}
      }
      else if (piece[i] == (piece[3] + piece[4] + piece[5]) / 3  &&
	       (sub_sum == 20)){
	if (5 <= middle_3 && middle_3 <= 19){
	  printf("You Win\n");
            situation = 0;
            break;
	}
      }
      else if (piece[i] == (piece[3] + piece[4] + piece[5]) / 3  &&
               (sub_sum == 4)){
	if (mmod5 == 1|| mmod5 == 2 || mmod5 == 3){
          printf("You Win\n");
            situation = 0;
            break;
        }
      }
    }

    if (situation == 0)
      break;
    
    /* 移動 */
    srand((unsigned)time(NULL));

    int nonmove = 1;
    while (nonmove == 1){
    int mov_piece = rand()%3;
    
    int mov_dir = rand()%8;

    int temp_pos = piece[mov_piece];

    int num1 = temp_pos / 5 + 1;
    int alp1 = temp_pos % 5;
    char output[] = "0000";
    
    if (alp1 == 0)
      output[0] = 65;
    else if (alp1 == 1)
      output[0] = 66;
    else if (alp1 == 2)
      output[0] = 67;
    else if (alp1 == 3)
      output[0] = 68;
    else
      output[0] = 69;

    output[1] = num1 + 48;

    int stop = 1;
    
    if (mov_dir == 0){
      while (stop){
	int next_pos = temp_pos - 5;
	if (next_pos < 0)
	  stop = 0;
	else{
	  int count = 0;
	  for (i = 0; i < 6; i++){
	    if (piece[i] == next_pos)
	      count++;
	  }
	  if (count == 1)
	    stop = 0;
	}
	if (stop == 0)
	  break;
	temp_pos = temp_pos - 5;
      }
    }

    else if (mov_dir == 1){
      while (stop){
        int next_pos = temp_pos - 4;
        if (next_pos < 0)
          stop = 0;
	else if (next_pos % 5 == 0)
	  stop = 0;
        else{
          int count = 0;
          for (i = 0; i < 6; i++){
            if (piece[i] == next_pos)
              count++;
          }
          if (count == 1)
            stop = 0;
	}
	if (stop == 0)
	  break;
	temp_pos = temp_pos - 4;
      }
    }

    else if (mov_dir == 2){
      while (stop){
	int next_pos = temp_pos + 1;
	if (next_pos % 5 == 0)
          stop = 0;
	else{
          int count = 0;
          for (i = 0; i < 6; i++){
            if (piece[i] == next_pos)
              count++;
          }
          if (count == 1)
            stop = 0;
	}
        if (stop == 0)
          break;
	temp_pos = temp_pos + 1;
      }
    }

    else if (mov_dir == 3){
      while (stop){
	int next_pos = temp_pos + 6;
	if (next_pos > 24)
          stop = 0;
	else if (next_pos % 5 == 0)
	  stop = 0;
	else{
          int count = 0;
          for (i = 0; i < 6; i++){
            if (piece[i] == next_pos)
              count++;
          }
          if (count == 1)
            stop = 0;
	}
        if (stop == 0)
          break;
	temp_pos = temp_pos + 6;
      }
    }

    else if (mov_dir == 4){
      while (stop){
	int next_pos = temp_pos + 5;
	if (next_pos > 24)
          stop = 0;
	else{
          int count = 0;
          for (i = 0; i < 6; i++){
            if (piece[i] == next_pos)
              count++;
          }
          if (count == 1)
            stop = 0;
	}
        if (stop == 0)
          break;
	temp_pos = temp_pos + 5;
      }
    }

    else if (mov_dir == 5){
      while (stop){
	int next_pos = temp_pos + 4;
	if (next_pos > 24)
          stop = 0;
        else if	(next_pos % 5 == 4)
          stop = 0;
	else{
          int count = 0;
          for (i = 0; i < 6; i++){
            if (piece[i] == next_pos)
              count++;
          }
          if (count == 1)
            stop = 0;
	}
        if (stop == 0)
          break;
	temp_pos = temp_pos + 4;
      }
    }

    else if (mov_dir == 6){
      while (stop){
	int next_pos = temp_pos - 1;
	if (next_pos % 5 == 4)
          stop = 0;
	else if (next_pos < 0)
	  stop = 0;
	else{
          int count = 0;
          for (i = 0; i < 6; i++){
            if (piece[i] == next_pos)
              count++;
          }
          if (count == 1)
            stop = 0;
	}
        if (stop == 0)
          break;
	temp_pos = temp_pos - 1;
      }
    }

    else{
      while (stop){
	int next_pos = temp_pos - 6;
	if (next_pos < 0)
          stop = 0;
	else if (next_pos % 5 == 4)
	  stop = 0;
	else{
          int count = 0;
          for (i = 0; i < 6; i++){
            if (piece[i] == next_pos)
              count++;
          }
          if (count == 1)
            stop = 0;
	}
        if (stop == 0)
          break;
	temp_pos = temp_pos - 6;
      }
    }

    if (temp_pos != piece[mov_piece]){
      nonmove = 0;
    }

    /* 出力 */
    if (nonmove == 0){
    piece[mov_piece] = temp_pos;
    piece_temp[mov_piece] = temp_pos;
    
    int num2 = temp_pos / 5 + 1;
    int alp2 = temp_pos % 5;
    
    if (alp2 == 0)
      output[2] = 65;
    else if (alp2 == 1)
      output[2] = 66;
    else if (alp2 == 2)
      output[2] = 67;
    else if (alp2 == 3)
      output[2] = 68;
    else
      output[2] = 69;

    output[3] =	num2 + 48;
      
    printf("%s\n", output);

    /* 終了条件 */

    for (i = 0; i < 3; i++){
      int sub_sum = abs(piece[0] - piece[1]) + abs(piece[1] - piece[2]) + abs(piece[2] - piece[0]);
      int middle_3 = (piece[0] + piece[1] + piece[2]) / 3;
      if (middle_3 * 3 != (piece[0]+piece[1]+piece[2]))
	break;
      int mmod5 = middle_3 % 5;
      if (piece[i] == (piece[0] + piece[1] + piece[2]) / 3  &&
          (sub_sum == 16 || sub_sum == 24)){
        if ((mmod5 == 1|| mmod5 == 2 || mmod5 == 3) && (6 <= middle_3 && middle_3 <= 18)){
          printf("You Lose\n");
          situation = 0;
          break;
        }
      }
      else if (piece[i] == (piece[0] + piece[1] + piece[2]) / 3  &&
               (sub_sum == 20)){
        if (5 <= middle_3 && middle_3 <= 19){
          printf("You Lose\n");
            situation = 0;
            break;
        }
      }
      else if (piece[i] == (piece[0] + piece[1] + piece[2]) / 3  &&
               (sub_sum == 4)){
        if (mmod5 == 1|| mmod5 == 2 || mmod5 == 3){
          printf("You Lose\n");
            situation = 0;
            break;
        }
      }
    }
    
    for (i = 3; i < 6; i++){
      int sub_sum = abs(piece[3] - piece[4]) + abs(piece[4] - piece[5]) + abs(piece[5] - piece[3]);
      int middle_3 = (piece[3] + piece[4] + piece[5]) / 3;
      if (middle_3 * 3 != (piece[3]+piece[4]+piece[5]))
        break;
      int mmod5 = middle_3 % 5;
      if (piece[i] == (piece[3] + piece[4] + piece[5]) / 3  &&
          (sub_sum == 16 || sub_sum == 24)){
        if ((mmod5 == 1|| mmod5 == 2 || mmod5 == 3) && (6 <= middle_3 && middle_3 <= 18)){
          printf("You Win\n");
          situation = 0;
          break;
        }
      }
      else if (piece[i] == (piece[3] + piece[4] + piece[5]) / 3  &&
               (sub_sum == 20)){
        if (5 <= middle_3 && middle_3 <= 19){
          printf("You Win\n");
            situation = 0;
            break;
        }
      }
      else if (piece[i] == (piece[3] + piece[4] + piece[5]) / 3  &&
               (sub_sum == 4)){
        if (mmod5 == 1|| mmod5 == 2 || mmod5 == 3){
          printf("You Win\n");
            situation = 0;
            break;
        }
      }
    }
    }
    }
  }
}
