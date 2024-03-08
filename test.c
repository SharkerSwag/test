#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>

#define MAX_LOCATIONS 50
#define MAX 50
#define INF 99999999


// 地点结构体
typedef struct {
    char name[50];
    int x;
    int y;
    char description[256];
} Location;

// 邻接表节点
typedef struct Node {
    char locationName[50];
    int distance;
    struct Node* next;
} Node;

// 校园地图结构体
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

// 路径结果结构体
typedef struct {
    char locationName[50];
    int distance;
    char path[512];
} PathResult;

// 队列结构体
typedef struct {
    int front, rear, size;
    unsigned capacity;
    int* array;
} Queue;

// 创建队列
Queue* createQueue(unsigned capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(queue->capacity * sizeof(int));
    return queue;
}

// 队列是否为空
int isEmpty(Queue* queue) {
    return (queue->size == 0);
}

// 入队
void enqueue(Queue* queue, int item) {
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

// 出队
int dequeue(Queue* queue) {
    if (isEmpty(queue))
        return -1;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// 释放队列内存
void freeQueue(Queue* queue) {
    free(queue->array);
    free(queue);
}

//函数声明
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

    printf("从%s出发经过校园所有地点的最短路线长度为：%d\n", startLocation, tsp_dp(map, 1 << start, start));
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
//    // 得到从某一地点出发经过所有地点且距离最短的路径
//    PathResult shortestPath = shortestPathThroughAllLocations(campusMap, start);
//
//    // 找到最后一个地点并回到起点
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
//// 定义无穷大
//#define INF INT_MAX
//
//PathResult shortestPathThroughAllLocations(CampusMap* campusMap, int start) {
//    // 初始化记录已经访问过的地点
//    memset(campusMap->visited, 0, campusMap->numLocations * sizeof(int));
//    campusMap->visited[start] = 1;
//
//    // 初始化路线
//    campusMap->currentPath[0] = start;
//    campusMap->bestPath[0] = start;
//
//    // 初始化已知最短路线的长度为无穷大
//    int minLength = INF;
//
//    // 使用回溯法递归查找最短路线
//    backtrack(campusMap, start, 1, 0, &minLength);
//
//    // 构造结果
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
    //创建地图
    CampusMap* campusMap = createMap();

    //初始化地图
    initializeMap(campusMap);
    readLocationsFromFile("place.txt", campusMap);
    readRoadsFromFile("road.txt", campusMap);

    int choice;
    char locationName1[50], locationName2[50];
    int x, y;
    char description[100];

    do {
        //显示菜单
        displayMenu();
        printf("请输入您的选择: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:  // 添加地点
        {
            printf("请输入地点名称: ");
            scanf("%s", locationName1);
            printf("请输入坐标 (x y): ");
            scanf("%d %d", &x, &y);
            printf("请输入描述: ");
            scanf(" %[^\n]s", description);
            addLocation(campusMap, locationName1, x, y, description);
            saveLocationsToFile(campusMap, "place.txt");
            break;
        }

        case 2:  // 添加道路
        {
            printf("请输入起始地点名称: ");
            scanf("%s", locationName1);
            printf("请输入目标地点名称: ");
            scanf("%s", locationName2);
            addEdge(campusMap, locationName1, locationName2);
            saveEdgesToFile(campusMap, "road.txt");
            break;
        }

        case 3:  // 删除地点
        {
            printf("请输入要删除的地点名称: ");
            scanf("%s", locationName1);
            removeLocation(campusMap, locationName1);
            break;
        }

        case 4:  // 删除道路
        {
            printf("请输入起始地点名称: ");
            scanf("%s", locationName1);
            printf("请输入目标地点名称: ");
            scanf("%s", locationName2);
            removeEdge(campusMap, locationName1, locationName2);
            break;
        }

        case 5:  // 修改地点
        {
            printf("请输入要修改的地点名称: ");
            scanf("%s", locationName1);
            printf("请输入新的坐标 (x y): ");
            scanf("%d %d", &x, &y);
            printf("请输入新的描述: ");
            scanf(" %[^\n]s", description);
            modifyLocation(campusMap, locationName1, x, y, description);
            break;
        }

        case 6:  // 打印地图
        {
            printMap(campusMap);
            break;
        }

        case 7:  // 铺设线路规划
        {
            planElectricCircuit(campusMap);
            break;
        }

        case 8:  // 查询最短路径
        {
            printf("请输入起始地点名称: ");
            scanf("%s", locationName1);
            shortestPathToAll(campusMap, locationName1);
            break;
        }

        case 9:  // 查找从某地点出发经过所有地点的最短路线
        {
            printf("请输入起始地点名称: ");
            scanf("%s", locationName1);
            shortestTour(campusMap, locationName1);
            break;
        }

        case 0:  // 退出程序
            break;

        default:
            printf("无效选择，请输入0到10之间的数字。\n");
        }
    } while (choice != 0);

    // 释放内存
    free(campusMap->locations);
    free(campusMap->adjacencyList);
    free(campusMap->roadDistances);
    free(campusMap);
    freeMap(campusMap);

    return 0;
}

//显示菜单
void displayMenu() {
    printf("\n|*************************************************************|\n");
    printf("|****************** 武汉科技大学校园地图系统 *****************|\n");
    printf("|*************************************************************|\n");
    printf("|                       1. 添加地点                           |\n");
    printf("|                       2. 添加道路                           |\n");
    printf("|                       3. 删除地点                           |\n");
    printf("|                       4. 删除道路                           |\n");
    printf("|                       5. 修改地点                           |\n");
    printf("|                       6. 打印地图                           |\n");
    printf("|                       7. 铺设电路                           |\n");
    printf("|                       8. 查询最短路径                       |\n");
    printf("|                       9. 最短路线经过所有地点               |\n");
    printf("|                      10. 最短回路经过所有地点               |\n");
    printf("|                       0. 退出程序                           |\n");
    printf("|*************************************************************|\n");
}

//创建地图
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

// 初始化CampusMap结构体
void initializeMap(CampusMap* map) {
    map->visited = (int*)malloc(map->numLocations * sizeof(int));
    memset(map->visited, 0, map->numLocations * sizeof(int));
    map->currentPath = (int*)malloc(map->numLocations * sizeof(int));
    map->bestPath = (int*)malloc(map->numLocations * sizeof(int));
}

// 添加地点
void addLocation(CampusMap* map, const char* name, int x, int y, const char* description) {
    //确保地点名称唯一
    for (int i = 0; i < map->numLocations; i++) {
        if (strcmp(map->locations[i].name, name) == 0) {
            printf("Error: 地点名称必须唯一。\n");
            return;
        }
    }

    //分配新的空间
    map->locations = (Location*)realloc(map->locations, (map->numLocations + 1) * sizeof(Location));

    // 添加新地点
    strcpy(map->locations[map->numLocations].name, name);
    map->locations[map->numLocations].x = x;
    map->locations[map->numLocations].y = y;
    strcpy(map->locations[map->numLocations].description, description);

    // 更新地点数量
    map->numLocations++;

    printf("地点添加成功。\n");
}

//添加道路
void addEdge(CampusMap* map, const char* location1, const char* location2) {
    //查找两个地点在地点数组中的索引
    int index1 = -1, index2 = -1;
    for (int i = 0; i < map->numLocations; i++) {
        if (strcmp(map->locations[i].name, location1) == 0) {
            index1 = i;
        }
        else if (strcmp(map->locations[i].name, location2) == 0) {
            index2 = i;
        }
    }

    //添加边
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

        //更新道路长度数组
        map->roadDistances = (int*)realloc(map->roadDistances, (map->numRoads + 1) * sizeof(int));
        map->roadDistances[map->numRoads] = newNode1->distance;
        map->numRoads++;

        printf("道路添加成功。\n");
    }
    else {
        printf("Error: 一个或两个地点未找到。\n");
    }
}

