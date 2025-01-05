#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <limits>
#include <cstdint>
struct Coordinate{
    int x;
    int y;
    Coordinate(int i, int j): x(i), y(j){}
};

void create_anthinodes(std::unordered_map<char,std::vector<Coordinate>>& position_anthenes,std::vector<std::string>& map,char anthenna,int x,int y,std::set<std::pair<int,int>>& overlaps){
    int x_difference,y_difference,x_pos,y_pos;
    //lets find if there is any anthenne up this new anthene
    for(const Coordinate& pos : position_anthenes[anthenna]){
        x_difference = x - pos.x;
        y_difference = y - pos.y;
        //anthinode positions
        x_pos = pos.x - x_difference;
        y_pos = pos.y - y_difference;
        if(x_pos >= 0 && y_pos >=0 && x_pos < map.size() && y_pos < map[x_pos].size() ){
            if(map[x_pos][y_pos] == '.'){
                map[x_pos][y_pos] = '#';
            }else if (map[x_pos][y_pos] != '#'){
                overlaps.insert(std::make_pair(x_pos,y_pos));
            }                    
        }
    }
    position_anthenes[anthenna].push_back(Coordinate(x,y));
    for(int i = x; i < map.size(); i++){
        for(int j = 0; j < map[x].size(); j++){
            if(i == x && j <= y) continue;
            if(map[i][j] == anthenna){
                x_difference = i - x;
                y_difference = j - y;
                x_pos = i + x_difference;
                y_pos = j + y_difference;
                if(x_pos >= 0 && y_pos >=0 && x_pos < map.size() && y_pos < map[x_pos].size() ){
                    if(map[x_pos][y_pos] == '.'){
                        map[x_pos][y_pos] = '#';
                    }else if (map[x_pos][y_pos] != '#'){
                        overlaps.insert(std::make_pair(x_pos,y_pos));
                    }                               
                }
            }
        }
    }
}

int main() {
    std::ifstream file("day8_puzzle");
    if (!file) {
        std::cerr << "Error opening the file!" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::string> map;
    uint64_t number;
    // Lectura del archivo
    while (std::getline(file, line)) {
        map.push_back(line);
    }      
    // find all the anthenes and create anthinodes
    std::unordered_map<char,std::vector<Coordinate>> position_anthenes;
    //overlaps anthinodes
    std::set<std::pair<int,int>> overlaps;
    for (int i = 0; i < map.size(); i++) {
        for(int j = 0; j < map[i].size(); j++){
            if(map[i][j] != '.' && map[i][j]!='#'){
                create_anthinodes(position_anthenes,map,map[i][j],i,j,overlaps);
            }
        }
    }
    uint64_t result = 0;
    for(int i = 0; i < map.size(); i++){
        for(int j = 0; j < map[i].size(); j++){
            if(map[i][j] == '#'){
                result++;
            }
        }
    }
    result+=overlaps.size();
    std::cout << "Resultado final: " << result << std::endl;
    return 0;
}
