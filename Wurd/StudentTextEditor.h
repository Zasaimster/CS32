#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include "TextEditor.h"
#include <unordered_map>
#include <list>

class Undo;

class StudentTextEditor : public TextEditor {
public:

	StudentTextEditor(Undo* undo);
	~StudentTextEditor();
	bool load(std::string file);
	bool save(std::string file);
	void reset();
	void move(Dir dir);
	void del();
	void backspace();
	void insert(char ch);
	void enter();
	void getPos(int& row, int& col) const;
	int getLines(int startRow, int numRows, std::vector<std::string>& lines) const;
	void undo();

private:
	std::string getSpecificLine(int row);
	void insertSpecificChar(char ch, int countOfSpaces);
	void deleteLine();
	void deleteCharAt(int index);
	void addLineBreak();

	std::list<std::string> textFile;
	std::list<std::string>::iterator linePointer;

	int m_row;
	int m_col;
	int m_totalLines;
};

#endif // STUDENTTEXTEDITOR_H_
