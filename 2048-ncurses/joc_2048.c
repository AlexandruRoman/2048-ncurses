#include <panel.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/select.h>
#include <ctype.h>


WINDOW* WINDOW_INITIALIZARE_BACKGROUND();
WINDOW* WINDOW_INITIALIZARE_BANDA();
WINDOW* WINDOW_INITIALIZARE_MENIU();
WINDOW* WINDOW_INITIALIZARE_LOGO();
WINDOW* WINDOW_INITIALIZARE_FUNDALJOC();
WINDOW* WINDOW_INITIALIZARE_CONTROL();

void AFISARE_PARTICICA_COPERTINA();
void EFECT_COPERTINA();

int PRELUCREAZA_MUTAREA();
int PRELUCREAZA_MUTAREA_VIRTUAL();
void ACTUALIZEAZA_TABELA();
int VERIFICA_VALIDITATEA();

void WINDOW_INITIALIZARE_NUMERE();
void GENEREAZA_UN_ELEMENT_ALEATOR();
void REFRESH_CONTROL_PANEL();
void RESET_CONTROL_PANEL();

int main()
{
	/* ███████████████████████████████████████████████████████████████████ */
	/* ███ INITIALIZARI ██████████████████████████████████████████████████ */
	/* ███████████████████████████████████████████████████████████████████ */

	PANEL  *BACKGROUND_panel, *BANDA_panel, *MENIU_panel, *SELECT_panels[3], *LOGO_panel, *FUNDALJOC_panel, *PATRATELE_panels[16], *CONTROL_panel;
	WINDOW *BACKGROUND_window, *BANDA_window, *MENIU_window, *SELECT_windows[3], *LOGO_window, *FUNDALJOC_window, *PATRATELE_windows[16], *CONTROL_window, *TATA_window;

	int MAX_x, MAX_y, i, j, option = 81;
	int ch, m[4][4], aux, old_m[4][4], marcate[4][4], scor = 0, sel, undo[4][4];
	float cv = (float)(1000 / 255);
	int ok_exista_mutari, combinari_left, combinari_right, combinari_up, combinari_down, combinari_max, mutare_recomandata;
	int timp_ramas = 100, exista_joc = 0, ultimul_scor_adaugat = 0, ok_win = 0;

	fd_set read_descriptors;
	struct timeval timeout;

	FD_ZERO(&read_descriptors);
	FD_SET(0 , &read_descriptors);

	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;

	system("resize -s 37 120");

	/* ███████████████████████████████████████████████████████████████████ */
	/* ███ INITIALIZARE CURSES ███████████████████████████████████████████ */
	/* ███████████████████████████████████████████████████████████████████ */

	{

		TATA_window = initscr();
		getmaxyx(TATA_window, MAX_y, MAX_x);
		start_color();
		cbreak();
		noecho();
		nonl();
		keypad(stdscr, TRUE);
		curs_set(0);
		srand(time(NULL));
	}

	/* ███████████████████████████████████████████████████████████████████ */
	/* ███ CULORI ████████████████████████████████████████████████████████ */
	/* ███████████████████████████████████████████████████████████████████ */

	{
		init_pair(1, COLOR_BLACK, COLOR_BLACK);
		init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
		init_pair(3, COLOR_MAGENTA, COLOR_YELLOW);
		init_pair(4, COLOR_WHITE, COLOR_WHITE);
		init_pair(5, COLOR_WHITE, COLOR_BLACK);
		init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA);

		init_pair(11, COLOR_MAGENTA, COLOR_BLUE);
		init_pair(12, COLOR_MAGENTA, COLOR_RED);
		init_pair(13, COLOR_MAGENTA, COLOR_GREEN);
		init_pair(14, COLOR_MAGENTA, COLOR_YELLOW);
		init_pair(15, COLOR_MAGENTA, COLOR_CYAN);
		init_pair(16, COLOR_MAGENTA, COLOR_WHITE);

		init_pair(17, COLOR_RED, COLOR_BLACK);
		init_pair(18, COLOR_GREEN, COLOR_BLACK);
		init_pair(19, COLOR_CYAN, COLOR_BLACK);
		init_pair(20, COLOR_WHITE, COLOR_BLACK);
		init_pair(21, COLOR_MAGENTA, COLOR_YELLOW);

		init_pair(99, COLOR_MAGENTA, COLOR_WHITE);
		init_pair(100, COLOR_WHITE, COLOR_MAGENTA);

		init_color(COLOR_YELLOW, (int)(cv * 247), (int)(cv * 181), (int)(cv * 59));
		init_color(COLOR_BLACK, (int)(cv * 53), (int)(cv * 53), (int)(cv * 53));
		init_color(COLOR_WHITE, (int)(cv * 230), (int)(cv * 230), (int)(cv * 230));
		init_color(COLOR_GREEN, (int)(cv * 225), (int)(cv * 108), (int)(cv * 29));
		init_color(COLOR_BLUE, (int)(cv * 223), (int)(cv * 186), (int)(cv * 108));
		init_color(COLOR_CYAN, (int)(cv * 226), (int)(cv * 123), (int)(cv * 72));
		init_color(COLOR_RED, (int)(cv * 255), (int)(cv * 227), (int)(cv * 144));
		init_color(COLOR_MAGENTA, 0, 0, 0);
	}

	/* ███████████████████████████████████████████████████████████████████ */
	/* ███ WINDOWS and PANELS ████████████████████████████████████████████ */
	/* ███████████████████████████████████████████████████████████████████ */
	{
		BACKGROUND_window = WINDOW_INITIALIZARE_BACKGROUND(MAX_y, MAX_x);
		BANDA_window = WINDOW_INITIALIZARE_BANDA(MAX_y, MAX_x);
		MENIU_window = WINDOW_INITIALIZARE_MENIU(MAX_y, MAX_x);
		LOGO_window = WINDOW_INITIALIZARE_LOGO(MAX_y, MAX_x);
		FUNDALJOC_window = WINDOW_INITIALIZARE_FUNDALJOC();
		CONTROL_window = WINDOW_INITIALIZARE_CONTROL();

		BACKGROUND_panel = new_panel(BACKGROUND_window);
		BANDA_panel = new_panel(BANDA_window);
		MENIU_panel = new_panel(MENIU_window);
		LOGO_panel = new_panel(LOGO_window);
		FUNDALJOC_panel = new_panel(FUNDALJOC_window);
		CONTROL_panel = new_panel(CONTROL_window);

		SELECT_windows[0] = newwin(1, 40, (MAX_y - 23) / 2 + 5, 5);
		SELECT_windows[1] = newwin(1, 31, (MAX_y - 23) / 2 + 11, 5);
		SELECT_windows[2] = newwin(1, 17, (MAX_y - 23) / 2 + 17, 5);
		wattrset(SELECT_windows[0], COLOR_PAIR(4));
		wattrset(SELECT_windows[1], COLOR_PAIR(1));
		wattrset(SELECT_windows[2], COLOR_PAIR(1));
		for (i = 0; i < 40; i++) wprintw(SELECT_windows[0], " ");
		for (i = 0; i < 31; i++) wprintw(SELECT_windows[1], " ");
		for (i = 0; i < 17; i++) wprintw(SELECT_windows[2], " ");
		SELECT_panels[0] = new_panel(SELECT_windows[0]);
		SELECT_panels[1] = new_panel(SELECT_windows[1]);
		SELECT_panels[2] = new_panel(SELECT_windows[2]);

		for (i = 0; i < 16; i++)
		{
			PATRATELE_windows[i] = newwin(7, 15, 2 + (int)(i / 4) * 9, 3 + (i % 4) * 19);
			wattrset(PATRATELE_windows[i], COLOR_PAIR(4));
			for (j = 0; j < 7 * 15; j++) wprintw(PATRATELE_windows[i], " ");
			PATRATELE_panels[i] = new_panel(PATRATELE_windows[i]);
			hide_panel(PATRATELE_panels[i]);
		}

		//mvwprintw(MENIU_window, 2, 2, "%d x %d", MAX_y, MAX_x);
		hide_panel(FUNDALJOC_panel);
		hide_panel(CONTROL_panel);
	}

	REFRESH_CONTROL_PANEL(CONTROL_window, 0);

	/* ███████████████████████████████████████████████████████████████████ */
	/* ███ MENIU █████████████████████████████████████████████████████████ */
	/* ███████████████████████████████████████████████████████████████████ */
