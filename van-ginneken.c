#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define row 100
#define col 100
#define cwire 1
#define cbuff 1
#define rwire 1
#define rbuff 1
#define tb 1
#define rdriver 1

typedef struct cal
{
	int cap;	
	int rat;
	int buff;
	int slack;
	int prev_cap;
	int prev_rat;
}cal;


typedef struct nd
{
	cal sol;
	int index;
	struct nd *left;
	struct nd *right;
}nd;	

typedef struct level_backtrack
{
	int index;
	nd* ver;
}level_backtrack;	

typedef nd *tree1;

cal calssink[100];
int arr[100][100];
int traversal[100];
int element[100];
int levelmax;
		

void swap(cal *xp, cal *yp)
{
	cal temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void selectionSort(int n, int trav, int r, int c, cal ar[r][c])
{
	int i, j, min_idx;

	// One by one move boundary of unsorted subarray
	for (i = 0; i < n-1; i++)
	{
		// Find the minimum element in unsorted array
		min_idx = i;
		for (j = i+1; j < n; j++)
		if (ar[trav][j].cap < ar[trav][min_idx].cap)
			min_idx = j;

		// Swap the found minimum element with the first element
		swap(&ar[trav][min_idx], &ar[trav][i]);
	}
}

int prune(int n, int trav, int r, int c, cal ar[r][c])
{
	int i, j, pos;
	for(i=0;i<n-1;i++)
    	{
        	if(ar[trav][i].rat>ar[trav][i+1].rat)
        	{
        		pos=i+1;
        		for(j=pos; j<n-1; j++)
            			ar[trav][j] = ar[trav][j + 1];
        		n--;
        		i=0;
			continue;
        	}
        	else
        		continue;
    	}

	return n;
}


typedef struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
}Node;


typedef Node *Tree;


typedef struct Level
{
    int data;
    int level;
    Node *node;
}Level;

level_backtrack levels_backtrack[100];
Level levels[100];
Level nodelevel[100];


int edge_wt[100][100];
int prev[100];

int sortLevel(const void *a, const void *b)
{
    const Level *x = a;
    const Level *y = b;

    return x->level - y->level;

}

Tree constructTreeFromAncestorMatrix(int n, int ancestors[n][n])
{
    //Calculate level of each node (level is the num of ancestor nodes)
    

    int i, j;

    for(i = 0; i < n; i++)
    {
        levels[i].data = i;
        levels[i].level = 0;

        for(j = 0; j < n; j++)
            levels[i].level += ancestors[i][j];
    }


    //sort nodes based on level
    qsort(levels, n, sizeof(Level), sortLevel);


    //first node level should be zero
    if(levels[0].level != 0)
        return NULL;

    //Construct root node;
    Tree t = (Node *)calloc(1, sizeof(Node));
    t->data = levels[0].data;
    levels[0].node = t;


    //Link nodes to the  ancestor in prev level
    Node *node;

    int prevLevelStart = 0;
    int levelStart = 0;
    int curlevel = 0;

    for(i = 1; i < n; i++)
    {
        //allocate the node
        node = (Node *)calloc(1, sizeof(Node));
        node->data = levels[i].data;
        levels[i].node = node;

        //Check level change
        if(curlevel != levels[i].level)
        {
            prevLevelStart = levelStart;
            levelStart = i;
            curlevel = levels[i].level;
        }

        //find the parent in prev level
        for(j = prevLevelStart; j <levelStart; j++)
        {
            if(ancestors[levels[i].data][levels[j].data])
            {
                //attach to left or right based on free slot
                Node *parent = levels[j].node;
                if(!parent->left)
                {
                    parent->left = node;
			prev[levels[i].data] = levels[j].data;
                }
                else if(!parent->right)
                {
                    parent->right = node;
			prev[levels[i].data] = levels[j].data;
                }
                else //invalid binary tree
                {
                    return NULL;
                }

                break;
            }
        }
    }
    return t;
}
	
int i1=0;
void postorder(Tree t)
{
	
    if(!t) return;
	postorder(t->left);
	
	
	postorder(t->right);
   	
	//printf("%d %d", t->sol.cap, t->sol.rat);

	traversal[i1] = t->data;
	i1++;    
    
}

