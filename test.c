#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>

#define MAX_LOCATIONS 50
#define MAX 50
#define INF 99999999


// �ص�ṹ��
typedef struct {
    char name[50];
    int x;
    int y;
    char description[256];
} Location;

// �ڽӱ�ڵ�
typedef struct Node {
    char locationName[50];
    int distance;
    struct Node* next;
} Node;

// У԰��ͼ�ṹ��
typedef struct {
    Location* locations;
    Node** adjacencyList;
    int* roadDistances;
    int* visited;
    int* currentPath;
    int* bestPath;
    int numLocations;
    int numRoads;
} CampusMap;

// ·������ṹ��
typedef struct {
    char locationName[50];
    int distance;
    char path[512];
} PathResult;

// ���нṹ��
typedef struct {
    int front, rear, size;
    unsigned capacity;
    int* array;
} Queue;

// ��������
Queue* createQueue(unsigned capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(queue->capacity * sizeof(int));
    return queue;
}

// �����Ƿ�Ϊ��
int isEmpty(Queue* queue) {
    return (queue->size == 0);
}

// ���
void enqueue(Queue* queue, int item) {
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

// ����
int dequeue(Queue* queue) {
    if (isEmpty(queue))
        return -1;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// �ͷŶ����ڴ�
void freeQueue(Queue* queue) {
    free(queue->array);
    free(queue);
}

//��������
CampusMap* createMap();
void initializeMap(CampusMap* map);
void displayMenu();
void addLocation(CampusMap* map, const char* name, int x, int y, const char* description);
void addEdge(CampusMap* map, const char* location1, const char* location2);
void removeLocation(CampusMap* map, const char* name);
void removeEdge(CampusMap* map, const char* location1, const char* location2);
void modifyLocation(CampusMap* map, const char* name, int x, int y, const char* description);
void saveLocationsToFile(CampusMap* map, const char* filename);
void saveEdgesToFile(CampusMap* map, const char* filename);
int calculateDistance(Location* location1, Location* location2);
void readLocationsFromFile(const char* filename, CampusMap* map);
void readRoadsFromFile(const char* filename, CampusMap* map);
void freeMap(CampusMap* map);
void printMap(CampusMap* map);
void planElectricCircuit(CampusMap* map);
int comparePathResults(const void* a, const void* b);
void shortestPathToAll(CampusMap* map, const char* startLocation);



int** dp;
int** dist;
int visited_all;

int tsp_dp(CampusMap* map, int mask, int pos) {
    if (mask == visited_all) {
        return dist[pos][0];
    }
    if (dp[mask][pos] != -1) {
        return dp[mask][pos];
    }

    int ans = INF;

    for (int city = 0; city < map->numLocations; city++) {
        if ((mask & (1 << city)) == 0) {
            int newAns = dist[pos][city] + tsp_dp(map, mask | (1 << city), city);
            ans = min(ans, newAns);
        }
    }

    return dp[mask][pos] = ans;
}

void shortestTour(CampusMap* map, const char* startLocation) {
    int start = -1;
    for (int i = 0; i < map->numLocations; i++) {
        if (strcmp(map->locations[i].name, startLocation) == 0) {
            start = i;
            break;
        }
    }

    dp = (int**)malloc((1 << map->numLocations) * sizeof(int*));
    for (int i = 0; i < (1 << map->numLocations); i++) {
        dp[i] = (int*)malloc(map->numLocations * sizeof(int));
    }

    dist = (int**)malloc(map->numLocations * sizeof(int*));
    for (int i = 0; i < map->numLocations; i++) {
        dist[i] = (int*)malloc(map->numLocations * sizeof(int));
    }

    for (int i = 0; i < (1 << map->numLocations); i++) {
        for (int j = 0; j < map->numLocations; j++) {
            dist[i][j] = calculateDistance(&map->locations[i], &map->locations[j]);
        }
    }

    visited_all = (1 << map->numLocations) - 1;

    for (int i = 0; i < (1 << map->numLocations); i++) {
        for (int j = 0; j < map->numLocations; j++) {
            dp[i][j] = -1;
        }
    }

    printf("��%s��������У԰���еص�����·�߳���Ϊ��%d\n", startLocation, tsp_dp(map, 1 << start, start));
}







//int getIndexOfLocation(CampusMap* campusMap, char* locationName) {
//    for (int i = 0; i < campusMap->numLocations; i++) {
//        if (strcmp(campusMap->locations[i].name, locationName) == 0) {
//            return i;
//        }
//    }
//    return -1;
//}
//void backtrack(CampusMap* campusMap, int location, int numVisited, int pathLength, int* minLength) {
//    if (numVisited == campusMap->numLocations) {
//        if (pathLength < *minLength) {
//            *minLength = pathLength;
//            memcpy(campusMap->bestPath, campusMap->currentPath, campusMap->numLocations * sizeof(int));
//        }
//        return;
//    }
//
//    Node* road = campusMap->adjacencyList[location];
//    while (road != NULL) {
//        int nextLocation = getIndexOfLocation(campusMap, road->locationName);
//        campusMap->visited[nextLocation] = 1;
//        campusMap->currentPath[numVisited] = nextLocation;
//        backtrack(campusMap, nextLocation, numVisited + 1, pathLength + road->distance, minLength);
//        campusMap->visited[nextLocation] = 0;
//        road = road->next;
//    }
//}
//PathResult shortestCycleThroughAllLocations(CampusMap* campusMap, int start) {
//    // �õ���ĳһ�ص�����������еص��Ҿ�����̵�·��
//    PathResult shortestPath = shortestPathThroughAllLocations(campusMap, start);
//
//    // �ҵ����һ���ص㲢�ص����
//    int end = campusMap->bestPath[campusMap->numLocations - 1];
//    Node* road = campusMap->adjacencyList[end];
//    while (road != NULL) {
//        if (strcmp(road->locationName, campusMap->locations[start].name) == 0) {
//            shortestPath.distance += road->distance;
//            strcat(shortestPath.path, " -> ");
//            strcat(shortestPath.path, campusMap->locations[start].name);
//            break;
//        }
//        road = road->next;
//    }
//
//    return shortestPath;
//}
//// ���������
//#define INF INT_MAX
//
//PathResult shortestPathThroughAllLocations(CampusMap* campusMap, int start) {
//    // ��ʼ����¼�Ѿ����ʹ��ĵص�
//    memset(campusMap->visited, 0, campusMap->numLocations * sizeof(int));
//    campusMap->visited[start] = 1;
//
//    // ��ʼ��·��
//    campusMap->currentPath[0] = start;
//    campusMap->bestPath[0] = start;
//
//    // ��ʼ����֪���·�ߵĳ���Ϊ�����
//    int minLength = INF;
//
//    // ʹ�û��ݷ��ݹ�������·��
//    backtrack(campusMap, start, 1, 0, &minLength);
//
//    // ������
//    PathResult result;
//    result.distance = minLength;
//    for (int i = 0; i < campusMap->numLocations - 1; i++) {
//        strcat(result.path, campusMap->locations[campusMap->bestPath[i]].name);
//        strcat(result.path, " -> ");
//    }
//    strcat(result.path, campusMap->locations[campusMap->bestPath[campusMap->numLocations - 1]].name);
//
//    return result;
//}



int main() {
    //������ͼ
    CampusMap* campusMap = createMap();

    //��ʼ����ͼ
    initializeMap(campusMap);
    readLocationsFromFile("place.txt", campusMap);
    readRoadsFromFile("road.txt", campusMap);

    int choice;
    char locationName1[50], locationName2[50];
    int x, y;
    char description[100];

    do {
        //��ʾ�˵�
        displayMenu();
        printf("����������ѡ��: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:  // ��ӵص�
        {
            printf("������ص�����: ");
            scanf("%s", locationName1);
            printf("���������� (x y): ");
            scanf("%d %d", &x, &y);
            printf("����������: ");
            scanf(" %[^\n]s", description);
            addLocation(campusMap, locationName1, x, y, description);
            saveLocationsToFile(campusMap, "place.txt");
            break;
        }

        case 2:  // ��ӵ�·
        {
            printf("��������ʼ�ص�����: ");
            scanf("%s", locationName1);
            printf("������Ŀ��ص�����: ");
            scanf("%s", locationName2);
            addEdge(campusMap, locationName1, locationName2);
            saveEdgesToFile(campusMap, "road.txt");
            break;
        }

        case 3:  // ɾ���ص�
        {
            printf("������Ҫɾ���ĵص�����: ");
            scanf("%s", locationName1);
            removeLocation(campusMap, locationName1);
            break;
        }

        case 4:  // ɾ����·
        {
            printf("��������ʼ�ص�����: ");
            scanf("%s", locationName1);
            printf("������Ŀ��ص�����: ");
            scanf("%s", locationName2);
            removeEdge(campusMap, locationName1, locationName2);
            break;
        }

        case 5:  // �޸ĵص�
        {
            printf("������Ҫ�޸ĵĵص�����: ");
            scanf("%s", locationName1);
            printf("�������µ����� (x y): ");
            scanf("%d %d", &x, &y);
            printf("�������µ�����: ");
            scanf(" %[^\n]s", description);
            modifyLocation(campusMap, locationName1, x, y, description);
            break;
        }

        case 6:  // ��ӡ��ͼ
        {
            printMap(campusMap);
            break;
        }

        case 7:  // ������·�滮
        {
            planElectricCircuit(campusMap);
            break;
        }

        case 8:  // ��ѯ���·��
        {
            printf("��������ʼ�ص�����: ");
            scanf("%s", locationName1);
            shortestPathToAll(campusMap, locationName1);
            break;
        }

        case 9:  // ���Ҵ�ĳ�ص�����������еص�����·��
        {
            printf("��������ʼ�ص�����: ");
            scanf("%s", locationName1);
            shortestTour(campusMap, locationName1);
            break;
        }

        case 0:  // �˳�����
            break;

        default:
            printf("��Чѡ��������0��10֮������֡�\n");
        }
    } while (choice != 0);

    // �ͷ��ڴ�
    free(campusMap->locations);
    free(campusMap->adjacencyList);
    free(campusMap->roadDistances);
    free(campusMap);
    freeMap(campusMap);

    return 0;
}

//��ʾ�˵�
void displayMenu() {
    printf("\n|*************************************************************|\n");
    printf("|****************** �人�Ƽ���ѧУ԰��ͼϵͳ *****************|\n");
    printf("|*************************************************************|\n");
    printf("|                       1. ��ӵص�                           |\n");
    printf("|                       2. ��ӵ�·                           |\n");
    printf("|                       3. ɾ���ص�                           |\n");
    printf("|                       4. ɾ����·                           |\n");
    printf("|                       5. �޸ĵص�                           |\n");
    printf("|                       6. ��ӡ��ͼ                           |\n");
    printf("|                       7. �����·                           |\n");
    printf("|                       8. ��ѯ���·��                       |\n");
    printf("|                       9. ���·�߾������еص�               |\n");
    printf("|                      10. ��̻�·�������еص�               |\n");
    printf("|                       0. �˳�����                           |\n");
    printf("|*************************************************************|\n");
}

//������ͼ
CampusMap* createMap() {
    CampusMap* map = (CampusMap*)malloc(sizeof(CampusMap));
    map->locations = NULL;
    map->adjacencyList = NULL;
    map->roadDistances = NULL;
    /* map->shortestDistances = NULL;
     calculateShortestDistances(map);*/
    map->numLocations = 0;
    map->numRoads = 0;
    return map;
}

// ��ʼ��CampusMap�ṹ��
void initializeMap(CampusMap* map) {
    map->visited = (int*)malloc(map->numLocations * sizeof(int));
    memset(map->visited, 0, map->numLocations * sizeof(int));
    map->currentPath = (int*)malloc(map->numLocations * sizeof(int));
    map->bestPath = (int*)malloc(map->numLocations * sizeof(int));
}

// ��ӵص�
void addLocation(CampusMap* map, const char* name, int x, int y, const char* description) {
    //ȷ���ص�����Ψһ
    for (int i = 0; i < map->numLocations; i++) {
        if (strcmp(map->locations[i].name, name) == 0) {
            printf("Error: �ص����Ʊ���Ψһ��\n");
            return;
        }
    }

    //�����µĿռ�
    map->locations = (Location*)realloc(map->locations, (map->numLocations + 1) * sizeof(Location));

    // ����µص�
    strcpy(map->locations[map->numLocations].name, name);
    map->locations[map->numLocations].x = x;
    map->locations[map->numLocations].y = y;
    strcpy(map->locations[map->numLocations].description, description);

    // ���µص�����
    map->numLocations++;

    printf("�ص���ӳɹ���\n");
}

//��ӵ�·
void addEdge(CampusMap* map, const char* location1, const char* location2) {
    //���������ص��ڵص������е�����
    int index1 = -1, index2 = -1;
    for (int i = 0; i < map->numLocations; i++) {
        if (strcmp(map->locations[i].name, location1) == 0) {
            index1 = i;
        }
        else if (strcmp(map->locations[i].name, location2) == 0) {
            index2 = i;
        }
    }

    //��ӱ�
    if (index1 != -1 && index2 != -1) {
        Node* newNode1 = (Node*)malloc(sizeof(Node));
        strcpy(newNode1->locationName, location2);
        newNode1->distance = (int)ceil(sqrt(pow(map->locations[index1].x - map->locations[index2].x, 2) + pow(map->locations[index1].y - map->locations[index2].y, 2)));

        newNode1->next = map->adjacencyList[index1];
        map->adjacencyList[index1] = newNode1;

        Node* newNode2 = (Node*)malloc(sizeof(Node));
        strcpy(newNode2->locationName, location1);
        newNode2->distance = newNode1->distance;

        newNode2->next = map->adjacencyList[index2];
        map->adjacencyList[index2] = newNode2;

        //���µ�·��������
        map->roadDistances = (int*)realloc(map->roadDistances, (map->numRoads + 1) * sizeof(int));
        map->roadDistances[map->numRoads] = newNode1->distance;
        map->numRoads++;

        printf("��·��ӳɹ���\n");
    }
    else {
        printf("Error: һ���������ص�δ�ҵ���\n");
    }
}

//ɾ���ص�
void removeLocation(CampusMap* map, const char* name) {
    //���ҵص��ڵص������е�����
    int index = -1;
    for (int i = 0; i < map->numLocations; i++) {
        if (strcmp(map->locations[i].name, name) == 0) {
            index = i;
            break;
        }
    }

    //ɾ���ص㼰����صı�
    if (index != -1) {
        //ɾ����õص���صı�
        for (int i = 0; i < map->numLocations; i++) {
            Node* current = map->adjacencyList[i];
            Node* prev = NULL;

            while (current != NULL) {
                if (strcmp(current->locationName, name) == 0) {
                    if (prev == NULL) {
                        map->adjacencyList[i] = current->next;
                    }
                    else {
                        prev->next = current->next;
                    }

                    free(current);
                    break;
                }

                prev = current;
                current = current->next;
            }
        }

        //ɾ���ص�
        for (int i = index; i < map->numLocations - 1; i++) {
            map->locations[i] = map->locations[i + 1];
        }

        //���µص�����
        map->numLocations--;

        //���µص��ļ�
        saveLocationsToFile(map, "place.txt");

        printf("�ص�ɾ���ɹ���\n");
    }
    else {
        printf("Error: �ص�δ�ҵ���\n");
    }
}

//ɾ����·
void removeEdge(CampusMap* map, const char* location1, const char* location2) {
    //���������ص��ڵص������е�����
    int index1 = -1, index2 = -1;
    for (int i = 0; i < map->numLocations; i++) {
        if (strcmp(map->locations[i].name, location1) == 0) {
            index1 = i;
        }
        else if (strcmp(map->locations[i].name, location2) == 0) {
            index2 = i;
        }
    }

    //ɾ����
    if (index1 != -1 && index2 != -1) {
        //ɾ�����������ص���صı�
        Node* current1 = map->adjacencyList[index1];
        Node* prev1 = NULL;

        while (current1 != NULL) {
            if (strcmp(current1->locationName, location2) == 0) {
                if (prev1 == NULL) {
                    map->adjacencyList[index1] = current1->next;
                }
                else {
                    prev1->next = current1->next;
                }

                free(current1);
                break;
            }

            prev1 = current1;
            current1 = current1->next;
        }

        Node* current2 = map->adjacencyList[index2];
        Node* prev2 = NULL;

        while (current2 != NULL) {
            if (strcmp(current2->locationName, location1) == 0) {
                if (prev2 == NULL) {
                    map->adjacencyList[index2] = current2->next;
                }
                else {
                    prev2->next = current2->next;
                }

                free(current2);
                break;
            }

            prev2 = current2;
            current2 = current2->next;
        }

        //���µ�·��������
        for (int i = 0; i < map->numRoads - 1; i++) {
            map->roadDistances[i] = map->roadDistances[i + 1];
        }

        //���µ�·����
        map->numRoads--;

        //���µ�·�ļ�
        saveEdgesToFile(map, "road.txt");

        printf("��·ɾ���ɹ���\n");
    }
    else {
        printf("Error: һ���������ص�δ�ҵ���\n");
    }
}

//�޸ĵص�
void modifyLocation(CampusMap* map, const char* name, int x, int y, const char* description) {
    //���ҵص��ڵص������е�����
    int index = -1;
    for (int i = 0; i < map->numLocations; i++) {
        if (strcmp(map->locations[i].name, name) == 0) {
            index = i;
            break;
        }
    }

    //�޸ĵص���Ϣ
    if (index != -1) {
        map->locations[index].x = x;
        map->locations[index].y = y;
        strcpy(map->locations[index].description, description);

        //���µص��ļ�
        saveLocationsToFile(map, "place.txt");

        printf("�ص��޸ĳɹ���\n");
    }
    else {
        printf("Error: �ص�δ�ҵ���\n");
    }
}

//����ص���Ϣ���ļ�
void saveLocationsToFile(CampusMap* map, const char* filename) {
    FILE* file = fopen("place.txt", "w");
    if (file == NULL) {
        printf("Error: �޷����ļ� %s ����д�롣\n", filename);
        return;
    }

    for (int i = 0; i < map->numLocations; i++) {
        fprintf(file, "%s %d %d %s\n", map->locations[i].name, map->locations[i].x, map->locations[i].y, map->locations[i].description);
    }

    fclose(file);
    printf("�ص���Ϣ�ѳɹ����浽�ļ� %s��\n", filename);
}

//�������Ϣ���ļ�
void saveEdgesToFile(CampusMap* map, const char* filename) {
    FILE* file = fopen("road.txt", "w");
    if (file == NULL) {
        printf("Error: �޷����ļ� %s ����д�롣\n", filename);
        return;
    }

    for (int i = 0; i < map->numLocations; i++) {
        Node* current = map->adjacencyList[i];
        while (current != NULL) {
            fprintf(file, "%s %s\n", map->locations[i].name, current->locationName);
            current = current->next;
        }
    }

    fclose(file);
    printf("����Ϣ�ѳɹ����浽�ļ� %s��\n", filename);
}

//��ȡ�ص���Ϣ����䵽 CampusMap �ṹ����
void readLocationsFromFile(const char* filename, CampusMap* map) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: �޷����ļ� %s ���ж�ȡ��\n", filename);
        exit(EXIT_FAILURE);
    }

    //����ص�����
    map->numLocations = 0;
    while (!feof(file)) {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            map->numLocations++;
        }
    }
    rewind(file);

    //����ռ�
    map->locations = (Location*)malloc(map->numLocations * sizeof(Location));

    //��ȡ�ص���Ϣ
    for (int i = 0; i < map->numLocations; i++) {
        fscanf(file, "%s %d %d %[^\n]", map->locations[i].name, &map->locations[i].x,
            &map->locations[i].y, map->locations[i].description);
    }

    fclose(file);
}

