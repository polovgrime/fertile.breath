
typedef struct vector2 {
	int x;
	int y;
} vector2;

typedef struct game_object {
	int index;//gobject index, should indexate when creating
	vector2 position;// i strongly believe it should not be a reference field.
	char* name;
	int name_length;
} game_object;

void render_field();
void handle_game_logic();
void print_game_object(game_object *gobject);
void print_vector2(vector2 pos);
game_object *create_gobject(char *name, int name_length);

int move_gobject(game_object *gobject, vector2 position, int is_relative); // should return 0 if move was succesful, other results as a code for move didnt happen reason

game_object *find_gobject(int index); //NULL if didn't find

typedef struct camera {
	vector2 point;//offset? do i need it?
	int view_distance;
	game_object *target;
} camera;
camera *create_cam(int view_distance);
void free_cam(camera *cam);
int attach_camera(camera *cam, game_object *gobject); //return error code, please
void display_camera(camera *cam);

void write_gobjects();//writes gobjects to field

void write_grid(vector2 pos, char symbol);

void recreate_gobjects(int length);

void clear_screen();

typedef struct col_obj {
	game_object *gobject;
	vector2* polygon; // is this even optimal?
	int polygon_length;
} col_obj;

int check_collision(vector2 pos, col_obj *collider);

typedef struct sym_view {
	game_object *gobject;
	char symbol;
} sym_view;


