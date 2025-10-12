
#include <fstream>
#include "parse.cpp"
#include <unordered_map>
#include <any>
#include <functional>

using namespace std; // bad practices hello
class Plugin{
	
	public:
		string name="";
		vector<unique_ptr<Plugin>>  subplugins;
		function<string(vector<string>)> call;
};
vector<unique_ptr<Plugin> > allPlugins;
vector<unique_ptr<Plugin> > FENV;
void prettyprintAST(const vector< unique_ptr<Node> >& AST,int indentAmount){
	string indent="";
	for(int i=0; i<indentAmount;++i){
		indent+="    ";
	}
	for(const auto& i : AST){
		cout<<COLOR_Green<<"\n"<<indent<<"Type: "<<i->type<<"\n"<<indent<<"Literal: "<<i->literal;
		cout<<"\n"<<indent<<COLOR_Magenta<<"Children: {"<<indent<<"\n";
		prettyprintAST(i->children,indentAmount+1);
		
		cout<<COLOR_Magenta<<"\n"<<indent<<"},"<<COLOR_Reset<<"\n"<<indent;
		
	}
}
string handleFNCall(const vector<unique_ptr<Node> >& fnParams,string l, const vector<unique_ptr<Plugin> >& functionEnv){
	vector<string> p;
	
	for(const auto& node: fnParams){
		if(node->type=="FNCall"){
			cout<<"hi";
			p.push_back(handleFNCall(node->children,node->literal,FENV));
		}else if(node->type!="Require"){
			p.push_back(node->literal);
		}
	}
	for(const unique_ptr<Plugin>& plugin: functionEnv){
		if(plugin->name==l){
			//cout<<plugin->name<<l;
			return plugin->call(p);
		}else if(plugin->subplugins.size()!=0){
			return handleFNCall(fnParams,l,plugin->subplugins);
		}
	}
	return "";
	
}
vector<string> interpretAST(const vector< unique_ptr<Node> >& AST) {
	for(const auto& i : AST){
		if(i->type=="FNCall"){
			
			handleFNCall(i->children,i->literal,FENV);
		}
	}
	return {""};
}
string eval_call(vector<string> x){
	if(x.size()==1){
		auto lexed=lex(x[0]);
		auto ast=parse(lexed);
		prettyprintAST(ast,0);
		interpretAST(ast);
	}else{
		badError();
	}
	return "";
}
string log_call(vector<string> x){
	if(x.size()==1){
		cout<<x[0];
	}else{
		badError();
	}
	return "";
}
string instantError(vector<string> x){
	badError();
	return "";
}
void createPlugin(const string& name, const function<string(vector<string>)>& call, vector<unique_ptr<Plugin>>& destination){
	auto p=make_unique<Plugin>();
	p->name=name;
	p->call=call;
	destination.push_back(std::move(p));

}
void loadPlugins(){
	// MY WILL TO NOT USE HEADERS IS UNDYING!!!!

	//Built-ins
	auto builtin=make_unique<Plugin>();
	
	builtin->name=".wrapperBUILTIN_21749238272890209918748639821796348254892643"; // Random numbers so this isn't called on accident
	builtin->call=instantError;
	FENV.push_back(std::move(builtin));
	createPlugin("LOG",log_call,builtin->subplugins);
}
int main(){
	loadPlugins();
	auto lexed=lex("!LOG-\"hello\"-\\");
	auto ast=parse(lexed);
	//prettyprintAST(ast,0);
	interpretAST(ast);
}