//��ȡ��·��Ϣ����䵽 CampusMap �ṹ����
void readRoadsFromFile(const char* filename, CampusMap* map) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: �޷����ļ� %s ���ж�ȡ��\n", filename);
        exit(EXIT_FAILURE);
    }

    //�����·����
    map->numRoads = 0;
    while (!feof(file)) {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            map->numRoads++;
        }
    }
    rewind(file);

    //����ռ�
    map->adjacencyList = (Node**)malloc(map->numLocations * sizeof(Node*));
    map->roadDistances = (int*)malloc(map->numLocations * map->numLocations * sizeof(int));

    //��ʼ���ڽӱ�͵�·��������
    for (int i = 0; i < map->numLocations; i++) {
        map->adjacencyList[i] = NULL;
        for (int j = 0; j < map->numLocations; j++) {
            map->roadDistances[i * map->numLocations + j] = 0;
        }
    }

    //��ȡ����Ϣ
    for (int i = 0; i < map->numRoads; i++) {
        char location1[50], location2[50];

        fscanf(file, "%s %s", location1, location2);

        //�ҵ��ص��ڵص������е�����
        int index1 = -1, index2 = -1;
        for (int j = 0; j < map->numLocations; j++) {
            if (strcmp(map->locations[j].name, location1) == 0) {
                index1 = j;
            }
            if (strcmp(map->locations[j].name, location2) == 0) {
                index2 = j;
            }
        }

        // �������
        int distance = calculateDistance(&map->locations[index1], &map->locations[index2]);

        //��ӱߵ��ڽӱ�
        if (index1 != -1 && index2 != -1 && index1 != index2) {
            Node* newNode1 = (Node*)malloc(sizeof(Node));
            strcpy(newNode1->locationName, location2);
            newNode1->distance = distance;
            newNode1->next = map->adjacencyList[index1];
            map->adjacencyList[index1] = newNode1;

            //���µ�·��������
            map->roadDistances[index1 * map->numLocations + index2] = distance;
            map->roadDistances[index2 * map->numLocations + index1] = distance;
        }
    }

    fclose(file);
}

