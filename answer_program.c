/* An efficient search algorithm for 2D points:
 *
 * Skeleton code written by Jianzhong Qi, April 2024
 * Edited by: [Add your name and student ID here for automatic recognition]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STAGE_NUM_ONE 1							/* stage numbers */
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_HEADER "Stage %d\n==========\n"	/* stage header format string */

#define DATA_SET_SIZE 50						/* number of input POIs */
#define DATA_DIM 2								/* POI point dimensionality */
#define QUERY_SET_SIZE 5						/* number of input queries */
#define QUERY_BOUNDS 4							/* query range bounds */

#define NUM_BITS 3								/* number of bits for col_/row_num */

#define BS_NOT_FOUND (-1)						/* return status of binary search */
#define BS_FOUND 0
/****************************************************************/

/* function prototypes */
void print_stage_header(int stage_num);
void int_swap(int *p1, int *p2);
void sort_by_x(int ids[], double coordinates[][DATA_DIM], int num_pois);
int cal_curve_value(int col_num, int row_num);
int cmp(int *x1, int *x2);
int binary_search(int curve_values[], int lo, int hi, 
	int *key, int *locn);

void stage_one(int ids[], double coordinates[][DATA_DIM], int num_pois, 
	double queries[][QUERY_BOUNDS], int num_queries);
void stage_two(int ids[], double coordinates[][DATA_DIM], int num_pois, 
	double queries[][QUERY_BOUNDS], int num_queries);
void stage_three(int ids[], double coordinates[][DATA_DIM], int curve_values[], 
	int num_pois);
void stage_four(int ids[], double coordinates[][DATA_DIM], int curve_values[], 
	int num_pois, double queries[][QUERY_BOUNDS], int num_queries);

/* add your own function prototypes here */

// determine whether a point lies in a query range
// (x, y) is the coordinate of that point
// (x_lb, y_lb, x_ub, y_ub) are coordinates of bottom-left and above-right endpoints
inline static int lie_in(double x, double y, double x_lb, double y_lb, double x_ub, double y_ub);

// exchange values of two doubles
void double_swap(double *p1, double *p2);

// according to the algorithm descriptions in the assignemtns, compute curve values given a point (x, y)
inline static int calculate_curve_value_by_coordinates(double x, double y);

// insertion sort based on curve values, into the non-decreasing order
void sort_by_curve_values(int ids[], double coordinates[][DATA_DIM], int num_pois, int curve_values[]);


// according to the algorithm descriptions in the assignments, compute upper and lower bounds of a query range
void calculate_lb_ub_for_query(double queries[][QUERY_BOUNDS], int id, int *ptr_to_v_lb, int *ptr_to_v_ub);

/****************************************************************/

/* main function controls all the action */
int
main(int argc, char *argv[]) {
	/* to hold all input data */
	int ids[DATA_SET_SIZE];
	double coordinates[DATA_SET_SIZE][DATA_DIM];
	double queries[QUERY_SET_SIZE][QUERY_BOUNDS];
	int curve_values[DATA_SET_SIZE];

	/* stage 1: read the input and answer the first query */
	stage_one(ids, coordinates, DATA_SET_SIZE, queries, QUERY_SET_SIZE);
	
	/* stage 2: sort and query POIs by the x-coordinates */
	stage_two(ids, coordinates, DATA_SET_SIZE, queries, QUERY_SET_SIZE);
	
	/* stage 3: calculate and sort by curve values */ 
	stage_three(ids, coordinates, curve_values, DATA_SET_SIZE);
	
	/* stage 4: query by curve values */
	stage_four(ids, coordinates, curve_values, DATA_SET_SIZE, 
		queries, QUERY_SET_SIZE);
	
	/* all done; take some rest */
	return 0;
}

/* print stage header given stage number */
void 
print_stage_header(int stage_num) {
	printf(STAGE_HEADER, stage_num);
}

/****************************************************************/
/* add your code below */

inline static int lie_in(double x, double y, double x_lb, double y_lb, double x_ub, double y_ub)
{
	if(x < x_lb || x > x_ub) return 0;
	if(y < y_lb || y > y_ub) return 0;
	return 1;
}