void postorder_new(tree1 t)
{
	
    if(!t) return;
	postorder_new(t->left);
	
	
	postorder_new(t->right);
   	
	printf("%d %d", t->sol.cap, t->sol.rat);
	printf("\n");

	//traversal[i1] = t->data;
	//i1++;    
    
}


int max_level(int n)
{
	int max = -999;
	int i;
	for(i=0; i<n;i++)
	{
		if(nodelevel[i].level > max)
			max = nodelevel[i].level;
	}
	return max;
}

void backtrack(cal slack_max, int ee, int cnt[ee], int cntl1[ee], int cntr1[ee], int c, int ro, cal taul[ro][c], cal taur[ro][c], cal tau[ro][c]);

void propagation(int n)
{	
	int v = 2*n;
	int k,i, a, b, ew, cnt[100];
	cal s0;
	cal taul[100][100], taur[100][100], tau[100][100];
		
	int cntl, cntr;
	int cntl1[100], cntr1[100];
	for(i=1;i<=n;i++)
	{
		tau[i][0].cap = calssink[i].cap;
		tau[i][0].rat = calssink[i].rat;
		cnt[i] = 1;
	}

	for(i=0; i<2*n; i++)
	{
		cntl =0; 
		cntr =0;
		if(((traversal[i]>=(n+1)) && (traversal[i]<=(2*n-1))) || (traversal[i]==0) )
		{
		   if(nodelevel[traversal[i]].node->left)
		    {					
			if((nodelevel[traversal[i]].node->left->data >=1) && (nodelevel[traversal[i]].node->left->data<=n))
			{	
				a = nodelevel[traversal[i]].node->left->data;
				ew = edge_wt[traversal[i]][a];
				taul[traversal[i]][cntl].cap =  calssink[a].cap + ew * cwire;
				s0.cap = taul[traversal[i]][cntl].cap;
				taul[traversal[i]][cntl].rat = calssink[a].rat - ((rwire)*(cwire)*(ew*ew)*(0.5)) - ((rwire)*(calssink[a].cap)*ew);
				s0.rat = taul[traversal[i]][cntl].rat;
				taul[traversal[i]][cntl].buff = 0;
				taul[traversal[i]][cntl].prev_cap = calssink[a].cap;
				taul[traversal[i]][cntl].prev_rat = calssink[a].rat;							
				cntl++;
				taul[traversal[i]][cntl].cap = cbuff;
				taul[traversal[i]][cntl].rat = s0.rat - tb - (rbuff*s0.cap);
				taul[traversal[i]][cntl].buff = 1;
				taul[traversal[i]][cntl].prev_cap = calssink[a].cap;
				taul[traversal[i]][cntl].prev_rat = calssink[a].rat;							

				cntl++;
			}

			else if((nodelevel[traversal[i]].node->left->data >=n+1) && (nodelevel[traversal[i]].node->left->data<= 2*n-1))
			{
				a = nodelevel[traversal[i]].node->left->data;
				ew = edge_wt[traversal[i]][a];
				for(k=0; k< cnt[a]; k=k+1)
				{	
					taul[traversal[i]][cntl].cap = tau[a][k].cap + ew*cwire; 
					s0.cap = taul[traversal[i]][cntl].cap;
					taul[traversal[i]][cntl].rat = tau[a][k].rat - ((rwire)*(cwire)*(ew*ew)*(0.5)) - ((rwire)*(tau[a][k].cap)*ew);
					s0.rat = taul[traversal[i]][cntl].rat;
					taul[traversal[i]][cntl].buff = 0;
					taul[traversal[i]][cntl].prev_cap = tau[a][k].cap;
					taul[traversal[i]][cntl].prev_rat = tau[a][k].rat;							

					cntl++;				
					taul[traversal[i]][cntl].cap = cbuff;
					taul[traversal[i]][cntl].rat = s0.rat - tb - (rbuff*s0.cap);
					taul[traversal[i]][cntl].buff = 1;
					taul[traversal[i]][cntl].prev_cap = tau[a][k].cap;
					taul[traversal[i]][cntl].prev_rat = tau[a][k].rat;							

					cntl++;
				}
			}
			selectionSort(cntl, traversal[i], row, col, taul);
			cntl = prune(cntl, traversal[i], row, col, taul);
			cntl1[traversal[i]] = cntl;
			}
		    
		     if(nodelevel[traversal[i]].node->right)		
                      {
			if((nodelevel[traversal[i]].node->right->data >=1) && (nodelevel[traversal[i]].node->right->data<=n))
			{	
				a = nodelevel[traversal[i]].node->right->data;
				ew = edge_wt[traversal[i]][a];
				taur[traversal[i]][cntr].cap =  calssink[a].cap + ew * cwire;
				s0.cap = taur[traversal[i]][cntr].cap;
				taur[traversal[i]][cntr].rat = calssink[a].rat - ((rwire)*(cwire)*(ew*ew)*(0.5)) - ((rwire)*(calssink[a].cap)*ew);
				s0.rat = taur[traversal[i]][cntr].rat;
				taur[traversal[i]][cntr].buff = 0;
				taur[traversal[i]][cntr].prev_cap = calssink[a].cap;
				taur[traversal[i]][cntr].prev_rat = calssink[a].rat;							
						
				cntr++;
				taur[traversal[i]][cntr].cap = cbuff;
				taur[traversal[i]][cntr].rat = s0.rat - tb - (rbuff*s0.cap);
				taur[traversal[i]][cntr].buff = 1;
				taur[traversal[i]][cntr].prev_cap = calssink[a].cap;
				taur[traversal[i]][cntr].prev_rat = calssink[a].rat;							
	
				cntr++;
				
			}

			else if((nodelevel[traversal[i]].node->right->data >=n+1) && (nodelevel[traversal[i]].node->right->data<= 2*n-1))
			{
				a = nodelevel[traversal[i]].node->right->data;
				ew = edge_wt[traversal[i]][a];
				for(k=0; k< cnt[a]; k=k+1)
				{	
					taur[traversal[i]][cntr].cap = tau[a][k].cap + ew*cwire;
					s0.cap = taur[traversal[i]][cntr].cap;
					taur[traversal[i]][cntr].rat = tau[a][k].rat - ((rwire)*(cwire)*(ew*ew)*(0.5)) - ((rwire)*(tau[a][k].cap)*ew);
					s0.rat = taur[traversal[i]][cntr].rat;
					taur[traversal[i]][cntr].buff = 0;
					taur[traversal[i]][cntr].prev_cap = tau[a][k].cap;
					taur[traversal[i]][cntr].prev_rat = tau[a][k].rat;							

					cntr++;				
					taur[traversal[i]][cntr].cap = cbuff;
					taur[traversal[i]][cntr].rat = s0.rat - tb - (rbuff*s0.cap);
					taur[traversal[i]][cntr].buff = 1;
					taur[traversal[i]][cntr].prev_cap = tau[a][k].cap;
					taur[traversal[i]][cntr].prev_rat = tau[a][k].rat;							

					cntr++;
					
					//printf("%d\n", cnt[a]);
				}
			}
			
			selectionSort(cntr, traversal[i], row, col, taur);
			cntr = prune(cntr, traversal[i], row, col, taur);
			cntr1[traversal[i]] = cntr;
			}

			int q=0, e=0, r=0;
			// merge taul and taur into tau
			if(nodelevel[traversal[i]].node->left && nodelevel[traversal[i]].node->right)
			{
				for(q=0; q<cntl;q++)
				{
					for(e=0; e<cntr; e++)
					{
						tau[traversal[i]][r].cap = taul[traversal[i]][q].cap + taur[traversal[i]][e].cap;
						if(taul[traversal[i]][q].rat<=taur[traversal[i]][e].rat)
							tau[traversal[i]][r].rat =  taul[traversal[i]][q].rat;
						else
							tau[traversal[i]][r].rat = taur[traversal[i]][e].rat;
						r++;
					}
				}
			}

			else if(nodelevel[traversal[i]].node->left)
			{
				for(q=0; q<cntl; q++)
					tau[traversal[i]][q] = taul[traversal[i]][q];
			}	 
						
			cnt[traversal[i]] = cntl + cntr;
			selectionSort(cnt[traversal[i]], traversal[i], row, col, tau);
			cnt[traversal[i]] = prune(cnt[traversal[i]], traversal[i], row, col, tau);
				
		}		
	}
	cal  slacks[100];
	cal slack_max;
	for(k=0; k<cnt[0]; k++)
	{
			slacks[k].rat =	tau[0][k].rat;
			slacks[k].cap = tau[0][k].cap;	 
			slacks[k].slack = tau[0][k].rat - (rbuff)*(tau[0][k].cap);
	}

	int max = -999;
	for(k=0; k<cnt[0];k++)
	{
		if(slacks[k].slack >= max)
		{	
			max = slacks[k].slack;
			slack_max = slacks[k];	
		}
	}
	
	backtrack(slack_max, v, cnt, cntl1, cntr1, row, col, taul, taur, tau);
}


