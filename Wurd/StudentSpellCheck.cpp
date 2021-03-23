#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
	freeTree(root);
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
	ifstream infile(dictionaryFile);

	if (!infile)
		return false;

	freeTree(root);
	root = new Node;

	string s;

	while (getline(infile, s)) {
		Node* curr = root;

		for (int i = 0; i < s.length(); i++) {
			char c = tolower(s[i]);
			int index = getIndex(c);

			if (!(isalpha(c) || c == '\''))
				continue;

			if (curr->children[index] == nullptr) {
				curr->children[index] = new Node;
			}
			curr = curr->children[index];
		}
		if (curr->children[27] != nullptr)
			delete curr->children[27];
		curr->children[27] = new Node;
	}

	return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	for (int i = 0; i < word.length(); i++)
		word[i]  = tolower(word[i]);

	Node* curr = root;
	bool isWord = true;


	for (char c : word) {
		if (!isWord) break;

		if (!(isalpha(c) || c == '\''))
			continue;

		int index = getIndex(c);

		if (curr->children[index] == nullptr) {
			isWord = false;
		}
		else {
			curr = curr->children[index];
		}
	}

	if (isWord && curr->children[27] == nullptr) isWord = false;
	if (isWord) return true;

	suggestions.clear();

	for (int i = 0; i < word.length(); i++) {
		addSpellCheckWords(word, i, max_suggestions, suggestions);
	}

	return false;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	problems.clear();
	vector<SpellCheck::Position> wordIndex = breakLineIntoWords(line);
	for (int i = 0; i < wordIndex.size(); i++) {
		SpellCheck::Position pos = wordIndex[i];

		if (!checkIfWordExistsInDictionary(line.substr(pos.start, pos.end - pos.start + 1))) {
			problems.push_back(pos);
		}
	}

}

vector<SpellCheck::Position> StudentSpellCheck::breakLineIntoWords(const string& line) {
	vector<SpellCheck::Position> listOfWords;
	int startIndex = 0;

	for (int i = 0; i < line.length(); i++) {
		char c = line[i];
		if (!(isalpha(c) || c == '\'')) {
			if (i - startIndex > 0) {
				SpellCheck::Position pos;
				pos.start = startIndex;
				pos.end = i - 1;

				listOfWords.push_back(pos);
				
				startIndex = i + 1;
			}
			else {
				startIndex = i + 1;
			}
		}
	}

	if (line.length() - startIndex > 0) {
		SpellCheck::Position pos;
		pos.start = startIndex;
		pos.end = line.length() - 1;

		listOfWords.push_back(pos);
	}


	return listOfWords;
}

void StudentSpellCheck::addSpellCheckWords(string word, int index, int &maxSuggestions, std::vector<std::string>& suggestions) {
	Node* p = root;

	for (int i = 0; i < index; i++) {
		int childrenIndex = getIndex(word[i]);
		if (p->children[childrenIndex] == nullptr) {
			return;
		}
		p = p->children[childrenIndex];
	}

	for (int i = 0; i < 27; i++) {
		bool isWord = true;
		Node* tempPointer = p;

		if (tempPointer->children[i] != nullptr) {
			tempPointer = tempPointer->children[i];

			for (int j = index + 1; j < word.length(); j++) {
				int childrenIndex = getIndex(word[j]);

				if (tempPointer->children[childrenIndex] != nullptr) {
					tempPointer = tempPointer->children[childrenIndex];
				}
				else {
					isWord = false;
					break;
				}
			}
			if (isWord) {
				if (tempPointer->children[27] != nullptr) {
					string newWord = word;
					newWord[index] = i == 26 ? '\'' : 'a' + i;
					suggestions.push_back(newWord);
					maxSuggestions--;
					if (maxSuggestions == 0) return;
				}
			}
		}
	}
}

bool StudentSpellCheck::checkIfWordExistsInDictionary(string str) {
	Node* curr = root;
	for (int i = 0; i < str.length(); i++) {
		int index = getIndex(tolower(str[i]));

		if (curr->children[index] == nullptr)
			return false;
		else
			curr = curr->children[index];
	}

	if (curr->children[27] != nullptr)
		return true;
	else
		return false;
}


int StudentSpellCheck::getIndex(char c) {
	return c == '\'' ? 26 : c - 'a';
}

void StudentSpellCheck::freeTree(Node* curr) {
	if (curr == nullptr)
		return;

	for (int i = 0; i < 28; i++) {
		freeTree(curr->children[i]);
	}

	delete curr;
}