/* stage 1: read the input and answer the first query */
void 
stage_one(int ids[], double coordinates[][DATA_DIM], int num_pois, 
	double queries[][QUERY_BOUNDS], int num_queries) {
	/* add code for stage 1 */
	/* print stage header */
	print_stage_header(STAGE_NUM_ONE);
	int index;
	char temp_str[1024];
	double x_cood, y_cood;
	do
	{
		scanf("%d%lf%lf", &index, &x_cood, &y_cood);
		scanf("%s", temp_str);
		// printf("%d %.1lf %.1lf;\n", index, x_cood, y_cood);
		ids[index] = index;
		coordinates[index][0] = x_cood;
		coordinates[index][1] = y_cood;
		if(index + 1 == num_pois) 
			break;
	} while (1);
	
	double x_lb, y_lb, x_ub, y_ub;
	int query_range_count = 0;
	do
	{
		if(query_range_count == num_queries) 
			break;
		scanf("%lf%lf%lf%lf", &x_lb, &y_lb, &x_ub, &y_ub);
		// printf("having read %.1lf %.1lf %.1lf %.1lf\n", x_lb, y_lb, x_ub, y_ub);
		queries[query_range_count][0] = x_lb;
		queries[query_range_count][1] = y_lb;
		queries[query_range_count][2] = x_ub;
		queries[query_range_count][3] = y_ub;	
		query_range_count++;
		
	} while (1);

	// working on query 1
	x_lb = queries[0][0];
	y_lb = queries[0][1];
	x_ub = queries[0][2];
	y_ub = queries[0][3];
	// printf("%.1lf %.1lf %.1lf %.1lf\n", x_lb, y_lb, x_ub, y_ub);
	printf("POIs in Q0: ");
	int poi_exist = 0;
	for(int i = 0; i < DATA_SET_SIZE; i++)
	{
		double x = coordinates[i][0];
		double y = coordinates[i][1];
		// if(i == 4) printf("%.1lf %.1lf\n", x, y);
		if(lie_in(x, y, x_lb, y_lb, x_ub, y_ub))
		{
			printf("%d ", i);	
			poi_exist = 1;
		}	
	}
	if(!poi_exist)
	{
		printf("none");
	}
	printf("\n");
	printf("\n");
}

/* stage 2: sort and query POIs by the x-coordinates */
void 
stage_two(int ids[], double coordinates[][DATA_DIM], int num_pois, 
	double queries[][QUERY_BOUNDS], int num_queries) {
	/* add code for stage 2 */
	/* print stage header */
	print_stage_header(STAGE_NUM_TWO);
	
	sort_by_x(ids, coordinates, num_pois);
	// printf("after sorting by x-coord\n");
	// for(int i = 0; i < num_pois; i++)
	// {
	// 	printf("%d %.1lf %.1lf\n", ids[i], coordinates[i][0], coordinates[i][1]);
	// }

	for(int query_id = 0; query_id < num_queries; query_id++)
	{
		double x_lb = queries[query_id][0]; 
		double y_lb = queries[query_id][1];
		double x_ub = queries[query_id][2];
		double y_ub = queries[query_id][3];
		int num_of_point_in_query_tests = 0;
		int number_of_points_inside = 0;
		printf("POIs in Q%d:", query_id);
		for(int i = 0; i < num_pois; i++)
		{
			
			int point_id = ids[i];
			double x = coordinates[i][0];
			double y = coordinates[i][1];
			if(x < x_lb)
				continue;
			if(x > x_ub)
				break;
			num_of_point_in_query_tests++;
			if(y < y_lb || y > y_ub) 
				continue;
			// printf("x_lb, y_lb, x_ub, y_ub: %.1lf, %.1lf, %.1lf, %.1lf\n", x_lb, y_lb, x_ub, y_ub);
			// printf("x, y: %.1lf, %.1lf\n", x, y);
			printf(" %d", point_id);
			number_of_points_inside++;
		}
		if(!number_of_points_inside)
		{
			printf(" none");
		}
		printf("; ");
		printf("No. POI-in-query tests: %d\n", num_of_point_in_query_tests);
	}
	printf("\n");
}