menu:

	/* ███████████████████████████████████████████████████████████████████ */
	/* ███ PREGATESTE MENIUL █████████████████████████████████████████████ */
	/* ███████████████████████████████████████████████████████████████████ */
	flushinp();
	option = 81;
	wattrset(SELECT_windows[0], COLOR_PAIR(1));
	wattrset(SELECT_windows[1], COLOR_PAIR(1));
	wattrset(SELECT_windows[2], COLOR_PAIR(1));

	for (i = 0; i < 40; i++) mvwprintw(SELECT_windows[0], 0, i, " ");
	for (i = 0; i < 31; i++) mvwprintw(SELECT_windows[1], 0, i, " ");
	for (i = 0; i < 17; i++) mvwprintw(SELECT_windows[2], 0, i, " ");
	wattrset(SELECT_windows[0], COLOR_PAIR(4));
	for (i = 0; i < 40; i++) mvwprintw(SELECT_windows[0], 0, i, " ");
	update_panels();
	doupdate();

	/* ███████████████████████████████████████████████████████████████████ */
	/* ███ BUCLA DE MENIU ████████████████████████████████████████████████ */
	/* ███████████████████████████████████████████████████████████████████ */
	while ((ch = getch()))
	{
		switch (ch)
		{

		case KEY_UP:
			option --;
			if (option % 3 == 1 && exista_joc == 0)
				option--;
			break;

		case KEY_DOWN:
			option ++;
			if (option % 3 == 1 && exista_joc == 0)
				option++;
			break;

		case 13:

			/* ███████████████████████████████████████████████████████████████████ */
			/* ███ DACA ai ales NEW GAME █████████████████████████████████████████ */
			/* ███████████████████████████████████████████████████████████████████ */

			if (option % 3 == 0)
			{
				/* ███████████████████████████████████████████████████████████████████ */
				/* ███ PREGATESTE TABELA PENTRU JOC ██████████████████████████████████ */
				/* ███████████████████████████████████████████████████████████████████ */
				exista_joc = 1;
				ok_win = 0;
				ultimul_scor_adaugat = 0;
				scor = 0;

				for (i = 0; i < 4; i++)
					for (j = 0; j < 4; j++)
					{
						m[i][j] = 0;
						old_m[i][j] = 0;
						marcate[i][j] = 0;
					}
				GENEREAZA_UN_ELEMENT_ALEATOR(m);
				GENEREAZA_UN_ELEMENT_ALEATOR(m);
				for (i = 0; i < 4; i++)
					for (j = 0; j < 4; j++)
					{
						undo[i][j] = old_m[i][j] = m[i][j];
					}

				ACTUALIZEAZA_TABELA(m, old_m, undo, PATRATELE_windows);
				RESET_CONTROL_PANEL(CONTROL_window);
				update_panels();
				doupdate();

				EFECT_COPERTINA(1, 2, 1, BANDA_panel, MENIU_panel, LOGO_panel, SELECT_panels, FUNDALJOC_panel, PATRATELE_panels, CONTROL_panel);
				goto joc;
			}

			/* ███████████████████████████████████████████████████████████████████ */
			/* ███ DACA ai ales RESUME ███████████████████████████████████████████ */
			/* ███████████████████████████████████████████████████████████████████ */
			if (option % 3 == 1)
			{
				EFECT_COPERTINA(1, 2, 1, BANDA_panel, MENIU_panel, LOGO_panel, SELECT_panels, FUNDALJOC_panel, PATRATELE_panels, CONTROL_panel);
				goto joc;
			}

			/* ███████████████████████████████████████████████████████████████████ */
			/* ███ DACA ai ales QUIT █████████████████████████████████████████████ */
			/* ███████████████████████████████████████████████████████████████████ */
			if (option % 3 == 2)
			{
				system("pkill joc_2048");
			}
			break;

		}

		/* ███████████████████████████████████████████████████████████████████ */
		/* ███ ARATA ce avem SELECTAT ████████████████████████████████████████ */
		/* ███████████████████████████████████████████████████████████████████ */
		wattrset(SELECT_windows[0], COLOR_PAIR(1));
		wattrset(SELECT_windows[1], COLOR_PAIR(1));
		wattrset(SELECT_windows[2], COLOR_PAIR(1));
		for (i = 0; i < 40; i++) mvwprintw(SELECT_windows[0], 0, i, " ");
		for (i = 0; i < 31; i++) mvwprintw(SELECT_windows[1], 0, i, " ");
		for (i = 0; i < 17; i++) mvwprintw(SELECT_windows[2], 0, i, " ");
		switch (option % 3)
		{
		case 0:
			wattrset(SELECT_windows[0], COLOR_PAIR(4));
			for (i = 0; i < 40; i++) mvwprintw(SELECT_windows[0], 0, i, " ");
			break;

		case 1:
			wattrset(SELECT_windows[1], COLOR_PAIR(4));
			for (i = 0; i < 31; i++) mvwprintw(SELECT_windows[1], 0, i, " ");
			break;

		case 2:
			wattrset(SELECT_windows[2], COLOR_PAIR(4));
			for (i = 0; i < 17; i++) mvwprintw(SELECT_windows[2], 0, i, " ");
			break;
		}



		update_panels();
		doupdate();
	}


	/* ███████████████████████████████████████████████████████████████████ */
	/* ███ JOC ███████████████████████████████████████████████████████████ */
	/* ███████████████████████████████████████████████████████████████████ */