//�ͷŵ�ͼ�ڴ�
void freeMap(CampusMap* map) {
    free(map->locations);

    for (int i = 0; i < map->numLocations; i++) {
        Node* current = map->adjacencyList[i];
        while (current != NULL) {
            Node* next = current->next;
            free(current);
            current = next;
        }
    }
    free(map->adjacencyList);
    free(map->roadDistances);
    free(map->visited);
    free(map->currentPath);
    free(map->bestPath);
}

//��ӡ��ͼ
void printMap(CampusMap* map) {
    char campusMap[15 * 3][15 * 5];  // ÿ���ص�ռ��һ��3x5�ľŹ���
    char locationsMap[50];            // ʹ����ĸ "A" �� "Z" �� "a" �� "z" ��ʾ�ص�
    int locationIndex = 0;

    //��ʼ����ͼ
    for (int i = 0; i < 15 * 3; i++) {
        for (int j = 0; j < 15 * 5; j++) {
            campusMap[i][j] = '.';
        }
    }

    //��ǵص�
    for (int i = 0; i < map->numLocations; i++) {
        int x = map->locations[i].x * 3;
        int y = map->locations[i].y * 5;

        //ʹ����ĸ "A" �� "Z" �� "a" �� "z" ��ʾ�ص�
        if (locationIndex < 26) {
            locationsMap[locationIndex] = 'A' + locationIndex;
        }
        else {
            locationsMap[locationIndex] = 'a' + (locationIndex - 26);
        }

        //�ڵ�ͼ�ϱ�ǵص�
        campusMap[x][y] = locationsMap[locationIndex];
        //�������ұ߿�
        campusMap[x - 1][y] = campusMap[x + 1][y] = '-';
        campusMap[x][y - 1] = campusMap[x][y + 1] = '|';
        //�ĸ��ǵı߿�
        campusMap[x - 1][y - 1] = campusMap[x - 1][y + 1] = campusMap[x + 1][y - 1] = campusMap[x + 1][y + 1] = '+';

        locationIndex++;
    }

    //��������
    printf("*************************�� �人�Ƽ���ѧУ԰��ͼ������ *************************\n");

    //����ϱ߿�
    for (int i = 0; i < 15 * 5 + 2; i++) {
        printf("-");
    }
    printf("\n");

    //��ӡ��ͼ
    for (int i = 0; i < 15 * 3; i++) {
        printf("|");
        for (int j = 0; j < 15 * 5; j++) {
            printf("%c", campusMap[i][j]);
        }
        printf("|\n");
    }

    //����±߿�
    for (int i = 0; i < 15 * 5 + 2; i++) {
        printf("-");
    }
    printf("\n");

    //��ӡ�ص���Ϣ
    for (int i = 0; i < locationIndex; i++) {
        printf("%c: %s", locationsMap[i], map->locations[i].name);
        printf("   ����: (%d, %d)", map->locations[i].x, map->locations[i].y);
        printf("   ����: %s\n", map->locations[i].description);
    }
}

