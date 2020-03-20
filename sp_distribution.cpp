#include <iostream>
using namespace std;
#include <vector>
#include<random>
#include <cstdio>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <time.h>
#include <chrono>
#include <ctime>

//time variables
std::chrono::time_point<std::chrono::system_clock> fstart, fend, fstart_loc, fend_loc;
// file writing stuff ///////////////////////////////
char picdiry [ FILENAME_MAX ];
char picdirz [ FILENAME_MAX ];
int sample_steps, sample_all;
unsigned long long mygraph_edge_no, mygraph_size, pairs_cal_no;
unsigned long long a,b, max_sp;
unsigned int  seedvar=(unsigned int)time (NULL);
mt19937_64 myrandom(seedvar);
unsigned long long maxdegree; // the largest degree of any node
int max_deg;
vector <unsigned long long> high_vec;
unordered_map<unsigned long long,int > sp_distribution, double_count, sp_map;
unordered_map<unsigned long long,double > prob_distr_map;
vector < vector <unsigned long long > > weight_vec;
vector < unsigned long long  > temp;
vector < vector <unsigned long long > > mygraph;

void initialization(){//cout<<"		creating graph array...";
mygraph.resize(mygraph_size);
vector <unsigned long long> temp; temp.resize(0);
for (unsigned long long i=0; i<mygraph_size; i++){mygraph.push_back(temp);}//cout<<"...done"<<endl;
}

unsigned long long pairfun(unsigned long long x, unsigned long long y){
    return ((x+y)*(x+y)+x+3*y)/2; //(x+y-1)*(x+y)/2 + (1-y);
}

void sp_fun(unsigned long long bfs_root){//does BFS from a
	unordered_map<unsigned long long,int > used; vector <unsigned long long> next_level_0, next_level_1;
    unsigned long long pivot, explored, dala, kala, counter=0; next_level_0.push_back(bfs_root); used[bfs_root]=1;
    while (next_level_0.size()>0){counter++;
        for (unsigned long long i=0;i<next_level_0.size();i++){pivot=next_level_0[i];
          for (unsigned long long j=0;j<mygraph[pivot].size();j++){explored=mygraph[pivot][j];
 			if (used[explored]==0){dala=pairfun(bfs_root, explored); kala=pairfun(explored,bfs_root);
				next_level_1.push_back(explored); used[explored]=1; 
				sp_map[dala]=counter; sp_map[kala]=counter;
				if (double_count[dala]==0){sp_distribution[counter]=sp_distribution[counter]+1; double_count[dala]=1; double_count[kala]=1;
					//cout<<bfs_root<< " and "<<explored<<" has distance "<< counter<<endl;
				}
			}} }
        next_level_0=next_level_1; next_level_1.resize(0);
	}
if (max_sp<counter){max_sp=counter;}
}


int main(int argc, const char * argv[]) {
srand (seedvar); cout<<endl;
int hist_flag=1, count_samp=0;
	sample_steps=1; sample_all=5;	
	sprintf(picdirz, "%s", "friendster_asa");
	cout<<"reading size of graph "<<picdirz<<" ...";
	sprintf(picdiry, "%s%s%s", "./facebook/", picdirz, ".txt");
	ifstream infile(picdiry); unsigned long long countero=0;
	while (infile >> a >> b){mygraph_edge_no++;
	if (countero < a){countero=a;}
	if (countero < b){countero=b;}}
	countero++;
	mygraph_size=countero; cout<<"...done: "<<mygraph_size<<" nodes and "<<mygraph_edge_no<<" edges"<<endl;
		
	cout<<"initialization of arrays..."; initialization(); cout<<"...done"<<endl;
	cout<<"reading the graph..."; ifstream gnfile(picdiry); while (gnfile >> a >> b){mygraph[a].push_back(b); mygraph[b].push_back(a); } cout<<"...done"<<endl;
		 
cout<<"calculating distances... ("<<sample_all<<" BFSs in steps of "<<sample_steps<<")"<<endl<<endl;

	 fstart = std::chrono::system_clock::now();
	 fstart_loc = std::chrono::system_clock::now();
for (int i=0; i<= sample_all; i++){//iterating BFS  
	sp_fun(rand()%mygraph_size); 
	if (i%sample_steps==0 and i>0){

		fend_loc = std::chrono::system_clock::now();
		long long elapsed_seconds_loc = std::chrono::duration_cast<std::chrono::seconds> (fend_loc-fstart_loc).count();
		fstart_loc = std::chrono::system_clock::now();
		
		pairs_cal_no=0;//if remarkable bfs
		for (int z=0; z<= max_sp; z++){pairs_cal_no=pairs_cal_no+sp_distribution[z];}//calculate number of pairs so far
		for (int j=1; j<= max_sp; j++){//print_distribution
		if (hist_flag==0){cout<<endl; printf("%3d   ", j); printf("  %0.7f  ", (double)sp_distribution[j]/(double)pairs_cal_no); cout<<endl<<endl;}
		else{prob_distr_map[pairfun(count_samp,j)]=(double)sp_distribution[j]/(double)pairs_cal_no;}			 
		}
		printf("	finished %4d  BFSs in %4lld seconds", i,elapsed_seconds_loc); cout<<endl; count_samp++;
	fend_loc = std::chrono::system_clock::now();}
}

cout<<endl;

if (hist_flag==1){
for (int i=1; i<= max_sp; i++){printf("%3d   ", i); 
for (int j=0; j<count_samp; j++){printf("  %0.7f  ", prob_distr_map[pairfun(j,i)]);}
cout<<endl;}}

fend = std::chrono::system_clock::now();
// long long elapsed_minutes = std::chrono::duration_cast<std::chrono::minutes> (fend-fstart).count();
 long long elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds> (fend-fstart).count();
 cout<<endl<<"finished all in "<<elapsed_seconds<<" seconds"<<endl<<endl;
return 0;
}