//删除地点
void removeLocation(CampusMap* map, const char* name) {
    //查找地点在地点数组中的索引
    int index = -1;
    for (int i = 0; i < map->numLocations; i++) {
        if (strcmp(map->locations[i].name, name) == 0) {
            index = i;
            break;
        }
    }

    //删除地点及其相关的边
    if (index != -1) {
        //删除与该地点相关的边
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

        //删除地点
        for (int i = index; i < map->numLocations - 1; i++) {
            map->locations[i] = map->locations[i + 1];
        }

        //更新地点数量
        map->numLocations--;

        //更新地点文件
        saveLocationsToFile(map, "place.txt");

        printf("地点删除成功。\n");
    }
    else {
        printf("Error: 地点未找到。\n");
    }
}

//删除道路
void removeEdge(CampusMap* map, const char* location1, const char* location2) {
    //查找两个地点在地点数组中的索引
    int index1 = -1, index2 = -1;
    for (int i = 0; i < map->numLocations; i++) {
        if (strcmp(map->locations[i].name, location1) == 0) {
            index1 = i;
        }
        else if (strcmp(map->locations[i].name, location2) == 0) {
            index2 = i;
        }
    }

    //删除边
    if (index1 != -1 && index2 != -1) {
        //删除与这两个地点相关的边
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

        //更新道路长度数组
        for (int i = 0; i < map->numRoads - 1; i++) {
            map->roadDistances[i] = map->roadDistances[i + 1];
        }

        //更新道路数量
        map->numRoads--;

        //更新道路文件
        saveEdgesToFile(map, "road.txt");

        printf("道路删除成功。\n");
    }
    else {
        printf("Error: 一个或两个地点未找到。\n");
    }
}

