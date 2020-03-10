//#include <iostream.h>
#include <bits/stdc++.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

Mat img1=imread("rrt.jpg",1);
Mat img2=imread("rrt.jpg",1);
int flag=0;
float step=10;
float radius=12;

int l1 =1,l2 = 1;


struct node
{
	int x;
	int y;
	int parent;
	float cost;
};

float distance(node a,node b)
{
	return sqrt(pow((b.x-a.x),2) + pow((a.y-b.y),2));
}

node start_tree[9999999], end_tree[9999999];
int rows=img1.rows;
int cols=img1.cols;

node start()
{
	srand(time(NULL));
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j <cols; ++j)
 			{
				if(img1.at<Vec3b>(i,j)[0]< 100 && img1.at<Vec3b>(i,j)[1]>200 && img1.at<Vec3b>(i,j)[2] <100)
					{
 						node start;
 						start.x = i;
 						start.y = j;
 						start.cost = 0;
 						start.parent = -1;
 						return start;
					}
			}
	}
}

node end()
{
	srand(time(NULL));
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j <cols; ++j)
 			{
				if(img1.at<Vec3b>(i,j)[0]< 100 && img1.at<Vec3b>(i,j)[2]>200 && img1.at<Vec3b>(i,j)[1] <100)
					{
 						node end;
 						end.x = i;
 						end.y = j;
 						end.cost = 0;
 						end.parent = -1;
 						return end;
					}
			}
	}
}

int near_node(node tree[],node rnode,int len)
{
	float min_dist = 9999.0,dist;
	int nnode = 0, i = 0;
  	for(i=0; i<len; i++)
  	{
    		dist = distance(tree[i],rnode);
    		if(dist<min_dist)
    		{
      			min_dist = dist;
      			nnode = i;
    		}
  	}
  	return nnode;
}

node random_node()
{
	node rnode;
	rnode.x = rand()%(rows);
	rnode.y = rand()%(cols);
	return rnode;
}


int check(node nod1,node nod2)
{
	if(nod1.x = nod2.x)
	{
		int p,q;
		if(nod1.y<nod2.y)
		{
			 p=nod1.y,q=nod2.y;
		}
		else
	 	q=nod1.y,p=nod2.y;	
	for (int u = p; u <q+1; ++u)
	{
		if(img1.at<Vec3b>(nod1.x,u)[0] > 100 &&img1.at<Vec3b>(nod1.x,u)[2]>100 && img1.at<Vec3b>(nod1.x,u)[1] >100)
  		return 0;
  		if(img1.at<Vec3b>(nod1.x+10,u)[0] > 100 &&img1.at<Vec3b>(nod1.x+10,u)[2]>100 && img1.at<Vec3b>(nod1.x+10,u)[1] >100)
	  	return 0;
  		if(img1.at<Vec3b>(nod1.x-10,u)[0] > 100 &&img1.at<Vec3b>(nod1.x-10,u)[2]>100 && img1.at<Vec3b>(nod1.x-10,u)[1] >100)
  		return 0;
  	}
  	return 1;	
	}

	else
	{
		float slope = (float(nod1.y)-nod2.y)/(float(nod1.x)-nod2.x);
		int p,q,py,qy;
		if(nod1.x>nod2.x)
		{
			p=nod2.x,q=nod1.x;
			py = nod2.y,qy = nod1.y;
		}
		else
		{
			q=nod2.x,p=nod1.x;
			qy = nod2.y,py = nod1.y;
		}
		for (int z = p; z <q+1 ; ++z)
		{ 
			int yp = int(slope*(z-p)+py);
		 	if(img1.at<Vec3b>(z,yp)[0] > 100 &&img1.at<Vec3b>(z,yp)[2]>100 && img1.at<Vec3b>(z,yp)[1] >100)
	  		return 0;
		  	if(img1.at<Vec3b>(z+10,yp)[0] > 100 &&img1.at<Vec3b>(z+10,yp)[2]>100 && img1.at<Vec3b>(z+10,yp)[1] >100)
		  	return 0;
		  	if(img1.at<Vec3b>(z-10,yp)[0] > 100 &&img1.at<Vec3b>(z-10,yp)[2]>100 && img1.at<Vec3b>(z-10,yp)[1] >100)
		  	return 0;
		}
  	return 1;
	}
}