// ��������֮��ľ���
int calculateDistance(Location* location1, Location* location2) {
    int xDiff = location1->x - location2->x;
    int yDiff = location1->y - location2->y;
    return (int)round(sqrt(xDiff * xDiff + yDiff * yDiff));
}

// Dijkstra �㷨�������·��
void dijkstra(CampusMap* map, int startLocation, int* parent, int* distance) {
    int* visited = (int*)malloc(map->numLocations * sizeof(int));

    for (int i = 0; i < map->numLocations; i++) {
        visited[i] = 0;
        parent[i] = -1;
        distance[i] = INT_MAX;
    }

    distance[startLocation] = 0;

    for (int i = 0; i < map->numLocations; i++) {
        int minDistance = INT_MAX;
        int currentLocation = -1;

        // ���������û��������ʼ�ص㿪ʼ����
        if (i == 0) {
            currentLocation = startLocation;
        }
        else {
            for (int j = 0; j < map->numLocations; j++) {
                if (!visited[j] && distance[j] < minDistance) {
                    minDistance = distance[j];
                    currentLocation = j;
                }
            }
        }

        if (currentLocation == -1) {
            break;  // No more reachable locations
        }

        visited[currentLocation] = 1;

        Node* current = map->adjacencyList[currentLocation];
        while (current != NULL) {
            int neighbor = -1;
            for (int k = 0; k < map->numLocations; k++) {
                if (strcmp(map->locations[k].name, current->locationName) == 0) {
                    neighbor = k;
                    break;
                }
            }

            if (neighbor != -1 && !visited[neighbor] && (distance[currentLocation] != INT_MAX) &&
                (distance[currentLocation] + current->distance < distance[neighbor])) {
                distance[neighbor] = distance[currentLocation] + current->distance;
                parent[neighbor] = currentLocation;
            }

            current = current->next;
        }
    }

    free(visited);
}

