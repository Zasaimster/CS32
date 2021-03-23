void listAll(const File* f, string path)  // two-parameter overload
{
	const vector<File*>* files = f->files();
	if (files == nullptr)
		return;

	if (files == 0)
		cout << "how";

	//cout << "size: " << files->size() << endl;

	string newPath = path + "/" + f->name();
	cout << newPath << endl;

	vector<File*>::const_iterator it;
	for (it = files->begin(); it != files->end(); it++) {
		const vector<File*>* newFiles = (*it)->files();
		if (newFiles != nullptr) {
			if (newFiles != nullptr) {
				listAll(*it, newPath);
			}
		} else {
			cout << newPath << "/" << (*it)->name() << endl;
		}
	}



	return;
}