void wire(node n1,node n2,node n[],int w)
{
	if(n1.cost > n2.cost+distance(n1,n2) && check(n1,n2))
	{
		line(img1, Point(n1.y, n1.x), Point((n[n1.parent]).y, (n[n1.parent]).x), Scalar(0, 0, 0), 1, 8);
		line(img1, Point(n1.y, n1.x), Point(n2.y, n2.x), Scalar(255, 255, 255), 1, 8);
		n1.parent = w;
		n1.cost = n2.cost+distance(n1,n2);
	}

}


void rewire(node list[],int len,int pos)
{
	float mind = 999999.0;
	int f = -1;
	for (int ii = 0; ii < len; ++ii)
	{ 
		if(ii!=pos && distance(list[ii],list[pos])<radius && mind >(list[ii].cost+distance(list[ii],list[pos])) &&(check(list[ii],list[pos])))
		{
			mind = (list[ii].cost+distance(list[ii],list[pos]));
			f= ii;
		}
	}
	if(f !=list[pos].parent)
	{
			line(img1, Point(list[pos].y, list[pos].x), Point(list[list[pos].parent].y, list[list[pos].parent].x), Scalar(0, 0, 0), 1, 8);
			list[pos].parent = f;
			list[pos].cost = mind;
			line(img1, Point(list[pos].y, list[pos].x), Point(list[f].y, list[f].x), Scalar(255, 255, 255), 1, 8);
			for (int iii = 0; iii < len; ++iii)
			{
				if(iii!=pos && distance(list[iii],list[pos])<radius)
					wire(list[iii],list[pos],list,pos);
				
			}
		
	}
}




node step_node(node tree[],int * l)
{
	int len=*l;
	node rnode=random_node();
	int nnode=near_node(tree,rnode,len);
	float base =distance(tree[nnode],rnode);
	if(base)
	{
		tree[len].x = int(tree[nnode].x + ((rnode.x - tree[nnode].x)*step)/base);
		tree[len].y = int(tree[nnode].y + ((rnode.y - tree[nnode].y)*step)/base);
		tree[len].parent = nnode;
		tree[len].cost =  tree[nnode].cost + distance(tree[nnode],tree[len]);
		if(check(tree[len],tree[nnode]))
		{
			line(img1, Point(tree[len].y, tree[len].x), Point(tree[nnode].y, tree[nnode].x), Scalar(220, 123, 255), 1, 8);
			(*l)++;
			rewire(tree,len+1,len);
		}
	}

}

void check_end(node qw,node lis[],int len,node lis1[])
{
	int uy = near_node(lis,qw,len);
	if(step>distance(lis[uy], qw))
	{
		node n01 = qw,n02 = lis[uy];
		line(img2, Point(n01.y, n01.x), Point(n02.y, n02.x), Scalar(255, 255, 255), 1, 8);
		while(n01.parent !=-1 || n02.parent!=-1)
		{
			if(n01.parent !=-1 )
			{
				line(img2, Point(n01.y, n01.x), Point(lis1[n01.parent].y, lis1[n01.parent].x), Scalar(255, 255, 255), 1, 8);
			 	n01= lis1[n01.parent];
			}
			if(n02.parent !=-1 )
			{
				line(img2, Point(n02.y, n02.x), Point(lis[n02.parent].y, lis[n02.parent].x), Scalar(255, 255, 255), 1, 8);
				 n02= lis[n02.parent];}
				 imshow("output",img2);
				 waitKey(100);
			 }
	flag++;
	}
	return;
}




int main()
{
	namedWindow("output",0);
	start_tree[0]=start();
	end_tree[0]=end();
	while(flag==0)
	{
		step_node(start_tree,&l1);
		check_end(start_tree[l1-1],end_tree,l2,start_tree);
		
		step_node(end_tree,&l2);
		check_end(end_tree[l2-1],start_tree,l1,end_tree);

		if(flag!=0)
		break;	
		
		imshow("output",0);
		waitKey(60000);	
	}
	namedWindow("output2",0);
	imshow("output2",img2);
	waitKey(0);
	return 0;
}