joc:
	flushinp();
	while (1)
	{
		sel = select(1, &read_descriptors, NULL, NULL, &timeout);

		switch (sel)
		{
		case 1:
			ch = getch();
			timp_ramas = 100;
			switch (ch)
			{
			case KEY_UP:
				ultimul_scor_adaugat = PRELUCREAZA_MUTAREA(-1, 0, m, marcate, &scor);
				break;

			case KEY_DOWN:
				ultimul_scor_adaugat = PRELUCREAZA_MUTAREA(1, 0, m, marcate, &scor);
				break;

			case KEY_RIGHT:
				ultimul_scor_adaugat = PRELUCREAZA_MUTAREA(0, 1, m, marcate, &scor);
				break;

			case KEY_LEFT:
				ultimul_scor_adaugat = PRELUCREAZA_MUTAREA(0, -1, m, marcate, &scor);
				break;

			case 'q':
				/* ███████████████████████████████████████████████████████████████████ */
				/* ███ BACK to MENU ██████████████████████████████████████████████████ */
				/* ███████████████████████████████████████████████████████████████████ */
				EFECT_COPERTINA(2, 1, 1, BANDA_panel, MENIU_panel, LOGO_panel, SELECT_panels, FUNDALJOC_panel, PATRATELE_panels, CONTROL_panel);
				goto menu;
				break;

			case 'u':
				/* ███████████████████████████████████████████████████████████████████ */
				/* ███ UNDO ██████████████████████████████████████████████████████████ */
				/* ███████████████████████████████████████████████████████████████████ */
				timp_ramas = 100;
				for (i = 0; i < 4; i++)
					for (j = 0; j < 4; j++)
					{
						m[i][j] = old_m[i][j] = undo[i][j];
					}
				scor -= ultimul_scor_adaugat;
				ultimul_scor_adaugat = 0;
				ACTUALIZEAZA_TABELA(m, old_m, undo, PATRATELE_windows);
				RESET_CONTROL_PANEL(CONTROL_window);
				REFRESH_CONTROL_PANEL(CONTROL_window, scor);
				break;

			case 'a':
				/* ███████████████████████████████████████████████████████████████████ */
				/* ███ AUTO MOVE █████████████████████████████████████████████████████ */
				/* ███████████████████████████████████████████████████████████████████ */
				ok_exista_mutari = 0;
				combinari_max = -1;
				mutare_recomandata = 1;
				/* ███████████████████████████████████████████████████████████████████ */
				/* ███ CALCULEAZA EFICIENTA pentru fiecare MUTARE ████████████████████ */
				/* ███████████████████████████████████████████████████████████████████ */
				//up
				combinari_up = PRELUCREAZA_MUTAREA_VIRTUAL(-1, 0, m, marcate);
				if (VERIFICA_VALIDITATEA(m, old_m)) ok_exista_mutari = 1;
				else combinari_up = -2;
				for (i = 0; i < 4; i++)
					for (j = 0; j < 4; j++)
						m[i][j] = old_m[i][j];

				//down
				combinari_down = PRELUCREAZA_MUTAREA_VIRTUAL(1, 0, m, marcate);
				if (VERIFICA_VALIDITATEA(m, old_m)) ok_exista_mutari = 1;
				else combinari_down = -2;
				for (i = 0; i < 4; i++)
					for (j = 0; j < 4; j++)
						m[i][j] = old_m[i][j];

				//right
				combinari_right = PRELUCREAZA_MUTAREA_VIRTUAL(0, 1, m, marcate);
				if (VERIFICA_VALIDITATEA(m, old_m)) ok_exista_mutari = 1;
				else combinari_right = -2;
				for (i = 0; i < 4; i++)
					for (j = 0; j < 4; j++)
						m[i][j] = old_m[i][j];

				//left
				combinari_left = PRELUCREAZA_MUTAREA_VIRTUAL(0, -1, m, marcate);
				if (VERIFICA_VALIDITATEA(m, old_m)) ok_exista_mutari = 1;
				else combinari_left = -2;
				for (i = 0; i < 4; i++)
					for (j = 0; j < 4; j++)
						m[i][j] = old_m[i][j];

				//Daca nu mai exista mutari valide
				if (ok_exista_mutari == 0)
				{
					usleep(2000000);
					exista_joc = 0;
					scor = 0;
					EFECT_COPERTINA(2, 1, 2, BANDA_panel, MENIU_panel, LOGO_panel, SELECT_panels, FUNDALJOC_panel, PATRATELE_panels, CONTROL_panel);
					goto menu;
				}

				//Verifica daca am castigat
				for (i = 0; i < 4; i++)
					for (j = 0; j < 4; j++)
						if (m[i][j] == 2048) ok_win = 1;

				//Daca am castigat
				if (ok_win)
				{
					usleep(2000000);
					exista_joc = 0;
					scor = 0;
					EFECT_COPERTINA(2, 1, 3, BANDA_panel, MENIU_panel, LOGO_panel, SELECT_panels, FUNDALJOC_panel, PATRATELE_panels, CONTROL_panel);
					goto menu;
				}

				/* ███████████████████████████████████████████████████████████████████ */
				/* ███ Se pregateste sa mute singur ██████████████████████████████████ */
				/* ███████████████████████████████████████████████████████████████████ */

				if (combinari_max < combinari_up)
				{
					combinari_max = combinari_up;
					mutare_recomandata = 1;
				}
				if (combinari_max < combinari_down)
				{
					combinari_max = combinari_down;
					mutare_recomandata = 2;
				}
				if (combinari_max < combinari_right)
				{
					combinari_max = combinari_right;
					mutare_recomandata = 3;
				}
				if (combinari_max < combinari_left)
				{
					combinari_max = combinari_left;
					mutare_recomandata = 4;
				}

				if (mutare_recomandata == 1) ultimul_scor_adaugat = PRELUCREAZA_MUTAREA(-1, 0, m, marcate, &scor);
				if (mutare_recomandata == 2) ultimul_scor_adaugat = PRELUCREAZA_MUTAREA(1, 0, m, marcate, &scor);
				if (mutare_recomandata == 3) ultimul_scor_adaugat = PRELUCREAZA_MUTAREA(0, 1, m, marcate, &scor);
				if (mutare_recomandata == 4) ultimul_scor_adaugat = PRELUCREAZA_MUTAREA(0, -1, m, marcate, &scor);

				GENEREAZA_UN_ELEMENT_ALEATOR(m);
				ACTUALIZEAZA_TABELA(m, old_m, undo, PATRATELE_windows);
				REFRESH_CONTROL_PANEL(CONTROL_window, scor);
				update_panels();
				doupdate();
				break;
			}

			/* ███████████████████████████████████████████████████████████████████ */
			/* ███ DACA avem mutare corecta, ACTUALIZEAZA ████████████████████████ */
			/* ███████████████████████████████████████████████████████████████████ */
			if (VERIFICA_VALIDITATEA(m, old_m))
			{
				GENEREAZA_UN_ELEMENT_ALEATOR(m);
				ACTUALIZEAZA_TABELA(m, old_m, undo, PATRATELE_windows);
				update_panels();
				doupdate();
			}
			REFRESH_CONTROL_PANEL(CONTROL_window, scor);
			update_panels();
			doupdate();

		case 0:
			/* ████████████████████████████████████████████████████████████████████ */
			/* █ Se ruleaza daca utilizatorul nu apasa nimic timp de o secunda/10 █ */
			/* ████████████████████████████████████████████████████████████████████ */
			timp_ramas--;
			REFRESH_CONTROL_PANEL(CONTROL_window, scor);
			update_panels();
			doupdate();

			/* ███████████████████████████████████████████████████████████████████ */
			/* ███ AUTO MOVE █████████████████████████████████████████████████████ */
			/* ███████████████████████████████████████████████████████████████████ */
			ok_exista_mutari = 0;
			combinari_max = -1;
			mutare_recomandata = 1;
			/* ███████████████████████████████████████████████████████████████████ */
			/* ███ CALCULEAZA EFICIENTA pentru fiecare MUTARE ████████████████████ */
			/* ███████████████████████████████████████████████████████████████████ */
			//up
			combinari_up = PRELUCREAZA_MUTAREA_VIRTUAL(-1, 0, m, marcate);
			if (VERIFICA_VALIDITATEA(m, old_m)) ok_exista_mutari = 1;
			else combinari_up = -2;
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					m[i][j] = old_m[i][j];

			//down
			combinari_down = PRELUCREAZA_MUTAREA_VIRTUAL(1, 0, m, marcate);
			if (VERIFICA_VALIDITATEA(m, old_m)) ok_exista_mutari = 1;
			else combinari_down = -2;
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					m[i][j] = old_m[i][j];

			//right
			combinari_right = PRELUCREAZA_MUTAREA_VIRTUAL(0, 1, m, marcate);
			if (VERIFICA_VALIDITATEA(m, old_m)) ok_exista_mutari = 1;
			else combinari_right = -2;
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					m[i][j] = old_m[i][j];

			//left
			combinari_left = PRELUCREAZA_MUTAREA_VIRTUAL(0, -1, m, marcate);
			if (VERIFICA_VALIDITATEA(m, old_m)) ok_exista_mutari = 1;
			else combinari_left = -2;
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					m[i][j] = old_m[i][j];

			//Daca nu mai exista mutari valide
			if (ok_exista_mutari == 0)
			{
				usleep(2000000);
				exista_joc = 0;
				scor = 0;
				EFECT_COPERTINA(2, 1, 2, BANDA_panel, MENIU_panel, LOGO_panel, SELECT_panels, FUNDALJOC_panel, PATRATELE_panels, CONTROL_panel);
				goto menu;
			}

			//Verifica daca am castigat
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					if (m[i][j] == 2048) ok_win = 1;

			//Daca am castigat
			if (ok_win)
			{
				usleep(2000000);
				exista_joc = 0;
				scor = 0;
				EFECT_COPERTINA(2, 1, 3, BANDA_panel, MENIU_panel, LOGO_panel, SELECT_panels, FUNDALJOC_panel, PATRATELE_panels, CONTROL_panel);
				goto menu;
			}
			if (timp_ramas == 0)
			{
				timp_ramas = 100;
				/* ███████████████████████████████████████████████████████████████████ */
				/* ███ Se pregateste sa mute singur ██████████████████████████████████ */
				/* ███████████████████████████████████████████████████████████████████ */

				if (combinari_max < combinari_up)
				{
					combinari_max = combinari_up;
					mutare_recomandata = 1;
				}
				if (combinari_max < combinari_down)
				{
					combinari_max = combinari_down;
					mutare_recomandata = 2;
				}
				if (combinari_max < combinari_right)
				{
					combinari_max = combinari_right;
					mutare_recomandata = 3;
				}
				if (combinari_max < combinari_left)
				{
					combinari_max = combinari_left;
					mutare_recomandata = 4;
				}

				if (mutare_recomandata == 1) ultimul_scor_adaugat = PRELUCREAZA_MUTAREA(-1, 0, m, marcate, &scor);
				if (mutare_recomandata == 2) ultimul_scor_adaugat = PRELUCREAZA_MUTAREA(1, 0, m, marcate, &scor);
				if (mutare_recomandata == 3) ultimul_scor_adaugat = PRELUCREAZA_MUTAREA(0, 1, m, marcate, &scor);
				if (mutare_recomandata == 4) ultimul_scor_adaugat = PRELUCREAZA_MUTAREA(0, -1, m, marcate, &scor);

				GENEREAZA_UN_ELEMENT_ALEATOR(m);
				ACTUALIZEAZA_TABELA(m, old_m, undo, PATRATELE_windows);
				REFRESH_CONTROL_PANEL(CONTROL_window, scor);
				update_panels();
				doupdate();
			}
			break;
		}

		FD_SET(0, &read_descriptors);
		timeout.tv_sec = 0;
		timeout.tv_usec = 100000;
	}


	getch();
	endwin();
	return 0;
}





