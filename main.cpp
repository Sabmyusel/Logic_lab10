#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <queue>

int** G;
int size;
int max = 0, min = 0;
int* ex;
int* FT;
std::queue<int> Q;

int** gen(bool orient, bool weight) {
	int temp = 0;
	int rnd = 2, delta = 0;
	int** arr;
	arr = new int* [size];
	for (int i = 0; i < size; i++)
		arr[i] = new int[size] {0};

	if (weight) {
		rnd = 9;
		delta = 1;
	}

	srand(time(NULL));
	if (!orient) {
		for (int i = 0; i < size; i++)
			for (int k = i + 1; k < size; k++) {
				temp = rand() % 2;
				if (temp == 1)
					temp = rand() % rnd + delta;
				arr[i][k] = temp;
				arr[k][i] = temp;
			}
	}
	else {
		for (int i = 0; i < size; i++)
			for (int k = 0; k < size; k++) {
				if (i != k) {
					temp = rand() % 2;
					if (temp == 1)
						temp = rand() % rnd + delta;
					arr[i][k] = temp;
				}
			}
	}
	return arr;
}

void print() {

	printf("   ");
	for (int i = 0; i < size; i++)
		printf(" %d ", i + 1);

	printf("\n    ");
	for (int i = 0; i < size; i++)
		printf("-- ");

	for (int i = 0; i < size; i++) {
		printf("\n %d | ", i + 1);
		for (int k = 0; k < size; k++) {
			printf("%d  ", G[i][k]);
		}
	}
}

void check(int v, int i, int** dist) {
	for (int k = 0; k < size; k++)
		if (G[k][i] >= 1 && dist[v][k] != -1)
			if (dist[v][i] >= dist[v][k] + G[k][i])
				dist[v][i] = dist[v][k] + G[k][i];
}

int Max(int v, int** dist) {
	int temp = 0;

	for (int k = 0; k < size; k++)
		if (dist[v][k] >= temp)
			temp = dist[v][k];

	return temp;
}

void Max() {
	min = 1000;
	for (int i = 0; i < size; i++)
		if (ex[i] >= max)
			max = ex[i];
		else if (ex[i] <= min)
			min = ex[i];
}

void BFSD(int v, int** dist) {
	int tmp = v;

	dist[v][tmp] = 0;
	Q.push(v);

	while (!Q.empty()) {
		tmp = Q.front();
		Q.pop();

		printf("%d  ", tmp + 1);
		for (int i = 0; i < size; i++)
			if (G[tmp][i] >= 1 && dist[v][i] == -1) {
				dist[v][i] = dist[v][tmp] + G[tmp][i];
				check(v, i, dist);
				Q.push(i);
			}
	}
	printf("\tРасстояния :  ");
	for (int i = 0; i < size; i++) {
		printf("%d  ", dist[v][i]);
		FT[v] += dist[v][i];
	}
	printf("\nЭксцентриситет : %d", (ex[v] = Max(v, dist)));
}

void InDep() {
	ex = new int[size] {0};
	FT = new int[size] {0};
	int** dist = new int* [size];
	for (int i = 0; i < size; i++)
		dist[i] = new int[size];
	for (int i = 0; i < size; i++)
		for (int k = 0; k < size; k++)
			dist[i][k] = -1;

	for (int v = 0; v < size; v++) {
		printf("\n\nОбход №%d: ", v + 1);
		BFSD(v, dist);
	}

}

void subset() {

	std::queue<int> R, D, T;
	for(int i = 0; i < size; i++)
		if (ex[i] == min)
			R.push(i);
		else if (ex[i] == max)
			D.push(i);



	printf("\nЦентральные вершины: ");
	while (!R.empty()) {
		printf("%d  ", R.front() + 1);
		R.pop();
	}

	printf("\nПериферийные вершины: ");
	while (!D.empty()) {
		printf("%d  ", D.front() + 1);
		D.pop();
	}
	min = 1000;
	for (int i = 0; i < size; i++)
		if (FT[i] <= min)
			min = FT[i];

	for (int i = 0; i < size; i++)
		if (FT[i] == min)
			T.push(i);

	printf("\nЦентр тяжести: ");
	while (!T.empty()) {
		printf("%d  ", T.front() + 1);
		T.pop();
	}
	printf("\n\n");
}

int main(int argc, char* argv[]) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	bool W = false, O = false;
	int flag = 0;
	if (argc > 3) {
		printf("Ошибка\n");
		system("pause");
		return -1;
	}
	else if(argc > 1)
		for (int i = 0; i < argc; ++i)
		{
			if (strcmp(argv[i], "-w") == 0) {
				W = true;
				flag--;
			}
			else
				flag++;

			if (strcmp(argv[i], "-o") == 0) {
				O = true;
				flag--;
			}
			else 
				flag++;
		}
	if(flag >= 1)
		printf("Ошибка: неверный аргумент\n");

	if(W)
		printf("Взвешенный ");
	if(O)
		printf("ориентированный граф \n");
	else
		printf("неориентированный граф \n");

	while (1) {
		int tmp = size;
		printf("Введите кол-во вершин: ");
		scanf("%d", &size);

		if (tmp >= 0 && size != 0) {
			G = gen(O, W);
		}
		else
			size = tmp;

		print();
		InDep();
		printf("\n\n");

		Max();
		printf("Радиус = %d \tДиаметр = %d\n\n", min, max);

		print();
		subset();

		system("pause");
		system("cls");
	}
	return 0;
}