//修改地点
void modifyLocation(CampusMap* map, const char* name, int x, int y, const char* description) {
    //查找地点在地点数组中的索引
    int index = -1;
    for (int i = 0; i < map->numLocations; i++) {
        if (strcmp(map->locations[i].name, name) == 0) {
            index = i;
            break;
        }
    }

    //修改地点信息
    if (index != -1) {
        map->locations[index].x = x;
        map->locations[index].y = y;
        strcpy(map->locations[index].description, description);

        //更新地点文件
        saveLocationsToFile(map, "place.txt");

        printf("地点修改成功。\n");
    }
    else {
        printf("Error: 地点未找到。\n");
    }
}

//保存地点信息到文件
void saveLocationsToFile(CampusMap* map, const char* filename) {
    FILE* file = fopen("place.txt", "w");
    if (file == NULL) {
        printf("Error: 无法打开文件 %s 进行写入。\n", filename);
        return;
    }

    for (int i = 0; i < map->numLocations; i++) {
        fprintf(file, "%s %d %d %s\n", map->locations[i].name, map->locations[i].x, map->locations[i].y, map->locations[i].description);
    }

    fclose(file);
    printf("地点信息已成功保存到文件 %s。\n", filename);
}

//保存边信息到文件
void saveEdgesToFile(CampusMap* map, const char* filename) {
    FILE* file = fopen("road.txt", "w");
    if (file == NULL) {
        printf("Error: 无法打开文件 %s 进行写入。\n", filename);
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
    printf("边信息已成功保存到文件 %s。\n", filename);
}

//读取地点信息并填充到 CampusMap 结构体中
void readLocationsFromFile(const char* filename, CampusMap* map) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: 无法打开文件 %s 进行读取。\n", filename);
        exit(EXIT_FAILURE);
    }

    //计算地点数量
    map->numLocations = 0;
    while (!feof(file)) {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            map->numLocations++;
        }
    }
    rewind(file);

    //分配空间
    map->locations = (Location*)malloc(map->numLocations * sizeof(Location));

    //读取地点信息
    for (int i = 0; i < map->numLocations; i++) {
        fscanf(file, "%s %d %d %[^\n]", map->locations[i].name, &map->locations[i].x,
            &map->locations[i].y, map->locations[i].description);
    }

    fclose(file);
}

