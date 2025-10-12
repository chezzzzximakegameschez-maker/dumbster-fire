#include <iostream>
#include <vector>
#include "lex.cpp"
#include "colors.cpp"
using namespace std;
class Node{
	public:
		string type="";
		bool hasChildren=false;
		vector<unique_ptr<Node> > children={};
		Node* parent = nullptr;
		string literal="";
		Node(string t, string l) : type(t) {
			type=t;
			literal=l;
		}
};
void badError(){
	string errormessage=COLOR_Red;
	errormessage+="You are a bad programmer";
	errormessage+=COLOR_Reset;
	errormessage+="\n";
	throw runtime_error(errormessage);
}
vector< unique_ptr<Node> > parse(vector<string> lexed){
	
	bool isString=false;
	bool isLiteral=false;
	vector< unique_ptr<Node> > output;
	vector< unique_ptr<Node> >* moveTo =&output;
	Node* moveToParent = nullptr;

	string n="";
	string currentFunction="";
	int character=1;
	for(int i=0; i<lexed.size(); ++i){
		string v=lexed[i];
		character+=v.size();
		if(isLiteral){
			if(v=="n"){
				v="\n";
			}
		}else if(isString){
			if(v=="\""){
				isString=false;
				auto s = make_unique<Node>("String",n);
				s->parent=moveToParent;
				moveTo->push_back(std::move(s));
				n = "";
				
			}else if(v=="\\"){
				isLiteral=true;
			}else{
				n+=v;
			}
		}else{
			if(v=="$"){
				i++;
				auto s = make_unique<Node>("Require",lexed[i]);
				output.push_back(std::move(s));
			}
			if(v=="!"){
				i++;
				currentFunction=lexed[i];
				auto s = make_unique<Node>("FNCall",lexed[i]);
				Node* newNode = s.get();
				// Set parent pointer for all children added later
				newNode->parent = moveToParent;
				moveTo->push_back(std::move(s));
				moveToParent = newNode;
				moveTo = &(moveToParent->children);
			}
			if(v=="\\"){
				i++;
				moveToParent=moveToParent->parent;
				if(moveToParent==nullptr){
					moveTo=&output;
				}else{
					moveTo=&(moveToParent->children);
				}
				
			}
			if(v=="\""){
				isString=!isString;
			}
		}
		
	}
	if(moveTo!=&output){
		badError();
	}
	return output;
}