/* ███████████████████████████████████████████████████████████████████ */
/* ███ CREARE WINDOWS ████████████████████████████████████████████████ */
/* ███████████████████████████████████████████████████████████████████ */
WINDOW* WINDOW_INITIALIZARE_BACKGROUND(int MAX_y, int MAX_x)
{
	WINDOW *win;
	int i;
	win = newwin(MAX_y, MAX_x, 0, 0);
	wattrset(win, COLOR_PAIR(5));
	for (i = 0; i < MAX_x * MAX_y; i++)
	{
		/*
		waddch(win, ACS_BULLET);
		waddch(win, ' ');
		waddch(win, '.');
		waddch(win, ' ');
		*/
		wprintw(win, "      /");
	}
	return win;
}

WINDOW* WINDOW_INITIALIZARE_BANDA(int MAX_y, int MAX_x)
{
	WINDOW *win;
	int i;
	win = newwin(MAX_y, 45, 0, 5);
	wattrset(win, COLOR_PAIR(2));

	for (i = 0; i < MAX_y * 45; i++)
	{
		wprintw(win, "*");
	}


	return win;
}

WINDOW* WINDOW_INITIALIZARE_MENIU(int MAX_y, int MAX_x)
{
	WINDOW *win;
	int i, j;
	win = newwin(18, 44, (MAX_y - 20) / 2, 6);
	wattrset(win, COLOR_PAIR(3));

	FILE *text_meniu;
	text_meniu = fopen("Meniu", "r");

	char text[50];

	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			fgets(text, 46, text_meniu);
			text[44] = '\0';
			wprintw(win, "%s", text);
		}
		wprintw(win, "                                            ");
		wprintw(win, "                                            ");
		wprintw(win, "                                            ");
	}

	return win;
}

