#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int R_buffer=1,C_buffer=1,Buffer_Delay=1,Wire_Cap=1,Wire_Res=1,Driver_Res=1,sinks,sink_cap[10000],sink_RAT[10000],edge_wt[1000][1000],edge[1000][1000],traversal[1000];

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
int sortLevel(const void *a, const void *b)
{
    const Level *x = a;
    const Level *y = b;
    return x->level - y->level;
}
Level levels[1000];
Level unsorted[1000];
int previous[1000];
Tree constructTreeFromAncestorMatrix(int n, int ancestors[n][n])
{

    int i, j;
    for(i = 0; i < n; i++)
    {
        levels[i].data = i;
        levels[i].level = 0;
        for(j = 0; j < n; j++)
            levels[i].level += ancestors[i][j];
    }
    for(i=0;i<n;i++)
    unsorted[i].level=levels[i].level;
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
                    previous[levels[i].data]=levels[j].data;
                }
                else if(!parent->right)
                {
                    parent->right = node;
                    previous[levels[i].data]=levels[j].data;
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
   printf("%d ", t->data);
   traversal[i1]=t->data;
   printf("%d \n", traversal[i1]);
   i1++;
}
int maximum(int nodes)
{
 int max=-9,i;
 for(i=0;i<nodes;i++)
 {
   if(unsorted[i].level>max)
   {
     max=unsorted[i].level;
   }
 }
 printf("%d is the maximum value",max);
return max; 
} 
int main(int argc, char *argv[])
{
    int i,j,v1,v2;
    int nodes;
    int k=1;
    for(i=0;i<1000;i++)
    {
        for(j=0;j<1000;j++)
        edge[i][j]=0;
        edge_wt[i][j]=0;
    }
    FILE *infile;
    infile = fopen(argv[1],"r");
    if(infile==NULL)
    {
        printf("Not a valid filename");
        exit(0);
    }
    fscanf(infile,"%d",&sinks);
    printf("sinks = %d \n",sinks);
    nodes=2*sinks;
    int matrix[nodes][nodes];
    for(i=1;i<=sinks;i++)
    {
        fscanf(infile,"%d ",&sink_cap[i]);
        fscanf(infile,"%d ",&sink_RAT[i]);
    }
    for(i=1;i<=sinks;i++)
        printf("%d %d \n",sink_cap[i],sink_RAT[i]);
    while(k<=(2*sinks-1))
        {
            fscanf(infile,"%d ",&v1);
            fscanf(infile,"%d ",&v2);
            fscanf(infile,"%d ",&edge_wt[v1][v2]);
            edge[v1][v2]=1;
            k++;
        }
    for(i=0;i<nodes;i++)
    {
        for(j=0;j<nodes;j++)
            printf("%d ",edge_wt[i][j]);        
        printf("\n");
    } 
    printf("\n");
    for(i=0;i<nodes;i++)
    {
        for(j=0;j<nodes;j++)
            printf("%d ",edge[i][j]);        
        printf("\n");
    } 
   	int reach[nodes][nodes], z;
	  for (i = 0; i < nodes; i++)
		for (j = 0; j < nodes; j++)
			reach[i][j] = edge[i][j];
	  for (z = 0; z < nodes; z++)
		for (i = 0; i < nodes; i++)
		for (j = 0; j < nodes; j++)
				reach[i][j] = reach[i][j] || (reach[i][z] && reach[z][j]);
	  printf ("\nFollowing matrix is transitive closure of the given graph\n");
	  for (i = 0; i < nodes; i++)
    {
      for (j = 0; j < nodes; j++)
			printf ("%d ", reach[i][j]);
		  printf("\n");
	  }
    for (i = 0; i < nodes; i++)
    {
      for (j = 0; j < nodes; j++)
			matrix[j][i]=reach[i][j];
	  }
    printf("\n");
    for (i = 0; i < nodes; i++)
    {
      for (j = 0; j < nodes; j++)
			printf ("%d ", matrix[i][j]);
		  printf("\n");
	  }
    Tree t = constructTreeFromAncestorMatrix(nodes, matrix);
    postorder(t);
    for(i=0;i<nodes;i++)
    {
    for(j=0;j<nodes;j++)
      {  
        if(levels[i].data==j)
          unsorted[j]=levels[i];
      }
    }
    printf("%d\n", previous[unsorted[6].data]);  
    int x= maximum(nodes);
    printf("\n%d \n",x);
    int y[x+1][nodes],element[x+1];
    for(i=0;i<=x;i++)
    {
      for(j=0;j<nodes;j++)
      {  
        y[i][j]=-99;
      }
    }
    for(i=0;i<=x;i++)
    {
      int z=0;
      for(j=0;j<nodes;j++)
        {
          if(unsorted[j].level==i)
          {
          y[i][z]=j;
          z++;
          }
          element[i]=z;
        }
    }
    for(i=0;i<=x;i++)
    {
      for(j=0;j<nodes;j++)
      {
      if(y[i][j]!=-99)
      printf ("%d ", y[i][j]);
      }
      printf("\n");
    }
    for(i=0;i<=x;i++)
    printf("%d\n",element[i]);
    
}

