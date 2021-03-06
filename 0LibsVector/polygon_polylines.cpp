/* Current version of code assumes a polygon intersects just once with the other polygon for EFFICIENCY purpose */


#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

#include "polygon_polylines.h"
#include "./../0LibsShape/shapefil.h"

using namespace std;

//#define dist(x1,y1,x2,y2) sqrt(pow((x1-x2),2) + pow((y1-y2),2))  
#define dist(x1,y1,x2,y2) fabs(x1-x2)+fabs(y1-y2)  

void sortArray(int ***junction, int* jctCount, int recordCount)
{
	int temp;
	int a, b, c, d;
	for(int i=0; i<recordCount; i++)
	{
		for(int j=0; j<jctCount[i]; j++)
		{
			for(int k=0; k<j; k++)
			{
				if(junction[i][j][1] < junction[i][k][1])
				{
					temp = junction[i][j][2];
					junction[i][j][2] = junction[i][k][2];
					junction[i][k][2] = temp;
					
					temp = junction[i][j][1];
					junction[i][j][1] = junction[i][k][1];
					junction[i][k][1] = temp;

					temp = junction[i][j][0];
					junction[i][j][0] = junction[i][k][0];
					junction[i][k][0] = temp;
				}
			}
		}
		for(int j=0; j<jctCount[i]-1; j++)
		{
			if(junction[i][j][1]==junction[i][j+1][1])
			{
				if(junction[i][j][0]<junction[i][j+1][0])
				{
					int k=j+1;
					temp = junction[i][j][2];
					junction[i][j][2] = junction[i][k][2];
					junction[i][k][2] = temp;
					
					temp = junction[i][j][1];
					junction[i][j][1] = junction[i][k][1];
					junction[i][k][1] = temp;

					temp = junction[i][j][0];
					junction[i][j][0] = junction[i][k][0];
					junction[i][k][0] = temp;
				}
			}
		}
		if(jctCount[i]>=2)
		{
			a = junction[i][jctCount[i]-2][0];
			b = junction[i][jctCount[i]-2][1];
			c = junction[i][jctCount[i]-1][0];
			d = junction[i][jctCount[i]-1][1];
			if(c<=a && b<=d)
			{
				junction[i][jctCount[i]-1][0] = d;
				junction[i][jctCount[i]-1][1] = c;
			}
		}
		
	}
}


void calPts(int start, int end, SHPObject* handle, double *X, double *Y, double *Z, int node, int *vertices)
{
	//SHPObject *newobj;
	*vertices = 0;
	X = new double[node];
	Y = new double[node];
	Z = new double[node];
	for(int i=start; i<=end; i++)
	{
		X[*vertices]=handle->padfX[i];
		Y[*vertices]=handle->padfY[i];
		Z[*vertices]=0;
		(*vertices)++;
	}
}


