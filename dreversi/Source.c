#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <windows.h>

#define BOARD_SIZE		8
#define PARCEL_SIZE		100
#define BOARD_WIDTH (BOARD_SIZE * PARCEL_SIZE)
#define BOARD_HEIGHT (BOARD_SIZE * PARCEL_SIZE)

ALLEGRO_FONT* font;

int make_move(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char player);
int valid_moves(char board[BOARD_SIZE][BOARD_SIZE], char moves[BOARD_SIZE][BOARD_SIZE], char player);
void computer_move(char board[BOARD_SIZE][BOARD_SIZE], char player);
int best_move(char board[BOARD_SIZE][BOARD_SIZE], char moves[BOARD_SIZE][BOARD_SIZE], char player);
int get_score(char board[BOARD_SIZE][BOARD_SIZE], char player);
int make_move2(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char player);
int counter(char board[BOARD_SIZE][BOARD_SIZE]);

int main()
{
	int i, j, ctrl;
	char tahta[BOARD_SIZE][BOARD_SIZE];
	int a, b, gotoagain = 0, kimde, pos_x, pos_y;

	al_init();

	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_mouse();
	al_install_keyboard();

	ALLEGRO_EVENT ev2;
	ALLEGRO_BITMAP* background = al_load_bitmap("othello-board.bmp");
	ALLEGRO_BITMAP* menu = al_load_bitmap("menu.bmp");
	font = al_load_font("black.ttf", 36, NULL);

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_DISPLAY* display = al_create_display(BOARD_WIDTH + 250, BOARD_HEIGHT);
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_keyboard_event_source());

	while (1)
	{
		if (gotoagain == -1) break;

		kimde = 1;
		gotoagain = 0;

		for (a = 0; a < BOARD_SIZE; a++) {
			for (b = 0; b < BOARD_SIZE; b++) {
				tahta[a][b] = 'n';
			}
		}

		tahta[(BOARD_SIZE - 1) / 2][(BOARD_SIZE - 1) / 2] = 's';
		tahta[(BOARD_SIZE - 1) / 2][(BOARD_SIZE - 1) / 2 + 1] = 'b';
		tahta[(BOARD_SIZE - 1) / 2 + 1][(BOARD_SIZE - 1) / 2] = 'b';
		tahta[(BOARD_SIZE - 1) / 2 + 1][(BOARD_SIZE - 1) / 2 + 1] = 's';

		al_draw_bitmap(menu, 0, 0, NULL);
		al_flip_display();
		al_wait_for_event(queue, &ev2);

		if (ev2.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			break;
		}
		else if (ev2.keyboard.keycode == ALLEGRO_KEY_SPACE)
		{
			al_draw_bitmap(background, 0, 0, NULL);

			al_draw_filled_circle((BOARD_SIZE - 1) * PARCEL_SIZE / 2, (BOARD_SIZE - 1) * PARCEL_SIZE / 2, 35, al_map_rgb(0, 0, 0));
			al_draw_filled_circle((BOARD_SIZE - 1) * PARCEL_SIZE / 2 + PARCEL_SIZE, (BOARD_SIZE - 1) * PARCEL_SIZE / 2 + PARCEL_SIZE, 35, al_map_rgb(0, 0, 0));
			al_draw_filled_circle((BOARD_SIZE - 1) * PARCEL_SIZE / 2 + PARCEL_SIZE, (BOARD_SIZE - 1) * PARCEL_SIZE / 2, 35, al_map_rgb(255, 255, 255));
			al_draw_filled_circle((BOARD_SIZE - 1) * PARCEL_SIZE / 2, (BOARD_SIZE - 1) * PARCEL_SIZE / 2 + PARCEL_SIZE, 35, al_map_rgb(255, 255, 255));

			al_flip_display();

			while (gotoagain == 0)
			{
				int counter_black = 0;

				ctrl = 1;

				for (a = 0; a < BOARD_SIZE; a++)
					for (b = 0; b < BOARD_SIZE; b++)
						if (tahta[a][b] == 'n')
						{
							ctrl = 0;
							break;
						}
						else if (tahta[a][b] == 's')
						{
							counter_black++;
						}

				if (ctrl)
				{
					if (counter_black > BOARD_SIZE * BOARD_SIZE / 2)
					{
						al_draw_text(font, al_map_rgb(100, 100, 100), BOARD_WIDTH, 200, NULL, "YOU WIN!");
						al_flip_display();
						Sleep(3000);
						al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH, 200, NULL, "YOU WIN!");
						al_flip_display();
						
					}
					else if (counter_black == BOARD_SIZE * BOARD_SIZE / 2)
					{
						al_draw_text(font, al_map_rgb(100, 100, 100), BOARD_WIDTH, 200, NULL, "DEUCE!");
						al_flip_display();
						Sleep(3000);
						al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH, 200, NULL, "DEUCE!");
						al_flip_display();
						
					}
					else
					{
						al_draw_text(font, al_map_rgb(100, 100, 100), BOARD_WIDTH, 200, NULL, "GAME OVER:(");
						al_flip_display();
						Sleep(3000);
						al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH, 200, NULL, "GAME OVER:(");
						al_flip_display();
						Sleep(3000);
						
					}
					 

					gotoagain = 1;
					counter(tahta);
					break;
				}
				else if (kimde) 
				{
					if (player_pass(tahta) != EXIT_SUCCESS)
					{
						al_draw_text(font, al_map_rgb(36, 100, 100), BOARD_WIDTH, 400, NULL, "PASS!");
						al_flip_display();
						Sleep(3000);
						al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH, 400, NULL, "PASS!");
						al_flip_display();
						kimde = !kimde;
					}
					else
					{
						ALLEGRO_EVENT ev;
						
						al_wait_for_event(queue, &ev);

						if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
						{
							gotoagain = -1;
							break;
						}
						else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
						{
							pos_x = ev.mouse.x;
							pos_y = ev.mouse.y;

							if (ev.mouse.button & 2)
							{
								gotoagain = 1;
							}
							else if (ev.mouse.button & 1)
							{
								i = pos_x / PARCEL_SIZE;
								j = pos_y / PARCEL_SIZE;

								if (make_move(tahta, i, j, 's') == EXIT_SUCCESS)
								{
									kimde = !kimde;
								}
							}
						}
					}
				}
				else
				{
					Sleep(1000);
					computer_move(tahta, 'b');
					kimde = !kimde;
				}
			}
		}
	}

	al_destroy_bitmap(background);
	al_destroy_bitmap(menu);
	al_destroy_display(display);

	return 0;
}

