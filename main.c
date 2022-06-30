#include<stdio.h>
#include<malloc.h>
#include<math.h>

#include"definitions.h"

void scan_point(struct control_point **conpts, FILE *fp)
{
	fscanf(fp,"%lf", &((*conpts)->Point.x));
	fscanf(fp,"%lf", &((*conpts)->Point.y));
	fscanf(fp,"%lf", &((*conpts)->Point.z));
	fscanf(fp,"%lf", &((*conpts)->weight));
}

void print_control_point(struct control_point *controlpointnode)
{
	struct control_point *temp,*conpts;
	conpts=controlpointnode;
	
	while(controlpointnode!=NULL)
	{
		printf("next_v\n");
		temp=controlpointnode;
	    while(temp!=NULL)
	    {
		    printf("(%lf, %lf, %lf)	",temp->Point.x,temp->Point.y,temp->Point.z);
			//printf("%lf",temp->weight);
		    temp=temp->next_along_u;
        }
		printf("\n");
		controlpointnode=controlpointnode->next_along_v;
	}
	printf("\n\n");

	controlpointnode=conpts;
	while(controlpointnode!=NULL)
	{
		printf("next_u\n");
		temp=controlpointnode;
	    while(temp!=NULL)
	    {
		    printf("(%lf, %lf, %lf)	",temp->Point.x,temp->Point.y,temp->Point.z);
			//printf("%lf",temp->weight);
		    temp=temp->next_along_v;
        }
		printf("\n");
		controlpointnode=controlpointnode->next_along_u;
	}
	printf("\n\n");
}

void allocate_control_point(struct control_point **controlpointnode)
{
	*controlpointnode=(struct control_point*)malloc(sizeof(struct control_point));
}

void get_new_and_prev_conpts_head(struct control_point **new_conpts_head,struct control_point **prev_conpts_head,struct control_point **control_point_head,char parameter)
{
	struct control_point *temp;
	temp=*control_point_head;

	if(parameter=='u')
	{
		if(temp->next_along_v==NULL)
		{
			allocate_control_point(&((*control_point_head)->next_along_v));
			*new_conpts_head=(*control_point_head)->next_along_v;
			*prev_conpts_head=*control_point_head;
		}
		else
		{
			while(temp->next_along_v->next_along_v!=NULL)
				temp=temp->next_along_v;
			allocate_control_point(&(temp->next_along_v->next_along_v));
			*new_conpts_head=temp->next_along_v->next_along_v;
			*prev_conpts_head=temp->next_along_v;
		}
	}

	if(parameter=='v')
	{
		if(temp->next_along_u==NULL)
		{
			allocate_control_point(&((*control_point_head)->next_along_u));
			*new_conpts_head=(*control_point_head)->next_along_u;
			*prev_conpts_head=*control_point_head;
		}
		else
		{
			while(temp->next_along_u->next_along_u!=NULL)
				temp=temp->next_along_u;
			allocate_control_point(&(temp->next_along_u->next_along_u));
			*new_conpts_head=temp->next_along_u->next_along_u;
			*prev_conpts_head=temp->next_along_u;
		}
	}
}

