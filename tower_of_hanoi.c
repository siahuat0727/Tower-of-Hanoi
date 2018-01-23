#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define UP    'w'
#define DOWN  's'
#define LEFT  'a'
#define RIGHT 'd'

int DISKS[10][3];
int N_DISK;
struct {
	int column;
	int disk_size;
} ARROW;

void putchars(char c, int num)
{
	for(int i = 0; i < num; ++i)
		putchar(c);
}

void show_disk(int disk_size, char mid)
{
	putchars(' ', N_DISK - disk_size + 1);
	putchars('_', disk_size);
	putchar(disk_size > 0 ? disk_size + '0' : mid);
	putchars('_', disk_size);
	putchars(' ', N_DISK - disk_size + 1);
}

void show_disks()
{
	for(int i = 0; i <= N_DISK; ++i) {
		putchar('.');
		for(int j = 0; j < 3; ++j) {
			show_disk(DISKS[i][j], '|');
			putchar('.');
		}
		puts("");
	}
	puts("");
}

void show()
{
	printf("\n\n\n");
	putchar(' ');
	for(int i = 0; i < 3; ++i) {
		show_disk(0, ARROW.column == i ? 'x' : ' ');
		putchar(' ');
	}
	puts("");
	putchar(' ');
	for(int i = 0; i < 3; ++i) {
		show_disk(ARROW.column == i ? ARROW.disk_size : 0, ' ');
		putchar(' ');
	}
	printf("\n\n");
	show_disks();
}

bool move_up()
{
	// return false if sth was picked
	if(ARROW.disk_size != 0)
		return false;
	// find the top disk of current column
	int i;
	for(i = 1; i <= N_DISK; ++i)
		if(DISKS[i][ARROW.column] != 0)
			break;
	// return false if nothing can be picked
	if(i > N_DISK)
		return false;
	// pick
	ARROW.disk_size = DISKS[i][ARROW.column];
	DISKS[i][ARROW.column] = 0;
	return true;
}

bool move_down()
{
	// return false if nothing was picked
	if(ARROW.disk_size == 0)
		return false;
	// find the top disk of current column
	int i;
	for(i = 1; i <= N_DISK; ++i)
		if(DISKS[i][ARROW.column] != 0)
			break;
	// return false if there exist some disks and the top disk is smaller than the disk picked
	if(DISKS[i][ARROW.column] != 0 && DISKS[i][ARROW.column] < ARROW.disk_size)
		return false;
	// put the disk picked
	DISKS[i-1][ARROW.column] = ARROW.disk_size;
	ARROW.disk_size = 0;
	return true;
}

bool move_left()
{
	// return false if can't move left
	if(ARROW.column - 1 < 0)
		return false;
	ARROW.column--;
	return true;
}

bool move_right()
{
	// return false if can't move right
	if(ARROW.column + 1 > 2)
		return false;
	ARROW.column++;
	return true;
}

bool move(char dir)
{
	bool success;
	switch(dir) {
	case UP   :
		success = move_up();
		break;
	case DOWN :
		success = move_down();
		break;
	case LEFT :
		success = move_left();
		break;
	case RIGHT:
		success = move_right();
		break;
	default   :
		printf("invalid input :%c ", dir);
		return false;
	}
	if(!success) {
		printf("can't move :%c(%s) ", dir,
		       dir==UP ? "up" : dir==DOWN ? "down" : dir==LEFT ? "left" : "right");
		return false;
	}
	return true;
}

void moves(char* dirs)
{
	int len = strlen(dirs);
	for(int i = 0; i < len; ++i)
		if(move(dirs[i]) == false) { // return if can't move
			printf("(at %d st/nd/rd/th input)\n", i+1);
			return;
		}
}

char* move_to(int dest)
{
	int src = ARROW.column;
	return src == dest ? "" : src+1 == dest ? "d" : src+2 == dest ? "dd" : src-1 ==
	       dest ? "a" : "aa";
}

void auto_move(char dir)
{
	printf("Press enter to continue:");
	while(getchar() != '\n'); // consume all input until get '\n'
	move(dir);
	show();
}

void auto_moves(char* dirs)
{
	int len = strlen(dirs);
	for(int i = 0; i < len; ++i)
		auto_move(dirs[i]);
}

// move a disk from src to dest
void move_disk(int src, int dest)
{
	auto_moves(move_to(src));
	auto_move(UP);
	auto_moves(move_to(dest));
	auto_move(DOWN);
}

// move n_disk disks from src to dest
void auto_mode(int src, int dest, int n_disk)
{
	if(n_disk == 1) {
		move_disk(src, dest);
		return;
	}
	int another_column = 3 - src - dest;
	auto_mode(src, another_column, n_disk-1);
	move_disk(src, dest);
	auto_mode(another_column, dest, n_disk-1);
}

bool gameover()
{
	for(int i = 1; i <= N_DISK; ++i)
		if(DISKS[i][2] != i)
			return false;
	return true;
}

void initialize()
{
	for(int i = 1; i <= N_DISK; ++i)
		DISKS[i][0] = i;
}

int main()
{
	bool invalid_input;
	int n;
	do {
		printf("Please enter the size (3~9): ");
		invalid_input = false;
		n = scanf("%d", &N_DISK);
		while(getchar() != '\n') // consume all input entered
			invalid_input = true;
	} while(n != 1 || invalid_input || N_DISK < 3 || N_DISK > 9);

	char mode;
	do {
		printf("Enter 'p' to play and 's' for solution: [p/s] ");
		invalid_input = false;
		scanf("%c", &mode);
		while(getchar() != '\n') // consume all input entered
			invalid_input = true;
	} while(invalid_input || (mode != 'p' && mode != 's'));

	initialize();
	show();

	if(mode == 'p') {
		while(!gameover()) {
			char dirs[100];
			puts("w=up, s=down, a=left, d=right. Support multiple input: wds=up->right->down");
			printf("Please enter your movement: ");
			scanf("%s%*c", dirs);
			moves(dirs);
			show();
		}
		puts("You WIN !!!");
	} else
		auto_mode(0, 2, N_DISK);

	puts("Press enter to exit...");
	while(getchar() != '\n');

	return 0;
}