//读取道路信息并填充到 CampusMap 结构体中
void readRoadsFromFile(const char* filename, CampusMap* map) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: 无法打开文件 %s 进行读取。\n", filename);
        exit(EXIT_FAILURE);
    }

    //计算道路数量
    map->numRoads = 0;
    while (!feof(file)) {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            map->numRoads++;
        }
    }
    rewind(file);

    //分配空间
    map->adjacencyList = (Node**)malloc(map->numLocations * sizeof(Node*));
    map->roadDistances = (int*)malloc(map->numLocations * map->numLocations * sizeof(int));

    //初始化邻接表和道路长度数组
    for (int i = 0; i < map->numLocations; i++) {
        map->adjacencyList[i] = NULL;
        for (int j = 0; j < map->numLocations; j++) {
            map->roadDistances[i * map->numLocations + j] = 0;
        }
    }

    //读取边信息
    for (int i = 0; i < map->numRoads; i++) {
        char location1[50], location2[50];

        fscanf(file, "%s %s", location1, location2);

        //找到地点在地点数组中的索引
        int index1 = -1, index2 = -1;
        for (int j = 0; j < map->numLocations; j++) {
            if (strcmp(map->locations[j].name, location1) == 0) {
                index1 = j;
            }
            if (strcmp(map->locations[j].name, location2) == 0) {
                index2 = j;
            }
        }

        // 计算距离
        int distance = calculateDistance(&map->locations[index1], &map->locations[index2]);

        //添加边到邻接表
        if (index1 != -1 && index2 != -1 && index1 != index2) {
            Node* newNode1 = (Node*)malloc(sizeof(Node));
            strcpy(newNode1->locationName, location2);
            newNode1->distance = distance;
            newNode1->next = map->adjacencyList[index1];
            map->adjacencyList[index1] = newNode1;

            //更新道路长度数组
            map->roadDistances[index1 * map->numLocations + index2] = distance;
            map->roadDistances[index2 * map->numLocations + index1] = distance;
        }
    }

    fclose(file);
}

//释放地图内存
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

//打印地图
void printMap(CampusMap* map) {
    char campusMap[15 * 3][15 * 5];  // 每个地点占据一个3x5的九宫格
    char locationsMap[50];            // 使用字母 "A" 到 "Z" 和 "a" 到 "z" 表示地点
    int locationIndex = 0;

    //初始化地图
    for (int i = 0; i < 15 * 3; i++) {
        for (int j = 0; j < 15 * 5; j++) {
            campusMap[i][j] = '.';
        }
    }

    //标记地点
    for (int i = 0; i < map->numLocations; i++) {
        int x = map->locations[i].x * 3;
        int y = map->locations[i].y * 5;

        //使用字母 "A" 到 "Z" 和 "a" 到 "z" 表示地点
        if (locationIndex < 26) {
            locationsMap[locationIndex] = 'A' + locationIndex;
        }
        else {
            locationsMap[locationIndex] = 'a' + (locationIndex - 26);
        }

        //在地图上标记地点
        campusMap[x][y] = locationsMap[locationIndex];
        //上下左右边框
        campusMap[x - 1][y] = campusMap[x + 1][y] = '-';
        campusMap[x][y - 1] = campusMap[x][y + 1] = '|';
        //四个角的边框
        campusMap[x - 1][y - 1] = campusMap[x - 1][y + 1] = campusMap[x + 1][y - 1] = campusMap[x + 1][y + 1] = '+';

        locationIndex++;
    }

    //输出大标题
    printf("*************************☆ 武汉科技大学校园地图概览☆ *************************\n");

    //输出上边框
    for (int i = 0; i < 15 * 5 + 2; i++) {
        printf("-");
    }
    printf("\n");

    //打印地图
    for (int i = 0; i < 15 * 3; i++) {
        printf("|");
        for (int j = 0; j < 15 * 5; j++) {
            printf("%c", campusMap[i][j]);
        }
        printf("|\n");
    }

    //输出下边框
    for (int i = 0; i < 15 * 5 + 2; i++) {
        printf("-");
    }
    printf("\n");

    //打印地点信息
    for (int i = 0; i < locationIndex; i++) {
        printf("%c: %s", locationsMap[i], map->locations[i].name);
        printf("   坐标: (%d, %d)", map->locations[i].x, map->locations[i].y);
        printf("   描述: %s\n", map->locations[i].description);
    }
}

// 计算两点之间的距离
int calculateDistance(Location* location1, Location* location2) {
    int xDiff = location1->x - location2->x;
    int yDiff = location1->y - location2->y;
    return (int)round(sqrt(xDiff * xDiff + yDiff * yDiff));
}

// Dijkstra 算法计算最短路径
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

        // 修正：从用户输入的起始地点开始计算
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