// ��·�滮(Dijkstra �㷨�������·��)
void planElectricCircuit(CampusMap* map) {
    // ��ʼ������
    int* parent = (int*)malloc(map->numLocations * sizeof(int));
    int* distance = (int*)malloc(map->numLocations * sizeof(int));
    int totalDistance = 0;

    // �û�������ʼ�ص�
    char startLocationName[50];
    printf("��������ʼ�ص������: ");
    scanf("%s", startLocationName);

    int start = -1;
    for (int i = 0; i < map->numLocations; i++) {
        if (strcmp(map->locations[i].name, startLocationName) == 0) {
            start = i;
            break;
        }
    }

    if (start == -1) {
        printf("����δ�ҵ���ʼ�ص㡣\n");
        return;
    }

    // ��ָ���ص㿪ʼ�������·��
    dijkstra(map, start, parent, distance);

    // ��������
    Queue* pathQueue = createQueue(map->numLocations);

    // ��ָ���ص㿪ʼ�������·��
    dijkstra(map, start, parent, distance);

    // ��·����˳��������
    enqueue(pathQueue, start);
    for (int i = 0; i < map->numLocations; i++) {
        if (parent[i] != -1 && i != start) {
            enqueue(pathQueue, i);
        }
    }

    // ��ӡ���·��
    printf("\n��%s��ʼ�����������·����:\n", startLocationName);

    // ���Ӳ���ӡ·��
    int pathNumber = 1;
    while (!isEmpty(pathQueue)) {
        int currentLocation = dequeue(pathQueue);

        if (parent[currentLocation] != -1) {
            int currentDistance = distance[currentLocation] - distance[parent[currentLocation]];
            printf("%d��%s -> %s (���룺%d)\n", pathNumber, map->locations[parent[currentLocation]].name,
                map->locations[currentLocation].name, currentDistance);
            totalDistance += currentDistance;
            pathNumber++;
        }
    }

    // ��ӡ�ܳ���
    printf("\n���������·�ܳ���: %d\n", totalDistance);

    // �ͷŶ����ڴ�
    freeQueue(pathQueue);

    free(parent);
    free(distance);
}