int make_move(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char player)
{
	int rowdelta = 0;                   /* Row increment              */
	int coldelta = 0;                   /* Column increment           */
	int xm = 0;                          /* Row index for searching    */
	int ym = 0;                          /* Column index for searching */
	char opponent = (player == 's') ? 'b' : 's';  /* Identify opponent */
	int retVal = EXIT_FAILURE;
	int t_row = row, t_col = col;
	/* Check all the squares around this square */
	/* for the opponents counter                */
	for (rowdelta = -1; rowdelta <= 1; rowdelta++)
		for (coldelta = -1; coldelta <= 1; coldelta++)
		{
			/* Don't check off the board, or the current square */
			if (row + rowdelta < 0 || row + rowdelta >= BOARD_SIZE ||
				col + coldelta < 0 || col + coldelta >= BOARD_SIZE ||
				(rowdelta == 0 && coldelta == 0))
				continue;

			/* Now check the square */
			if (board[row + rowdelta][col + coldelta] == opponent)
			{
				/* If we find the opponent, search in the same direction */
				/* for a player counter                                  */
				xm = row + rowdelta;        /* Move to opponent */
				ym = col + coldelta;        /* square           */

				for (;;)
				{
					xm += rowdelta;           /* Move to the      */
					ym += coldelta;           /* next square      */

					/* If we are off the board give up */
					if (xm < 0 || xm >= BOARD_SIZE || ym < 0 || ym >= BOARD_SIZE)
						break;

					/* If the square is blank give up */
					if (board[xm][ym] == 'n')
						break;

					/* If we find the player counter, go backwards from here */
					/* changing all the opponents counters to player         */
					if (board[xm][ym] == player)
					{
						while (board[xm -= rowdelta][ym -= coldelta] == opponent) { /* Opponent? */
							board[xm][ym] = player;
							if (opponent == 'b') {
								al_draw_filled_circle(PARCEL_SIZE * xm + 50, PARCEL_SIZE * ym + 50, 35, al_map_rgb(0, 0, 0));
								al_flip_display();

							}
							if (opponent == 's') {
								al_draw_filled_circle(PARCEL_SIZE * xm + 50, PARCEL_SIZE * ym + 50, 35, al_map_rgb(250, 250, 250));
								al_flip_display();
							}
							retVal = EXIT_SUCCESS;
						} /* Yes, change it */
						if (retVal == EXIT_SUCCESS)
						{
							board[t_row][t_col] = player;           /* Place the player counter   */
							if (player == 's') {
								al_draw_filled_circle(PARCEL_SIZE * xm + 50, PARCEL_SIZE * ym + 50, 35, al_map_rgb(0, 0, 0));
								al_flip_display();

							}
							if (player == 'b') {
								al_draw_filled_circle(PARCEL_SIZE * xm + 50, PARCEL_SIZE * ym + 50, 35, al_map_rgb(250, 250, 250));
								al_flip_display();
							}
						}
						break;                     /* We are done    */
					}
				}
			}
		}

	return retVal;
}

