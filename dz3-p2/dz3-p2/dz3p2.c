#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INFTY 9999999

void printGraph(int** graph, int nodes) {
	int i;
	int j;

	for (i = 0; i < nodes; i++) {
		for (j = 0; j < nodes; j++) {
			printf("%d ", graph[i][j]);
		}
		printf("\n");
	}
}

int** loadGraph(int* n, int* e, int** costs) {
	int** graph;
	int nodes;
	int edges;
	int neighbour;
	int weight;
	int i;
	int j;

	printf("Broj cvorova? ");
	scanf("%d", &nodes);
	printf("Broj grana? ");
	scanf("%d", &edges);
	graph = malloc(sizeof(int*)* nodes);
	if (graph == NULL) {
		exit(1);
	}
	for (i = 0; i < nodes; i++) {
		graph[i] = malloc(sizeof(int)* nodes);
		if (graph[i] == NULL) {
			exit(1);
		}
	}
	if ((*costs) != NULL) {
		free((*costs));
	}
	(*costs) = malloc(sizeof(int)* nodes);
	if ((*costs) == NULL) {
		exit(1);
	}
	for (i = 0; i < nodes; i++) {
		for (j = 0; j < nodes; j++) {
			if (i != j) {
				graph[i][j] = INFTY;
			}
			else {
				graph[i][j] = 0;
			}
		}
	}
	for (i = 0; i < nodes; i++) {
		printf("Cena pridruzena cvoru %d: ", i);
		scanf("%d", &((*costs)[i]));
		printf("Susedi cvora %d i tezine grana koje ih spajaju, tezina = -1 za kraj\n", i);
		while (true) {
			scanf("%d%d", &neighbour, &weight);
			if (weight != -1 && neighbour < nodes) {
				graph[i][neighbour] = graph[neighbour][i] = weight;
			}
			else {
				break;
			}
		}
	}
	*n = nodes;
	*e = edges;

	return graph;
}

int* dijkstra(int** graph, int nodes, int start, int end, int** prev) {
	int* distance;
	int* visited;
	int i, j;
	int nextNode;
	int minDistance;
	int count;

	distance = malloc(sizeof(int)* nodes);
	if (distance == NULL) {
		exit(1);
	}
	visited = malloc(sizeof(int)* nodes);
	if (visited == NULL) {
		exit(1);
	}
	if ((*prev) != NULL) {
		free((*prev));
	}
	(*prev) = malloc(sizeof(int)* nodes);
	if ((*prev) == NULL) {
		exit(1);
	}
	for (i = 0; i < nodes; i++) {
		distance[i] = graph[start][i];
		visited[i] = false;
		(*prev)[i] = graph[start][i] != INFTY ? start : -1;
	}
	distance[start] = 0;
	visited[start] = true;
	count = 1;
	while (count < nodes - 1) {
		minDistance = INFTY;
		nextNode = end;
		for (i = 0; i < nodes; i++) {
			if (distance[i] < minDistance && !visited[i]) {
				minDistance = distance[i];
				nextNode = i;
			}
		}
		if (nextNode == end) {
			break;
		}
		visited[nextNode] = true;
		for (i = 0; i < nodes; i++) {
			if (!visited[i]) {
				if (minDistance + graph[nextNode][i] < distance[i]) {
					distance[i] = minDistance + graph[nextNode][i];
					(*prev)[i] = nextNode;
				}
			}
		}
		count++;
	}

	return distance;
}

void shortestPath(int** graph, int nodes, int* cost) {
	int start;
	int end;
	int money;
	int sum;
	int* prev = NULL;
	int* distance = NULL;
	int i, j;

	printf("Unesite pocetni i krajnji cvor i sumu novca koji je na raspolaganju: ");
	scanf("%d%d%d", &start, &end, &money);
	if (start < 0 || start >= nodes || end < 0 || end >= nodes) {
		printf("Nepostojeci cvorovi\n");
		return;
	}
	while (true) {
		sum = 0;
		distance = dijkstra(graph, nodes, start, end, &prev);
		if (distance[end] == INFTY) {
			printf("Do trazenog cvora je nemoguce doci\n");
			return;
		}
		i = end;
		while (i != start) {
			sum += cost[i];
			i = prev[i];
		}
		if (sum <= money) {
			break;
		}
		else {
			i = end;
			while (i != start) {
				j = i;
				i = prev[i];
			}
			graph[start][j] = graph[j][start] = INFTY;
		}
	}
	printf("Putanja od cvora %d do cvora %d: %d", start, end, end);
	i = end;
	while (i != start) {
		i = prev[i];
		printf("<-%d", i);
	}
	printf("\n Duzina je %d, cena %d\n", distance[end], sum);
}

void printMenu() {
	printf("1. Ucitavanje grafa\n");
	printf("2. Odredjivanje najkraceg puta\n");
	printf("0. Kraj rada\n");
}

int main(void) {
	int choice;
	int nodes;
	int edges;
	int* cost = NULL;
	int** graph = NULL;
	bool radi;

	int i;

	radi = true;
	while (radi) {
		printMenu();
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			graph = loadGraph(&nodes, &edges, &cost);
			printf("Uneti graf:\n");
			printGraph(graph, nodes);
			printf("\nCene pridruzene cvorovima:\n");
			for (i = 0; i < nodes; i++) {
				printf("%d ", cost[i]);
			}
			printf("\n");
			break;
		case 2:
			shortestPath(graph, nodes, cost);
			break;
		case 0:
			radi = false;
			break;
		default:
			printf("Nepostojeca opcija\n");
		}
	}

	return 0;
}