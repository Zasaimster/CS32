#include "StudentTextEditor.h"
#include "Undo.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
	: TextEditor(undo) {
	m_row = 0;
	m_col = 0;
}

StudentTextEditor::~StudentTextEditor()
{
	
}

bool StudentTextEditor::load(string file) {
	ifstream infile(file);

	if (!infile)
		return false;


	reset();

	string s;

	while (getline(infile, s)) {
		if (s.size() != 0 && (s.back() == '\r' || s.back() == '\n')) {
			s.pop_back();
		}
		//textFile[m_row] = s;
		textFile.push_back(s);
	}

	linePointer = textFile.begin();

	return true;


}

bool StudentTextEditor::save(string file) {
	ofstream outfile(file);

	if (!outfile)
		return false;

	for (auto &line : textFile) {
		string currentLine = line;
		outfile << currentLine << endl;
	}

	return true;
}

void StudentTextEditor::reset() {
	textFile.clear();
	m_row = 0;
	m_col = 0;
}

void StudentTextEditor::move(Dir dir) {
	switch (dir) {
		case Dir::UP:
		{
			if (m_row > 0) {
				linePointer--;
				if (linePointer->length() < m_col) {
					m_col = linePointer->length();
				}
				m_row--;
			}
			break;
		}
		case Dir::RIGHT:
		{
			if (m_col < linePointer->length()) {
				m_col++;
			}
			else if (m_row != textFile.size() - 1) {
				linePointer++;
				m_col = 0;
				m_row++;
			}
			break;
		}
		case Dir::DOWN:
		{
			if (m_row < textFile.size() - 1) {
				linePointer++;
				if (linePointer->length() < m_col) {
					m_col = linePointer->length();
				}
				m_row++;
			}
			break;
		}
		case Dir::LEFT:
		{
			if (m_col > 0) {
				m_col--;
			}
			else if(m_row != 0){
				linePointer--;
				m_col = linePointer->length();
				m_row--;
			}
			break;
		}
		case Dir::HOME:
		{
			m_col = 0;
			break;
		}
		case Dir::END:
		{
			m_col = linePointer->length();
			break;
		}
		default:
			break;
		
	}

}

void StudentTextEditor::del() {
	char currChar = (*linePointer)[m_col];
	if (linePointer->size() == 0) {
		if (m_row != textFile.size() - 1) {
			linePointer = textFile.erase(linePointer);
			TextEditor::getUndo()->submit(Undo::Action::JOIN, m_row, m_col);
		}
	}
	else if (m_col == linePointer->size() && m_row != textFile.size() - 1) {
		m_row++;
		m_col = 0;
		linePointer++;
		deleteLine();
		TextEditor::getUndo()->submit(Undo::Action::JOIN, m_row, m_col);
	}
	else {
		deleteCharAt(m_col);
		TextEditor::getUndo()->submit(Undo::Action::DELETE, m_row, m_col, currChar);
	}
}

void StudentTextEditor::backspace() {
	char currChar;
	if (m_col == 0) {
		if (m_row != 0) {
			deleteLine();
			currChar = (*linePointer)[m_col];
			TextEditor::getUndo()->submit(Undo::Action::JOIN, m_row, m_col);
		}
	}
	else {
		currChar = (*linePointer)[m_col - 1];
		deleteCharAt(m_col - 1);
		m_col--;
		TextEditor::getUndo()->submit(Undo::Action::DELETE, m_row, m_col, currChar);
	}
}

void StudentTextEditor::insert(char ch) {
	(ch == '\t') ? insertSpecificChar(ch, 4) : insertSpecificChar(ch, 1);
}

void StudentTextEditor::insertSpecificChar(char ch, int countOfSpaces) {
	for (int i = 0; i < countOfSpaces; i++) {
		*linePointer += " ";
	}

	for (int i = linePointer->length() - 1; i >= m_col; i--) {
		if (i > countOfSpaces - 1) {
			(*linePointer)[i] = (*linePointer)[i - countOfSpaces];
		}
	}
	if (ch == '\t') {
		for (int i = 0; i < 4; i++) {
			(*linePointer)[m_col + i] = ' ';
			TextEditor::getUndo()->submit(Undo::Action::INSERT, m_row, m_col + i, ' ');
		}
	}
	else {
		(*linePointer)[m_col] = ch;
		TextEditor::getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ch);
	}
	

	m_col += countOfSpaces;
}

void StudentTextEditor::enter() {
	TextEditor::getUndo()->submit(Undo::Action::SPLIT, m_row, m_col);
	addLineBreak();
}

void StudentTextEditor::getPos(int& row, int& col) const {
	row = m_row;
	col = m_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
	if (startRow < 0 || numRows < 0 || startRow >= textFile.size()) {
		return -1;
	}

	if (startRow + numRows > textFile.size()) {
		numRows = textFile.size() - startRow;
	}

	list<string>::iterator tempPointer = linePointer;
	int deltaRow = m_row - startRow;
	
	for (int i = 0; i < abs(deltaRow); i++) {
		deltaRow > 0 ? tempPointer-- : tempPointer++;
	}

	for (int i = 0; i < numRows; i++) {
		if (i >= lines.size()) {
			lines.push_back("");
		}

		lines[i] = *tempPointer;
		tempPointer++;
	}

	return lines.size();
}

void StudentTextEditor::undo() {
	Undo* undo = TextEditor::getUndo();

	int row = 0, col = 0, count;
	string text;

	Undo::Action action = undo->get(row, col, count, text);
	if (action == Undo::Action::ERROR)
		return;

	int deltaRow = m_row - row;

	for (int i = 0; i < abs(deltaRow); i++) {
		deltaRow > 0 ? linePointer-- : linePointer++;
		deltaRow > 0 ? m_row-- : m_row++;

	}
	
	m_col = col;

	switch (action) {
		case Undo::Action::INSERT:
		{
			string temp = *linePointer;
			string toKeep = temp.substr(0, m_col);
			string toAdd = temp.substr(col);
			toKeep += text;
			toKeep += toAdd;

			*linePointer = toKeep;
			
			break;
		}
		case Undo::Action::DELETE:
		{
			int index = m_col;
			for (int i = 0; i < count; i++) {
				deleteCharAt(index);
				index--;
				if(m_col > 0) m_col--;
			}

			break;
		}
		case Undo::Action::SPLIT:
		{
			addLineBreak();

			break;
		}
		case Undo::Action::JOIN:
		{
			m_row++;
			m_col = 0;
			linePointer++;
			deleteLine();

			break;
		}
		default:
			return;
	}

}

string StudentTextEditor::getSpecificLine(int row) {
	return *linePointer;
}

void StudentTextEditor::deleteLine() {
	string toSave = *linePointer;
	linePointer = textFile.erase(linePointer);
	linePointer--;
	int newCol = linePointer->size();
	*linePointer += toSave;
	m_row--;
	m_col = newCol;
}

void StudentTextEditor::deleteCharAt(int index) {
	string originalLine = *linePointer;
	string newLine = "";
	for (int i = 0; i < originalLine.size(); i++) {
		if (i != index)
			newLine += originalLine[i];
	}
	*linePointer = newLine;
}

void StudentTextEditor::addLineBreak() {
	string subStringToCarryOver = linePointer->substr(m_col);
	*linePointer = linePointer->substr(0, m_col);

	linePointer++;
	textFile.insert(linePointer, subStringToCarryOver);
	linePointer--;

	m_row++;
	m_col = 0;
}