int polygon_polylines(const char* shpFileName, const char* dbfFileName, const char *newshpFileName, const char *newdbfFileName)
{
	int recordCount;
	//int tmp;
	SHPHandle shp = SHPOpen(shpFileName, "rb");
	DBFHandle dbf = DBFOpen(dbfFileName, "rb");

	SHPHandle newshp = SHPCreate(newshpFileName, SHPT_ARC);
	DBFHandle newdbf = DBFCreate(newdbfFileName);

    if ( shp == NULL || dbf == NULL )
        return 109;
    if ( newshp == NULL || newdbf == NULL )
        return 111;

    int InfoShpType;
    SHPGetInfo (shp, NULL, &InfoShpType, NULL, NULL);
    if ( InfoShpType != SHPT_POLYGON )
    {
        cout << "Not a SHPT_POLYGON: ... " << shpFileName << "\n";
        cout << "SHAPE TYPE = " << InfoShpType << "\n";
        return InfoShpType;
    }

	int field  = DBFAddField(newdbf, "Field", FTInteger, 5, 0);

    if ( field == -1 )
        return 120;
	
	SHPObject *obj1, *obj2;
	recordCount = DBFGetRecordCount(dbf);

	int ***junction = new int**[recordCount];
	for(int i=0; i<recordCount; i++)
	{
		junction[i] = new int*[30];
		for(int j=0; j<30; j++)
		{
			junction[i][j]=new int[3];
		}
	}
	int *vertex    = new int[recordCount];
	int *jctCount  = new int[recordCount];
	int jct1[2];
	int jct2[2];
	double x1, x2, x3, x4, y1, y2, y3, y4;
	int *pts1, *pts2, pts1count, pts2count;
	for( int i=0; i<recordCount; i++)
	{
		//cout<<"\ni = "<<i<<"\n-----------------------\n";
		obj1 = SHPReadObject(shp, i);
		vertex[i]=obj1->nVertices;
		jctCount[i]=0;
		
		for( int j=0; j<i; j++)
		{
			//cout<<"j= "<<j<<" ";
			
			obj2 = SHPReadObject(shp, j);
			int ii, jj, iii, jjj;
			//int flag=0;
			int ptcount=0;
			int breakFlag=0;
			if(!((obj1->dfXMax<obj2->dfXMin)||(obj2->dfXMax<obj1->dfXMin)||(obj1->dfYMax<obj2->dfYMin)||(obj2->dfYMax<obj1->dfYMin)))
			{
				//Decide potential intersecting polygon
				//cout<<"\npolygon1= "<<i<<" polygon2= "<<j<<"\n";
				pts1 = new int[obj1->nVertices]; 
				pts1count = 0;
				pts2 = new int[obj2->nVertices];
				pts2count = 0;
				//Find points of poly 1 falling in the bounding rectangle of the polygon 2
				for(int p1=0; p1<obj1->nVertices-1; p1++)
				{
					if( (obj2->dfXMin-1<=obj1->padfX[p1]&&obj1->padfX[p1]<=obj2->dfXMax+1) && (obj2->dfYMin-1<=obj1->padfY[p1]&&obj1->padfY[p1]<=obj2->dfYMax+1))
					{
						pts1[pts1count++]=p1;
					}
				}
				//Find points of poly 2 falling in the bounding rectangle of the polygon 1
				for(int p2=0; p2<obj2->nVertices-1; p2++)
				{
					if( (obj1->dfXMin-1<=obj2->padfX[p2]&&obj2->padfX[p2]<=obj1->dfXMax+1) && (obj1->dfYMin-1<=obj2->padfY[p2]&&obj2->padfY[p2]<=obj1->dfYMax+1))
					{
						pts2[pts2count++]=p2;
					}
				}

				for(iii=0; iii<pts1count; iii++)
				{
					for(jjj=0; jjj<pts2count; jjj++)
					{
						ii=pts1[iii];
						jj=pts2[jjj];
						x1 = obj1->padfX[ii];
						x2 = obj2->padfX[jj];
						y1 = obj1->padfY[ii];
						y2 = obj2->padfY[jj];
						
						if(dist(x1,y1,x2,y2)<0.001)
						{
							/*
							if(ii==obj1->nVertices-1){ x1=obj1->padfX[1]; y1=obj1->padfY[1];}
							else{x1=obj1->padfX[ii+1]; y1=obj1->padfY[ii+1];}
							if(ii==0){ x2=obj1->padfX[obj1->nVertices-2]; y2=obj1->padfY[obj1->nVertices-2];}
							else{x2=obj1->padfX[ii-1]; y2=obj1->padfY[ii-1];}
							if(jj==obj2->nVertices-1){ x3=obj2->padfX[1]; y3=obj2->padfY[1];}
							else{x3=obj2->padfX[jj+1]; y3=obj2->padfY[jj+1];}
							if(jj==0){x4=obj2->padfX[obj2->nVertices-2]; y4=obj2->padfY[obj2->nVertices-2];}
							else{x4=obj2->padfX[jj-1]; y4=obj2->padfY[jj-1];}
							*/
							
							x1 = ii==obj1->nVertices-1?obj1->padfX[1]:obj1->padfX[ii+1];
							x2 = ii==0?obj1->padfX[obj1->nVertices-2]:obj1->padfX[ii-1];
							x3 = jj==obj2->nVertices-1?obj2->padfX[1]:obj2->padfX[jj+1];
							x4 = jj==0?obj2->padfX[obj2->nVertices-2]:obj2->padfX[jj-1];

							y1 = ii==obj1->nVertices-1?obj1->padfY[1]:obj1->padfY[ii+1];
							y2 = ii==0?obj1->padfY[obj1->nVertices-2]:obj1->padfY[ii-1];
							y3 = jj==obj2->nVertices-1?obj2->padfY[1]:obj2->padfY[jj+1];
							y4 = jj==0?obj2->padfY[obj2->nVertices-2]:obj2->padfY[jj-1];
							
							if((dist(x1, y1, x3, y3) < 0.001))
							{
								if(dist(x2, y2, x4, y4) >= 0.001)
								{
									//jct found
									jct1[ptcount]=ii;
									jct2[ptcount]=jj;
									ptcount++;
									//printf("\n%d %d -- %d %d",i,ii,j,jj);
								}
							}
							if((dist(x1, y1, x4, y4) < 0.001))
							{
								if(dist(x2, y2, x3, y3) >= 0.001)
								{
									//jct found
									jct1[ptcount]=ii;
									jct2[ptcount]=jj;
									ptcount++;
									//printf("\n%d %d -- %d %d",i,ii,j,jj);
								}
							}
							if((dist(x2, y2, x4, y4) < 0.001))
							{
								if( dist(x1, y1, x3, y3)>= 0.001)
								{
									//jct found
									jct1[ptcount]=ii;
									jct2[ptcount]=jj;
									ptcount++;
									//printf("\n%d %d -- %d %d",i,ii,j,jj);
								}
							}
							if((dist(x2, y2, x3, y3) < 0.001))
							{
								if( dist(x1, y1, x4, y4)>= 0.001)
								{
									//jct found
									jct1[ptcount]=ii;
									jct2[ptcount]=jj;
									ptcount++;
									//printf("\n%d %d -- %d %d",i,ii,j,jj);
								}
							}
							//cout<<"\nptcount= "<<ptcount<<"\n";
							
						}
						//This assumes a polygon interects just once with the other polygon
						if(ptcount==2)
						{
							breakFlag=1;
							break;
						}
					}
					//This assumes a polygon interects just once with the other polygon
					if(breakFlag==1)
						break;
				}
			}
			//cout<<"\nptcount= "<<ptcount<<"\n";
			if(ptcount==2)
			{
				junction[i][jctCount[i]][0]=jct1[0]<jct1[1]?jct1[0]:jct1[1];
				junction[i][jctCount[i]][1]=jct1[0]<jct1[1]?jct1[1]:jct1[0];
				junction[i][jctCount[i]][2]=1;
				jctCount[i]++;

				junction[j][jctCount[j]][0]=jct2[0]<jct2[1]?jct2[0]:jct2[1];
				junction[j][jctCount[j]][1]=jct2[0]<jct2[1]?jct2[1]:jct2[0];
				junction[j][jctCount[j]][2]=0;
				jctCount[j]++;

				//ptcount=0;
			}
		}
	}
	//getchar(); getchar();
	
	for(int i=0; i<recordCount; i++)
	{
		obj1 = SHPReadObject(shp, i);
		
		if(jctCount[i]==0)
		{
			jctCount[i]=1;
			junction[i][0][0]=0;
			junction[i][0][1]=obj1->nVertices-1;
			junction[i][0][2]=1;
		}
	}

	sortArray(junction, jctCount, recordCount);
/*
	for(int i=0; i<recordCount; i++){
		cout<<"\ni= " <<i<<"\n---------------\n";
		//cout<<"junctions= "<<jctCount[i]<<"\n";
		for(int j=0; j<jctCount[i]; j++){
			cout<<junction[i][j][0]<<" "<<junction[i][j][1]<<" "<<junction[i][j][2]<<" | ";
		}
	}
*/
	int record=0, start, end;//, val;
//	cout<<"\n\nFINAL \n------------\n";
	SHPObject *newobj;
	for(int i=0; i<recordCount; i++)
	{
		obj1=SHPReadObject(shp, i);
		int nodes=obj1->nVertices;
		double *X= new double[nodes];
		double *Y= new double[nodes];
		double *Z= new double[nodes];
		int vertices;
		//cout<<"\ni= " <<i<<"\n---------------\n";
		if(junction[i][jctCount[i]-1][1]<junction[i][jctCount[i]-1][0])
		{
			if(junction[i][jctCount[i]-1][2]==1)
			{
				if(junction[i][jctCount[i]-1][1]!=0)
				{
					//cout<<"0"<<" "<<junction[i][jctCount[i]-1][1]<<" | ";
					/**********************************************************/
					start=0;
					end=junction[i][jctCount[i]-1][1];
					vertices=0;
					for(int i=start; i<=end; i++)
					{
						X[vertices]=obj1->padfX[i];
						Y[vertices]=obj1->padfY[i];
						Z[vertices]=0;
						vertices++;
					}
					newobj = SHPCreateSimpleObject(SHPT_ARC, vertices, X, Y, Z);
                    if ( SHPWriteObject(newshp, -1, newobj) < 0 )
                        return 348;
                    if ( ! DBFWriteIntegerAttribute(newdbf, record, field, record+1) )
                        return 350;
                    else
                        record++;
					/**********************************************************/
				}
				//cout<<junction[i][jctCount[i]-1][0]<<" "<<vertex[i]-1<<" | ";
				/**********************************************************/
				start=junction[i][jctCount[i]-1][0];
				end=vertex[i]-1;
					vertices=0;
					for(int i=start; i<=end; i++)
					{
						X[vertices]=obj1->padfX[i];
						Y[vertices]=obj1->padfY[i];
						Z[vertices]=0;
						vertices++;
					}
					newobj = SHPCreateSimpleObject(SHPT_ARC, vertices, X, Y, Z);
                    if ( SHPWriteObject(newshp, -1, newobj) < 0 )
                        return 369;
                    if ( ! DBFWriteIntegerAttribute(newdbf, record, field, record+1) )
                        return 371;
                    else
                        record++;
				/**********************************************************/
			}
			if(junction[i][0][0]>junction[i][jctCount[i]-1][1])
			{
					//cout<<junction[i][jctCount[i]-1][1]<<" "<<junction[i][0][0]<<" | ";
					/**********************************************************/
					start=junction[i][jctCount[i]-1][1];
					end=junction[i][0][0];
					vertices=0;
					for(int i=start; i<=end; i++)
					{
						X[vertices]=obj1->padfX[i];
						Y[vertices]=obj1->padfY[i];
						Z[vertices]=0;
						vertices++;
					}
					newobj = SHPCreateSimpleObject(SHPT_ARC, vertices, X, Y, Z);
                    if ( SHPWriteObject(newshp, -1, newobj) < 0 )
                        return 392;
                    if ( ! DBFWriteIntegerAttribute(newdbf, record, field, record+1) )
                        return 394;
                    else
                        record++;
					/**********************************************************/
			}
		}
		else{
			if(junction[i][0][0]!=0)
			{
				//cout<<"0"<<" "<<junction[i][0][0]<<" | ";
				/**********************************************************/
				start=0; 
				end=junction[i][0][0];
					vertices=0;
					for(int i=start; i<=end; i++)
					{
						X[vertices]=obj1->padfX[i];
						Y[vertices]=obj1->padfY[i];
						Z[vertices]=0;
						vertices++;
					}
					newobj = SHPCreateSimpleObject(SHPT_ARC, vertices, X, Y, Z);
                    if ( SHPWriteObject(newshp, -1, newobj) < 0 )
                        return 417;
                    if ( ! DBFWriteIntegerAttribute(newdbf, record, field, record+1) )
                        return 419;
                    else
                        record++;
					/**********************************************************/
			}
			if(junction[i][jctCount[i]-1][2]==1)
			{
				//cout<<junction[i][jctCount[i]-1][0]<<" "<<junction[i][jctCount[i]-1][1]<<" | ";
				/**********************************************************/
				start=junction[i][jctCount[i]-1][0];
				end=junction[i][jctCount[i]-1][1];
					vertices=0;
					for(int i=start; i<=end; i++)
					{
						X[vertices]=obj1->padfX[i];
						Y[vertices]=obj1->padfY[i];
						Z[vertices]=0;
						vertices++;
					}
					newobj = SHPCreateSimpleObject(SHPT_ARC, vertices, X, Y, Z);
                    if ( SHPWriteObject(newshp, -1, newobj) < 0 )
                        return 440;
                    if ( ! DBFWriteIntegerAttribute(newdbf, record, field, record+1) )
                        return 442;
                    else
                        record++;
					/**********************************************************/
			}
			if(junction[i][jctCount[i]-1][1]<vertex[i]-1)
			{
				//cout<<junction[i][jctCount[i]-1][1]<<" "<<vertex[i]-1<<" | ";
				/**********************************************************/
				start=junction[i][jctCount[i]-1][1];
				end=vertex[i]-1;	
				vertices=0;
					for(int i=start; i<=end; i++)
					{
						X[vertices]=obj1->padfX[i];
						Y[vertices]=obj1->padfY[i];
						Z[vertices]=0;
						vertices++;
					}
					newobj = SHPCreateSimpleObject(SHPT_ARC, vertices, X, Y, Z);
                    if ( SHPWriteObject(newshp, -1, newobj) < 0 )
                        return 463;
                    if ( ! DBFWriteIntegerAttribute(newdbf, record, field, record+1) )
                        return 465;
                    else
                        record++;
					/**********************************************************/
			}
		}
		for(int j=0; j<jctCount[i]-1; j++)
		{
			if(junction[i][j][2]==1)
			{
				//cout<<junction[i][j][0]<<" "<<junction[i][j][1]<<" | ";
				/**********************************************************/
				start=junction[i][j][0];
				end=junction[i][j][1];	
				vertices=0;
					for(int i=start; i<=end; i++)
					{
						X[vertices]=obj1->padfX[i];
						Y[vertices]=obj1->padfY[i];
						Z[vertices]=0;
						vertices++;
					}
					newobj = SHPCreateSimpleObject(SHPT_ARC, vertices, X, Y, Z);
                    if ( SHPWriteObject(newshp, -1, newobj) < 0 )
                        return 489;
                    if ( ! DBFWriteIntegerAttribute(newdbf, record, field, record+1) )
                        return 491;
                    else
                        record++;
					/**********************************************************/
			}
			if(junction[i][j][1]<junction[i][j+1][0])
			{
				//cout<<junction[i][j][1]<<" "<<junction[i][j+1][0]<<" | ";
				/**********************************************************/
				start=junction[i][j][1];
				end=junction[i][j+1][0];
					vertices=0;
					for(int i=start; i<=end; i++)
					{
						X[vertices]=obj1->padfX[i];
						Y[vertices]=obj1->padfY[i];
						Z[vertices]=0;
						vertices++;
					}
					newobj = SHPCreateSimpleObject(SHPT_ARC, vertices, X, Y, Z);
                    if ( SHPWriteObject(newshp, -1, newobj) < 0 )
                        return 512;
                    if ( ! DBFWriteIntegerAttribute(newdbf, record, field, record+1) )
                        return 515;
                    else
                        record++;
					
					/**********************************************************/
			}
		}
	}
	SHPClose(shp);
	DBFClose(dbf);
	SHPClose(newshp);
	DBFClose(newdbf);

    return(0);
}	
	


