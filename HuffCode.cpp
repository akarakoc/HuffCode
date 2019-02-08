//============================================================================
// Name        : HuffCode
// Author      : Ali Karakoç
// Version     : 1.0
// Description : Compress/Decompress .txt file via Huffman Encoding Algorithm
//============================================================================

#include <bits/stdc++.h>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <bitset>
using namespace std;

//input string and its corresponding wordcode map scheme
map<string, string> wordcode;

struct Node {

    string data;
    double freq;
    Node *left;
    Node *right;
    Node(string Dt, double Fq)

    {
        left = NULL;
        right = NULL;
        data = Dt;
        freq = Fq;
    }

};


//Stores codewords into the map and print to a file
void CodeMapFile(struct Node* root, string str)
{
    if (root==NULL)
        return;
    if (root->data[0] != '$'){
    	wordcode[root->data]=str;
        ofstream fn;
        	   	   fn.open("codemap.txt", std::ios_base::app | std::ios_base::out);
        	   	   fn << root->data << " => " << str << endl;
        fn.close();
    }

    CodeMapFile(root->left, str + "0");
    CodeMapFile(root->right, str + "1");
}

//MinHeap comparison struct
struct cmp {
    bool operator()(Node* left, Node* right)
    {
        return (left->freq > right->freq);
    }
};

priority_queue<Node*, vector<Node*>, cmp> min_heap;

//Constructs HuffmanTree by using priority_queue mininum heap structure
void HTree(string data[], int freq[], int size)

{
    struct Node *left, *right, *top;
    for (int i = 0; i < size; ++i)
        min_heap.push(new Node(data[i], freq[i]));

    while (min_heap.size() != 1) {
        left = min_heap.top();
        min_heap.pop();
        right = min_heap.top();
        min_heap.pop();
        string key(1, '$');
        top = new Node(key, left->freq + right->freq);
        top->left = left;
        top->right = right;
        min_heap.push(top);
    }
    CodeMapFile(min_heap.top(), "");
}

//Decodes Huffman encoded binary string
string HDecode(struct Node* root, string str)
{
    string dec = "";
    struct Node* temp = root;
    for (int i=0;i<str.size();i++)
    {
        if (str[i] == '0')
           temp = temp->left;
        else
           temp = temp->right;
        if (temp->left==NULL and temp->right==NULL)
        {
            dec += temp->data;
            temp = root;
        }
    }
    return dec;
}

//Converts ASCII characters into binary
string char_to_str(char ch)
{
    bitset<8> temp(ch);
    return temp.to_string();
}





int main()
{

	//Reads string from the file with determined size character groups
       ifstream fn;
             fn.open("infile.txt");
             string line;
             char ch;
             std::map<string,int> mymap;
             std::map<string,double> myfreqmap;
             string str("");
             int N;
             double sum=0;
             cout<<"ENTER THE CHAR-GROUP NUMBER:"<< endl;
             cin >> N;

             while (getline(fn, line))
             {
                    istringstream iss(line);
                    iss >> std::noskipws;
                    while(iss){
                          str="";
                          for (int i=0; i<N; i++){
                                 iss >> ch;
                                 str+=ch;
                          }
                          //Stores strings and their counts
                          pair<map<string,int>::iterator,bool> ret;
                          ret = mymap.insert( std::pair<string,int>(str,1) );
                           if (ret.second==false) {
                                   ret.first->second++;
                               if(iss.eof()) break;
                           }
                    }

             }

             //Stores strings and their frequencies
             std::map<string,int>::iterator it = mymap.begin();
             std::map<string,double>::iterator it2 = myfreqmap.begin();
             for (it=mymap.begin(); it!=mymap.end(); ++it){
                    sum += it->second;
             }

             for (it=mymap.begin(); it!=mymap.end(); ++it){
                    string str2=it->first;
                    double frq=100*(it->second)/sum;
                    pair<map<string,double>::iterator,bool> ret2;
                    ret2 = myfreqmap.insert( std::pair<string,double>(str2,frq));

                    if (ret2.second==false) {
                            ret2.first->second=frq;
                    }

             }

             //Constructs arrays from map

             string arr[myfreqmap.size()];
             int freq[myfreqmap.size()];
             int k=0;
             for (it2=myfreqmap.begin(); it2!=myfreqmap.end(); ++it2){

                 ofstream fr;
                 	   	   fr.open("freqmap.txt", std::ios_base::app | std::ios_base::out);
                 	   	   fr << it2->first << " => " << it2->second << endl;
                 fr.close();
                   arr[k]=it2->first;
                   freq[k]=it2->second;
                   k++;
             }
             fn.close();

             int size = sizeof(arr) / sizeof(arr[0]);
             HTree(arr, freq, size);
             string encodedString;

             ifstream fn1;
                   fn1.open("infile.txt");
                   string line1;
                   string str1("");
                   char ch1;
                   while (getline(fn1, line1))
                   {
                          istringstream iss1(line1);
                          iss1 >> std::noskipws;
                          while(iss1){
                                str1="";
                                for (int i=0; i<N; i++){
                                       iss1 >> ch1;
                                       str1+=ch1;
                                }
                                encodedString+=wordcode[str1];
                          }
                   }
               fn1.close();

               string data = encodedString;
               stringstream ssencode(data);
               ssencode >> std::noskipws;
               string output;
               while(ssencode)
               {
                   std::bitset<8> bits;
                   ssencode >> bits;
                   char c = char(bits.to_ulong());
                   output += c;
               }

               ofstream fn3;
               	   	   fn3.open("EncodedAscii.huf", ios::out | ios::binary);
               	   	   fn3 << output;
               fn3.close();


               ofstream fn4;

               fn4.open("EncodedBinary.txt", ios::out | ios::binary);
               	   fn4 << encodedString;
               fn4.close();

               ifstream fn5;

                        fn5.open("EncodedAscii.huf");
                        string line2;
                        string dec("");
                        char ch2;
                        while (getline(fn5, line2))
                        {
                               istringstream iss2(line2);
                               iss2 >> std::noskipws;
                               while(iss2){
                                     iss2 >> ch2;
                                     dec+=char_to_str(ch2);
                               }

                        }
               fn5.close();

               ofstream fn6;
                        fn6.open("DecodedBinary.txt", ios::out | ios::binary);
                        fn6 << dec;
               fn6.close();

               ofstream fn7;
                        fn7.open("DecodedAscii.txt", ios::out | ios::binary);
                        fn7 << HDecode(min_heap.top(), dec);
               fn7.close();


    return 0;

}
