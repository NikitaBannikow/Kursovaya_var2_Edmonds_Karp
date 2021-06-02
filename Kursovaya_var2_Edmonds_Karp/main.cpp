// This program was made by Bannikov Nikita, group 9309 
#include "Header.h"

void load_from_file() {
	try {
		Graph g;
		cout << "Loading graph from a 'graph_data.txt' file..." << endl;
		g.load_from_file("graph_data.txt");
		printf("  Max.flow: %lld", g.maxFlow());
		cout << endl << "  Graph state: " << g.stateDump() <<  endl;
	}
	catch (const std::exception& exc) {
		std::cerr << "Error: " << exc.what();
	}
}

void graph_from_wikipedia() {
	Graph* graph = new Graph;

	cout << endl;
	cout << "Create a graph from wikipedia example and set debug mode to get graph state on each step" << endl;

	graph->setDebugMode(true);
	
	// See https://en.wikipedia.org/wiki/Edmonds%E2%80%93Karp_algorithm
    // A -> S
    // G -> T

	graph->addEdge("S", "B", 3);
	graph->addEdge("S", "D", 3);
	graph->addEdge("B", "C", 4);
	graph->addEdge("C", "S", 3);
	graph->addEdge("C", "D", 1);
	graph->addEdge("C", "E", 2);
	graph->addEdge("E", "B", 1);
	graph->addEdge("D", "E", 2);
	graph->addEdge("D", "F", 6);
	graph->addEdge("F", "T", 9);
	graph->addEdge("E", "T", 1);

	cout << endl;
	printf(" Max.flow: %lld", graph->maxFlow());

	delete graph;
}

int main() {
	// Kursovaya
	load_from_file();

	// Debug graph form WikiPedia (iterative state)
	graph_from_wikipedia();

	return 0;
}