void add_control_points(struct control_point **control_point_head, char text_file[20], char parameter)
{
	struct B_Spline temp;
	struct control_point *temp_to_link;
	struct control_point *new_conpts_head;
	struct control_point *prev_conpts_head;

	FILE *fp;
	fp=fopen(text_file,"r");

	if(*control_point_head==NULL)
	{
		allocate_control_point(control_point_head);
		temp.control_point_head=*control_point_head;
		new_conpts_head=*control_point_head;
		temp_to_link=NULL;
		temp.control_point_head->next_along_u=temp.control_point_head->next_along_v=NULL;
        scan_point(&(temp.control_point_head),fp);
	}
	else
	{
        get_new_and_prev_conpts_head(&new_conpts_head,&prev_conpts_head,control_point_head,parameter);

		temp_to_link=prev_conpts_head;
	    temp.control_point_head=new_conpts_head;
	    temp.control_point_head->next_along_u=temp.control_point_head->next_along_v=NULL;
        scan_point(&(temp.control_point_head),fp);
	}

	if (parameter=='u')
	{
	    while(!feof(fp))
	    {
	        allocate_control_point(&(temp.control_point_head->next_along_u));
			if(temp_to_link!=NULL && temp_to_link->next_along_u!=NULL)
			{
			    temp_to_link=temp_to_link->next_along_u;
				temp_to_link->next_along_v=temp.control_point_head->next_along_u;
			}
		    scan_point(&(temp.control_point_head->next_along_u),fp);
	        temp.control_point_head->next_along_u->next_along_v=temp.control_point_head->next_along_u->next_along_u=NULL;
		    temp.control_point_head=temp.control_point_head->next_along_u;
	    }

	    temp.control_point_head=new_conpts_head;
	    while(temp.control_point_head->next_along_u->next_along_u!=NULL)
	    {
		    temp.control_point_head=temp.control_point_head->next_along_u;
	    }
	    temp.control_point_head->next_along_u=NULL;
	}

	if (parameter=='v')
	{
	    while(!feof(fp))
	    {
	        allocate_control_point(&(temp.control_point_head->next_along_v));
			if(temp_to_link!=NULL && temp_to_link->next_along_v!=NULL)
			{
			    temp_to_link=temp_to_link->next_along_v;
				temp_to_link->next_along_u=temp.control_point_head->next_along_v;
			}
		    scan_point(&(temp.control_point_head->next_along_v),fp);
	        temp.control_point_head->next_along_v->next_along_v=temp.control_point_head->next_along_v->next_along_u=NULL;
		    temp.control_point_head=temp.control_point_head->next_along_v;
	    }

	    temp.control_point_head=new_conpts_head;
	    while(temp.control_point_head->next_along_v->next_along_v!=NULL)
	    {
		    temp.control_point_head=temp.control_point_head->next_along_v;
	    }
	    temp.control_point_head->next_along_v=NULL;
	}

	fclose(fp);
}

void get_knotvector(struct knotvector **knots_head, char text_file[40])
{
	struct knotvector *temp;

	FILE *fp;
	fp=fopen(text_file,"r");

	allocate_knotvector(knots_head);
	temp=*knots_head;
	fscanf(fp,"%lf",&(*knots_head)->knot);

	while(!feof(fp))
	{
		allocate_knotvector(&(temp->next));
	    fscanf(fp,"%lf",&(temp->next)->knot);
		if(!feof(fp))
		{
		    temp=temp->next;
		    temp->next=NULL;
		}
		else
			temp->next=NULL;
	}

}

void allocate_knotvector(struct knotvector **node)
{
	*node=(struct knotvector*)malloc(sizeof(struct knotvector));
}

void print_knotvector(struct knotvector *knotvector_head)
{
	while(knotvector_head!=NULL)
	{
		printf("%lf  ",knotvector_head->knot);
		knotvector_head=knotvector_head->next;
	}
	printf("\n");
}

int main()
{
	struct B_Spline bsplinesurface;

	bsplinesurface.control_point_head=NULL;
	bsplinesurface.knots_along_u_head=bsplinesurface.knots_along_v_head=NULL;
	bsplinesurface.knot_segments_head=NULL;

	add_control_points(&(bsplinesurface.control_point_head),"inputu1.txt",'u');
	add_control_points(&(bsplinesurface.control_point_head),"inputu2.txt",'u');
	add_control_points(&(bsplinesurface.control_point_head),"inputu3.txt",'u');
	add_control_points(&(bsplinesurface.control_point_head),"inputv1.txt",'v');
	
	printf("displaying control points in both directions\n\n");
	print_control_point(bsplinesurface.control_point_head);

	get_knotvector(&(bsplinesurface.knots_along_u_head),"knotvector_along_u.txt");
	get_knotvector(&(bsplinesurface.knots_along_v_head),"knotvector_along_v.txt");

	//print_knotvector(bsplinesurface.knots_along_u_head);
	//print_knotvector(bsplinesurface.knots_along_v_head);

	getchar();
	getchar();
}