WINDOW* WINDOW_INITIALIZARE_LOGO(int MAX_y, int MAX_x)
{
	WINDOW *win;
	int i;
	win = newwin(7, 46, (MAX_y - 7) / 2, 48 + (MAX_x - 47 - 45) / 2);
	wattrset(win, COLOR_PAIR(5));

	FILE * text_logo;
	text_logo = fopen("2048", "r");

	char text[50];

	for (i = 0; i < 7; i++)
	{
		fgets(text, 47, text_logo);
		wprintw(win, "%s", text);
	}

	return win;
}

WINDOW* WINDOW_INITIALIZARE_FUNDALJOC()
{
	WINDOW *win;
	int i;
	win = newwin(38, 79, 0, 0);
	wattrset(win, COLOR_PAIR(6));

	for (i = 0; i < 38 * 79; i++)
	{
		wprintw(win, "*");
	}


	return win;
}

WINDOW* WINDOW_INITIALIZARE_CONTROL()
{
	WINDOW *win;
	int i;
	win = newwin(14, 19, 0, 90);
	wattrset(win, COLOR_PAIR(100));
	for (i = 0; i < 12 * 19; i++)
	{
		wprintw(win, " ");
	}
	mvwprintw(win, 8, 0, " a .......... auto ");
	mvwprintw(win, 9, 0, " arrows ..... move ");
	mvwprintw(win, 10, 0, " u .......... undo ");
	mvwprintw(win, 11, 0, " q .. back to menu ");
	mvwprintw(win, 12, 0, "                   ");

	wattrset(win, COLOR_PAIR(99));
	mvwprintw(win, 13, 0, "   CONTROL-PANEL   ");

	return win;
}