// 电路规划(Dijkstra 算法计算最短路径)
void planElectricCircuit(CampusMap* map) {
    // 初始化变量
    int* parent = (int*)malloc(map->numLocations * sizeof(int));
    int* distance = (int*)malloc(map->numLocations * sizeof(int));
    int totalDistance = 0;

    // 用户输入起始地点
    char startLocationName[50];
    printf("请输入起始地点的名称: ");
    scanf("%s", startLocationName);

    int start = -1;
    for (int i = 0; i < map->numLocations; i++) {
        if (strcmp(map->locations[i].name, startLocationName) == 0) {
            start = i;
            break;
        }
    }

    if (start == -1) {
        printf("错误：未找到起始地点。\n");
        return;
    }

    // 从指定地点开始计算最短路径
    dijkstra(map, start, parent, distance);

    // 创建队列
    Queue* pathQueue = createQueue(map->numLocations);

    // 从指定地点开始计算最短路径
    dijkstra(map, start, parent, distance);

    // 将路径按顺序加入队列
    enqueue(pathQueue, start);
    for (int i = 0; i < map->numLocations; i++) {
        if (parent[i] != -1 && i != start) {
            enqueue(pathQueue, i);
        }
    }

    // 打印最短路径
    printf("\n从%s开始的最短铺设线路方案:\n", startLocationName);

    // 出队并打印路径
    int pathNumber = 1;
    while (!isEmpty(pathQueue)) {
        int currentLocation = dequeue(pathQueue);

        if (parent[currentLocation] != -1) {
            int currentDistance = distance[currentLocation] - distance[parent[currentLocation]];
            printf("%d、%s -> %s (距离：%d)\n", pathNumber, map->locations[parent[currentLocation]].name,
                map->locations[currentLocation].name, currentDistance);
            totalDistance += currentDistance;
            pathNumber++;
        }
    }

    // 打印总长度
    printf("\n最短铺设线路总长度: %d\n", totalDistance);

    // 释放队列内存
    freeQueue(pathQueue);

    free(parent);
    free(distance);
}

//比较函数用于排序
int comparePathResults(const void* a, const void* b) {
    const PathResult* pathA = (const PathResult*)a;
    const PathResult* pathB = (const PathResult*)b;

    //先比较距离，如果距离相同，则按照地点的原本顺序
    if (pathA->distance == pathB->distance) {
        return strcmp(pathA->locationName, pathB->locationName);
    }

    return (pathA->distance < pathB->distance) ? -1 : 1;
}

//查询最短路径
void shortestPathToAll(CampusMap* map, const char* startLocation) {
    //初始化变量
    int start = -1;
    for (int i = 0; i < map->numLocations; i++) {
        if (strcmp(map->locations[i].name, startLocation) == 0) {
            start = i;
            break;
        }
    }

    if (start == -1) {
        printf("错误：未找到起始地点。\n");
        return;
    }

    int* distance = (int*)malloc(map->numLocations * sizeof(int));
    int* parent = (int*)malloc(map->numLocations * sizeof(int));

    //初始化 distance 和 parent 数组
    for (int i = 0; i < map->numLocations; i++) {
        distance[i] = INT_MAX;
        parent[i] = -1;
    }

    //设置起始点的距离为 0
    distance[start] = 0;

    //计算最短路径
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

    //存储查询结果
    PathResult* pathResults = (PathResult*)calloc(map->numLocations, sizeof(PathResult));

    //打印查询结果
    printf("\n从%s到所有地点的最短路径:\n", startLocation);
    for (int i = 0; i < map->numLocations; i++) {
        if (i != start) {
            // 存储查询结果到结构体数组中
            pathResults[i].distance = distance[i];
            strcpy(pathResults[i].locationName, map->locations[i].name);
            strcpy(pathResults[i].path, map->locations[i].name);

            // 输出路径
            int current = i;
            while (parent[current] != -1) {
                // 输出到查询结果的路径字符串中
                strcat(pathResults[i].path, " <- ");
                strcat(pathResults[i].path, map->locations[parent[current]].name);

                current = parent[current];
            }

            printf("到：%s\t距离：%d\t路径：%s\n", map->locations[i].name, distance[i], pathResults[i].path);
        }
    }

    //排序查询结果
    qsort(pathResults, map->numLocations, sizeof(PathResult), comparePathResults);

    //输出排序后的查询结果
    printf("\n按距离从近到远的顺序显示:\n");
    for (int i = 1; i < map->numLocations; i++) {
        printf("到：%s\t距离：%d\t路径：%s\n", pathResults[i].locationName, pathResults[i].distance, pathResults[i].path);
    }

    //释放内存
    free(distance);
    free(parent);
    free(pathResults);
}
