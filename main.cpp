//
//  main.cpp
//  xmlParser
//  This is a program that can parse XML format files.
//  Created by huidong wang on 10/23/22.
//

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <fstream>
using namespace std;


struct XmlNode{
    string tag;
    string attribution;
    string text;
    queue <XmlNode *> childXml;
};
stack <XmlNode*>tagStack;
XmlNode *root = new XmlNode();

void parse(string path);
void parseLine(string Line);
void print(XmlNode *root, int factor);

int main(int argc, const char * argv[]) {
    string path;
    path = "/Users/huidongwang/Desktop/MyLearningHub/CppLearning/xmlParser/xmlParser/example.xml";
//    cout << "Here is mini-XML parser\nplease input the file path: \n";
//    getline(cin, path);
    parse(path);
    print(root, 0);
    
    return 0;
}


void print(XmlNode *root, int factor){
    for(int i = 0; i < factor; ++i){
        if(i < factor - 6)
            cout << " ";
        else if (i == factor - 6)
            cout << "+";
        else
            cout << "-";
    }
        
    cout << "tag: " << root -> tag << endl;
    //attribution
    for(int i = 0; i < factor; ++i){

        cout << " ";
    }
    cout << "attribution: ";
    if(root -> attribution == "")
        cout << "NaN" << endl;
    else
        cout << root -> attribution << endl;
    //text
    for(int i = 0; i < factor; ++i){
        cout << " ";
    }
    cout << "text: ";
    if(root -> text == "")
        cout << "NaN" << endl;
    else
        cout << root -> text << endl;
    cout << endl;
    
    if(root -> childXml.empty()){
        return;
    }
    
    while(!root -> childXml.empty()){
        XmlNode * next = root -> childXml.front();
        root -> childXml.pop();
        print(next, factor + 6);
 
    }
 
}

void parse(string path){
    //open the file
    fstream targetFile;
    targetFile.open(path, ios::in);
    if(!targetFile.is_open()){
        cout << "Could Not Find the File" << endl;
        return;
    }
    
    string content;
    getline(targetFile, content, '<');
    getline(targetFile, content, '>');
    root -> tag = content;
    tagStack.push(root);
    
    while(getline(targetFile, content))
        parseLine(content);
    
    targetFile.close();
    return;
}

void parseLine(string Line){
    string text;
    string attrName;
    string tagHead;
    string tagTail;
   
    int flag = 0; // 0 for text, 1 for <> </>
    
    if(Line.find('<') == variant_npos){
        tagStack.top() -> text.append(Line);
        return;
    }

    for(int i = 0; i < Line.length(); ++i){
        if(Line[i] == '<'){
            flag = 1;
            if(Line[i + 1] == '/'){
                i += 2;
                tagTail = "";
                while(Line[i] != '>'){
                    if(i >= Line.length())
                        break;
                    tagTail += Line[i];
                    ++i;
                }
                if(tagTail == tagStack.top() -> tag){
                    tagStack.pop();
                    if(tagStack.empty()){
                        cout << "parsing finished" << endl << endl;
                        return;
                    }
                }
                    
                else{
                    cout << "close tag: " << tagTail << endl;
                    cout << "start tag: " << tagStack.top() -> tag << endl;
                    cout << "Tags Do Not Match" << endl;
                    exit(-1);
                }
                
                if(Line[i] == '>'){
                    flag = 0;
                    continue;
                }
                    
            }else{
                tagHead = "";
                ++i;
                while(Line[i] != '>'){
                    if(Line[i] == ' ')
                        break;
                    if(i >= Line.length()){
                        cout << "Out boundary 2\n";
                        exit(-1);
                    }
                    tagHead += Line[i];
                    ++i;
                }
 
                XmlNode * p = new XmlNode();
                p -> tag = tagHead;
                
                if(!tagStack.empty()){
                    tagStack.top() -> childXml.push(p);
                }
                tagStack.push(p);
 
                //attributions
                if(Line[i] == ' ')
                    while(Line[i] == ' '){
                        
                        if(i > Line.length()){
                            cout << "Out boundary 3";
                            exit(-1);
                        }
                        ++i;
                    }
                       
                if(Line[i] == '>'){
                    flag = 0;
                    continue;
                }else{
                    //attribution
                    attrName = "";
                    
                    while(Line[i] != '>'){
                        if(i > Line.length()){
                            cout << "Out boundary 5";
                            exit(-1);
                        }
                        attrName += Line[i];
                        ++i;
                    }
                    if(Line[i] == '>'){
                        tagStack.top() -> attribution = attrName;
                        flag = 0;
                        continue;
                    }
                }
            }
        }
        if(Line[i] == '<')
            flag = 1;
        if(!flag){
            text = "";
            while(Line[i] != '<'){
                if(i >= Line.length()){
                    break;
                }
                text += Line[i];
                ++i;
            }
            tagStack.top() -> text.append(text);
            if(Line[i] == '<')
                --i;
        }
    }
    
}