/* ███████████████████████████████████████████████████████████████████ */
/* ███ COPERTINA █████████████████████████████████████████████████████ */
/* ███████████████████████████████████████████████████████████████████ */
void AFISARE_PARTICICA_COPERTINA(int linie, int MAX_x, WINDOW *win, int mesaj)
{
	FILE *text_copertina;
	switch (mesaj)
	{
	case 1:
		text_copertina = fopen("mesaj-obisnuit", "r");
		break;
	case 2:
		text_copertina = fopen("mesaj-lose", "r");
		break;
	case 3:
		text_copertina = fopen("mesaj-won", "r");
		break;
	}

	char text[130];

	wattrset(win, COLOR_PAIR(99));
	int i;
	for (i = 0; i < 37 - linie; i++)
	{
		fgets(text, 130, text_copertina);
	}

	for (i = 1; i < linie; i++)
	{
		fgets(text, 130, text_copertina);
		text[120] = '\0';
		wprintw(win, "%s", text);
	}
}

void EFECT_COPERTINA(int ce_ascund, int ce_arat, int mesaj, PANEL *BANDA_panel, PANEL *MENIU_panel, PANEL *LOGO_panel, PANEL *SELECT_panels[], PANEL *FUNDALJOC_panel, PANEL *PATRATELE_panels[], PANEL *CONTROL_panel)
{
	PANEL *panel;
	WINDOW *old_win, *temp_win;

	old_win = newwin(1, 120, 0, 0);
	panel = new_panel(old_win);
	AFISARE_PARTICICA_COPERTINA(1, 120, old_win, mesaj);
	int i;
	for (i = 2; i < 38; i++)
	{
		usleep(700 * (10 + i));
		old_win = panel_window(panel);
		temp_win = newwin(i, 120, 0, 0);
		replace_panel(panel, temp_win);
		AFISARE_PARTICICA_COPERTINA(i, 120, temp_win, mesaj);
		delwin(old_win);

		update_panels();
		doupdate();
	}

	usleep(3000000);
	flushinp();
	if (mesaj == 2)
		usleep(3000000);
	if (mesaj == 3)
		getch();

	if (ce_ascund == 1)
	{
		hide_panel(BANDA_panel);
		hide_panel(MENIU_panel);
		hide_panel(LOGO_panel);
		for (i = 0; i < 3; i++) hide_panel(SELECT_panels[i]);
	}

	if (ce_ascund == 2)
	{
		hide_panel(FUNDALJOC_panel);
		hide_panel(CONTROL_panel);
		for (i = 0; i < 16; i++) hide_panel(PATRATELE_panels[i]);
	}

	if (ce_arat == 1)
	{
		show_panel(BANDA_panel);
		show_panel(MENIU_panel);
		show_panel(LOGO_panel);
		for (i = 0; i < 3; i++) show_panel(SELECT_panels[i]);
	}

	if (ce_arat == 2)
	{
		show_panel(FUNDALJOC_panel);
		show_panel(CONTROL_panel);
		for (i = 0; i < 16; i++) show_panel(PATRATELE_panels[i]);
	}

	top_panel(panel);

	for (i = 1; i < 38; i++)
	{
		usleep(700 * (50 - i));
		old_win = panel_window(panel);
		temp_win = newwin((38 - i), 120, 0, 0);
		replace_panel(panel, temp_win);
		AFISARE_PARTICICA_COPERTINA((38 - i), 120, temp_win, mesaj);
		delwin(old_win);

		update_panels();
		doupdate();
	}


	delwin(temp_win);
	del_panel(panel);
}

