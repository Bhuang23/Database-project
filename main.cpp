/*main.cpp*/

//
// myDB project using AVL trees
//
// <<Brian Huang>>
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project #04
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <cctype>

#include "avl.h"
#include "util.h"

using namespace std;


//
// tokenize
//
// Given a string, breaks the string into individual tokens (words) based
// on spaces between the tokens.  Returns the tokens back in a vector.
//
// Example: "select * from students" would be tokenized into a vector
// containing 4 strings:  "select", "*", "from", "students".
//
vector<string> tokenize(string line)
{
  vector<string> tokens;
  stringstream  stream(line);
  string token;

  while (getline(stream, token, ' '))
  {
    tokens.push_back(token);
  }

  return tokens;
}
//buildTrees
//build a vector of avltrees with the data file and index vector and returns the vector of avltrees
vector<avltree<string, streamoff>> buildTrees(string tablename, int recordSize, int numColumns, vector<int> index)
{
	string   filename = tablename + ".data";
   ifstream data(filename, ios::in | ios::binary);
	data.seekg(0, data.end);  // move to the end to get length of file:
   streamoff length = data.tellg();
	//create vector of avltrees
	vector<avltree<string, streamoff>> avltrees;
	for(int i = 0; i < numColumns; i++)
	{
		if(index[i] == 1)
		{
			int recordsize = 0;
			//create avltree
			avltree<string, streamoff> avltree; 
			vector<string> record;
			while(recordsize < length)
			{
				//increase the data-uin/netid/name/id which we got from getRecord into the tree until the recordsize = length or the max recordsize
				avltree.insert((GetRecord(tablename, recordsize, numColumns)[i]), recordsize);
				recordsize+=recordSize;
			}
			//push the built tree into the vector
			avltrees.push_back(avltree);
		}
	}
	//return the vector
	return avltrees;
}
//checktokenname
//check if the token is a valid name by looping through name 
//and checking if the name is equal to the token and
//returns 1 if token is a valid name and 0 if not
int checktokenname( vector<string> name, string token )
{
	//loop through name and check if the name is equal to the token
	for(int i = 0; i < name.size(); i++)
	{
		if(name[i] == token)
		{
			return 1;
		}
	}
	return 0;
}
//tokencolumn
//checks if the token is a valid name looping through name 
//and checking if the name is equal to the token and returns
//the position of the token if found and 0 if not found
int tokencolumn( vector<string> name, string token )
{
	//loop through name and check if the name is equal to the token
	for(int i = 0; i < name.size(); i++)
	{
		if(name[i] == token)
		{
			return i;
		}
	}
	return 0;
}
int main()
{
  string tablename; // = "students";

  cout << "Welcome to myDB, please enter tablename> ";
  getline(cin, tablename);

  cout << "Reading meta-data..." << endl;
  //
  // TODO:
  //
  //
  string value;
  //create data file
  string   filename = tablename + ".meta";
  ifstream data(filename, ios::in | ios::binary);
  if (!data.good())//check if data file can be opened
  {
    cout << "**Error: couldn't open data file '" << filename << "'." << endl;
    return 0;
  }
  int recordsize;
  int numcolumn;
  vector<string> metadata;
  data >> value;
  while(!data.eof())
  {
	  metadata.push_back(value);
	  data >> value;
  }
  //get recordsize and number of columns from metadata
  recordsize = stoi(metadata[0]);
  numcolumn = stoi(metadata[1]);
  vector<int> meta_data;
  vector<string> metaname;
  //after the recordsize and number of columns push all 
  //the integers into meta_data and names of the columns into metaname
  for(int i = 2; i < metadata.size(); i++)
  {
	  string str = metadata[i];
	  //check if the str is an integer
	  if(isdigit(str[0]))
	  {
		  //push_back all the integers into meta_data
		  meta_data.push_back(int(str[0])-48);
	  }
	  else{
		  //push_back all the names of the columns into metaname
		  metaname.push_back(metadata[i]);
	  }
  }
  //initalize the vector of avltrees
  vector<avltree<string, streamoff>> avltrees;
  cout << "Building index tree(s)..." << endl;
  //
  // TODO:
  //
  //build the vector of avltrees
  avltrees = buildTrees(tablename, recordsize, numcolumn, meta_data);
  //
  // Main loop to input and execute queries from the user:
  //
  for(int i = 0; i < meta_data.size(); i++)
  {
	  int avlnum = 0;
	  if(meta_data[i] == 1)
	  {
		  cout << "Index column: " << metaname[i] << endl;
		  cout << "  Tree size: " << avltrees[avlnum].size() << endl;
		  cout << "  Tree height: " << avltrees[avlnum].height() << endl;
		  avlnum++;
	  }
  }
  string query;
  
  cout << endl;
  cout << "Enter query> ";
  getline(cin, query);

  while (query != "exit")
  {
    vector<string> tokens = tokenize(query);
    //
    // TODO:
    //
    //check the query to see if it is valid query
    if (tokens[0] != "select" && tokens[0] != "exit")  
	{
		cout << "Unknown query, ignored..." << endl;
	}
	else if (tokens[0] == "select"  && tokens[1] != "*" && checktokenname(metaname, tokens[1]) == 0)
	{
		cout << "Invalid select column, ignored..." << endl;
	}
	else if (tokens.size() != 8)
	{
		cout << "Invalid select query, ignored..." << endl;
	}
	else if (tokens[2] != "from")
	{
		cout << "Unknown query, ignored..." << endl;
	}
	else if (tablename != tokens[3])
	{
		cout << "Invalid table name, ignored..." << endl;
	}
	else if (tokens[4] != "where")
	{
	    cout << "Unknown query, ignored..." << endl;
	}
	else if (checktokenname(metaname, tokens[5]) == 0)
	{
		cout << "Invalid where column, ignored..." << endl;
	}
	else if (tokens[6] != "=")
	{
		cout << "Unknown query, ignored..." << endl;
	}
	else if(LinearSearch(tablename, recordsize, numcolumn, tokens[7], tokencolumn(metaname, tokens[5])+1).size()==0)
	{
		
		//select * from students where uin = 124679
		cout << "Not found...\n"; 
	}
	else
	{
		//You have a valid input, do some stuff
		if(tokens[1] == "*")//output all the info
		{
			int number = 0;
			//loop through the meta_data
			  for(int i = 0; i < meta_data.size(); i++)
			  {
				  if(meta_data[i] == 1 && tokens[7] == metaname[i])//in the tree and correct name
				  {
					  //find position of tokens[7] using avltree search function
					  int k = *avltrees[number].search(tokens[7]);
					  //make vector of data from the position of tokens[7]
					  vector<string> data1 = GetRecord(tablename, k, numcolumn);
					  //loop through metaname and print out all the data in data1
					  for(int j = 0 ; j < metaname.size(); j++)
						{
							cout << metaname[j] << ":" << " " << data1[j] << endl;
						}
				  }
				  else if(meta_data[i] == 1 && tokens[7] != metaname[i])//in the tree but wrong name
				  {
					  number++;
				  }
				  
			  }
			//not in the tree
			//make vector vec and make it equal to vector with of all
			// the positions of tokens[7] in column of tokens[5]
			vector<streamoff> vec = LinearSearch(tablename,recordsize,numcolumn,tokens[7], tokencolumn(metaname, tokens[5])+1);
			//loop through vec for each position in vector vec
			for(int i = 0; i < vec.size(); i++)
			{
				//make vector of data2 from the positions of tokens[7] that we get from vector vec
				vector<string> data2 = GetRecord(tablename, vec[i], numcolumn);
				//loop through metaname and print out all the data in data1
				for(int j =0 ; j < metaname.size(); j++)
				{
					cout << metaname[j] << ":" << " " << data2[j] << endl;
				}
			}
		}
		else//output the token[1] info 
		{
			//loop through the meta_data
			  for(int i = 0; i < meta_data.size(); i++)
			  {
				  int number = 0;
				  if(meta_data[i] == 1 && tokens[7] == metaname[i])//in the tree and correct name
				  {
					  //find position of tokens[7] using avltree search function
					  int k = *avltrees[number].search(tokens[7]);
					  //make vector of data1 from the position of tokens[7]
					  vector<string> data1 = GetRecord(tablename, k, numcolumn);
					  //print out token 1 using the column of tokens[1]
					  cout << tokens[1] << ":" << " " << data1[tokencolumn(metaname, tokens[1])] << endl;
				  }
				  else if(meta_data[i] == 1 && tokens[7] != metaname[i])//in the tree but wrong name
				  {
					  number++;
				  }
			  }
			//not in the tree
			//make vector vec and make it equal to vector with of all
			// the positions of tokens[7] in column of tokens[5]
			vector<streamoff> vec = LinearSearch(tablename,recordsize,numcolumn,tokens[7], tokencolumn(metaname, tokens[5])+1);
			//loop through vec for each position in vector vec
			for(int i = 0; i < vec.size(); i++)
			{
				//make vector of data2 from the positions of tokens[7] that we get from vector vec
				vector<string> data2 = GetRecord(tablename, vec[i], numcolumn);
				//print out token 1 using the column of tokens[1]
				cout << tokens[1] << ":" << " " << data2[tokencolumn(metaname, tokens[1])] << endl;
			}
		}
	}

    cout << endl;
    cout << "Enter query> ";
    getline(cin, query);
  }

  //
  // done:
  //
  return 0;
}
