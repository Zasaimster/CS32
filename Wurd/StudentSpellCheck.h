#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck() {
		root = new Node;
	}	
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
	struct Node;
	void addSpellCheckWords(std::string word, int index, int &maxSuggestions, std::vector<std::string>& suggestions);
	std::vector<SpellCheck::Position> breakLineIntoWords(const std::string &line);
	bool checkIfWordExistsInDictionary(std::string word);
	int getIndex(char c);
	void freeTree(Node* curr);

	struct Node
	{
		Node() {
			//0-25 for the alphabet, 26 for apostrophe, 27 for period 
			for (int i = 0; i < 28; i++)
				children.push_back(nullptr);
		}

		std::vector<Node*> children;

	};

	Node* root;
};

#endif  // STUDENTSPELLCHECK_H_
