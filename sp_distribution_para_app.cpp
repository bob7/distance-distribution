// pthread_join() subroutine blocks the calling thread until the specified threadid thread terminates.

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
#include <mutex>  
#include <pthread.h>
#define NUM_THREADS 50

//time variables
std::chrono::time_point<std::chrono::system_clock> fstart, fend, fstart_loc, fend_loc;
// file writing stuff ///////////////////////////////
char picdiry [ FILENAME_MAX ];
char picdirz [ FILENAME_MAX ];
int sample_steps, sample_all;
unordered_map< unsigned long long, unsigned long long > compo_to_graph_map, compo_to_graph_map_rev;
unsigned long long mygraph_edge_no, mygraph_size, pairs_cal_no, mygraph_edge_no_o, mygraph_size_o;
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
vector < vector <unsigned long long > > mygraph,mygraph_o;
mutex mtx; 

void initialization(){//cout<<"		creating graph array...";
mygraph.resize(mygraph_size);
vector <unsigned long long> temp; temp.resize(0);
for (unsigned long long i=0; i<mygraph_size; i++){mygraph.push_back(temp);}//cout<<"...done"<<endl;
}

unsigned long long pairfun(unsigned long long x, unsigned long long y){
    return ((x+y)*(x+y)+x+3*y)/2; //(x+y-1)*(x+y)/2 + (1-y);
}

 
void* perform_work(void* argument) {
  unsigned long long bfs_root;
  int max_sp_loc;
  bfs_root = *((unsigned long long*) argument);
unordered_map<unsigned long long,int > sp_distribution_loc;
	unordered_map<unsigned long long,int > used; vector <unsigned long long> next_level_0, next_level_1;
    unsigned long long pivot, explored, dala, kala, counter=0; next_level_0.push_back(bfs_root); used[bfs_root]=1;
	
    while (next_level_0.size()>0){counter++;
        for (unsigned long long i=0;i<next_level_0.size();i++){pivot=next_level_0[i];
          for (unsigned long long j=0;j<mygraph[pivot].size();j++){explored=mygraph[pivot][j];
 			if (used[explored]==0){//dala=pairfun(bfs_root, explored); kala=pairfun(explored,bfs_root);
				next_level_1.push_back(explored); used[explored]=1;
				// mtx.lock();
				//if (double_count[dala]==0){
					sp_distribution_loc[counter]=sp_distribution_loc[counter]+1; //double_count[dala]=1; double_count[kala]=1;
					//}
				//mtx.unlock();
			}} }
        next_level_0=next_level_1; next_level_1.resize(0);
	}
	max_sp_loc=counter;
	mtx.lock();
		for (int i=1; i<= max_sp; i++){sp_distribution[i]=sp_distribution[i]+sp_distribution_loc[i];}
		if (max_sp<max_sp_loc){max_sp=max_sp_loc;}
 	mtx.unlock();
  return NULL;
}

void read_original_graph(){
	cout<<endl<<"reading size of graph "<<picdirz<<" ...";
	unordered_map<unsigned long long,int > used;
	sprintf(picdiry, "%s%s%s", "./facebook/", picdirz, ".txt");
	ifstream infile(picdiry); unsigned long long countero=0;
	while (infile >> a >> b){mygraph_edge_no_o++;
	if (used[a]==0){countero++; used[a]=1;}
	if (used[b]==0){countero++; used[b]=1;}}
	mygraph_size_o=countero; cout<<"...done: "<<mygraph_size_o<<" nodes and "<<mygraph_edge_no_o<<" edges"<<endl;
	mygraph_o.resize(mygraph_size_o);
	temp.resize(0);
	for (unsigned long long i=0; i<mygraph_size_o; i++){mygraph_o.push_back(temp);}//cout<<"...done"<<endl;
	cout<<"reading the graph..."; ifstream gnfile(picdiry); while (gnfile >> a >> b){mygraph_o[a].push_back(b); mygraph_o[b].push_back(a); } cout<<"...done"<<endl;
}

int giant_search(unsigned long long bfs_root, vector < vector <unsigned long long > > mygraph_loc){//does BFS from bfs_root
	unordered_map<unsigned long long,int > used; vector <unsigned long long> next_level_0, next_level_1;
    unsigned long long pivot, explored, counter=0; next_level_0.push_back(bfs_root); used[bfs_root]=1;
    while (next_level_0.size()>0){
        for (unsigned long long i=0;i<next_level_0.size();i++){pivot=next_level_0[i];
          for (unsigned long long j=0;j<mygraph_loc[pivot].size();j++){explored=mygraph_loc[pivot][j];
 			if (used[explored]==0){next_level_1.push_back(explored); used[explored]=1; counter++; 
				}
			} }
        next_level_0=next_level_1; next_level_1.resize(0);
	}
return counter+1;
}