int player_pass(char board[BOARD_SIZE][BOARD_SIZE])
{
	int row, col, im, jm;
	int retVal = EXIT_FAILURE;
	char temp_board[BOARD_SIZE][BOARD_SIZE];

	for (row = 0; row < BOARD_SIZE; row++)
		for (col = 0; col < BOARD_SIZE; col++)
		{
			if (board[row][col] != 'n')
				continue;

			/* First make copies of the board and moves arrays */
			for (im = 0; im < BOARD_SIZE; im++)
				for (jm = 0; jm < BOARD_SIZE; jm++)
					temp_board[im][jm] = board[im][jm];

			/* Now make this move on the temporary board */
			if (make_move2(temp_board, row, col, 's') == EXIT_FAILURE)
				continue;
			retVal = EXIT_SUCCESS;
		}
	return retVal;
}

void computer_move(char board[BOARD_SIZE][BOARD_SIZE], char player)
{
	int row = 0;
	int col = 0;
	int best_row = -1;
	int best_col = -1;
	int im = 0;
	int jm = 0;
	int new_score = 0;
	int score = 100;
	char temp_board[BOARD_SIZE][BOARD_SIZE];
	int temp_moves[BOARD_SIZE][BOARD_SIZE];
	char opponent = (player == 's') ? 'b' : 's';

	for (row = 0; row < BOARD_SIZE; row++)
		for (col = 0; col < BOARD_SIZE; col++)
		{
			if (board[row][col] != 'n')
				continue;

			/* First make copies of the board and moves arrays */
			for (im = 0; im < BOARD_SIZE; im++)
				for (jm = 0; jm < BOARD_SIZE; jm++)
					temp_board[im][jm] = board[im][jm];

			/* Now make this move on the temporary board */
			if (make_move2(temp_board, row, col, player) == EXIT_FAILURE)
				continue;

			/* find valid moves for the opponent after this move */
			valid_moves(temp_board, temp_moves, opponent);

			/* Now find the score for the opponents best move */
			new_score = best_move(temp_board, temp_moves, opponent);

			if (new_score < score)    /* Is it worse?           */
			{                      /* Yes, so save this move */
				score = new_score;   /* Record new lowest opponent score */
				best_row = row;  /* Record best move row             */
				best_col = col;  /* and column    */
			}
		}

	if (best_row == -1 || best_col == -1)
	{
		al_draw_text(font, al_map_rgb(45, 15, 10), BOARD_WIDTH, 400, NULL, "PASS!");
		al_flip_display();
		Sleep(3000);
		al_draw_text(font, al_map_rgb(0, 0, 0), BOARD_WIDTH, 400, NULL, "PASS!");
		al_flip_display();
		return;
	}

	al_draw_filled_circle(PARCEL_SIZE * best_row + 50, PARCEL_SIZE * best_col + 50, 35, al_map_rgb(250, 250, 250));
	al_flip_display();
	make_move(board, best_row, best_col, 'b');
}
int valid_moves(char board[BOARD_SIZE][BOARD_SIZE], char moves[BOARD_SIZE][BOARD_SIZE], char player)
{
	int rowdelta = 0;     /* Row increment around a square    */
	int coldelta = 0;     /* Column increment around a square */
	int row = 0;          /* Row index                        */
	int col = 0;          /* Column index                     */
	int xz = 0;            /* Row index when searching         */
	int yz = 0;            /* Column index when searching      */
	int no_of_moves = 0;  /* Number of valid moves            */

	/* Set the opponent            */
	char opponent = (player == 's') ? 'b' : 's';

	/* Initialize moves array to zero */
	for (row = 0; row < BOARD_SIZE; row++)
		for (col = 0; col < BOARD_SIZE; col++)
			moves[row][col] = 0;

	/* Find squares for valid moves.                           */
	/* A valid move must be on a blank square and must enclose */
	/* at least one opponent square between two player squares */
	for (row = 0; row < BOARD_SIZE; row++)
		for (col = 0; col < BOARD_SIZE; col++)
		{
			if (board[row][col] != 'n')   /* Is it a blank square?  */
				continue;                  /* No - so on to the next */

			  /* Check all the squares around the blank square  */
			  /* for the opponents counter                      */
			for (rowdelta = -1; rowdelta <= 1; rowdelta++)
				for (coldelta = -1; coldelta <= 1; coldelta++)
				{
					/* Don't check outside the array, or the current square */
					if (row + rowdelta < 0 || row + rowdelta >= BOARD_SIZE ||
						col + coldelta < 0 || col + coldelta >= BOARD_SIZE ||
						(rowdelta == 0 && coldelta == 0))
						continue;

					/* Now check the square */
					if (board[row + rowdelta][col + coldelta] == opponent)
					{
						/* If we find the opponent, move in the delta direction  */
						/* over opponent counters searching for a player counter */
						xz = row + rowdelta;                /* Move to          */
						yz = col + coldelta;                /* opponent square  */

						/* Look for a player square in the delta direction */
						for (;;)
						{
							xz += rowdelta;                  /* Go to next square */
							yz += coldelta;                  /* in delta direction*/

							/* If we move outside the array, give up */
							if (xz < 0 || xz >= BOARD_SIZE || yz < 0 || yz >= BOARD_SIZE)
								break;

							/* If we find a blank square, give up */
							if (board[xz][yz] == 'n')
								break;
							/*  If the square has a player counter */
							/*  then we have a valid move          */
							if (board[xz][yz] == player)
							{
								moves[row][col] = 1;   /* Mark as valid */
								no_of_moves++;         /* Increase valid moves count */
								break;                 /* Go check another square    */
							}
						}
					}
				}
		}
	return no_of_moves;
}
int best_move(char board[BOARD_SIZE][BOARD_SIZE], char moves[BOARD_SIZE][BOARD_SIZE], char player)
{
	int row = 0;     /* Row index    */
	int col = 0;     /* Column index */
	int ib = 0;       /* Loop index   */
	int jb = 0;       /* Loop index   */

	char opponent = (player == 's') ? 'b' : 's'; /* Identify opponent */

	char new_board[BOARD_SIZE][BOARD_SIZE] = { 0 };  /* Local copy of board    */
	int score = 0;                       /* Best score             */
	int new_score = 0;                   /* Score for current move */

	/* Check all valid moves to find the best */
	for (row = 0; row < BOARD_SIZE; row++)
		for (col = 0; col < BOARD_SIZE; col++)
		{
			if (board[row][col] != 'n')
				continue;

			if (!moves[row][col])             /* Not a valid move?      */
				continue;                      /* Go to the next         */

			  /* Copy the board */
			for (ib = 0; ib < BOARD_SIZE; ib++)
				for (jb = 0; jb < BOARD_SIZE; jb++)
					new_board[ib][jb] = board[ib][jb];

			/* Make move on the board copy */
			make_move2(new_board, row, col, player);

			/* Get score for move */
			new_score = get_score(new_board, player);

			if (score < new_score)         /* Is it better?               */
				score = new_score;  /* Yes, save it as best score  */
		}
	return score;                   /* Return best score           */
}
int get_score(char board[BOARD_SIZE][BOARD_SIZE], char player)
{
	int score = 0;      /* Score for current position */
	int row = 0;        /* Row index                  */
	int col = 0;        /* Column index               */
	char opponent = player == 's' ? 'b' : 's';  /* Identify opponent */

	/* Check all board squares */
	for (row = 0; row < BOARD_SIZE; row++)
		for (col = 0; col < BOARD_SIZE; col++)
		{
			score -= board[row][col] == opponent; /* Decrement for opponent */
			score += board[row][col] == player;   /* Increment for player   */
		}
	return score;
}
int make_move2(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char player)
{
	int rowdelta = 0;                   /* Row increment              */
	int coldelta = 0;                   /* Column increment           */
	int xq = 0;                          /* Row index for searching    */
	int yq = 0;                          /* Column index for searching */
	char opponent = (player == 's') ? 'b' : 's';  /* Identify opponent */
	int retVal = EXIT_FAILURE;

	board[row][col] = player;           /* Place the player counter   */

	/* Check all the squares around this square */
	/* for the opponents counter                */
	for (rowdelta = -1; rowdelta <= 1; rowdelta++)
		for (coldelta = -1; coldelta <= 1; coldelta++)
		{
			/* Don't check off the board, or the current square */
			if (row + rowdelta < 0 || row + rowdelta >= BOARD_SIZE ||
				col + coldelta < 0 || col + coldelta >= BOARD_SIZE ||
				(rowdelta == 0 && coldelta == 0))
				continue;

			/* Now check the square */
			if (board[row + rowdelta][col + coldelta] == opponent)
			{
				/* If we find the opponent, search in the same direction */
				/* for a player counter                                  */
				xq = row + rowdelta;        /* Move to opponent */
				yq = col + coldelta;        /* square           */

				for (;;)
				{
					xq += rowdelta;           /* Move to the      */
					yq += coldelta;           /* next square      */

					/* If we are off the board give up */
					if (xq < 0 || xq >= BOARD_SIZE || yq < 0 || yq >= BOARD_SIZE)
						break;

					/* If the square is blank give up */
					if (board[xq][yq] == 'n')
						break;

					/* If we find the player counter, go backwards from here */
					/* changing all the opponents counters to player         */
					if (board[xq][yq] == player)
					{
						while (board[xq -= rowdelta][yq -= coldelta] == opponent) /* Opponent? */ {
							board[xq][yq] = player;    /* Yes, change it */
							retVal = EXIT_SUCCESS;
						}
						break;                     /* We are done    */
					}
				}
			}
		}

	return retVal;
}
int counter(char board[BOARD_SIZE][BOARD_SIZE])
{
	int s, m;
	int user_score = 0, pc_score = 0;

	for (s = 0; s < BOARD_SIZE; s++) {
		for (m = 0; m < BOARD_SIZE; m++) {
			if (board[s][m] == 'b')
			{
				pc_score++;
			}
		    if (board[s][m] == 's')
			{
				user_score++;
			}
		}
	}
		al_draw_textf(font, al_map_rgb(100, 100, 100), BOARD_WIDTH, 100, "user:%d computer:%d", user_score, pc_score);
		al_flip_display();
		Sleep(3000);
		al_draw_textf(font, al_map_rgb(0, 0, 0), BOARD_WIDTH, 100, "user:%d computer:%d", user_score, pc_score);
		al_flip_display();

		return 0;

	}

