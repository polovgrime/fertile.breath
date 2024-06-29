#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "main.h"

char *map[] = { 
	"     ---    1111	....	\0", 
	"     - -        111	....	\0",
	"--   - -    3333333	....	\0",
	"           hughhghgy	........\0",
	"			........\0",
	"test      test		........\0"};

char **grid;

int grid_x_length;
int grid_y_length;

game_object **gobjects;
int gobjects_length;

void grid_free()
{
	if (!grid)
	{
		return;
	}
	
	int i = 0;
	for (i = 0; i < grid_y_length; i++)
	{
		free(grid[i]);
	}
	free(grid);
}

void grid_init(int x, int y)
{
	if (grid)
	{
		grid_free();
	}
	grid_x_length = x;
	grid_y_length = y;

	grid = malloc(sizeof(char*) * y);

	int i = 0;

	for (i = 0; i < y; i++)
	{
		grid[i] = malloc(sizeof(char) * x);
	}
}

camera *main_cam;
int read_inputs()
{

        int c = -1;
        return c;
}
void read_write_map()
{
	int map_rows = sizeof(map) / sizeof(char*);

	int i = 0;
	printf("map rows %d\n", map_rows);
	for (i = 0; i < map_rows; i++)
	{
		int j = 0;
		while (map[i][j] != '\0')
		{
			char cell = map[i][j];
			if (cell != ' ')
			{
				vector2 pos = {.x = j, .y = i};
				game_object *gobj = create_gobject("obstacle", 9);
				if (!gobj)
				{
					printf("cant create gobj\n");
					break;
					//log?...
				}
				move_gobject(gobj, pos, 0);
				print_game_object(gobj);
			}

			j++;
		}
		printf("map columns %d\n", j);
	}

	printf("test \n");
}
int main()
{
	recreate_gobjects(120);
	read_write_map();
	//sleep(5);
	grid_init(50, 40);
	//game_object *gobject = malloc(sizeof(game_object));
	//gobject->name = "test name";
	//gobject->name_length = 10;
	game_object *gobject = create_gobject("test name2", 11); 
	//gobject->position.x = 10;
	//gobject->position.y = 10;
	if (gobject == NULL)
	{
		return 1;
	}
	
	vector2 pos = {.x = 0, .y = 3};
	move_gobject(gobject, pos, 0);

	camera *cam = create_cam(5);

	attach_camera(cam, gobject);
	main_cam = cam;
	while (1)
	{
		clear_screen();
		handle_game_logic();	
		display_camera(main_cam);
		render_field();
		print_game_object(main_cam->target);
		sleep(1);
	}


	free_cam(main_cam);
	//free game objects.

	return 0;
}

void render_field()
{

	if (grid == NULL)
	{
		printf("field is null");
		return;
	}

	int y;
	int x;

	for (y = 0; y < grid_y_length; y++)
	{
		if (grid[y] == NULL)
		{
			printf("field row (at y = %d) is null", y);
			continue;
		}
		for (x = 0; x < grid_x_length; x++)
		{
			printf("%c", grid[y][x]);
		}
		printf("\n");
	}
	printf("\n");
}

void handle_game_logic()
{
	int input = read_inputs();
	printf("%d\n", input);

	vector2 right = { .x = 1, .y = 0};

	game_object *gobj = main_cam->target;

	move_gobject(gobj, right, 1);


}

void print_game_object(game_object *gobject)
{
	printf("gameobject\n");
	printf("name: ");
	// todo find a simple string validation technic
	if (!gobject)
	{
		printf("gobject is not valid, null");
	}

	if (gobject->name_length == 0)
	{
		printf("no name for gobject");
	}
	else 
	{
		int length = gobject->name_length;
		if (gobject->name[length-1] != '\0')
		{
			gobject->name[length - 1] = '\0';
		}

		printf("%s\n", gobject->name);
	}

	printf("position: ");
	print_vector2(gobject->position);
}