void giant_search_input(unsigned long long bfs_root, vector < vector <unsigned long long > > mygraph_loc){//does BFS from bfs_root
	unordered_map<unsigned long long,int > used; vector <unsigned long long> next_level_0, next_level_1;
    unsigned long long pivot, explored, counter=0; next_level_0.push_back(bfs_root); used[bfs_root]=1; compo_to_graph_map[0]=bfs_root; compo_to_graph_map_rev[bfs_root]=0;
    while (next_level_0.size()>0){
        for (unsigned long long i=0;i<next_level_0.size();i++){pivot=next_level_0[i];
          for (unsigned long long j=0;j<mygraph_loc[pivot].size();j++){explored=mygraph_loc[pivot][j];
 			if (used[explored]==0){next_level_1.push_back(explored); used[explored]=1; counter++; compo_to_graph_map[counter]=explored; compo_to_graph_map_rev[explored]=counter; 
				}
			} }
        next_level_0=next_level_1; next_level_1.resize(0);
	}
temp.resize(0); for (unsigned long long i=0; i<mygraph_size; i++){mygraph.push_back(temp);}//cout<<"...done"<<endl;
int candidate;
for (unsigned long long i=0; i<mygraph_size; i++){	
	for (unsigned long long j=0; j<mygraph_loc[compo_to_graph_map[i]].size(); j++){	
		candidate=compo_to_graph_map_rev[mygraph_loc[compo_to_graph_map[i]][j]];
		if (used[mygraph_loc[compo_to_graph_map[i]][j]]==1){mygraph[i].push_back(candidate);}
		}
	}
	//removing dublicates in mygraph
	for (int i=0; i<mygraph.size(); i++){
			//for (int j=0; j<mygraph[i].size(); j++){
				sort( mygraph[i].begin(), mygraph[i].end() );
				mygraph[i].erase( unique(mygraph[i].begin(), mygraph[i].end() ), mygraph[i].end() );
				//}
	}
}

void largest_component(){
	int bfs_root, bfs_root_loc, compo_size,compo_size_loc;
	int coulo=0;
	for (int i=0; i<100 and (10*compo_size< 6*mygraph_size_o); i++){bfs_root_loc=rand()%mygraph_size_o; compo_size_loc=giant_search(bfs_root_loc, mygraph_o); 
		if (compo_size_loc>compo_size){bfs_root=bfs_root_loc; compo_size=compo_size_loc;} coulo++;}
	mygraph_size=compo_size;
	cout<<"Found largest component after " << coulo<< " BFSs, with "<<mygraph_size<<" nodes ("<< 100* ((double)mygraph_size/(double)mygraph_size_o)<<"% of the original graph)"<<endl;
 	giant_search_input(bfs_root, mygraph_o);//setting mygraph to be the largest component

	// cout<<"Original subgraph is:"<<endl;
	// for (int i=0; i<mygraph.size(); i++){cout<< compo_to_graph_map[i]<<": "; for (int j=0; j<mygraph[i].size(); j++){cout<<compo_to_graph_map[mygraph[i][j]]<<",";} cout<<endl;}
	// cout<<"Isomorphic subgraph is:"<<endl;
	//  for (int i=0; i<mygraph_size; i++){cout<< i<<": "; for (int j=0; j<mygraph[i].size(); j++){cout<<mygraph[i][j]<<",";} cout<<endl;}
	 // cout<<endl;
}


int main(int argc, char** argv) {
	srand (seedvar); cout<<endl;
	int hist_flag=1, count_samp=0;
		sample_steps=1; sample_all=NUM_THREADS;	
		sprintf(picdirz, "%s", "twiter");
		read_original_graph();
		largest_component();
		// cout<<"reading size of graph "<<picdirz<<" ...";
		// sprintf(picdiry, "%s%s%s", "./facebook/", picdirz, ".txt");
		// ifstream infile(picdiry); unsigned long long countero=0;
		// while (infile >> a >> b){mygraph_edge_no++;
		// if (countero < a){countero=a;}
		// if (countero < b){countero=b;}}
		// countero++;
		// mygraph_size=countero; cout<<"...done: "<<mygraph_size<<" nodes and "<<mygraph_edge_no<<" edges"<<endl;
		cout<<"initialization of arrays..."; initialization(); cout<<"...done"<<endl;
		// cout<<"reading the graph..."; ifstream gnfile(picdiry); while (gnfile >> a >> b){mygraph[a].push_back(b); mygraph[b].push_back(a); } cout<<"...done"<<endl;
		// cout<<"calculating distances... ("<<sample_all<<" BFSs in steps of "<<sample_steps<<")"<<endl<<endl;

		cout<<"Doing "<<NUM_THREADS<<" BFSs..."<<endl;
	    pthread_t threads[NUM_THREADS];
	    unsigned long long thread_args[NUM_THREADS];
	    int index;

	    // create all threads one by one
	    for (index = 0; index < NUM_THREADS; index++) {
	      thread_args[ index ] = rand()%mygraph_size;
		  		  // cout<<"mygraph_size is "<<mygraph_size <<endl;
		  // cout<<"thread argument is "<<thread_args[ 0 ] <<endl;
	      //printf("Creating thread %d with beacon %llu\n", index,  thread_args[ index ]);
	      pthread_create(&threads[index], NULL, perform_work, &thread_args[index]);
	    }

	    // wait for each thread to complete
			    for (index = 0; index < NUM_THREADS; ++index) {
			      // block until thread 'index' completes
			      pthread_join(threads[index], NULL);
			     // printf("Thread %d has completed\n", index);
			    }

		cout<<"All threads are completed"<<endl<<endl;
		
		cout<<endl; pairs_cal_no=0;
		for (int z=0; z<= max_sp; z++){pairs_cal_no=pairs_cal_no+sp_distribution[z];}//calculate number of pairs so far
		for (int j=1; j<= max_sp; j++){prob_distr_map[j]=(double)sp_distribution[j]/(double)pairs_cal_no;}
		
		for (int i=1; i<= max_sp; i++){printf("%3d   ", i);
		for (int j=0; j<1; j++){printf("  %0.7f  ", prob_distr_map[i]);}
		cout<<endl;}
		cout<<endl;
  exit(EXIT_SUCCESS);
}