/* ███████████████████████████████████████████████████████████████████ */
/* ███ Functii ce tin de jocul propriuzis ████████████████████████████ */
/* ███████████████████████████████████████████████████████████████████ */
int PRELUCREAZA_MUTAREA(int dx, int dy, int m[4][4], int marcate[4][4], int *scor)
{
	int k, i, j;
	int numar_schimbari = 0;
	int dscor = 0;

	//Prima lipire de perete
	for (k = 0; k < 5; k++)
	{
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
				if (i + dx < 4 && i + dx >= 0 && j + dy < 4 && j + dy >= 0)
				{

					if (m[i + dx][j + dy] == 0)
					{
						m[i + dx][j + dy] = m[i][j];
						m[i][j] = 0;
					}
				}
		}
	}

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			marcate[i][j] = 0;

		}
	}

	//Combinare


	//Left
	if (dx == 0 && dy == -1)
	{
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
				if (i + dx < 4 && i + dx >= 0 && j + dy < 4 && j + dy >= 0)
				{

					if (m[i + dx][j + dy] == m[i][j] && marcate[i][j] == 0 && marcate[i + dx][j + dy] == 0 && m[i][j] != 0)
					{
						m[i + dx][j + dy] *= 2;
						marcate[i + dx][j + dy] = 1;
						m[i][j] = 0;
						numar_schimbari++;
						dscor += m[i + dx][j + dy];
						*scor = *scor + m[i + dx][j + dy];
					}
				}
		}
	}

	//Right
	if (dx == 0 && dy == 1)
	{
		for (i = 0; i < 4; i++)
		{
			for (j = 3; j >= 0; j--)
				if (i + dx < 4 && i + dx >= 0 && j + dy < 4 && j + dy >= 0)
				{

					if (m[i + dx][j + dy] == m[i][j] && marcate[i][j] == 0 && marcate[i + dx][j + dy] == 0 && m[i][j] != 0)
					{
						m[i + dx][j + dy] *= 2;
						marcate[i + dx][j + dy] = 1;
						m[i][j] = 0;
						numar_schimbari++;
						dscor += m[i + dx][j + dy];
						*scor = *scor + m[i + dx][j + dy];
					}
				}
		}
	}
	//Up
	if (dx == -1 && dy == 0)
	{
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
				if (i + dx < 4 && i + dx >= 0 && j + dy < 4 && j + dy >= 0)
				{

					if (m[i + dx][j + dy] == m[i][j] && marcate[i][j] == 0 && marcate[i + dx][j + dy] == 0 && m[i][j] != 0)
					{
						m[i + dx][j + dy] *= 2;
						marcate[i + dx][j + dy] = 1;
						m[i][j] = 0;
						numar_schimbari++;
						dscor += m[i + dx][j + dy];
						*scor = *scor + m[i + dx][j + dy];
					}
				}
		}
	}
	//Down
	if (dx == 1 && dy == 0)
	{
		for (i = 3; i >= 0; i--)
		{
			for (j = 0; j < 4; j++)
				if (i + dx < 4 && i + dx >= 0 && j + dy < 4 && j + dy >= 0)
				{

					if (m[i + dx][j + dy] == m[i][j] && marcate[i][j] == 0 && marcate[i + dx][j + dy] == 0 && m[i][j] != 0)
					{
						m[i + dx][j + dy] *= 2;
						marcate[i + dx][j + dy] = 1;
						m[i][j] = 0;
						numar_schimbari++;
						dscor += m[i + dx][j + dy];
						*scor = *scor + m[i + dx][j + dy];
					}
				}
		}
	}



	//A doua lipire de perete
	for (k = 0; k < 5; k++)
	{
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
				if (i + dx < 4 && i + dx >= 0 && j + dy < 4 && j + dy >= 0)
				{
					if (m[i + dx][j + dy] == 0)
					{
						m[i + dx][j + dy] = m[i][j];
						m[i][j] = 0;
					}
				}
		}
	}

	return dscor;
}

int PRELUCREAZA_MUTAREA_VIRTUAL(int dx, int dy, int m[4][4], int marcate[4][4])
{
	int k, i, j;
	int numar_schimbari = 0;

	//Prima lipire de perete
	for (k = 0; k < 5; k++)
	{
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
				if (i + dx < 4 && i + dx >= 0 && j + dy < 4 && j + dy >= 0)
				{

					if (m[i + dx][j + dy] == 0)
					{
						m[i + dx][j + dy] = m[i][j];
						m[i][j] = 0;
					}
				}
		}
	}

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			marcate[i][j] = 0;

		}
	}

	//Combinare


	//Left
	if (dx == 0 && dy == -1)
	{
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
				if (i + dx < 4 && i + dx >= 0 && j + dy < 4 && j + dy >= 0)
				{

					if (m[i + dx][j + dy] == m[i][j] && marcate[i][j] == 0 && marcate[i + dx][j + dy] == 0 && m[i][j] != 0)
					{
						m[i + dx][j + dy] *= 2;
						marcate[i + dx][j + dy] = 1;
						m[i][j] = 0;
						numar_schimbari++;
					}
				}
		}
	}

	//Right
	if (dx == 0 && dy == 1)
	{
		for (i = 0; i < 4; i++)
		{
			for (j = 3; j >= 0; j--)
				if (i + dx < 4 && i + dx >= 0 && j + dy < 4 && j + dy >= 0)
				{

					if (m[i + dx][j + dy] == m[i][j] && marcate[i][j] == 0 && marcate[i + dx][j + dy] == 0 && m[i][j] != 0)
					{
						m[i + dx][j + dy] *= 2;
						marcate[i + dx][j + dy] = 1;
						m[i][j] = 0;
						numar_schimbari++;
					}
				}
		}
	}
	//Up
	if (dx == -1 && dy == 0)
	{
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
				if (i + dx < 4 && i + dx >= 0 && j + dy < 4 && j + dy >= 0)
				{

					if (m[i + dx][j + dy] == m[i][j] && marcate[i][j] == 0 && marcate[i + dx][j + dy] == 0 && m[i][j] != 0)
					{
						m[i + dx][j + dy] *= 2;
						marcate[i + dx][j + dy] = 1;
						m[i][j] = 0;
						numar_schimbari++;
					}
				}
		}
	}
	//Down
	if (dx == 1 && dy == 0)
	{
		for (i = 3; i >= 0; i--)
		{
			for (j = 0; j < 4; j++)
				if (i + dx < 4 && i + dx >= 0 && j + dy < 4 && j + dy >= 0)
				{

					if (m[i + dx][j + dy] == m[i][j] && marcate[i][j] == 0 && marcate[i + dx][j + dy] == 0 && m[i][j] != 0)
					{
						m[i + dx][j + dy] *= 2;
						marcate[i + dx][j + dy] = 1;
						m[i][j] = 0;
						numar_schimbari++;
					}
				}
		}
	}



	//A doua lipire de perete
	for (k = 0; k < 5; k++)
	{
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
				if (i + dx < 4 && i + dx >= 0 && j + dy < 4 && j + dy >= 0)
				{
					if (m[i + dx][j + dy] == 0)
					{
						m[i + dx][j + dy] = m[i][j];
						m[i][j] = 0;
					}
				}
		}
	}

	return numar_schimbari;
}

