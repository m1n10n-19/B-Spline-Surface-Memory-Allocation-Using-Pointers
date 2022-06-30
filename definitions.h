struct point
{
	double x,
		y,
		z;
};

struct control_point
{
	struct point Point;
	double weight;
	struct control_point *next_along_u,
		*next_along_v;
};

struct knotvector
{
	double knot;
	struct knotvector *next;
};

/*struct knot_segment//contains head point of all possible knot segments in a linked list.
{
    struct point *head_point;
	struct knot_segment *next;
};*/

struct B_Spline
{
	struct control_point *control_point_head;

	struct knotvector *knots_along_u_head,
		*knots_along_v_head;

	struct knot_segment *knot_segments_head;
	
	/*int order,
		n_along_u,
		n_along_v;*/
};

#define RESOLUTION 0.01
#define TOLERANCE 0.001

void scan_point(struct control_point **conpts, FILE *fp);

void print_control_point(struct control_point * controlpointnode);

void allocate_control_point(struct control_point **controlpointnode);

void add_control_points(struct control_point **control_point_head, char text_file[20], char parameter);

void get_new_and_prev_conpts_head(struct control_point **new_conpts_head,struct control_point **prev_conpts_head,struct control_point **control_point_head,char parameter);

void get_knotvector(struct knotvector **knots_head,char text_file[40]);

void allocate_knotvector(struct knotvector **node);

void print_knotvector(struct knotvector *knotvector_head);