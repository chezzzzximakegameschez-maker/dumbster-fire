#include <iostream>
#include <vector>
using namespace std;

vector<string> lex(string x){
	vector<char> keywords={'"','!','-','\\','_','$'};
	vector<string> output;
	string y="";
	for(int i=0; i<x.size(); ++i){
		char v=x[i];
		bool isKeyword=0;
		for(char k: keywords){
			if(v==k){
				if(y.size()==0){
					y+=v;
					isKeyword=true;
					output.push_back(y);
					break;
				}else{
					output.push_back(y);
					isKeyword=true;
					i--;
					break;
				}
			}
		}
		if(isKeyword){y=""; continue;}
		y+=v;
	}
	output.push_back(y);
	return output;
}