/* exchange the values of the two int variables indicated 
	by the arguments, from
	https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/insertionsort.c */
void
int_swap(int *p1, int *p2) {
	int tmp;
	tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

void
double_swap(double *p1, double *p2)
{
	double tmp;
	tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

/* insertion sort by x-coordinates, adapted from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/insertionsort.c
*/
void
sort_by_x(int ids[], double coordinates[][DATA_DIM], int num_pois) {
	/* modify the code below to perform insertion sort to sort coordinates 
		and ids by the x-coordinates
	*/
	
	int i, j; 
	// assume that A[0] to A[n-1] have valid values
	for (i = 1; i < num_pois; i++) {
		// swap A[i] left into correct position 
		for (j = i - 1; j >= 0 && coordinates[j+1][0] < coordinates[j][0]; j--) {
			// not there yet 
			double_swap(&coordinates[j][0], &coordinates[j+1][0]);
			double_swap(&coordinates[j][1], &coordinates[j+1][1]);
			int_swap(&ids[j], &ids[j+1]);
		}
	}
}

inline static int 
calculate_curve_value_by_coordinates(double x, double y)
{
	double cell_side = (double) 100 / 8; // refer to assumptions in the assignment
	int col_num = (int)ceil(x / cell_side) - 1;
	int row_num = (int)ceil(y / cell_side) - 1;
	// printf("col_num, row_num: %d, %d\n", col_num, row_num);
	return cal_curve_value(col_num, row_num);
}

void
sort_by_curve_values(int ids[], double coordinates[][DATA_DIM], int num_pois, int curve_values[])
{
	int i, j;
	for (i = 0; i < num_pois; i++)
	{
		
		double x = coordinates[i][0];
		double y = coordinates[i][1];
		curve_values[i] = calculate_curve_value_by_coordinates(x, y);
		// printf("curve values of %d-th (%d, %.1lf, %.1lf) is %d\n", i + 1, ids[i], x, y, curve_values[i]);
	}

	for (i = 1; i < num_pois; i++)
	{
		for (j = i - 1; 
				j >= 0 && ((curve_values[j+1] < curve_values[j]) || (curve_values[j+1] == curve_values[j] && ids[j+1] < ids[j])); 
						j--)
		{
			// not there yet 
			double_swap(&coordinates[j][0], &coordinates[j+1][0]);
			double_swap(&coordinates[j][1], &coordinates[j+1][1]);
			int_swap(&ids[j], &ids[j+1]);
			int_swap(&curve_values[j], &curve_values[j+1]);
		}
	}
}

/* stage 3: calculate and sort by curve values */ 
void 
stage_three(int ids[], double coordinates[][DATA_DIM], int curve_values[], 
	int num_pois) {
	/* add code for stage 3 */
	/* print stage header */
	print_stage_header(STAGE_NUM_THREE);
	sort_by_curve_values(ids, coordinates, num_pois, curve_values);
	// printf("after sorting by curve values (shown as the last)\n");
	// for(int i = 0; i < num_pois; i++)
	// {
	// 	printf("%d %d %.1lf %.1lf %d\n", i, ids[i], coordinates[i][0], coordinates[i][1], curve_values[i]);
	// }
	for(int i = 0; i < 5; i++)
	{
		printf("POI #%02d @ (%04.1f, %04.1f), curve value: %2d\n", ids[i], coordinates[i][0], coordinates[i][1], curve_values[i]);
	}
	printf("\n");
}

/* calculate the curve value given column number and row number
   DO NOT MODIFY THIS FUNCTION
*/
int 
cal_curve_value(int col_num, int row_num) {
	int i, result = 0;

	for (i = NUM_BITS-1; i >= 0; i--) {
		result += (row_num & (1 << i)) << (i+1);
		result += (col_num & (1 << i)) << i;
	}

	return result;
}

/* comparison function used by binary search, adapted from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/binarysearch.c 
*/
int
cmp(int *x1, int *x2) {
	return (*x1-*x2);
}

/* binary search over curve values, adapted from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/binarysearch.c
*/
int
binary_search(int curve_values[], int lo, int hi, 
		int *key, int *locn) {
	/* modify the code below to perform binary search and find the smallest 
		curve value greater than or equal to *key
	*/
	
	int mid, outcome;

	if (lo>=hi) {
		// printf("low and high: %d, %d, values: %d, %d (end)\n", lo, hi, curve_values[lo], curve_values[hi]);
		if(curve_values[hi] == *key)
			*locn = hi; // having found the first occurrence of the value pointed to by key
		else if(curve_values[hi] > *key)
			*locn = hi; // the last operation is low = mid + 1
		else
			*locn = hi + 1; // the last operation is high = mid
		// printf("return %d from bin search\n", *locn);
		// printf("%d ", *locn);
		return BS_NOT_FOUND;
	}

	// printf("low and high: %d, %d, values: %d, %d\n", lo, hi, curve_values[lo], curve_values[hi]);
	mid = (lo+hi)/2;
	printf("%d ", curve_values[mid]);
	// printf("curve values checked: %d \n", curve_values[mid]);

	if ((outcome = cmp(key, curve_values+mid)) < 0) {
		return binary_search(curve_values, lo, mid, key, locn);
	} 
	else if (outcome > 0) {

		return binary_search(curve_values, mid+1, hi, key, locn);
	} 
	else {
		return binary_search(curve_values, lo, mid, key, locn);
	}
	
	return BS_FOUND;
}

void calculate_lb_ub_for_query(double queries[][QUERY_BOUNDS], int id, int *ptr_to_v_lb, int *ptr_to_v_ub)
{
	double x_lb = queries[id][0], y_lb = queries[id][1]; 
	double x_ub = queries[id][2], y_ub = queries[id][3];
	// printf(": %.1lf, %.1lf, %.1lf, %.1lf\n", x_lb, y_lb, x_ub, y_ub);
	*ptr_to_v_lb = calculate_curve_value_by_coordinates(x_lb, y_lb); 
	*ptr_to_v_ub = calculate_curve_value_by_coordinates(x_ub, y_ub);
}

/* stage 4: query by curve values */
void 
stage_four(int ids[], double coordinates[][DATA_DIM], int curve_values[],
	int num_pois, double queries[][QUERY_BOUNDS], int num_queries) {
	/* add code for stage 4 */
	/* print stage header */
	print_stage_header(STAGE_NUM_FOUR);
	for(int query_id = 0; query_id < num_queries; query_id++)
	{
		double x_lb = queries[query_id][0]; 
		double y_lb = queries[query_id][1];
		double x_ub = queries[query_id][2];
		double y_ub = queries[query_id][3];
		int v_lb, v_ub;
		calculate_lb_ub_for_query(queries, query_id, &v_lb, &v_ub);
		printf("Q%d: [%02d, %02d]; ", query_id, v_lb, v_ub);
		int location = 0;
		printf("curve values compared: ");
		binary_search(curve_values, 0, num_pois, &v_lb, &location);
		// int curve_value_lb_poi_id = ids[location];
		int first_checked_index = location; // the first entry to check
		int index = first_checked_index; // use to iterate through the point array
		printf("\n");
		// printf("curve_value_lb_poi_id: %d\n", curve_value_lb_poi_id);
		printf("POIs in Q%d:", query_id);
		int point_test_count = 0;
		int point_inside_count = 0;
		while(curve_values[index] <= v_ub)
		{
			//
			point_test_count++;
			double x = coordinates[index][0];
			double y = coordinates[index][1];
			if(lie_in(x, y, x_lb, y_lb, x_ub, y_ub))
			{
				point_inside_count++;
				printf(" %d", ids[index]);
			}
			index++;
		}
		if(!point_inside_count)
		{
			printf(" none");
		}
		printf("; ");
		printf("No. POI-in-query tests: %d", point_test_count);
		printf("\n");
	}
	printf("\n");	
}