void print_vector2(vector2 pos)
{
	printf("(%d, %d)\n", pos.x, pos.y);
}

void recreate_gobjects(int length)
{
	if (length < gobjects_length)
	{
		printf("cant decrease gobjects length");
		return;
	}
	if (gobjects)
	{
		//todo refill new array with old values
		return;
	}

	gobjects = malloc(sizeof(game_object*) * length);
	gobjects_length = length;

//implement
}

game_object *create_gobject(char *name, int name_length)
{
	game_object *gobject = malloc(sizeof(game_object));
	gobject->name = name;
	gobject->name_length = name_length;
	gobject->index = -1;

	int i = 0;
	for (i = 0; i < gobjects_length; i++)
	{
		if(gobjects[i] == NULL)
		{
			gobject->index = i;
			gobjects[i] = gobject;
			break;
		}
	}

	if (gobject->index == -1)
	{
		printf("cant indexate gobject, returning null");
		free(gobject);
		return NULL;
	}

	return gobject;
}
int move_gobject(game_object *gobject, vector2 position, int is_relative)
{
	if (is_relative == 0)
	{
		gobject->position = position;
		return 0;
	}

	gobject->position.x += position.x;
	gobject->position.y += position.y;

	return 0;
}
game_object *find_gobject(int index)
{
	if(index > gobjects_length || index < 0)
	{
		return NULL;
	}
	game_object *result = gobjects[index];

	return result;
}

int attach_camera(camera *cam, game_object *gobject)
{
	cam->target = gobject;

	return 0;
}

double vector_distance(vector2 first, vector2 second)
{
	double sum_squares = (first.x - second.x) * (first.x - second.x) +
		(first.y - second.y) * (first.y - second.y);

	return sqrt(sum_squares);
}

void display_camera(camera *cam)
{
	if (cam == NULL)
	{
		printf("cam is null, discard camera display\n");
		return;
	}
	if (cam->target == NULL)
	{
		printf("camera is not attached to anything, discard camera display\n");
		return;
	}

	vector2 cam_pos = cam->target->position;
	int y;
	int x;
	for (y = 0; y < grid_y_length; y++)
	{
		for (x = 0; x < grid_x_length; x++)
		{
			vector2 pos = {.x = x, .y = y};
			write_grid(pos, ' ');

			if (pos.x == 0 || pos.x == grid_x_length - 1 || pos.y == 0 || pos.y == grid_y_length - 1)
			{
				write_grid(pos, 'X');
			}
		}
	}

	int i;

	for (i = 0; i < gobjects_length; i++)
	{
		game_object *gobj = gobjects[i];
		if (gobj == NULL)
		{
			continue;
		}
		double distance = vector_distance(gobj->position, 
		cam_pos);
	//	printf("gobjb#%d positiob (%d,%d)", gobj->index, gobj->position.x, gobj->position.y);
		if (distance < cam->view_distance)
		{
			write_grid(gobj->position, '#');
		}
	}

	write_grid(cam_pos, '@');
}

void write_gobjects()
{
	//what if they are out of bounds?
	
}

void write_grid(vector2 pos, char symbol)
{
	if (pos.x >= grid_x_length)
	{
		printf("x is out of bounds when writing (%d, %d)\n", pos.x, pos.y);
		return;
	}
	
	if (pos.y >= grid_y_length)
	{
		printf("x is out of bounds when writing (%d, %d)\n", pos.x, pos.y);
		return;
	}

	int y = pos.y;
	int x = pos.x;


	grid[y][x] = symbol;
}

camera *create_cam(int view_distance)
{
	camera *cam = malloc(sizeof(camera));

	cam->view_distance = view_distance;

	return cam;
}

void free_cam(camera *cam)
{
	cam->target = NULL;
	free(cam);
}

void clear_screen()
{
    printf("\e[1;1H\e[2J");
}