void backtrack(cal slack_max, int ee, int cnt[ee], int cntl1[ee], int cntr1[ee], int c, int ro, cal taul[ro][c], cal taur[ro][c], cal tau[ro][c])
{
	int n = (ee*0.5);
	int buff_insert[100][100];
	int count_buff=0, loop;
	int i, j, k, m, r, rem_cap, ew1;
	for(i=0;i<100;i++)
	{
		for(j=0;j<100;j++)
			buff_insert[i][j]=0;
	}	
	cal nw, nb;
	cal s0,s2;
	for(i = 0; i < 2*n; i++)
    	{
        	levels_backtrack[i].index = i;
	}
	
	
	tree1 t = (nd *)calloc(1, sizeof(nd));
    	t->sol = slack_max;
    	t->index = levels_backtrack[0].index;
  	levels_backtrack[0].ver = t;

	
	for(i=0; i<=levelmax; i++)
	{
		for(j=0; j<element[i]; j++)
		{
		   if(((nodelevel[arr[i][j]].node->data >= n+1) && (nodelevel[arr[i][j]].node->data <= 2*n-1)) ||(nodelevel[arr[i][j]].node->data==0))
		   {
      				if((nodelevel[arr[i][j]].node->left) && (nodelevel[arr[i][j]].node->right))	
				{
					for(k=0; k<cntl1[arr[i][j]];k++)
					{
						nd *parent = levels_backtrack[arr[i][j]].ver;
						if(parent->sol.rat == taul[arr[i][j]][k].rat)
						{
							s0 = taul[arr[i][j]][k];
							if(s0.buff == 1)
							{
								count_buff++; 
								buff_insert[arr[i][j]][nodelevel[arr[i][j]].node->left->data] = 1;
							}	
							//ew1 = edge_wt[arr[i][j]][nodelevel[arr[i][j]].node->left->data];
							nw.cap = s0.prev_cap; 
							nw.rat = s0.prev_rat;

							if(nodelevel[arr[i][j]].node->left->left && !(nodelevel[arr[i][j]].node->left->right))
									{
										
									for(loop=0; loop<cnt[nodelevel[arr[i][j]].node->left->data]; loop++)
									{	
									if((nw.cap == tau[nodelevel[arr[i][j]].node->left->data][loop].cap) && (nw.rat == tau[nodelevel[arr[i][j]].node->left->data][loop].rat))
										nw = tau[nodelevel[arr[i][j]].node->left->data][loop];
								
									}
									}
									
							
							tree1 newd = (nd *)calloc(1, sizeof(nd));
        						newd->sol = nw;
							newd->index = nodelevel[arr[i][j]].node->left->data;
        						levels_backtrack[newd->index].ver = newd;
							parent->left = newd;
									
							
							rem_cap = parent->sol.cap - s0.cap;
							for(m=0; m<cntr1[arr[i][j]];m++)
							{
								if(rem_cap == taur[arr[i][j]][m].cap)
								{
									s2 = taur[arr[i][j]][m];
									if(s2.buff == 1)
									{
										count_buff++;
										buff_insert[arr[i][j]][nodelevel[arr[i][j]].node->right->data] = 1;
									}				
									//ew1 = edge_wt[arr[i][j]][nodelevel[arr[i][j]].node->right->data];
									nw.cap = s2.prev_cap;
									nw.rat = s2.prev_rat;
									if(nodelevel[arr[i][j]].node->right->left && !(nodelevel[arr[i][j]].node->right->right))
									{
										
									for(loop=0; loop<cnt[nodelevel[arr[i][j]].node->right->data]; loop++)
									{	
									if((nw.cap == tau[nodelevel[arr[i][j]].node->right->data][loop].cap) && (nw.rat == tau[nodelevel[arr[i][j]].node->right->data][loop].rat))
										nw = tau[nodelevel[arr[i][j]].node->right->data][loop];
								
									}
									}
									
									tree1 newd = (nd *)calloc(1, sizeof(nd));
        								newd->sol = nw;
									newd->index = nodelevel[arr[i][j]].node->right->data;
        								levels_backtrack[newd->index].ver = newd;
									parent->right = newd;
											
								}
							}
						}
					}
			
					
/////////////////////////	///////////////
					for(k=0; k<cntr1[arr[i][j]];k++)
					{
						nd *parent = levels_backtrack[arr[i][j]].ver;
						if(parent->sol.rat == taur[arr[i][j]][k].rat)
						{
							s0 = taur[arr[i][j]][k];
							if(s0.buff == 1)
							{
								count_buff++; 
								buff_insert[arr[i][j]][nodelevel[arr[i][j]].node->right->data] = 1;
							}
							//ew1 = edge_wt[arr[i][j]][nodelevel[arr[i][j]].node->right->data];
							nw.cap = s0.prev_cap;
							nw.rat = s0.prev_rat;

							if(nodelevel[arr[i][j]].node->right->left && !(nodelevel[arr[i][j]].node->right->right))
									{
										
									for(loop=0; loop<cnt[nodelevel[arr[i][j]].node->right->data]; loop++)
									{	
									if((nw.cap == tau[nodelevel[arr[i][j]].node->right->data][loop].cap) && (nw.rat == tau[nodelevel[arr[i][j]].node->right->data][loop].rat))
										nw = tau[nodelevel[arr[i][j]].node->right->data][loop];
								
									}
									}
									
									
							tree1 newd = (nd *)calloc(1, sizeof(nd));
        						newd->sol = nw;
							newd->index = nodelevel[arr[i][j]].node->right->data;
        						levels_backtrack[newd->index].ver = newd;
							parent->right = newd;
							
							rem_cap = parent->sol.cap - s0.cap;
							for(m=0; m<cntl1[arr[i][j]];m++)
							{
								if(rem_cap == taul[arr[i][j]][m].cap)
								{
									s2 = taul[arr[i][j]][m];
									if(s2.buff == 1)
									{
										count_buff++;
										buff_insert[arr[i][j]][nodelevel[arr[i][j]].node->left->data] = 1;
									}
									//ew1 = edge_wt[arr[i][j]][nodelevel[arr[i][j]].node->left->data];
									nw.cap = s2.prev_cap;
									nw.rat = s2.prev_rat;

									
							if(nodelevel[arr[i][j]].node->left->left && !(nodelevel[arr[i][j]].node->left->right))
									{
										
									for(loop=0; loop<cnt[nodelevel[arr[i][j]].node->left->data]; loop++)
									{	
									if((nw.cap == tau[nodelevel[arr[i][j]].node->left->data][loop].cap) && (nw.rat == tau[nodelevel[arr[i][j]].node->left->data][loop].rat))
										nw = tau[nodelevel[arr[i][j]].node->left->data][loop];
								
									}
									}
									
									tree1 newd = (nd *)calloc(1, sizeof(nd));
        								newd->sol = nw;
									newd->index = nodelevel[arr[i][j]].node->left->data;
        								levels_backtrack[newd->index].ver = newd;
									parent->left = newd;

								}
							}
						}
					}

				}
				else if((nodelevel[arr[i][j]].node->left) && !(nodelevel[arr[i][j]].node->right))
				{
					for(k=0; k<cntl1[arr[i][j]];k++)
					{
							nd *parent = levels_backtrack[arr[i][j]].ver;
							s0 = parent->sol;
							
							if(s0.buff == 1)
							{
								count_buff++;
								buff_insert[arr[i][j]][nodelevel[arr[i][j]].node->left->data] = 1;
							}
							//ew1 = edge_wt[arr[i][j]][nodelevel[arr[i][j]].node->left->data];
							nw.cap = s0.prev_cap;
							nw.rat = s0.prev_rat;
									
							tree1 newd = (nd *)calloc(1, sizeof(nd));
        						newd->sol = nw;
							newd->index = nodelevel[arr[i][j]].node->left->data;
        						levels_backtrack[newd->index].ver = newd;
							parent->left = newd;
								
					}
				}
			}
		}
	}

	//postorder_new(t);
	printf("\n Total number of buffers inserted = %d\n\n", (count_buff-1));

	for(i=0;i<2*n;i++)
	{
		for(j=0;j<2*n;j++)
		{
			if(buff_insert[i][j]==1)
			{
				printf(" buffer is inserted at node %d in branch %d and %d\n", i, i, j);
			}
		}
	}

	printf("\n The required arrival time at driver node is %d\n\n", slack_max.slack);
	
}
void main(int argc, char *argv[])
{

	FILE *file = fopen(argv[1], "r");
	int n;
	fscanf(file, "%d", &n);
	
	int i, j;
	int up, down;
	for(i=1; i<=n; i++)
	{
		fscanf(file, "%d", &calssink[i].cap);
		fscanf(file, "%d", &calssink[i].rat);
		//printf("rat = %d\n", calssink[i].rat);
	}

	int edge[100][100];
	

	for(i=0; i<100;i++)
	{
		for(j=0; j<100; j++)
			edge[i][j]=0;
	}

	for(i=0; i<100;i++)
	{
		for(j=0; j<100; j++)
			edge_wt[i][j]=0;
	}

	int k =1;
	while(k<= ((2*n - 1)))
	{
 		fscanf(file, "%d", &up);
		fscanf(file, "%d", &down);
		edge[up][down] = 1;
		fscanf(file, "%d", &edge_wt[up][down]);
		k=k+1;
	}

	int reach[100][100];
	for(i=0; i<(2*n); i++)
	{
		for(j=0; j<(2*n); j++)
		{
			reach[i][j] = edge[i][j];
		}
	}
	
	 for (k = 0; k < (2*n); k++)
   	 {
                	for (i = 0; i < (2*n); i++)
        		{
              			  for (j = 0; j < (2*n); j++)
        	    		  {
                			reach[i][j] = reach[i][j] || (reach[i][k] && reach[k][j]);
            			   }
        		}
    	}
	int v = (2*n);
	int ancestor[v][v];
	for (int i = 0; i < (2*n); i++)
    	{
        	for (int j = 0; j < (2*n); j++)
        	{    
			ancestor[i][j] = reach[j][i];
		}
     
    	}

	Tree t = constructTreeFromAncestorMatrix(v, ancestor);

    	postorder(t);

	
	for(i=0;i<v;i++)
	{
		for(j=0;j<v;j++)
		{
			if((levels[i].data) == j)
				nodelevel[j] = levels[i];
		}
	}
	
	levelmax = max_level(v);
	int  w;
	
	for(i=0;i<100;i++)
	{
		for(j=0;j<100; j++)
			arr[i][j] = -1;
	}

	for(i=0;i<=levelmax; i++)
	{
		w=0;
		for(j=0; j<v; j++)
		{
			if(nodelevel[j].level == i)
			{
				arr[i][w] = j;
				w = w+1;
			}
		}
		element[i] = w;
	}
	/*for(i=0; i<=levelmax; i++)
	{
		for(j=0; j<element[i]; j++)
		{
			printf("%d ", arr[i][j]);
			
		}
	printf("\n");
	}*/	

	propagation(n);			
}

	