void ACTUALIZEAZA_TABELA(int m[4][4], int old_m[4][4], int undo[4][4], WINDOW *PATRATELE_windows[])
{
	int i, j;

	for (i = 0; i < 16; i++)
	{
		wattrset(PATRATELE_windows[i], COLOR_PAIR(4));
		for (j = 0; j < 7 * 15; j++) mvwprintw( PATRATELE_windows[i], j / 15, j % 15, " ");
	}

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (m[i][j] != 0)
			{
				WINDOW_INITIALIZARE_NUMERE(m[i][j], PATRATELE_windows[i * 4 + j]);

			}
			undo[i][j] = old_m[i][j];
			old_m[i][j] = m[i][j];
		}
	}



	update_panels();
	doupdate();
}

int VERIFICA_VALIDITATEA(int m[4][4], int old_m[4][4])
{
	int ok = 0, i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (m[i][j] != old_m[i][j]) ok = 1; //e valida
		}
	}

	return ok;
}

/* ███████████████████████████████████████████████████████████████████ */
/* ███ ALTE FUNCTII AJUTATOARE ███████████████████████████████████████ */
/* ███████████████████████████████████████████████████████████████████ */
void REFRESH_CONTROL_PANEL(WINDOW *win, int scor)
{
	char text_timp[9], text_data[20];
	time_t t = time(NULL);
	struct tm * st = localtime(&t);
	strftime(text_timp, 10, "%H:%M:%S", st);
	strftime(text_data, 20, "%Y:%m:%d", st);

	int aux = scor, nr_cifre = 0;

	while (aux)
	{
		nr_cifre++;
		aux /= 10;
	}



	wattrset(win, COLOR_PAIR(100));
	mvwprintw(win, 0, 0, "       DATE:       ");
	mvwprintw(win, 1, 0, "    %s", text_data);
	mvwprintw(win, 2, 0, "       TIME:       ");
	mvwprintw(win, 3, 0, "     %s", text_timp);
	mvwprintw(win, 4, 0, "  ---------------  ");
	mvwprintw(win, 5, 0, "       Score       ");
	mvwprintw(win, 6, (19 - nr_cifre) / 2, "%d", scor);
	mvwprintw(win, 7, 0, "  ---------------  ");
}

void WINDOW_INITIALIZARE_NUMERE(int numar, WINDOW *win)
{

	if (numar == 2)
	{
		wattrset(win, COLOR_PAIR(11));
		mvwprintw(win, 0, 0, "%s", "                     ___           |_  |          |  _|          |___|                                   ");
	}

	if (numar == 4)
	{
		wattrset(win, COLOR_PAIR(12));
		mvwprintw(win, 0, 0, "%s", "                     ___           | | |          |_  |            |_|                                   ");
	}

	if (numar == 8)
	{
		wattrset(win, COLOR_PAIR(13));
		mvwprintw(win, 0, 0, "%s", "                     ___           | . |          | . |          |___|                                   ");
	}

	if (numar == 16)
	{
		wattrset(win, COLOR_PAIR(14));
		mvwprintw(win, 0, 0, "%s", "                    _ ___         | |  _|        | | . |        |_|___|                                  ");
	}

	if (numar == 32)
	{
		wattrset(win, COLOR_PAIR(15));
		mvwprintw(win, 0, 0, "%s", "                   ___ ___       |_  |_  |      |_  |  _|      |___|___|                                 ");
	}

	if (numar == 64)
	{
		wattrset(win, COLOR_PAIR(16));
		mvwprintw(win, 0, 0, "%s", "                   ___ ___       |  _| | |      | . |_  |      |___| |_|                                 ");
	}

	if (numar == 128)
	{
		wattrset(win, COLOR_PAIR(17));
		mvwprintw(win, 0, 0, "%s", "                  _ ___ ___     | |_  | . |    | |  _| . |    |_|___|___|                                ");
	}

	if (numar == 256)
	{
		wattrset(win, COLOR_PAIR(18));
		mvwprintw(win, 0, 0, "%s", "                 ___ ___ ___   |_  |  _|  _|  |  _|_  | . |  |___|___|___|                               ");
	}

	if (numar == 512)
	{
		wattrset(win, COLOR_PAIR(19));
		mvwprintw(win, 0, 0, "%s", "                 ___  _  ___   |  _|| ||_  |  |_  || ||  _|  |___||_||___|                               ");
	}

	if (numar == 1024)
	{
		wattrset(win, COLOR_PAIR(20));
		mvwprintw(win, 0, 0, "%s", "                _ ___ ___ ___ | |   |_  | | || | | |  _|_  ||_|___|___| |_|                              ");
	}

	if (numar == 2048)
	{
		wattrset(win, COLOR_PAIR(21));
		mvwprintw(win, 0, 0, "%s", "                ___ __ ___ __ |_  |  | | | .||  _| ||_  | .||___|__| |_|__|                              ");
	}
}

void RESET_CONTROL_PANEL(WINDOW *win)
{
	char text_timp[9], text_data[20];
	time_t t = time(NULL);
	struct tm * st = localtime(&t);
	strftime(text_timp, 10, "%H:%M:%S", st);
	strftime(text_data, 20, "%Y:%m:%d", st);

	wattrset(win, COLOR_PAIR(100));
	mvwprintw(win, 0, 0, "       DATE:       ");
	mvwprintw(win, 1, 0, "    %s", text_data);
	mvwprintw(win, 2, 0, "       TIME:       ");
	mvwprintw(win, 3, 0, "     %s", text_timp);
	mvwprintw(win, 4, 0, "  ---------------  ");
	mvwprintw(win, 5, 0, "       Score       ");
	mvwprintw(win, 6, 0, "                   ");
	mvwprintw(win, 7, 0, "  ---------------  ");
}

void GENEREAZA_UN_ELEMENT_ALEATOR(int m[4][4])
{
	int r1, r2;
	while (1)
	{
		r1 = rand() % 4;
		r2 = rand() % 4;
		if (m[r1][r2] == 0)
		{
			m[r1][r2] = (1 + rand() % 2) * 2;
			break;
		}
	}
}




