#include<iostream>
#include<stdlib.h>
#define N 10
using namespace std;
bool Checkcyclic(int ed[][2], int edge, bool check[], int v) {  //to check for the cycle, on addition of a new edge in the random graph.
	int i;
	// If the current vertex is visited already, then the graph contains cycle.
	if(check[v] == true) {
		return false;
	} 
	else {
		check[v] = true;
		// For each vertex, go for all the vertex connected to it.
		for(i = edge; i >= 0; i--) {
			if(ed[i][0] == v) {
				return Checkcyclic(ed, edge, check, ed[i][1]);				
			}
		}
	}
	// In case, if the path ends then reassign the vertexes visited in that path to false again.
	check[v] = false;
	if(i == 0) return true;
}

void GenerateRandomGraphs(int e) {
	int i, j, ed[e][2], count, p;
	bool c[11];
	i = 0;
	while(i < e) {
		printf("%d", i);
		ed[i][0] = rand()%N+1;
		ed[i][1] = rand()%N+1;
		for(j = 1; j <= 10; j++)
			c[j] = false;
		if(Checkcyclic(ed, i, c, ed[i][0]) == true)
			i++;
	}
	cout<<"\nThe generated random random graph is: ";
	for(i = 0; i < N; i++) {
		count = 0;
		cout<<"\n\t"<<i+1<<"-> { ";
		for(j = 0; j < e; j++) {
			if(ed[j][0] == i+1) {
				cout<<ed[j][1]<<" ";
				count++;	
			} else if(ed[j][1] == i+1) {
				count++;			
			} else if(j == e-1 && count == 0)
				cout<<"Isolated Vertex!";		
		}
		cout<<" }";		
	}
}

int main() {
	int e;
	cout<<"Enter the number of edges for the random graphs: ";
	cin>>e;
	GenerateRandomGraphs(e);	
}
