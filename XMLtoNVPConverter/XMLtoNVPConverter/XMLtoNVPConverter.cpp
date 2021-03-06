
#include "pch.h"
#include <iostream>
#include <iterator>
#include <map>
#include <list>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;

//--------------------------------------
//Program will ask for file inputs

//Sample files:
//planes.xml
//cars.xml
//--------------------------------------

//Holds the datasets
struct NameValuePair {
	//Pass parents to this pointer
	NameValuePair* parent;

	//if NameValuePair has a list of members,  insert new namevaluepair into members
	map<int, NameValuePair*> members;

	string name;
	string value;

};

int main()
{
	//Used to count strings
	int tagOpen, tagClosed;

	//map<int, NameValuePair*>* lXMLToNVP = new map<int, NameValuePair*>;

	//input
	string fileName;

	//imported file
	ifstream inFileXML;

	//exported file
	ofstream outFileNVP;

	//Requires a valid filename to continue
	do {
		cout << "Which .xml file do you want to convert to a name value pair?" << endl;
		cin >> fileName;

		//Can be valid by including or ommitting .xml in input
		fileName.find(".xml") == -1 ? inFileXML.open(fileName + ".xml") : inFileXML.open(fileName);

	} while (!inFileXML.good());

	string line;
	string name;
	string value;

	getline(inFileXML, line, '\n');
	name = line.substr(line.find('<') + 1, line.find('>') - 1);
	int i = 0;

	//Container
	NameValuePair* dataContainer = new NameValuePair();

	//Pass by pointer when a tag has members (member's parent = currentParent), or a tag is done adding members (currentParent = currentParent's parent)
	NameValuePair* currentParent = new NameValuePair();

	//Set the head of the dictionary
	dataContainer->parent = nullptr;
	dataContainer->name = name;
	dataContainer->value = value;
	currentParent = dataContainer;

	//Open filename
	cout << name + ":{" << endl;
	outFileNVP.open(fileName + ".txt");

	outFileNVP << dataContainer->name << ":{" << endl;
	while (inFileXML.good()) {

		//Adds tab by how many parents deep the member is
		string t = "";

		//tab out members via how many parents deep
		for (NameValuePair *parent = currentParent; parent != 0; parent = parent->parent)
			t += "\t";

		//Member being used to set next NVP being added from file
		NameValuePair* tmpNVP = new NameValuePair();

		getline(inFileXML, line, '\n');

		//Set the tag lengths for substringing
		tagOpen = line.find_first_of('<');
		tagClosed = line.find_first_of('>');

		//Cut out tags and value
		name = line.substr(tagOpen + 1, tagClosed - tagOpen - 1);

		//Set name
		tmpNVP->name = name;

		//if there is no closing tag
		if (line.find('/') == -1) {

			//Inserts the NVP to the currentparents list of members
			currentParent->members.insert(pair<int, NameValuePair*>(i, tmpNVP));

			//Add to the currentparent's members
			cout << t << currentParent->members.find(i)->second->name << ":{" << endl;
			outFileNVP << t << currentParent->members.find(i)->second->name << ":{" << endl;
			i++;

			//Give parent to the opening tag
			tmpNVP->parent = currentParent;

			//Sets the new currentParent as the current tag
			currentParent = tmpNVP;
		}
		//If the end tag of the parent is found
		else if (line.find('/' + currentParent->name) != string::npos)
		{
			//Set a bracket, reducing the tabs by 1
			t.resize(t.length() - 1);
			cout << t << "}" << endl;
			outFileNVP << t << "}" << endl;

			//Bubbles pointer up to previous parent when a tag is closed
			if (currentParent->parent != nullptr)
				currentParent = currentParent->parent;
		}
		//If all else failes, this is another member of the currentParent
		else {

			//removes the front and back tags from the string
			value = line.substr(tagClosed + 1, line.length() - name.length() - tagClosed - 4);

			//Set values
			tmpNVP->name = name;
			tmpNVP->value = value;

			//Set the parent as the current parent
			tmpNVP->parent = currentParent;

			//Adds to the currentParent's members
			currentParent->members.insert(pair<int, NameValuePair*>(i, tmpNVP));
			cout << t << currentParent->members.find(i)->second->name << ":" << currentParent->members.find(i)->second->value << endl;
			outFileNVP << t << currentParent->members.find(i)->second->name << ":" << currentParent->members.find(i)->second->value << endl;
			i++;
		}
	}

	//close file
	outFileNVP.close();
	system("Pause");
}