//�ȽϺ�����������
int comparePathResults(const void* a, const void* b) {
    const PathResult* pathA = (const PathResult*)a;
    const PathResult* pathB = (const PathResult*)b;

    //�ȱȽϾ��룬���������ͬ�����յص��ԭ��˳��
    if (pathA->distance == pathB->distance) {
        return strcmp(pathA->locationName, pathB->locationName);
    }

    return (pathA->distance < pathB->distance) ? -1 : 1;
}

//��ѯ���·��
void shortestPathToAll(CampusMap* map, const char* startLocation) {
    //��ʼ������
    int start = -1;
    for (int i = 0; i < map->numLocations; i++) {
        if (strcmp(map->locations[i].name, startLocation) == 0) {
            start = i;
            break;
        }
    }

    if (start == -1) {
        printf("����δ�ҵ���ʼ�ص㡣\n");
        return;
    }

    int* distance = (int*)malloc(map->numLocations * sizeof(int));
    int* parent = (int*)malloc(map->numLocations * sizeof(int));

    //��ʼ�� distance �� parent ����
    for (int i = 0; i < map->numLocations; i++) {
        distance[i] = INT_MAX;
        parent[i] = -1;
    }

    //������ʼ��ľ���Ϊ 0
    distance[start] = 0;

    //�������·��
    for (int k = 0; k < map->numLocations - 1; k++) {
        for (int i = 0; i < map->numLocations; i++) {
            Node* current = map->adjacencyList[i];
            while (current != NULL) {
                int j = -1;
                for (int l = 0; l < map->numLocations; l++) {
                    if (strcmp(map->locations[l].name, current->locationName) == 0) {
                        j = l;
                        break;
                    }
                }

                if (j != -1 && (distance[i] != INT_MAX) && (distance[i] + current->distance < distance[j])) {
                    distance[j] = distance[i] + current->distance;
                    parent[j] = i;
                }

                current = current->next;
            }
        }
    }

    //�洢��ѯ���
    PathResult* pathResults = (PathResult*)calloc(map->numLocations, sizeof(PathResult));

    //��ӡ��ѯ���
    printf("\n��%s�����еص�����·��:\n", startLocation);
    for (int i = 0; i < map->numLocations; i++) {
        if (i != start) {
            // �洢��ѯ������ṹ��������
            pathResults[i].distance = distance[i];
            strcpy(pathResults[i].locationName, map->locations[i].name);
            strcpy(pathResults[i].path, map->locations[i].name);

            // ���·��
            int current = i;
            while (parent[current] != -1) {
                // �������ѯ�����·���ַ�����
                strcat(pathResults[i].path, " <- ");
                strcat(pathResults[i].path, map->locations[parent[current]].name);

                current = parent[current];
            }

            printf("����%s\t���룺%d\t·����%s\n", map->locations[i].name, distance[i], pathResults[i].path);
        }
    }

    //�����ѯ���
    qsort(pathResults, map->numLocations, sizeof(PathResult), comparePathResults);

    //��������Ĳ�ѯ���
    printf("\n������ӽ���Զ��˳����ʾ:\n");
    for (int i = 1; i < map->numLocations; i++) {
        printf("����%s\t���룺%d\t·����%s\n", pathResults[i].locationName, pathResults[i].distance, pathResults[i].path);
    }

    //�ͷ��ڴ�
    free(distance);
    free(parent);
    free(pathResults);
}
